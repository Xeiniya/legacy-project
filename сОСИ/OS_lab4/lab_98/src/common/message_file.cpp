#include "message_file.h"
#include <iostream>
#include <cstring>
#include <windows.h>
#include <io.h>
#include <fcntl.h>


// Создание нового файла сообщений с заданным количеством записей
bool MessageFile::Create(const std::string& filename, size_t numRecords) {
    std::ofstream file(filename.c_str(), std::ios::binary | std::ios::out);
    if (!file) {
        std::cerr << "Failed to create file: " << filename << std::endl;
        return false;
    }

    // Инициализация заголовка файла
    FileHeader header;
    header.numRecords = numRecords;
    header.readIndex = 0;
    header.writeIndex = 0;
    header.count = 0;

    file.write(reinterpret_cast<const char*>(&header), sizeof(FileHeader));

    // Запись пустых сообщений в файл
    Message emptyMessage;
    for (size_t i = 0; i < numRecords; ++i) {
        file.write(reinterpret_cast<const char*>(&emptyMessage), sizeof(Message));
    }

    file.close();
    return true;
}

// Открытие существующего файла сообщений
MessageFile* MessageFile::Open(const std::string& filename) {
    MessageFile* messageFile = new MessageFile(filename);
    if (!messageFile->Initialize()) {
        delete messageFile;
        return NULL;
    }
    return messageFile;
}

// Конструктор класса
MessageFile::MessageFile(const std::string& filename) : m_filename(filename) {
    m_hFile = INVALID_HANDLE_VALUE;
    m_hMutex = NULL;
}

// Деструктор класса
MessageFile::~MessageFile() {
    Close();
}

// Инициализация: открытие файла и создание синхронизации
bool MessageFile::Initialize() {
    m_file.open(m_filename.c_str(), std::ios::binary | std::ios::in | std::ios::out);
    if (!m_file) {
        std::cerr << "Failed to open file: " << m_filename << std::endl;
        return false;
    }

    // Чтение заголовка
    if (!ReadHeader()) {
        std::cerr << "Failed to read header" << std::endl;
        return false;
    }


    // Создание мьютекса по имени файла (Windows)
    std::string mutexName = "MessageFile_" + m_filename;
    for (size_t i = 0; i < mutexName.length(); ++i) {
        if (mutexName[i] == '\\' || mutexName[i] == '/' || mutexName[i] == ':') {
            mutexName[i] = '_';
        }
    }
    m_hMutex = CreateMutexA(NULL, FALSE, mutexName.c_str());
    if (!m_hMutex) {
        std::cerr << "Failed to create mutex" << std::endl;
        return false;
    }

    return true;
}

// Чтение заголовка файла
bool MessageFile::ReadHeader() {
    m_file.seekg(0, std::ios::beg);
    m_file.read(reinterpret_cast<char*>(&m_header), sizeof(FileHeader));
    return m_file.good();
}

// Запись заголовка в файл
bool MessageFile::WriteHeader() {
    m_file.seekp(0, std::ios::beg);
    m_file.write(reinterpret_cast<const char*>(&m_header), sizeof(FileHeader));
    m_file.flush();
    return m_file.good();
}

// Захват блокировки для синхронизации доступа
bool MessageFile::LockFile() {

    DWORD result = WaitForSingleObject(m_hMutex, INFINITE);
    return result == WAIT_OBJECT_0;
}

// Освобождение блокировки
void MessageFile::UnlockFile() {
    ReleaseMutex(m_hMutex);
}

// Запись одного сообщения в файл
bool MessageFile::WriteMessage(const std::string& message) {
    if (!m_file) return false;

    if (!LockFile()) return false;

    ReadHeader();

    // Проверка переполнения буфера
    if (m_header.count >= m_header.numRecords) {
        UnlockFile();
        return false;
    }

    // Подготовка сообщения
    Message newMessage;
    newMessage.occupied = true;
    strncpy(newMessage.text, message.c_str(), MAX_MESSAGE_LENGTH - 1);
    newMessage.text[MAX_MESSAGE_LENGTH - 1] = '\0';

    // Запись сообщения в позицию writeIndex
    size_t filePos = sizeof(FileHeader) + m_header.writeIndex * sizeof(Message);
    m_file.seekp(filePos, std::ios::beg);
    m_file.write(reinterpret_cast<const char*>(&newMessage), sizeof(Message));

    // Обновление индексов
    m_header.writeIndex = (m_header.writeIndex + 1) % m_header.numRecords;
    m_header.count++;
    WriteHeader();

    UnlockFile();
    return true;
}

// Чтение одного сообщения из файла
std::string MessageFile::ReadMessage() {
    if (!m_file) return "";

    if (!LockFile()) return "";

    ReadHeader();

    // Проверка наличия сообщений
    if (m_header.count == 0) {
        UnlockFile();
        return "";
    }

    // Чтение сообщения из позиции readIndex
    Message message;
    size_t filePos = sizeof(FileHeader) + m_header.readIndex * sizeof(Message);
    m_file.seekg(filePos, std::ios::beg);
    m_file.read(reinterpret_cast<char*>(&message), sizeof(Message));

    // Очистка позиции в файле
    Message emptyMessage;
    m_file.seekp(filePos, std::ios::beg);
    m_file.write(reinterpret_cast<const char*>(&emptyMessage), sizeof(Message));

    // Обновление индексов
    m_header.readIndex = (m_header.readIndex + 1) % m_header.numRecords;
    m_header.count--;
    WriteHeader();

    std::string result = message.text;
    UnlockFile();

    return result;
}

// Проверка, заполнен ли буфер сообщений
bool MessageFile::IsFull() {
    if (!LockFile()) return false;
    ReadHeader();
    bool full = (m_header.count >= m_header.numRecords);
    UnlockFile();
    return full;
}

// Проверка, пуст ли буфер сообщений
bool MessageFile::IsEmpty() {
    if (!LockFile()) return true;
    ReadHeader();
    bool empty = (m_header.count == 0);
    UnlockFile();
    return empty;
}

// Закрытие файла и очистка ресурсов
void MessageFile::Close() {
    if (m_file.is_open()) {
        m_file.close();
    }

    if (m_hMutex) {
        CloseHandle(m_hMutex);
        m_hMutex = NULL;
    }
}
