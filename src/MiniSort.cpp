// MiniSort.cpp
#include "MiniSort.h"

template <typename T>
void MiniSort<T>::sort(T *arr, int n) {
    std::sort(arr, arr + n);
}

// explizite Template-Instanziierungen
template class MiniSort<int>;
template class MiniSort<std::string>;
