#include "../src/Threads.h"
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

void test_min_max_basic() {
    vector<int> numbers = { 10, 20, 30, 40, 50 }; 

    Data* data = new Data(numbers);
    min_max(static_cast<void*>(data));
    cout << "Test: Basic min/max\n";
    cout << "Expected min: 10, Expected max: 50\n";
    cout << "Actual min: " << data->arr[data->min] << ", Actual max: " << data->arr[data->max] << "\n";
    assert(data->arr[data->min] == 10 && data->arr[data->max] == 50);
    delete data;
}

void test_min_max_with_negatives() {
    vector<int> numbers = { -10, -20, 15, 30, 5 };

    Data* data = new Data(numbers);
    min_max(static_cast<void*>(data));
    cout << "Test: Min/max with negative numbers\n";
    cout << "Expected min: -20, Expected max: 30\n";
    cout << "Actual min: " << data->arr[data->min] << ", Actual max: " << data->arr[data->max] << "\n";
    assert(data->arr[data->min] == -20 && data->arr[data->max] == 30);
    delete data;
}

void test_min_max_equal_values() {
    vector<int> numbers = { 100, 100, 100, 100, 100 };

    Data* data = new Data(numbers);
    min_max(static_cast<void*>(data));
    cout << "Test: Min/max with equal values\n";
    cout << "Expected min: 100, Expected max: 100\n";
    cout << "Actual min: " << data->arr[data->min] << ", Actual max: " << data->arr[data->max] << "\n";
    assert(data->arr[data->min] == 100 && data->arr[data->max] == 100);
    delete data;
}

void test_average_positive_values() {
    vector<int> numbers = { 5, 10, 15, 20 };

    Data* data = new Data(numbers);
    average(static_cast<void*>(data));
    cout << "Test: Average with positive values\n";
    cout << "Expected average: 12.5\n";
    cout << "Actual average: " << data->average << "\n";
    assert(data->average == 12.5);
    delete data;
}

void test_average_with_negatives_and_zero() {
    vector<int> numbers = { -5, 0, 5 };

    Data* data = new Data(numbers);
    average(static_cast<void*>(data));
    cout << "Test: Average with negative, zero, and positive values\n";
    cout << "Expected average: 0.0\n";
    cout << "Actual average: " << data->average << "\n";
    assert(data->average == 0.0);
    delete data;
}

void test_empty_array() {
    vector<int> numbers;

    Data* data = new Data(numbers);
    DWORD result_min_max = min_max(static_cast<void*>(data));
    DWORD result_avg = average(static_cast<void*>(data));

    cout << "Test: Empty array\n";
    cout << "Expected min/max result: 1, Expected average result: 1\n";
    cout << "Actual min/max result: " << result_min_max << ", Actual average result: " << result_avg << "\n";

    assert(result_min_max == 1 && result_avg == 1);
    delete data;
}

int main() {
    test_min_max_basic();
    test_min_max_with_negatives();
    test_min_max_equal_values();
    test_average_positive_values();
    test_average_with_negatives_and_zero();
    test_empty_array();

    cout << "\nAll tests completed successfully!\n";

    return 0;
}
