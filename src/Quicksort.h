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
    void sortP(int *liste, int lange, int neueThreadsBisEbene);
    void sortPM(int *liste, int lange, int neueThreadsBisEbene, int messEbene);
    void sortW(int *liste, int lange, int workerThreads);

    // public Static

private:
    // Funktionen
    static void quicksort(int *liste, const int links, const int rechts);
    static void quicksort(int *liste, const int links, const int rechts, const int aktuelleEbene, const int messEbene);
    static void quicksortM(int *liste, const int links, const int rechts, const int aktuelleEbene);
    static void quicksortP(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene);
    static void quicksortP(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene, const int messEbene);
    static void quicksortPM(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene);
    static void quicksortW(int *liste, const int links, const int rechts, const int workerThreads);
    static void partitioniere(int *liste, const int links, const int rechts, int &ml, int &mr);
    static void vertausche(int *liste, const int a, const int b);
};
