// Mergesort.cpp
#include "Mergesort.h"
#include <memory>

Mergesort::Mergesort() {};

void Mergesort::mergesort(int liste[], int links, int rechts) {
    int lange = rechts - links + 1;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        mergesort(liste, links, mitte);
        mergesort(liste, mitte + 1, rechts);
        mischen(liste, links, mitte, rechts, lange);
    }
};

void Mergesort::mischen(int liste[], int links, int mitte, int rechts, int lange) {
    std::unique_ptr<int[]> listeB = std::make_unique<int[]>(lange);
    // Kopiere nach listeB
    for (int i = links; i < mitte; i++) {
        listeB[i - links] = liste[i];
    }
    for (int i = mitte; i < rechts; i++) {
        listeB[i - mitte] = liste[i];
    }
    listeB[lange - 1] = liste[rechts];

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
};
