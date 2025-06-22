#include "common/message_file.h"
#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <filesystem>

const std::string TEST_FILENAME = "test_messages.bin";
const size_t TEST_NUM_RECORDS = 5;

class MessageFileTest : public ::testing::Test {
protected:
    void SetUp() override {
        // ������� �������� ����, ���� �� ����������
        if (std::filesystem::exists(TEST_FILENAME)) {
            std::filesystem::remove(TEST_FILENAME);
        }

        // ������� ����� �������� ����
        MessageFile::Create(TEST_FILENAME, TEST_NUM_RECORDS);
    }

    void TearDown() override {
        // ������� �������� ����
        if (std::filesystem::exists(TEST_FILENAME)) {
            std::filesystem::remove(TEST_FILENAME);
        }
    }
};

// ���� �������� � �������� �����
TEST_F(MessageFileTest, CreateAndOpen) {
    auto messageFile = MessageFile::Open(TEST_FILENAME);
    ASSERT_TRUE(messageFile != nullptr);

    // ���������, ��� ���� ����
    EXPECT_TRUE(messageFile->IsEmpty());
    EXPECT_FALSE(messageFile->IsFull());
}

// ���� ������ � ������ ���������
TEST_F(MessageFileTest, WriteAndReadMessage) {
    auto messageFile = MessageFile::Open(TEST_FILENAME);

    // ���������� ���������
    std::string testMessage = "Test message";
    EXPECT_TRUE(messageFile->WriteMessage(testMessage));

    // ���������, ��� ���� �� ����
    EXPECT_FALSE(messageFile->IsEmpty());

    // ������ ���������
    std::string readMessage = messageFile->ReadMessage();
    EXPECT_EQ(testMessage, readMessage);

    // ���������, ��� ���� ����� ����
    EXPECT_TRUE(messageFile->IsEmpty());
}

// ���� ���������� �����
TEST_F(MessageFileTest, FillingFile) {
    auto messageFile = MessageFile::Open(TEST_FILENAME);

    // ���������� ������������ ���������� ���������
    for (size_t i = 0; i < TEST_NUM_RECORDS; ++i) {
        std::string message = "Message " + std::to_string(i);
        EXPECT_TRUE(messageFile->WriteMessage(message));
    }

    // ���������, ��� ���� �����
    EXPECT_TRUE(messageFile->IsFull());

    // ������ ��� ���������
    for (size_t i = 0; i < TEST_NUM_RECORDS; ++i) {
        std::string message = messageFile->ReadMessage();
        EXPECT_FALSE(message.empty());
    }

    // ���������, ��� ���� ����� ����
    EXPECT_TRUE(messageFile->IsEmpty());
}

// ���� ������������� ������
TEST_F(MessageFileTest, MultiThreaded) {
    auto messageFile = MessageFile::Open(TEST_FILENAME);
    std::vector<std::thread> threads;

    // ��������� ��������� ������� ��� ������
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([messageFile, i]() {
            for (int j = 0; j < 10; ++j) {
                std::string message = "Thread " + std::to_string(i) + " message " + std::to_string(j);
                messageFile->WriteMessage(message);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            });
    }

    // ��������� ��������� ������� ��� ������
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([messageFile]() {
            for (int j = 0; j < 10; ++j) {
                std::string message = messageFile->ReadMessage();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            });
    }

    // ���� ���������� ���� �������
    for (auto& thread : threads) {
        thread.join();
    }

    // ����� ���������� ���� ������� ���� ������ ���� ����
    EXPECT_TRUE(messageFile->IsEmpty());
}