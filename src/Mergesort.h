// Mergesort.h
#pragma once
#include "Sortierverfaren.h"

class Mergesort : public Sortierverfaren {
private:
    // Vaibalen

public:
    // Funktionen
    Mergesort();
    void sortG(int *liste, int lange);
    void sortO(int *liste, int lange, Position *pos);

private:
    // Funktionen
    void mergesort(int *liste, const int links, const int rechts);
    void mergesort(int *liste, const int links, const int rechts, Position *pos);
    void mischen(int *liste, const int links, const int mitte, const int rechts, const int lange);
};
