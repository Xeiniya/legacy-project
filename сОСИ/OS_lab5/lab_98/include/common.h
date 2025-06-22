#ifndef COMMON_H
#define COMMON_H

#include <string>

struct employee {
    int num;           // идентификационный номер сотрудника
    char name[10];     // имя сотрудника
    double hours;      // количество отработанных часов
};

const char* const PIPE_NAME = "employee_pipe";  
const int MAX_EMPLOYEES = 1000;

std::string employee_to_string(const employee& emp);
void trim_string(char* str);
bool is_valid_employee_id(int id);

#endif // COMMON_H