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
    void sortParallel(int *liste, int lange);

    // public Static

private:
    // Funktionen
    static void quicksort(int *liste, int links, int rechts, Position *pos);
    static void quicksortParallel(int *liste, const int links, const int rechts);
    static void partitioniere(int *liste, int links, int rechts, int &ml, int &mr);
    static void vertausche(int *liste, int a, int b);
};
