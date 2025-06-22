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
        // Удаляем тестовый файл, если он существует
        if (std::filesystem::exists(TEST_FILENAME)) {
            std::filesystem::remove(TEST_FILENAME);
        }

        // Создаем новый тестовый файл
        MessageFile::Create(TEST_FILENAME, TEST_NUM_RECORDS);
    }

    void TearDown() override {
        // Удаляем тестовый файл
        if (std::filesystem::exists(TEST_FILENAME)) {
            std::filesystem::remove(TEST_FILENAME);
        }
    }
};

// Тест создания и открытия файла
TEST_F(MessageFileTest, CreateAndOpen) {
    auto messageFile = MessageFile::Open(TEST_FILENAME);
    ASSERT_TRUE(messageFile != nullptr);

    // Проверяем, что файл пуст
    EXPECT_TRUE(messageFile->IsEmpty());
    EXPECT_FALSE(messageFile->IsFull());
}

// Тест записи и чтения сообщений
TEST_F(MessageFileTest, WriteAndReadMessage) {
    auto messageFile = MessageFile::Open(TEST_FILENAME);

    // Записываем сообщение
    std::string testMessage = "Test message";
    EXPECT_TRUE(messageFile->WriteMessage(testMessage));

    // Проверяем, что файл не пуст
    EXPECT_FALSE(messageFile->IsEmpty());

    // Читаем сообщение
    std::string readMessage = messageFile->ReadMessage();
    EXPECT_EQ(testMessage, readMessage);

    // Проверяем, что файл снова пуст
    EXPECT_TRUE(messageFile->IsEmpty());
}

// Тест заполнения файла
TEST_F(MessageFileTest, FillingFile) {
    auto messageFile = MessageFile::Open(TEST_FILENAME);

    // Записываем максимальное количество сообщений
    for (size_t i = 0; i < TEST_NUM_RECORDS; ++i) {
        std::string message = "Message " + std::to_string(i);
        EXPECT_TRUE(messageFile->WriteMessage(message));
    }

    // Проверяем, что файл полон
    EXPECT_TRUE(messageFile->IsFull());

    // Читаем все сообщения
    for (size_t i = 0; i < TEST_NUM_RECORDS; ++i) {
        std::string message = messageFile->ReadMessage();
        EXPECT_FALSE(message.empty());
    }

    // Проверяем, что файл снова пуст
    EXPECT_TRUE(messageFile->IsEmpty());
}

// Тест многопоточной работы
TEST_F(MessageFileTest, MultiThreaded) {
    auto messageFile = MessageFile::Open(TEST_FILENAME);
    std::vector<std::thread> threads;

    // Запускаем несколько потоков для записи
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([messageFile, i]() {
            for (int j = 0; j < 10; ++j) {
                std::string message = "Thread " + std::to_string(i) + " message " + std::to_string(j);
                messageFile->WriteMessage(message);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            });
    }

    // Запускаем несколько потоков для чтения
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([messageFile]() {
            for (int j = 0; j < 10; ++j) {
                std::string message = messageFile->ReadMessage();
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            });
    }

    // Ждем завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    // После завершения всех потоков файл должен быть пуст
    EXPECT_TRUE(messageFile->IsEmpty());
}