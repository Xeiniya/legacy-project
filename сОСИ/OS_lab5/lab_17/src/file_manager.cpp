#include "../include/file_manager.h"
#include <fstream>
#include <iostream>
#include <optional>

FileManager::FileManager(const std::string& fname) : filename(fname) {
}

bool FileManager::create_file(const std::vector<employee>& employees) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot create file " << filename << '\n';
        return false;
    }

    for (const auto& emp : employees) {
        file.write(reinterpret_cast<const char*>(&emp), sizeof(employee));
    }

    return file.good();
}

bool FileManager::read_employee(int id, employee& emp) const {
    auto pos = find_employee_position(id);
    if (!pos.has_value()) {
        return false;
    }

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return false;
    }

    file.seekg(pos.value());
    file.read(reinterpret_cast<char*>(&emp), sizeof(employee));

    return file.good() && emp.num == id;
}

bool FileManager::write_employee(const employee& emp) {
    auto pos = find_employee_position(emp.num);
    if (!pos.has_value()) {
        return false;
    }

    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        return false;
    }

    file.seekp(pos.value());
    file.write(reinterpret_cast<const char*>(&emp), sizeof(employee));

    return file.good();
}

void FileManager::display_file() const {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Cannot open file " << filename << '\n';
        return;
    }

    employee emp;
    std::cout << "\n=== Employee Database ===\n";
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        std::cout << employee_to_string(emp) << '\n';
    }
    std::cout << "=========================\n";
}

bool FileManager::file_exists() const noexcept {
    return std::filesystem::exists(filename);
}

std::optional<long> FileManager::find_employee_position(int id) const {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return std::nullopt;
    }

    employee emp;
    long pos = 0;

    while (file.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        if (emp.num == id) {
            return pos;
        }
        pos += sizeof(employee);
    }

    return std::nullopt;
}