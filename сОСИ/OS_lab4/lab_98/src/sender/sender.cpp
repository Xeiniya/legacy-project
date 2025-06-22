#define WIN32_LEAN_AND_MEAN

#include "sender.h"
#include "common/message_file.h" 
#include <iostream>
#include <string>
#include <cstdlib>

#include <windows.h>
#define SIGNAL_PREFIX "Local\\" 


// Статическая функция для создания объекта Sender и открытия файла сообщений
Sender* Sender::Open(const std::string& fileName, int senderIndex) {
    MessageFile* messageFile = MessageFile::Open(fileName); // Открытие бинарного файла
    if (!messageFile) {
        std::cerr << "Failed to open message file" << std::endl;
        return NULL;
    }
    return new Sender(fileName, senderIndex, messageFile);
}

// Конструктор Sender — инициализация переданных параметров
Sender::Sender(const std::string& fileName, int senderIndex, MessageFile* messageFile)
    : m_fileName(fileName), m_senderIndex(senderIndex), m_messageFile(messageFile) {
}

// Деструктор — освобождает ресурсы
Sender::~Sender() {
    if (m_messageFile) {
        delete m_messageFile;
    }
}

// Функция отправки сообщения через MessageFile
bool Sender::SendMsg(const std::string& message) {
    if (!m_messageFile) {
        return false;
    }
    return m_messageFile->WriteMessage(message); // Запись сообщения в бинарный файл
}

// Отправка сигнала о готовности Receiver'у
bool Sender::SignalReady() {
    ReadySignal::Signal(m_senderIndex); // Используется событие 
    std::cout << "Ready signal sent to Receiver" << std::endl;
    return true;
}

// Основной цикл команд — взаимодействие с пользователем
void Sender::RunCommandLoop() {
    std::string command;
    bool running = true;

    while (running) {
        std::cout << "\nCommands:\n";
        std::cout << "  send <message> - Send message\n";
        std::cout << "  exit           - Exit program\n";
        std::cout << "Enter command: ";
        std::getline(std::cin, command);

        // Обработка конца файла
        if (command.empty() && std::cin.eof()) {
            running = false;
            continue;
        }

        // Разделение команды и текста сообщения
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

        // Обработка команды "send"
        if (cmd == "send") {
            if (message.empty()) {
                std::cout << "Enter message text: ";
                std::getline(std::cin, message);
            }

            // Ограничение на длину сообщения
            if (message.length() >= MAX_MESSAGE_LENGTH) {
                std::cout << "Message is too long (max " << (MAX_MESSAGE_LENGTH) << " chars)" << std::endl;
                message = message.substr(0, MAX_MESSAGE_LENGTH - 1);
            }

            std::cout << "Sending message: " << message << std::endl;
            if (this->SendMsg(message)) {
                std::cout << "Message sent successfully" << std::endl;
            }
            else {
                std::cout << "Failed to send message" << std::endl;
            }
        }
        // Обработка команды "exit"
        else if (cmd == "exit") {
            running = false;
        }
        // Неизвестная команда
        else {
            std::cout << "Unknown command: " << cmd << std::endl;
        }
    }

    // Закрытие файла при завершении
    if (m_messageFile) {
        m_messageFile->Close();
    }
}

// Отправка сигнала о готовности по платформе
void ReadySignal::Signal(int senderIndex) {
    std::string signalName = SIGNAL_PREFIX "sender_ready_";
    char indexBuffer[32];
    sprintf(indexBuffer, "%d", senderIndex); // Преобразование индекса в строку
    signalName += indexBuffer;

    // Отправка сигнала через событие
    HANDLE hEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, signalName.c_str());
    if (hEvent) {
        SetEvent(hEvent); // Установка события
        CloseHandle(hEvent); // Освобождение дескриптора
    }
    else {
        std::cerr << "Failed to open event: " << GetLastError() << std::endl;
    }
}

// Точка входа в программу Sender
int main(int argc, char* argv[]) {
    // Проверка аргументов командной строки
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <sender_index>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    int senderIndex = atoi(argv[2]);

    std::cout << "Sender " << senderIndex << " started with file: " << fileName << std::endl;

    // Создание объекта Sender
    Sender* sender = Sender::Open(fileName, senderIndex);
    if (!sender) {
        std::cerr << "Failed to open sender" << std::endl;
        return 1;
    }

    // Отправка сигнала о готовности
    if (!sender->SignalReady()) {
        std::cerr << "Failed to signal ready" << std::endl;
        delete sender;
        return 1;
    }

    // Ожидание команды от пользователя
    std::string dummy;
    std::getline(std::cin, dummy);

    // Запуск интерактивного цикла команд
    sender->RunCommandLoop();

    delete sender;
    return 0;
}
