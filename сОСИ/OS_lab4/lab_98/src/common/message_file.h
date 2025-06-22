#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <windows.h>


const size_t MAX_MESSAGE_LENGTH = 20;

struct Message {
    bool occupied;  // Флаг занятости записи
    char text[MAX_MESSAGE_LENGTH];  // Текст сообщения

    Message() : occupied(false) {
        for (size_t i = 0; i < MAX_MESSAGE_LENGTH; ++i) {
            text[i] = '\0';
        }
    }
};

// Заголовок файла
struct FileHeader {
    size_t numRecords;
    size_t readIndex;
    size_t writeIndex;
    size_t count;

    FileHeader() : numRecords(0), readIndex(0), writeIndex(0), count(0) {}
};

class MessageFile {
public:
    // Создать новый файл сообщений
    static bool Create(const std::string& filename, size_t numRecords);

    // Открыть существующий файл сообщений
    static MessageFile* Open(const std::string& filename);

    // Записать сообщение в файл
    bool WriteMessage(const std::string& message);

    // Прочитать сообщение из файла
    std::string ReadMessage();

    // Проверить заполненность файла
    bool IsFull();

    // Проверить пустоту файла
    bool IsEmpty();

    // Закрыть файл
    void Close();

    // Деструктор
    ~MessageFile();

private:
    MessageFile(const std::string& filename);
    bool Initialize();
    bool ReadHeader();
    bool WriteHeader();
    bool LockFile();
    void UnlockFile();

    std::string m_filename;
    std::fstream m_file;
    FileHeader m_header;

    HANDLE m_hFile;
    HANDLE m_hMutex;

};