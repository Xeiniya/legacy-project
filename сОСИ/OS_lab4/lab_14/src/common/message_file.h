#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <mutex>
#include <memory>
#include <atomic>
#include <condition_variable>

// ���������
constexpr size_t MAX_MESSAGE_LENGTH = 20;

// ��������� ���������
struct Message {
    bool occupied = false;  // ���� ��������� ������
    char text[MAX_MESSAGE_LENGTH] = { 0 };  // ����� ���������
};

// ����� ��� ������ � ������ ���������
class MessageFile {
public:
    // ������� ����� ���� ���������
    static bool Create(const std::string& filename, size_t numRecords);

    // ������� ������������ ���� ���������
    static std::shared_ptr<MessageFile> Open(const std::string& filename);

    // �������� ��������� � ����
    bool WriteMessage(const std::string& message);

    // ��������� ��������� �� �����
    std::string ReadMessage();

    // ��������� ������������� �����
    bool IsFull() const;

    // ��������� ������� �����
    bool IsEmpty() const;

    // ������� ����
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

    // ��������� ��������� ����� ��� ����������� ���������� �������
    void ReadHeader();
};