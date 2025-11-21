// Sortierverfaren.h
#pragma once
#include <chrono>

class Sortierverfaren {
protected:
    // Vaibalen

public:
    // Funktionen
    Sortierverfaren() {};
    void sort(int *liste, int lange);
    void sortParallel(int *liste, int lange);

protected:
    // Funktionen
};
