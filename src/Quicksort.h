// Quicksort.h
#pragma once
#include "Sortierverfaren.h"

class Quicksort : public Sortierverfaren {
private:
    // Vaibalen

public:
    // Funktionen
    Quicksort();
    void sort(int *liste, int lange);

private:
    // Funktionen
    void quicksort(int *liste, int links, int rechts);
    void partitioniere(int *liste, int links, int rechts, int &ml, int &mr);
};
