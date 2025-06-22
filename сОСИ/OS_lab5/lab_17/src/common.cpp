#include "../include/common.h"
#include <sstream>
#include <algorithm>
#include <cctype>

std::string employee_to_string(const employee& emp) {
    std::ostringstream oss;
    oss << "ID: " << emp.num
        << ", Name: " << emp.name.data()
        << ", Hours: " << emp.hours;
    return oss.str();
}

void trim_string(std::string& str) {
    // Trim right
    str.erase(std::find_if(str.rbegin(), str.rend(),
        [](unsigned char ch) { return !std::isspace(ch); }).base(),
        str.end());

    // Trim left
    str.erase(str.begin(),
        std::find_if(str.begin(), str.end(),
            [](unsigned char ch) { return !std::isspace(ch); }));
}

bool is_valid_employee_id(int id) noexcept {
    return id > 0 && id <= MAX_EMPLOYEES;
}