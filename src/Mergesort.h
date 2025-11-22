// Mergesort.h
#pragma once
#include "Sortierverfaren.h"

class Mergesort : public Sortierverfaren {
private:
    // Vaibalen

public:
    // Funktionen
    Mergesort();
    void sort(int *liste, int lange);

private:
    // Funktionen
    void mergesort(int *liste, const int links, const int rechts, Position *pos);
    void mischen(int *liste, const int links, const int mitte, const int rechts, const int lange);
};
