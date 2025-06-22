#include "../include/named_pipe.h"
#include <iostream>

NamedPipe::NamedPipe(std::string_view name, bool server)
    : pipe_name("\\\\.\\pipe\\" + std::string(name)), is_server(server) {
}

NamedPipe::~NamedPipe() {
    close();
}

bool NamedPipe::create() {
    if (!is_server) {
        return false;
    }

    hPipe = CreateNamedPipeA(
        pipe_name.c_str(),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1,
        sizeof(Message),
        sizeof(Message),
        0,
        nullptr);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "CreateNamedPipe failed, GLE=" << GetLastError() << '\n';
        return false;
    }

    return true;
}

bool NamedPipe::open() {
    if (is_server) {
        return ConnectNamedPipe(hPipe, nullptr) ?
            true : (GetLastError() == ERROR_PIPE_CONNECTED);
    }

    hPipe = CreateFileA(
        pipe_name.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr);

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "CreateFile failed, GLE=" << GetLastError() << '\n';
        return false;
    }

    DWORD mode = PIPE_READMODE_MESSAGE;
    if (!SetNamedPipeHandleState(hPipe, &mode, nullptr, nullptr)) {
        std::cerr << "SetNamedPipeHandleState failed, GLE=" << GetLastError() << '\n';
        close();
        return false;
    }

    return true;
}

void NamedPipe::close() noexcept {
    if (hPipe != INVALID_HANDLE_VALUE) {
        if (is_server) {
            DisconnectNamedPipe(hPipe);
        }
        CloseHandle(hPipe);
        hPipe = INVALID_HANDLE_VALUE;
    }
}

bool NamedPipe::send_message(const Message& msg) {
    if (!is_open()) {
        return false;
    }

    DWORD bytesWritten;
    return WriteFile(
        hPipe,
        &msg,
        sizeof(Message),
        &bytesWritten,
        nullptr) && (bytesWritten == sizeof(Message));
}

bool NamedPipe::receive_message(Message& msg) {
    if (!is_open()) {
        return false;
    }

    DWORD bytesRead;
    return ReadFile(
        hPipe,
        &msg,
        sizeof(Message),
        &bytesRead,
        nullptr) && (bytesRead == sizeof(Message));
}

bool NamedPipe::is_open() const noexcept {
    return hPipe != INVALID_HANDLE_VALUE;
}