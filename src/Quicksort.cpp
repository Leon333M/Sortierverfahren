// Quicksort.cpp
#include "Quicksort.h"

#include <iostream>

Quicksort::Quicksort() {};

void Quicksort::sort(int *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    quicksort(liste, links, rechts);
};

void Quicksort::quicksort(int *liste, int links, int rechts) {
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        quicksort(liste, links, ml);
        quicksort(liste, mr, rechts);
    }
};

void Quicksort::partitioniere(int *liste, int links, int rechts, int &ml, int &mr) {
    // for (int i = links; i < rechts + 1; i++) {
    //     std::cout << liste[i] << " ";
    // }
    // std::cout << std::endl;

    int i = links;
    int j = rechts;
    int p = liste[rechts];
    while (i <= j) {
        while (liste[i] < p) {
            i++;
        }
        while (liste[j] > p) {
            j--;
        }
        if (i <= j) {
            this->vertausche(liste, i, j);
            i++;
            j--;
        }
    };
    ml = j;
    mr = i;

    // for (int i = links; i < rechts + 1; i++) {
    //     std::cout << liste[i] << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "pivoelement : " << p << " an stelle : " << rechts << std::endl;
    // std::cout << "ende partitioniere" << std::endl;
};
