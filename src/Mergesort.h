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
    static void mergesort(int *liste, const int links, const int rechts);
    static void mergesort(int *liste, const int links, const int rechts, Position *pos);
    static void mischen(int *liste, const int links, const int mitte, const int rechts, const int lange);
};
