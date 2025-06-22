#include <gtest/gtest.h>
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Structure.h"

bool fileExists(const std::string& path) {
    std::ifstream file(path);
    return file.good();
}

std::string getExecutablePath(const std::string& exeName) {
    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    std::string currentDir(buffer);

    std::string exePath = currentDir + "\\" + exeName + "\\Debug\\" + exeName + ".exe";

    std::cout << "Looking for " << exeName << " at: " << exePath << std::endl;
    std::cout << "File exists: " << (fileExists(exePath) ? "Yes" : "No") << std::endl;

    return exePath;
}

bool runProcess(const std::string& exePath, const std::string& params) {
    if (!fileExists(exePath)) {
        std::cerr << "Executable not found at: " << exePath << std::endl;
        return false;
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::string command = "\"" + exePath + "\" " + params;
    char* cmdLine = new char[command.length() + 1];
    strcpy_s(cmdLine, command.length() + 1, command.c_str());

    std::cout << "Running command: " << command << std::endl;

    bool result = false;
    if (CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        DWORD exitCode;
        GetExitCodeProcess(pi.hProcess, &exitCode);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        std::cout << "Process completed with exit code: " << exitCode << std::endl;
        result = (exitCode == 0);
    }
    else {
        DWORD error = GetLastError();
        std::cerr << "Error launching: " << exePath << " (Error code: " << error << ")\n";

        char errorMsg[256];
        FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            errorMsg,
            sizeof(errorMsg),
            NULL
        );
        std::cerr << "Error message: " << errorMsg << std::endl;
    }

    delete[] cmdLine;
    return result;
}

// Test case for the main function
TEST(MainTest, ProcessCreation) {
    char currentDirBuffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, currentDirBuffer);
    std::string currentDir(currentDirBuffer);
    std::cout << "Current working directory: " << currentDir << std::endl;

    std::string binname = "test_integration.bin";
    int record = 1;
    std::string tname = "test_report";
    double price = 50.0;

    std::ofstream binFile(binname, std::ios::binary);
    ASSERT_TRUE(binFile.is_open()) << "Failed to create test binary file";

    employee e = { 1, "TestEmp", 40.0 };
    binFile.write(reinterpret_cast<char*>(&e), sizeof(e));
    binFile.close();

    std::cout << "Created test binary file: " << binname << std::endl;
    ASSERT_TRUE(fileExists(binname)) << "Binary file was not created successfully";

    std::string reporterPath = getExecutablePath("Reporter");
    if (!fileExists(reporterPath)) {
        reporterPath = "D:\\ËÀÁÛ\\Ëàáû_îñè\\Laba1\\build\\Reporter\\Debug\\Reporter.exe";
        std::cout << "Trying hard-coded path: " << reporterPath << std::endl;
        std::cout << "File exists: " << (fileExists(reporterPath) ? "Yes" : "No") << std::endl;
    }

    ASSERT_TRUE(fileExists(reporterPath)) << "Reporter executable not found";

    std::string reporterParams = binname + " " + tname + " " + std::to_string(price);
    ASSERT_TRUE(runProcess(reporterPath, reporterParams)) << "Failed to run Reporter.exe";

    std::string reportFilename = tname + ".txt";
    std::ifstream reportFile(reportFilename);
    ASSERT_TRUE(reportFile.good()) << "Report file was not created";

    if (reportFile.good()) {
        std::cout << "Report file contents:" << std::endl;
        std::string line;
        while (std::getline(reportFile, line)) {
            std::cout << line << std::endl;
        }
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}