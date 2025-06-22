

#include <iostream>
#include <windows.h>
#include <string>
#include "Structure.h"

int main() {
    setlocale(LC_ALL, "RUS");
    std::string binname;  // бин файл
    int record;           // кол-во записей
    std::string tname;    // файл отчета
    double price;         // зп/час

    std::cout << "Enter binary file name: ";
    std::cin >> binname;
    std::cout << "Enter number of records: ";
    std::cin >> record;

    // Настройка структур для запуска процессов
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Полный путь к Creator.exe
    std::string creatorPath = "D:\\ЛАБЫ\\Лабы_оси\\Laba1\\out\\build\\x64-debug\\Creator\\Creator.exe";
    std::string creatorCmd = "\"" + creatorPath + "\" " + binname + " " + std::to_string(record);

    // Преобразование std::string в char[] для передачи в CreateProcess
    char creatorCmdCStr[512];
    strcpy_s(creatorCmdCStr, creatorCmd.c_str());

    // Запуск Creator.exe
    if (CreateProcess(NULL, creatorCmdCStr, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE); // Ожидание завершения процесса
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        std::cerr << "Error creation Creator.exe\n";
    }

    std::cout << "Enter report file name: ";
    std::cin >> tname;
    std::cout << "Enter price: ";
    std::cin >> price;

    // Повторная инициализация структур для нового процесса
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Полный путь к Reporter.exe
    std::string reporterPath = "D:\\ЛАБЫ\\Лабы_оси\\Laba1\\out\\build\\x64-debug\\Reporter\\Reporter.exe";
    std::string reporterCmd = "\"" + reporterPath + "\" " + binname + " " + tname + " " + std::to_string(price);

    // Преобразование std::string в char[] для передачи в CreateProcess
    char reporterCmdCStr[512];
    strcpy_s(reporterCmdCStr, reporterCmd.c_str());

    // Запуск Reporter.exe
    if (CreateProcess(NULL, reporterCmdCStr, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE); // Ожидание завершения процесса
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else {
        std::cerr << "Error creation Reporter.exe\n";
    }

    return 0;
}
