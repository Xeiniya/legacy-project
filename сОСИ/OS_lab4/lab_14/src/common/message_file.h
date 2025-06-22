#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <mutex>
#include <memory>
#include <atomic>
#include <condition_variable>

// Константы
constexpr size_t MAX_MESSAGE_LENGTH = 20;

// Структура сообщения
struct Message {
    bool occupied = false;  // Флаг занятости записи
    char text[MAX_MESSAGE_LENGTH] = { 0 };  // Текст сообщения
};

// Класс для работы с файлом сообщений
class MessageFile {
public:
    // Создать новый файл сообщений
    static bool Create(const std::string& filename, size_t numRecords);

    // Открыть существующий файл сообщений
    static std::shared_ptr<MessageFile> Open(const std::string& filename);

    // Записать сообщение в файл
    bool WriteMessage(const std::string& message);

    // Прочитать сообщение из файла
    std::string ReadMessage();

    // Проверить заполненность файла
    bool IsFull() const;

    // Проверить пустоту файла
    bool IsEmpty() const;

    // Закрыть файл
    void Close();

private:
    MessageFile(const std::string& filename);

    std::string m_filename;
    std::fstream m_file;
    size_t m_numRecords = 0;
    std::atomic<size_t> m_numOccupied{ 0 };

    std::mutex m_mutex;
    std::condition_variable m_readCondition;
    std::condition_variable m_writeCondition;

    // Прочитать заголовок файла для определения количества записей
    void ReadHeader();
};