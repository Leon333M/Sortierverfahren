// Mergesort.cpp
#include "Mergesort.h"
#include <memory>

// #include <iostream>

Mergesort::Mergesort() {};

void Mergesort::sortG(int *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    mergesort(liste, links, rechts);
};

void Mergesort::sortM(int *liste, int lange, int messEbene) {
    int links = 0;
    int rechts = lange - 1;
    mergesort(liste, links, rechts, 1, messEbene);
};

void Mergesort::sortP(int *liste, int lange, int anzahlThreads) {
    int neueThreadsBisEbene = static_cast<int>(std::ceil(std::log2(static_cast<double>(anzahlThreads) + 1.0)));
};

void Mergesort::mergesort(int *liste, const int links, const int rechts) {
    int lange = rechts + 1 - links;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        mergesort(liste, links, mitte);
        mergesort(liste, mitte + 1, rechts);
        mischen(liste, links, mitte, rechts, lange);
    }
};

void Mergesort::mergesort(int *liste, const int links, const int rechts, const int aktuelleEbene, const int messEbene) {
    if (aktuelleEbene == messEbene) {
        mergesortM(liste, links, rechts, aktuelleEbene + 1);
    } else {
        int lange = rechts + 1 - links;
        if (lange > 1) {
            int mitte = (links + rechts) / 2;
            mergesort(liste, links, mitte, aktuelleEbene + 1, messEbene);
            mergesort(liste, mitte + 1, rechts, aktuelleEbene + 1, messEbene);
            mischen(liste, links, mitte, rechts, lange);
        }
    }
};

void Mergesort::mergesortM(int *liste, const int links, const int rechts, const int aktuelleEbene) {
    Position *pos = new Position();
    pos->start1 = std::chrono::high_resolution_clock::now();
    int lange = rechts + 1 - links;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        pos->start2 = std::chrono::high_resolution_clock::now();
        mergesort(liste, links, mitte);
        mergesort(liste, mitte + 1, rechts);
        pos->ende2 = std::chrono::high_resolution_clock::now();
        mischen(liste, links, mitte, rechts, lange);
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
    addMessDaten(aktuelleEbene, pos);
};

void Mergesort::mergesortP(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene) {
    int lange = rechts + 1 - links;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        mergesort(liste, links, mitte);
        mergesort(liste, mitte + 1, rechts);
        mischen(liste, links, mitte, rechts, lange);
    }
};

void Mergesort::mergesortP(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene, const int messEbene) {
    int lange = rechts + 1 - links;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        mergesort(liste, links, mitte);
        mergesort(liste, mitte + 1, rechts);
        mischen(liste, links, mitte, rechts, lange);
    }
};

void Mergesort::mergesortPM(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene) {
    int lange = rechts + 1 - links;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        mergesort(liste, links, mitte);
        mergesort(liste, mitte + 1, rechts);
        mischen(liste, links, mitte, rechts, lange);
    }
};

void Mergesort::mischen(int *liste, int links, const int mitte, const int rechts, const int lange) {
    int *listeB = new int[lange];

    // Kopiere nach listeB
    for (int i = links; i < mitte + 1; i++) {
        listeB[i - links] = liste[i];
    }
    for (int i = mitte + 1; i < rechts + 1; i++) {
        listeB[lange - 1 + mitte + 1 - i] = liste[i];
    }

    // Sortiere liste
    int i = 0;         // links
    int j = lange - 1; // rechts
    int k = links;     // links
    while (i < j) {
        if (listeB[i] < listeB[j]) {
            liste[k] = listeB[i];
            i++;
        } else {
            liste[k] = listeB[j];
            j--;
        }
        k++;
    }
    liste[rechts] = listeB[i];

    delete[] listeB;
};
