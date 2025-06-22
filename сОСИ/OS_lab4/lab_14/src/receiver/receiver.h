#pragma once

#include <string>
#include <vector>
#include <memory>

class MessageFile;

class Receiver {
public:
    static std::shared_ptr<Receiver> Create(const std::string& fileName, size_t numRecords);
    bool LaunchSenders(int numSenders);
    bool WaitForSenders();
    std::string ReadMessage();
    void RunCommandLoop();

private:
    Receiver(const std::string& fileName, std::shared_ptr<MessageFile> messageFile);

    std::string m_fileName;
    std::shared_ptr<MessageFile> m_messageFile;
    int m_numSenders;
};

class ReadySignal {
public:
    static void Create(int numSenders);
    static void WaitAll();
    static void Cleanup();
};
void LaunchSender(const std::string& senderPath, const std::string& fileName, int senderIndex);