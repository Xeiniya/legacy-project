#include "Threads.h"
#include <thread>
#include <chrono>

void min_max(ArrayData& data) {
    if (data.arr.empty()) {
        std::cout << "Empty array, can't find min and max\n";
        return;
    }

    size_t min_index = 0, max_index = 0;

    for (size_t i = 1; i < data.arr.size(); ++i) {
        if (data.arr[i] < data.arr[min_index]) min_index = i;
        if (data.arr[i] > data.arr[max_index]) max_index = i;
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }

    data.min = min_index;
    data.max = max_index;

    std::cout << "Min: " << data.arr[min_index] << " (index " << min_index << ")\n";
    std::cout << "Max: " << data.arr[max_index] << " (index " << max_index << ")\n";
}


void average(ArrayData& data) {
    if (data.arr.empty()) {
        std::cout << "Empty array, can't find average\n";
        return;
    }

    long long sum = 0;
    for (const auto& x : data.arr) {
        sum += x;
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }

    data.average = static_cast<double>(sum) / data.arr.size();
    std::cout << "Average: " << data.average << '\n';
}
