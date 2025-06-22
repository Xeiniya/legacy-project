#include <iostream>
#include "Threads.h"
using namespace std;

DWORD WINAPI min_max(LPVOID ArrData)
{
    Data* data = static_cast<Data*>(ArrData);

    if (data->arr.empty()) {
        cout << "Empty array, can't find min and max\n";
        return 1;
    }

    data->min = 0;
    data->max = 0;

    for (size_t i = 1; i < data->arr.size(); i++) {
        if (data->arr[i] < data->arr[data->min]) {
            data->min = i; 
        }
        Sleep(7);
        if (data->arr[i] > data->arr[data->max]) {
            data->max = i;
        }
        Sleep(7);  
    }

    cout << "Min index: " << data->min << ", value: " << data->arr[data->min] << '\n';
    cout << "Max index: " << data->max << ", value: " << data->arr[data->max] << '\n';

    return 0;
}


DWORD WINAPI average(LPVOID ArrData)
{
    Data* data = static_cast<Data*>(ArrData);

    if (data->arr.size() == 0) {
        cout << "Empty array, can't find average\n";
        return 1;
    }

    long sum = 0; 
    for (size_t i = 0; i < data->arr.size(); i++) {
        sum += data->arr[i];
        Sleep(12);
    }

    data->average = static_cast<double>(sum) / data->arr.size();

    cout << "Average: " << data->average << '\n';

    return 0;
}
