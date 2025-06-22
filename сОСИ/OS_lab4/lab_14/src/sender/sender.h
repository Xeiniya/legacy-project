#pragma once

#include <string>
#include <memory>

class MessageFile;
class Sender {
public:
    static std::shared_ptr<Sender> Open(const std::string& fileName, int senderIndex);
    bool SignalReady();
    bool SendMessage(const std::string& message);
    void RunCommandLoop();

private:
    Sender(const std::string& fileName, int senderIndex, std::shared_ptr<MessageFile> messageFile);

    std::string m_fileName;
    int m_senderIndex;
    std::shared_ptr<MessageFile> m_messageFile;
};

class ReadySignal {
public:
    static void Signal(int senderIndex);
};