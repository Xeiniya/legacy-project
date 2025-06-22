#ifndef ARRAYDATA_H
#define ARRAYDATA_H

#include <vector>

class ArrayData {
public:
    std::vector<int> arr;
    int min = 0;
    int max = 0;
    double average = 0.0;

    explicit ArrayData(const std::vector<int>& _arr) : arr(_arr) {}
};

#endif // ARRAYDATA_H
