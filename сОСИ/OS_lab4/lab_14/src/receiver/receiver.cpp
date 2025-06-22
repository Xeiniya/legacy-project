#include "receiver.h"
#include "../common/message_file.h"
#include <iostream>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#define SIGNAL_PREFIX "Local\\"
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#define SIGNAL_PREFIX "/"
#endif

#ifdef _WIN32
static std::vector<HANDLE> readyEvents;
#else
static std::vector<sem_t*> readySemaphores;
#endif

std::shared_ptr<Receiver> Receiver::Create(const std::string& fileName, size_t numRecords) {
    // Создаем бинарный файл для сообщений
    if (!MessageFile::Create(fileName, numRecords)) {
        std::cerr << "Failed to create message file" << std::endl;
        return nullptr;
    }

    // Открываем созданный файл
    auto messageFile = MessageFile::Open(fileName);
    if (!messageFile) {
        std::cerr << "Failed to open message file" << std::endl;
        return nullptr;
    }

    return std::shared_ptr<Receiver>(new Receiver(fileName, messageFile));
}

Receiver::Receiver(const std::string& fileName, std::shared_ptr<MessageFile> messageFile)
    : m_fileName(fileName), m_messageFile(messageFile), m_numSenders(0) {
}

bool Receiver::LaunchSenders(int numSenders) {
    m_numSenders = numSenders;

    // Создаем сигналы для синхронизации
    ReadySignal::Create(numSenders);

    // Запускаем процессы Sender
    for (int i = 0; i < numSenders; ++i) {
#ifdef _WIN32
        std::string senderPath = "sender.exe";
#else
        std::string senderPath = "./sender";
#endif
        LaunchSender(senderPath, m_fileName, i);
    }

    return true;
}

bool Receiver::WaitForSenders() {
    std::cout << "Waiting for all senders to be ready..." << std::endl;
    ReadySignal::WaitAll();
    std::cout << "All senders are ready!" << std::endl;
    return true;
}

std::string Receiver::ReadMessage() {
    if (!m_messageFile) {
        return "";
    }

    return m_messageFile->ReadMessage();
}

void Receiver::RunCommandLoop() {
    std::string command;
    bool running = true;

    while (running) {
        std::cout << "\nCommands:\n";
        std::cout << "  read  - Read message from file\n";
        std::cout << "  exit  - Exit program\n";
        std::cout << "Enter command: ";
        std::cin >> command;

        if (command == "read") {
            std::cout << "Waiting for message..." << std::endl;
            std::string message = ReadMessage();
            std::cout << "Received message: " << message << std::endl;
        }
        else if (command == "exit") {
            running = false;
        }
        else {
            std::cout << "Unknown command: " << command << std::endl;
        }
    }

    // Очищаем ресурсы
    if (m_messageFile) {
        m_messageFile->Close();
    }
    ReadySignal::Cleanup();
}

void ReadySignal::Create(int numSenders) {
#ifdef _WIN32
    for (int i = 0; i < numSenders; ++i) {
        std::string signalName = SIGNAL_PREFIX "sender_ready_" + std::to_string(i);
        HANDLE hEvent = CreateEventA(NULL, TRUE, FALSE, signalName.c_str());
        if (hEvent) {
            readyEvents.push_back(hEvent);
        }
    }
#else
    for (int i = 0; i < numSenders; ++i) {
        std::string signalName = SIGNAL_PREFIX "sender_ready_" + std::to_string(i);
        sem_unlink(signalName.c_str());
        sem_t* sem = sem_open(signalName.c_str(), O_CREAT, 0666, 0);
        if (sem != SEM_FAILED) {
            readySemaphores.push_back(sem);
        }
    }
#endif
}

void ReadySignal::WaitAll() {
#ifdef _WIN32
    if (!readyEvents.empty()) {
        WaitForMultipleObjects(static_cast<DWORD>(readyEvents.size()), readyEvents.data(), TRUE, INFINITE);
    }
#else
    for (auto sem : readySemaphores) {
        int value;
        do {
            sem_getvalue(sem, &value);
            if (value <= 0) {
                sem_wait(sem);
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } while (true);
    }
#endif
}

void ReadySignal::Cleanup() {
#ifdef _WIN32
    for (auto handle : readyEvents) {
        CloseHandle(handle);
    }
    readyEvents.clear();
#else
    for (int i = 0; i < readySemaphores.size(); ++i) {
        std::string signalName = SIGNAL_PREFIX "sender_ready_" + std::to_string(i);
        sem_close(readySemaphores[i]);
        sem_unlink(signalName.c_str());
    }
    readySemaphores.clear();
#endif
}

void LaunchSender(const std::string& senderPath, const std::string& fileName, int senderIndex) {
#ifdef _WIN32
    std::string commandLine = "\"" + senderPath + "\" \"" + fileName + "\" " + std::to_string(senderIndex);
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcessA(NULL, const_cast<char*>(commandLine.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        std::cerr << "Failed to launch sender process: " << GetLastError() << std::endl;
    }
#else
    pid_t pid = fork();
    if (pid == 0) {
        // Дочерний процесс
        execl(senderPath.c_str(), senderPath.c_str(), fileName.c_str(), std::to_string(senderIndex).c_str(), NULL);
        exit(EXIT_FAILURE); // В случае ошибки execl
    }
    else if (pid < 0) {
        std::cerr << "Failed to fork sender process" << std::endl;
    }
#endif
}

int main() {
    std::string fileName;
    size_t numRecords;
    int numSenders;

    std::cout << "Enter binary file name: ";
    std::cin >> fileName;
    std::cout << "Enter number of records in file: ";
    std::cin >> numRecords;

    auto receiver = Receiver::Create(fileName, numRecords);
    if (!receiver) {
        std::cerr << "Failed to create receiver" << std::endl;
        return 1;
    }

    std::cout << "Enter number of sender processes: ";
    std::cin >> numSenders;

    if (!receiver->LaunchSenders(numSenders)) {
        std::cerr << "Failed to launch sender processes" << std::endl;
        return 1;
    }

    if (!receiver->WaitForSenders()) {
        std::cerr << "Failed to wait for sender processes" << std::endl;
        return 1;
    }

    receiver->RunCommandLoop();

    return 0;
}