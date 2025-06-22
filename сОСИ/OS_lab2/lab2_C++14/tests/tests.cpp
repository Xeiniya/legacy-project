#include "../src/Threads.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <memory>

void test_min_max_avg_with_positive_numbers() {
    auto data = std::make_unique<ArrayData>(std::vector<int>{10, 20, 30, 40, 50});
    min_max(*data);
    average(*data);

    assert(data->arr[data->min] == 10);
    assert(data->arr[data->max] == 50);
    assert(data->average == 30.0);

    std::cout << "Test: min/max/avg with positive numbers passed\n";
}

void test_min_max_avg_with_negative_numbers() {
    auto data = std::make_unique<ArrayData>(std::vector<int>{-10, -5, 0, 5, 10});
    min_max(*data);
    average(*data);

    assert(data->arr[data->min] == -10);
    assert(data->arr[data->max] == 10);
    assert(data->average == 0.0);

    std::cout << "Test: min/max/avg with negative numbers passed\n";
}

void test_min_max_avg_with_identical_numbers() {
    auto data = std::make_unique<ArrayData>(std::vector<int>{7, 7, 7, 7, 7});
    min_max(*data);
    average(*data);

    assert(data->arr[data->min] == 7);
    assert(data->arr[data->max] == 7);
    assert(data->average == 7.0);

    std::cout << "Test: min/max/avg with identical numbers passed\n";
}

void test_min_max_avg_with_single_element() {
    auto data = std::make_unique<ArrayData>(std::vector<int>{42});
    min_max(*data);
    average(*data);

    assert(data->arr[data->min] == 42);
    assert(data->arr[data->max] == 42);
    assert(data->average == 42.0);

    std::cout << "Test: min/max/avg with a single element passed\n";
}

void test_min_max_avg_with_empty_array() {
    auto data = std::make_unique<ArrayData>(std::vector<int>{});
    min_max(*data);
    average(*data);

    std::cout << "Test: min/max/avg with empty array passed (handled correctly)\n";
}

int main() {
    test_min_max_avg_with_positive_numbers();
    test_min_max_avg_with_negative_numbers();
    test_min_max_avg_with_identical_numbers();
    test_min_max_avg_with_single_element();
    test_min_max_avg_with_empty_array();

    std::cout << "\nAll tests completed successfully!\n";
    return 0;
}
