#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "common.h"
#include <string>
#include <vector>
#include <filesystem>

class FileManager {
private:
    std::filesystem::path filename;

public:
    explicit FileManager(const std::string& fname);
    ~FileManager() = default;

    bool create_file(const std::vector<employee>& employees);
    bool read_employee(int id, employee& emp) const;
    bool write_employee(const employee& emp);
    void display_file() const;
    bool file_exists() const noexcept;

private:
    std::optional<long> find_employee_position(int id) const;
};

#endif // FILE_MANAGER_H