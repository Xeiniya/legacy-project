#include "sender.h"
#include "../common/message_file.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#define SIGNAL_PREFIX "Local\\"
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#define SIGNAL_PREFIX "/"
#endif

std::shared_ptr<Sender> Sender::Open(const std::string& fileName, int senderIndex) {
    auto messageFile = MessageFile::Open(fileName);
    if (!messageFile) {
        std::cerr << "Failed to open message file" << std::endl;
        return nullptr;
    }

    return std::shared_ptr<Sender>(new Sender(fileName, senderIndex, messageFile));
}

Sender::Sender(const std::string& fileName, int senderIndex, std::shared_ptr<MessageFile> messageFile)
    : m_fileName(fileName), m_senderIndex(senderIndex), m_messageFile(messageFile) {
}

bool Sender::SignalReady() {
    ReadySignal::Signal(m_senderIndex);
    std::cout << "Ready signal sent to Receiver" << std::endl;
    return true;
}

bool Sender::SendMessage(const std::string& message) {
    if (!m_messageFile) {
        return false;
    }

    return m_messageFile->WriteMessage(message);
}

void Sender::RunCommandLoop() {
    std::string command;
    bool running = true;

    while (running) {
        std::cout << "\nCommands:\n";
        std::cout << "  send <message> - Send message\n";
        std::cout << "  exit           - Exit program\n";
        std::cout << "Enter command: ";
        std::getline(std::cin, command);

        if (command.empty() && std::cin.eof()) {
            running = false;
            continue;
        }

        std::string cmd;
        std::string message;

        size_t spacePos = command.find(' ');
        if (spacePos != std::string::npos) {
            cmd = command.substr(0, spacePos);
            message = command.substr(spacePos + 1);
        }
        else {
            cmd = command;
        }

        if (cmd == "send") {
            if (message.empty()) {
                std::cout << "Enter message text: ";
                std::getline(std::cin, message);
            }

            if (message.length() >= MAX_MESSAGE_LENGTH) {
                std::cout << "Message is too long (max " << MAX_MESSAGE_LENGTH - 1 << " chars)" << std::endl;
                message = message.substr(0, MAX_MESSAGE_LENGTH - 1);
            }

            std::cout << "Sending message: " << message << std::endl;
            if (SendMessage(message)) {
                std::cout << "Message sent successfully" << std::endl;
            }
            else {
                std::cout << "Failed to send message" << std::endl;
            }
        }
        else if (cmd == "exit") {
            running = false;
        }
        else {
            std::cout << "Unknown command: " << cmd << std::endl;
        }
    }

    if (m_messageFile) {
        m_messageFile->Close();
    }
}

void ReadySignal::Signal(int senderIndex) {
    std::string signalName = SIGNAL_PREFIX "sender_ready_" + std::to_string(senderIndex);

#ifdef _WIN32
    HANDLE hEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, signalName.c_str());
    if (hEvent) {
        SetEvent(hEvent);
        CloseHandle(hEvent);
    }
    else {
        std::cerr << "Failed to open event: " << GetLastError() << std::endl;
    }
#else
    sem_t* sem = sem_open(signalName.c_str(), 0);
    if (sem != SEM_FAILED) {
        sem_post(sem);
        sem_close(sem);
    }
    else {
        std::cerr << "Failed to open semaphore" << std::endl;
    }
#endif
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <sender_index>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    int senderIndex = std::stoi(argv[2]);

    std::cout << "Sender " << senderIndex << " started with file: " << fileName << std::endl;

    auto sender = Sender::Open(fileName, senderIndex);
    if (!sender) {
        std::cerr << "Failed to open sender" << std::endl;
        return 1;
    }

    if (!sender->SignalReady()) {
        std::cerr << "Failed to signal ready" << std::endl;
        return 1;
    }

    std::string dummy;
    std::getline(std::cin, dummy);

    sender->RunCommandLoop();

    return 0;
}