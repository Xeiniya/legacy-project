#include "receiver.h"
#include "common/message_file.h"
#include <iostream>
#include <cstdlib>
#include <vector>

#include <windows.h>
#include <process.h>
#define SIGNAL_PREFIX "Local\\"


static std::vector<HANDLE> readyEvents; 


// �������� ���������� Receiver
Receiver* Receiver::Create(const std::string& fileName, size_t numRecords) {
    // �������� ��������� ����� ��� ���������
    if (!MessageFile::Create(fileName, numRecords)) {
        std::cerr << "Failed to create message file" << std::endl;
        return NULL;
    }

    // �������� ���������� �����
    MessageFile* messageFile = MessageFile::Open(fileName);
    if (!messageFile) {
        std::cerr << "Failed to open message file" << std::endl;
        return NULL;
    }

    return new Receiver(fileName, messageFile); // ���������� ��������� �� ����� ������
}

// ����������� Receiver
Receiver::Receiver(const std::string& fileName, MessageFile* messageFile)
    : m_fileName(fileName), m_messageFile(messageFile), m_numSenders(0) {
}

// ���������� Receiver
Receiver::~Receiver() {
    if (m_messageFile) {
        delete m_messageFile;
    }
}

// ����� ������� ���������-������������
bool Receiver::LaunchSenders(int numSenders) {
    m_numSenders = numSenders;

    ReadySignal::Create(numSenders); // �������� �������� ����������

    for (int i = 0; i < numSenders; ++i) {
        // ���������� ���� � sender.exe
        char path[MAX_PATH];
        GetModuleFileNameA(NULL, path, MAX_PATH);
        std::string exePath = path;
        std::string executableDir = exePath.substr(0, exePath.find_last_of("\\/"));

        std::string senderPath = executableDir + "\\sender.exe";

        // ���� ���� �� ������, ���� � �������������� ������
        if (GetFileAttributesA(senderPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
            std::vector<std::string> possiblePaths = {
                executableDir + "\\..\\sender\\sender.exe",
                executableDir + "\\..\\..\\sender.exe",
                executableDir + "\\..\\sender.exe",
                "sender.exe"
            };
 
            for (size_t j = 0; j < possiblePaths.size(); ++j) {
                if (GetFileAttributesA(possiblePaths[j].c_str()) != INVALID_FILE_ATTRIBUTES) {
                    senderPath = possiblePaths[j];
                    break;
                }
            }
        }

        LaunchSender(senderPath, m_fileName, i);
    }
    return true;
}

// �������� ���������� ���� ������������
bool Receiver::WaitForSenders() {
    std::cout << "Waiting for all senders to be ready..." << std::endl;
    ReadySignal::WaitAll(); // �������� ���� ��������
    std::cout << "All senders are ready!" << std::endl;
    return true;
}

// ������ ������ ��������� �� �����
std::string Receiver::ReadMessage() {
    if (!m_messageFile) {
        return "";
    }

    while (m_messageFile->IsEmpty()) {
        Sleep(100); // ��� 100 ��, ���� ���� ����
    }

    return m_messageFile->ReadMessage(); // ���������� ����������� ���������
}

// ���� ������ ������������
void Receiver::RunCommandLoop() {
    std::string command;
    bool running = true;

    while (running) {
        // ���� ������
        std::cout << "\nCommands:\n";
        std::cout << "  read  - Read message from file\n";
        std::cout << "  exit  - Exit program\n";
        std::cout << "Enter command: ";
        std::cin >> command;

        if (command == "read") {
            std::cout << "Waiting for message..." << std::endl;
            std::string message = ReadMessage();
            if (!message.empty()) {
                std::cout << "Received message: " << message << std::endl;
            }
            else {
                std::cout << "No message received" << std::endl;
            }
        }
        else if (command == "exit") {
            running = false;
        }
        else {
            std::cout << "Unknown command: " << command << std::endl;
        }
    }

    // �������� ����� � ������� ��������
    if (m_messageFile) {
        m_messageFile->Close();
    }
    ReadySignal::Cleanup();
}

// �������� �������� ����������
void ReadySignal::Create(int numSenders) {

    for (int i = 0; i < numSenders; ++i) {
        std::string signalName = SIGNAL_PREFIX "sender_ready_" + std::to_string(i);

        HANDLE hEvent = CreateEventA(NULL, TRUE, FALSE, signalName.c_str());
        if (hEvent) {
            readyEvents.push_back(hEvent);
        }
    }
}

// �������� ���� �������� ����������
void ReadySignal::WaitAll() {
    if (!readyEvents.empty()) {
        WaitForMultipleObjects(static_cast<DWORD>(readyEvents.size()), &readyEvents[0], TRUE, INFINITE);
    }
}

// ������� �������� ��������
void ReadySignal::Cleanup() {

    for (size_t i = 0; i < readyEvents.size(); ++i) {
        CloseHandle(readyEvents[i]);
    }
    readyEvents.clear();
}

// ������ �������� �����������
void LaunchSender(const std::string& senderPath, const std::string& fileName, int senderIndex) {
    std::string commandLine = "\"" + senderPath + "\" \"" + fileName + "\" " + std::to_string(senderIndex);

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (GetFileAttributesA(senderPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        std::cerr << "Sender executable not found at: " << senderPath << std::endl;
        return;
    }

    std::cout << "Launching sender with command line: " << commandLine << std::endl;
    char* cmdLine = new char[commandLine.length() + 1];
    strcpy(cmdLine, commandLine.c_str());

    if (CreateProcessA(NULL, cmdLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        std::cout << "Sender process launched successfully with PID: " << pi.dwProcessId << std::endl;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        DWORD error = GetLastError();
        std::cerr << "Failed to launch sender process. Error code: " << error << std::endl;
    }

    delete[] cmdLine;
}

// ����� �����
int main() {
    std::string fileName;
    size_t numRecords;
    int numSenders;

    std::cout << "Enter binary file name: ";
    std::cin >> fileName;
    std::cout << "Enter number of records in file: ";
    std::cin >> numRecords;

    Receiver* receiver = Receiver::Create(fileName, numRecords); // �������� ��������
    if (!receiver) {
        std::cerr << "Failed to create receiver" << std::endl;
        return 1;
    }

    std::cout << "Enter number of sender processes: ";
    std::cin >> numSenders;

    // ��������� ������������
    if (!receiver->LaunchSenders(numSenders)) {
        std::cerr << "Failed to launch sender processes" << std::endl;
        delete receiver;
        return 1;
    }

    // ������� ���������� ������������
    if (!receiver->WaitForSenders()) {
        std::cerr << "Failed to wait for sender processes" << std::endl;
        delete receiver;
        return 1;
    }

    // ��������� ��������� ����
    receiver->RunCommandLoop();

    delete receiver;
    return 0;
}
