#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "common.h"
#include <array>
#include <string_view>

enum class MessageType : int {
    ReadRequest = 1,
    WriteRequest = 2,
    ReadResponse = 3,
    WriteResponse = 4,
    WriteCommit = 5,
    ReleaseLock = 6,
    Error = 7,
    Success = 8
};

struct Message {
    MessageType type{ MessageType::Error };
    int employee_id{ -1 };
    employee emp_data{};
    int client_pid{ -1 };
    std::array<char, 256> error_msg{};
};

constexpr void initialize_message(Message& msg) noexcept;
void set_error_message(Message& msg, std::string_view error) noexcept;

#endif // PROTOCOL_H