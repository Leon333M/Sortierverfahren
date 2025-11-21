// Sortierverfaren.cpp
#include "Sortierverfaren.h"

void Sortierverfaren::vertausche(int *liste, int a, int b) {
    int temp = liste[a];
    liste[a] = liste[b];
    liste[b] = temp;
};
