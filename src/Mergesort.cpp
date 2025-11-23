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

void Mergesort::sortO(int *liste, int lange, Position *pos) {
    int links = 0;
    int rechts = lange - 1;
    mergesort(liste, links, rechts, pos);
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

void Mergesort::mergesort(int *liste, const int links, const int rechts, Position *pos) {
    // eine erhohte Laufzeit von +200% wegen Position erzeugung und Zeitmessungen
    // Overhead steigt je grosser das Array
    pos->start1 = std::chrono::high_resolution_clock::now();
    int lange = rechts + 1 - links;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        pos->start2 = std::chrono::high_resolution_clock::now();
        mergesort(liste, links, mitte, new Position(pos, 1));
        mergesort(liste, mitte + 1, rechts, new Position(pos, 2));
        pos->ende2 = std::chrono::high_resolution_clock::now();
        mischen(liste, links, mitte, rechts, lange);
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
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
