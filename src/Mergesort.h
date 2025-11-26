// Mergesort.h
#pragma once
#include "Sortierverfaren.h"

template <typename T>

class Mergesort : public Sortierverfaren<T> {
private:
    // Vaibalen

public:
    // Funktionen
    Mergesort();
    void sortG(T *liste, int lange);
    void sortM(T *liste, int lange, int messEbene);
    void sortP(T *liste, int lange, int neueThreadsBisEbene);
    void sortPM(T *liste, int lange, int neueThreadsBisEbene, int messEbene);

private:
    // Funktionen
    static void mergesort(T *liste, const int links, const int rechts);
    static void mergesort(T *liste, const int links, const int rechts, const int aktuelleEbene, const int messEbene);
    static void mergesortM(T *liste, const int links, const int rechts, const int aktuelleEbene);
    static void mergesortP(T *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene);
    static void mergesortP(T *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene, const int messEbene);
    static void mergesortPM(T *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene);
    static void mischen(T *liste, const int links, const int mitte, const int rechts, const int lange);
};
