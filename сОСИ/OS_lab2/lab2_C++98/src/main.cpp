#include <iostream>
#include <vector>
#include <Windows.h>
#include "Threads.h"
#include "Data.h"
#include <conio.h>
using namespace std;

void start(DWORD(*thread)(LPVOID), void* data) {
    HANDLE Thandle;

    Thandle = CreateThread(NULL, 0, thread, data, 0, NULL);

    if (Thandle == NULL) {
        cout << GetLastError();
        return;
    }

//    WaitForSingleObject(Thandle, INFINITE);
//    CloseHandle(Thandle);
}

int main() {

    vector<int> arr;
    int n;

    cout << "Enter array size\n";
    cin >> n;

    arr.resize(n);

    cout << "Enter array elements\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    Data* data = new Data(arr);

    start(min_max, (void*)data);
    иии
    start(average, (void*)data);
    ооо

    arr[data->max] = static_cast<int>(data->average);
    arr[data->min] = static_cast<int>(data->average);

    cout << "Changed array: ";
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete data;

    return 0;
}
