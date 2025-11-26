// MiniSort.h
#pragma once
#include <algorithm>
#include <iostream>
#include <string>

template <typename T>
class MiniSort {
public:
    MiniSort() = default;

    void sort(T *arr, int n);
};
