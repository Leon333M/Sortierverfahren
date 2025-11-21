// Quiksort.cpp
#include "Quiksort.h"

Quiksort::Quiksort() {};

void Quiksort::sort(int *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    quiksort(liste, links, rechts);
};

void Quiksort::quiksort(int *liste, int links, int rechts) {
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        quiksort(liste, links, ml);
        quiksort(liste, mr, rechts);
    }
};

void Quiksort::partitioniere(int *liste, int links, int rechts, int &ml, int &mr) {
    int i = links;
    int j = rechts;
    int p = liste[rechts];
    while (i <= j) {
        while (liste[i] <= p) {
            i++;
        }
        while (liste[j] >= p) {
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
};
