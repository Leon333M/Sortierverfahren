// Quiksort.h
#pragma once
#include "Sortierverfaren.h"

class Quiksort : public Sortierverfaren {
private:
    // Vaibalen

public:
    // Funktionen
    Quiksort();
    void sort(int *liste, int lange);

private:
    // Funktionen
    void quiksort(int *liste, int links, int rechts);
    void partitioniere(int *liste, int links, int rechts, int &ml, int &mr);
};
