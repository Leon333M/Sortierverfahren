// Mergesort.cpp
#include "Mergesort.h"
#include <memory>

#include <iostream>

Mergesort::Mergesort() {};

void Mergesort::sort(int *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    mergesort(liste, links, rechts);
};

void Mergesort::mergesort(int *liste, int links, int rechts) {
    int lange = rechts + 1 - links;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        mergesort(liste, links, mitte);
        mergesort(liste, mitte + 1, rechts);
        mischen(liste, links, mitte, rechts, lange);
    }
};

void Mergesort::mischen(int *liste, int links, int mitte, int rechts, int lange) {
    // std::unique_ptr<int[]> listeB = std::make_unique<int[]>(lange);
    int *listeB = new int[lange];
    // Kopiere nach listeB
    for (int i = links; i < mitte; i++) {
        listeB[i - links] = liste[i];
    }
    for (int i = mitte; i < rechts + 1; i++) {
        listeB[lange - 1 - i] = liste[i];
    }

    // listeA abschnitt
    for (int i = links; i < rechts + 1; i++) {
        std::cout << liste[i] << " ";
    }
    std::cout << std::endl;

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

    // listeA abschnitt
    for (int i = links; i < rechts + 1; i++) {
        std::cout << liste[i] << " ";
    }
    std::cout << std::endl;

    // listeB
    for (int i = 0; i < lange; ++i) {
        std::cout << listeB[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "ende mischen" << std::endl;

    // // listeA
    // for (int i = 0; i < 10; ++i) {
    //    std::cout << liste[i] << " ";
    //}
    // std::cout << std::endl;
};
