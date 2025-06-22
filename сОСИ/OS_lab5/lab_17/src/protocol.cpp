#include "../include/protocol.h"
#include <algorithm>
#include <cstring>

constexpr void initialize_message(Message& msg) noexcept {
    msg = Message{}; // Value initialization clears all fields
}

void set_error_message(Message& msg, std::string_view error) noexcept {
    msg.type = MessageType::Error;
    const auto copy_len = std::min(error.size(), msg.error_msg.size() - 1);
    std::copy_n(error.data(), copy_len, msg.error_msg.data());
    msg.error_msg[copy_len] = '\0';
}