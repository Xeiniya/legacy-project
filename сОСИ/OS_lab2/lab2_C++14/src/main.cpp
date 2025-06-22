#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include "Threads.h"
#include "ArrayData.h"
#include <conio.h>

void start_thread(void(*thread)(ArrayData&), ArrayData& data) {
    std::thread t(thread, std::ref(data));
    t.join();
}

int main() {

    int n;
    std::cout << "Enter array size\n";
    std::cin >> n;

    std::vector<int> arr(n);
    std::cout << "Enter array elements\n";
    for (auto& x : arr) {
        std::cin >> x;
    }

    auto data = std::make_unique<ArrayData>(arr);

    start_thread(min_max, *data);
    start_thread(average, *data);

    arr[data->max] = static_cast<int>(data->average);
    arr[data->min] = static_cast<int>(data->average);

    std::cout << "Changed array: ";
    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
