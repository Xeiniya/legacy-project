#include "lab3.h"
using namespace std;

int main()
{
    cout << "Enter size:\n";
    cin >> n;
    mas = vector<int>(n, 0);

    int thkol;
    cout << "Enter threads amount:\n";
    cin >> thkol;
    thrkol = thkol;

    HANDLE* hThreads = new HANDLE[thkol];
    StopEvent = new HANDLE[thkol];
    ResumeEvent = new HANDLE[thkol];
    DeleteEvent = new HANDLE[thkol];
    IDThreads.resize(thkol);
    isThreadWorks.resize(thkol, true);

    InitializeCriticalSection(&cs); // критическая секция для синхронизации
    InitializeCriticalSection(&csout); // критическая секция для вывода

    for (int i = 0; i < thkol; i++)
    {
        StopEvent[i] = CreateEvent(NULL, TRUE, FALSE, NULL); // событие остановки
        ResumeEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL); // событие восстановления
        DeleteEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        hThreads[i] = CreateThread(NULL, 0, marker, reinterpret_cast<LPVOID>(static_cast<intptr_t>(i)), 0, &IDThreads[i]);
    }

    while (thrkol > 0)
    {
        WaitForMultipleObjects(thkol, StopEvent, TRUE, INFINITE); // ждем пока все потоки не вызовут событие остановки

        cout << "Array before: ";
        EnterCriticalSection(&cs);
        for (int i = 0; i < n; i++)
        {
            cout << mas[i] << " ";
        }
        LeaveCriticalSection(&cs);

        int thrId;
        cout << endl << "Enter number of thread to stop (1-" << thkol << "): ";
        cin >> thrId;
        thrId--; 

        if (thrId >= 0 && thrId < thkol && isThreadWorks[thrId]) {
            SetEvent(DeleteEvent[thrId]); // событие удаления для выбранного потока
            WaitForSingleObject(hThreads[thrId], INFINITE);
            CloseHandle(hThreads[thrId]);

            cout << "Array after: ";
            EnterCriticalSection(&cs);
            for (int i = 0; i < n; i++)
            {
                cout << mas[i] << " ";
            }
            LeaveCriticalSection(&cs);
            cout << endl;

            for (int i = 0; i < thkol; i++)
            {
                if (isThreadWorks[i]) {
                    ResetEvent(StopEvent[i]);
                    SetEvent(ResumeEvent[i]);
                }
            }
        }
        else {
            cout << "Invalid thread number!" << endl;
            for (int i = 0; i < thkol; i++)
            {
                if (isThreadWorks[i]) {
                    ResetEvent(StopEvent[i]);
                    SetEvent(ResumeEvent[i]);
                }
            }
        }

        bool hasWorkingThreads = false;
        for (int i = 0; i < thkol; i++) {
            if (isThreadWorks[i]) {
                hasWorkingThreads = true;
                break;
            }
        }

        if (!hasWorkingThreads) {
            break; 
        }
    }

    DeleteCriticalSection(&cs);
    DeleteCriticalSection(&csout);

    for (int i = 0; i < thkol; i++)
    {
        CloseHandle(StopEvent[i]);
        CloseHandle(ResumeEvent[i]);
        CloseHandle(DeleteEvent[i]);
    }

    delete[] hThreads;
    delete[] StopEvent;
    delete[] ResumeEvent;
    delete[] DeleteEvent;

    return 0;
}