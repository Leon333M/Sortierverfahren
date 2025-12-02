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
    void sortP(int *liste, int lange, int neueThreadsBisEbene);
    void sortPM(int *liste, int lange, int neueThreadsBisEbene, int messEbene);

private:
    // Funktionen
    static void mergesort(int *liste, const int links, const int rechts);
    static void mergesort(int *liste, const int links, const int rechts, const int aktuelleEbene, const int messEbene);
    static void mergesortM(int *liste, const int links, const int rechts, const int aktuelleEbene);
    static void mergesortP(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene);
    static void mergesortP(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene, const int messEbene);
    static void mergesortPM(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene);
    static void mergesortW(int *liste, int links, int rechts, int workerThreads);
    static void mischen(int *liste, const int links, const int mitte, const int rechts, const int lange);
};
