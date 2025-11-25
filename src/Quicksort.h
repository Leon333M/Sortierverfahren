// Quicksort.h
#pragma once
#include "Sortierverfaren.h"

class Quicksort : public Sortierverfaren {
private:
    // Vaibalen

public:
    // Funktionen
    Quicksort();
    void sortG(int *liste, int lange);
    void sortM(int *liste, int lange, int messEbene);
    void sortPM(int *liste, int lange, int anzahlThreads, int messEbene) {};

    // public Static

private:
    // Funktionen
    static void quicksort(int *liste, int links, int rechts);
    static void quicksort(int *liste, int links, int rechts, int aktuelleEbene, int messEbene);
    static void quicksortM(int *liste, int links, int rechts, int aktuelleEbene);
    static void partitioniere(int *liste, int links, int rechts, int &ml, int &mr);
    static void vertausche(int *liste, int a, int b);
};
