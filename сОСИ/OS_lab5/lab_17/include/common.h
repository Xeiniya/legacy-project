#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <array>
#include <string_view>

struct employee {
    int num;           // ����������������� ����� ����������
    std::array<char, 10> name;  // ��� ���������� (fixed-size array)
    double hours;      // ���������� ������������ �����
};

constexpr std::string_view PIPE_NAME = "employee_pipe";
constexpr int MAX_EMPLOYEES = 1000;

std::string employee_to_string(const employee& emp);
void trim_string(std::string& str);
bool is_valid_employee_id(int id) noexcept;

#endif // COMMON_H