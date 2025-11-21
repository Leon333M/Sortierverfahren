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
    void mergesort(int *liste, int links, int rechts);
    void mischen(int *liste, int links, int mitte, int rechts, int lange);
};
