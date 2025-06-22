#pragma once

#include <string>
#include <vector>
#include "common/message_file.h"

class Receiver {
public:
    static Receiver* Create(const std::string& fileName, size_t numRecords);
    ~Receiver();

    bool LaunchSenders(int numSenders);
    bool WaitForSenders();
    std::string ReadMessage();
    void RunCommandLoop();

private:
    Receiver(const std::string& fileName, MessageFile* messageFile);

    std::string m_fileName;
    MessageFile* m_messageFile;
    int m_numSenders;
};

class ReadySignal {
public:
    static void Create(int numSenders);
    static void WaitAll();
    static void Cleanup();
};

void LaunchSender(const std::string& senderPath, const std::string& fileName, int senderIndex);