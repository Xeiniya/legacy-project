#include "../include/access_manager.h"
#include <iostream>

bool AccessManager::can_acquire_read_lock(int employee_id, int client_pid) const {
    auto it = locks.find(employee_id);
    if (it == locks.end()) {
        return true;
    }

    const auto& lock_info = it->second;

    return lock_info.type == Access::LOCK_NONE ||
        lock_info.type == Access::LOCK_READ ||
        lock_info.clients.contains(client_pid);
}

bool AccessManager::can_acquire_write_lock(int employee_id, int client_pid) const {
    auto it = locks.find(employee_id);
    if (it == locks.end()) {
        return true;
    }

    const auto& lock_info = it->second;

    return lock_info.type == Access::LOCK_NONE ||
        (lock_info.type == Access::LOCK_WRITE &&
            lock_info.clients.contains(client_pid));
}

bool AccessManager::acquire_read_lock(int employee_id, int client_pid) {
    if (!can_acquire_read_lock(employee_id, client_pid)) {
        return false;
    }

    auto& lock_info = locks[employee_id];
    lock_info.type = Access::LOCK_READ;
    lock_info.clients.insert(client_pid);

    return true;
}

bool AccessManager::acquire_write_lock(int employee_id, int client_pid) {
    if (!can_acquire_write_lock(employee_id, client_pid)) {
        return false;
    }

    auto& lock_info = locks[employee_id];
    lock_info.type = Access::LOCK_WRITE;
    lock_info.clients.clear();
    lock_info.clients.insert(client_pid);

    return true;
}

void AccessManager::release_lock(int employee_id, int client_pid) {
    auto it = locks.find(employee_id);
    if (it == locks.end()) {
        return;
    }

    auto& lock_info = it->second;
    lock_info.clients.erase(client_pid);

    if (lock_info.clients.empty()) {
        locks.erase(it);
    }
}

void AccessManager::release_all_locks(int client_pid) {
    for (auto it = locks.begin(); it != locks.end(); ) {
        auto& lock_info = it->second;
        lock_info.clients.erase(client_pid);

        if (lock_info.clients.empty()) {
            it = locks.erase(it);
        }
        else {
            ++it;
        }
    }
}

bool AccessManager::has_lock(int employee_id, int client_pid) const {
    auto it = locks.find(employee_id);
    if (it == locks.end()) {
        return false;
    }

    return it->second.clients.contains(client_pid);
}

Access::LockType AccessManager::get_lock_type(int employee_id, int client_pid) const {
    if (!has_lock(employee_id, client_pid)) {
        return Access::LOCK_NONE;
    }

    return locks.at(employee_id).type;
}

void AccessManager::display_locks() const {
    std::cout << "\n=== Current Locks ===" << std::endl;
    for (const auto& [employee_id, lock_info] : locks) {
        std::cout << "Employee " << employee_id << ": ";
        if (lock_info.type == Access::LOCK_READ) {
            std::cout << "READ lock by clients: ";
        }
        else if (lock_info.type == Access::LOCK_WRITE) {
            std::cout << "WRITE lock by client: ";
        }

        for (const auto& client : lock_info.clients) {
            std::cout << client << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "====================" << std::endl;
}