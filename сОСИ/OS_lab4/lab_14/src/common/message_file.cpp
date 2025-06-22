#include "message_file.h"
#include <iostream>
#include <thread>
#include <chrono>

bool MessageFile::Create(const std::string& filename, size_t numRecords) {
    // ������� ���� � ���������� ���������� ������� � ���������
    std::ofstream file(filename, std::ios::binary | std::ios::out);
    if (!file) {
        std::cerr << "Failed to create file: " << filename << std::endl;
        return false;
    }

    // ������ ������� � ����
    file.write(reinterpret_cast<const char*>(&numRecords), sizeof(numRecords));

    // ������������� ���� ������� � �����
    Message emptyMessage;
    for (size_t i = 0; i < numRecords; ++i) {
        file.write(reinterpret_cast<const char*>(&emptyMessage), sizeof(Message));
    }

    file.close();
    return true;
}

std::shared_ptr<MessageFile> MessageFile::Open(const std::string& filename) {
    auto messageFile = std::shared_ptr<MessageFile>(new MessageFile(filename));
    messageFile->ReadHeader();
    return messageFile;
}

MessageFile::MessageFile(const std::string& filename) : m_filename(filename) {
    m_file.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!m_file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
}

void MessageFile::ReadHeader() {
    if (!m_file) {
        return;
    }

    // ������ ���������� ������� �� ���������
    m_file.seekg(0, std::ios::beg);
    m_file.read(reinterpret_cast<char*>(&m_numRecords), sizeof(m_numRecords));

    // ������������ ������� ������
    m_numOccupied = 0;
    for (size_t i = 0; i < m_numRecords; ++i) {
        Message message;
        m_file.seekg(sizeof(m_numRecords) + i * sizeof(Message), std::ios::beg);
        m_file.read(reinterpret_cast<char*>(&message), sizeof(Message));
        if (message.occupied) {
            m_numOccupied++;
        }
    }
}

bool MessageFile::WriteMessage(const std::string& message) {
    if (!m_file) {
        return false;
    }

    std::unique_lock<std::mutex> lock(m_mutex);

    // ����, ���� �������� ��������� �����
    m_writeCondition.wait(lock, [this]() { return !IsFull(); });

    // ���� ��������� ������
    size_t freeIndex = 0;
    bool foundFree = false;

    for (size_t i = 0; i < m_numRecords; ++i) {
        Message msg;
        m_file.seekg(sizeof(m_numRecords) + i * sizeof(Message), std::ios::beg);
        m_file.read(reinterpret_cast<char*>(&msg), sizeof(Message));

        if (!msg.occupied) {
            freeIndex = i;
            foundFree = true;
            break;
        }
    }

    if (!foundFree) {
        return false;
    }

    // ������� � ���������� ����� ���������
    Message newMessage;
    newMessage.occupied = true;
    std::strncpy(newMessage.text, message.c_str(), MAX_MESSAGE_LENGTH - 1);

    m_file.seekp(sizeof(m_numRecords) + freeIndex * sizeof(Message), std::ios::beg);
    m_file.write(reinterpret_cast<const char*>(&newMessage), sizeof(Message));
    m_file.flush();

    // ����������� ������� ������� ����
    m_numOccupied++;

    // ���������� ��������� � ����� ���������
    lock.unlock();
    m_readCondition.notify_one();

    return true;
}

std::string MessageFile::ReadMessage() {
    if (!m_file) {
        return "";
    }

    std::unique_lock<std::mutex> lock(m_mutex);

    // ����, ���� �������� ���������
    m_readCondition.wait(lock, [this]() { return !IsEmpty(); });

    // ���� ������� ������
    size_t occupiedIndex = 0;
    bool foundOccupied = false;

    for (size_t i = 0; i < m_numRecords; ++i) {
        Message msg;
        m_file.seekg(sizeof(m_numRecords) + i * sizeof(Message), std::ios::beg);
        m_file.read(reinterpret_cast<char*>(&msg), sizeof(Message));

        if (msg.occupied) {
            occupiedIndex = i;
            foundOccupied = true;
            break;
        }
    }

    if (!foundOccupied) {
        return "";
    }

    // ������ ���������
    Message message;
    m_file.seekg(sizeof(m_numRecords) + occupiedIndex * sizeof(Message), std::ios::beg);
    m_file.read(reinterpret_cast<char*>(&message), sizeof(Message));

    // ����������� ������
    message.occupied = false;
    m_file.seekp(sizeof(m_numRecords) + occupiedIndex * sizeof(Message), std::ios::beg);
    m_file.write(reinterpret_cast<const char*>(&message), sizeof(Message));
    m_file.flush();

    // ��������� ������� ������� ����
    m_numOccupied--;

    std::string result = message.text;

    // ���������� ��������� � ��������� �����
    lock.unlock();
    m_writeCondition.notify_one();

    return result;
}

bool MessageFile::IsFull() const {
    return m_numOccupied >= m_numRecords;
}

bool MessageFile::IsEmpty() const {
    return m_numOccupied == 0;
}

void MessageFile::Close() {
    if (m_file) {
        m_file.close();
    }
}