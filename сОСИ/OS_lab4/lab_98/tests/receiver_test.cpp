#include <gtest/gtest.h>
#include "receiver.h"
#include "common/message_file.h"

using namespace std;

class ReceiverTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем временный файл для тестов
        test_filename = "test_message_file.bin";
        num_records = 10;
        receiver = Receiver::Create(test_filename, num_records);
    }

    void TearDown() override {
        // Удаляем временный файл после тестов
        remove(test_filename.c_str());
    }

    string test_filename;
    size_t num_records;
    shared_ptr<Receiver> receiver;
};

TEST_F(ReceiverTest, CreateSuccess) {
    EXPECT_NE(receiver, nullptr);
}

TEST_F(ReceiverTest, ReadEmpty) {
    EXPECT_TRUE(receiver->ReadMessage().empty());
}

TEST_F(ReceiverTest, WriteThenRead) {
    // Сначала записываем сообщение через MessageFile
    auto msg_file = MessageFile::Open(test_filename);
    string test_msg = "Test message";
    ASSERT_TRUE(msg_file->WriteMessage(test_msg));

    // Теперь читаем через Receiver
    string received = receiver->ReadMessage();
    EXPECT_EQ(received, test_msg);
}

TEST_F(ReceiverTest, LaunchSenders) {
    EXPECT_TRUE(receiver->LaunchSenders(2));
}