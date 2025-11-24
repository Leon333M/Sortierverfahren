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
    void sortM(int *liste, int lange, int messEbene);

private:
    // Funktionen
    static void mergesort(int *liste, const int links, const int rechts);
    static void mergesort(int *liste, const int links, const int rechts, const int aktuelleEbene, const int messEbene);
    static void mergesortM(int *liste, const int links, const int rechts, const int aktuelleEbene);
    static void mischen(int *liste, const int links, const int mitte, const int rechts, const int lange);
};
