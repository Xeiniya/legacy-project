#pragma once
#include <windows.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <random>

std::vector<int> mas;
std::vector<DWORD> IDThreads;
std::vector<bool> isThreadWorks;
HANDLE* StopEvent;
HANDLE* ResumeEvent;
HANDLE* DeleteEvent;
CRITICAL_SECTION cs, csout;
int n = 0;
int thrkol = 0;

DWORD WINAPI marker(LPVOID lpParam) {
    
    intptr_t threadIndex = reinterpret_cast<intptr_t>(lpParam);

    srand((unsigned int)threadIndex);
    int elmarked = 0;

    while (true) {
        int index = rand() % n;
        // Входим в критическую секцию
        EnterCriticalSection(&cs);
        if (mas[index] == 0) {
            Sleep(5);
            mas[index] = (int)threadIndex + 1;
            elmarked++;
            // Выходим из критической секции
            LeaveCriticalSection(&cs);
            Sleep(5);
        }
        else {
            LeaveCriticalSection(&cs);
            EnterCriticalSection(&csout);
            std::cout << "Thread number " << threadIndex + 1
                << "\nNumber of marked elements: " << elmarked
                << " elements.\nCan't mark " << index << std::endl;
            SetEvent(StopEvent[threadIndex]);
            LeaveCriticalSection(&csout);

            while (true) {
                if (WaitForSingleObject(DeleteEvent[threadIndex], 0) != WAIT_TIMEOUT) {
                    isThreadWorks[threadIndex] = false;
                    for (int i = 0; i < n; ++i) {
                        if (mas[i] == threadIndex + 1) {
                            mas[i] = 0;
                        }
                    }
                    thrkol--;
                    return 0;
                }
                else if (WaitForSingleObject(ResumeEvent[threadIndex], 0) != WAIT_TIMEOUT) {
                    ResetEvent(ResumeEvent[threadIndex]); // Сбрасываем событие
                    break;
                }
            }
        }
    }
}