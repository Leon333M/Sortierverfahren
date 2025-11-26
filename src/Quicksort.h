// Quicksort.h
#pragma once
#include "Sortierverfaren.h"

template <typename T>

class Quicksort : public Sortierverfaren<T> {
private:
    // Vaibalen

public:
    // Funktionen
    Quicksort();
    void sortG(T *liste, int lange);
    void sortM(T *liste, int lange, int messEbene);
    void sortP(T *liste, int lange, int neueThreadsBisEbene);
    void sortPM(T *liste, int lange, int neueThreadsBisEbene, int messEbene);

    // public Static

private:
    // Funktionen
    static void quicksort(T *liste, int links, int rechts);
    static void quicksort(T *liste, int links, int rechts, int aktuelleEbene, int messEbene);
    static void quicksortM(T *liste, int links, int rechts, int aktuelleEbene);
    static void quicksortP(T *liste, int links, int rechts, int aktuelleEbene, const int neueThreadsBisEbene);
    static void quicksortP(T *liste, int links, int rechts, int aktuelleEbene, const int neueThreadsBisEbene, int messEbene);
    static void quicksortPM(T *liste, int links, int rechts, int aktuelleEbene, const int neueThreadsBisEbene);
    static void partitioniere(T *liste, int links, int rechts, int &ml, int &mr);
    static void vertausche(T *liste, int a, int b);
};
