#ifndef NAMED_PIPE_H
#define NAMED_PIPE_H

#include "protocol.h"
#include <string>
#include <windows.h>
#include <string_view>

class NamedPipe {
private:
    std::string pipe_name;
    HANDLE hPipe{ INVALID_HANDLE_VALUE };
    bool is_server;

public:
    explicit NamedPipe(std::string_view name, bool server = false);
    ~NamedPipe();

    NamedPipe(const NamedPipe&) = delete;
    NamedPipe& operator=(const NamedPipe&) = delete;

    bool create();
    bool open();
    void close() noexcept;

    bool send_message(const Message& msg);
    bool receive_message(Message& msg);

    [[nodiscard]] bool is_open() const noexcept;
};

#endif // NAMED_PIPE_H