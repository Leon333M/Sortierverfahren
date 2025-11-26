// Quicksort.cpp
#include "Quicksort.h"
#include <chrono>
#include <thread>
#include <vector>

#include <iostream>

Quicksort::Quicksort() {};

void Quicksort::sortG(int *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    quicksort(liste, links, rechts);
};

void Quicksort::sortM(int *liste, int lange, int messEbene) {
    int links = 0;
    int rechts = lange - 1;
    quicksort(liste, links, rechts, 1, messEbene);
};

void Quicksort::sortP(int *liste, int lange, int anzahlThreads) {
    int links = 0;
    int rechts = lange - 1;
    int neueThreadsBisEbene = static_cast<int>(std::ceil(std::log2(static_cast<double>(anzahlThreads) + 1.0)));
    quicksortP(liste, links, rechts, 1, neueThreadsBisEbene);
};

void Quicksort::quicksort(int *liste, int links, int rechts) {
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        quicksort(liste, links, ml);
        quicksort(liste, mr, rechts);
    }
};

void Quicksort::quicksort(int *liste, int links, int rechts, int aktuelleEbene, int messEbene) {
    if (aktuelleEbene == messEbene) {
        quicksortM(liste, links, rechts, aktuelleEbene);
    } else {
        if (links < rechts) {
            int ml, mr;
            partitioniere(liste, links, rechts, ml, mr);
            quicksort(liste, links, ml, aktuelleEbene + 1, messEbene);
            quicksort(liste, mr, rechts, aktuelleEbene + 1, messEbene);
        }
    }
};

void Quicksort::quicksortM(int *liste, int links, int rechts, int aktuelleEbene) {
    Position *pos = new Position();
    pos->start1 = std::chrono::high_resolution_clock::now();
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        pos->start2 = std::chrono::high_resolution_clock::now();
        quicksort(liste, links, ml);
        quicksort(liste, mr, rechts);
        pos->ende2 = std::chrono::high_resolution_clock::now();
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
    addMessDaten(aktuelleEbene, pos);
};

void Quicksort::quicksortP(int *liste, int links, int rechts, int aktuelleEbene, const int neueThreadsBisEbene) {
    if (aktuelleEbene < neueThreadsBisEbene) {
        if (links < rechts) {
            int ml, mr;
            partitioniere(liste, links, rechts, ml, mr);
            std::vector<std::thread> threads;
            // quicksort(liste, links, ml);
            threads.emplace_back(
                static_cast<void (*)(int *, int, int, int, const int)>(&Quicksort::quicksortP),
                liste, links, ml, aktuelleEbene + 1, neueThreadsBisEbene);
            // quicksort(liste, mr, rechts);
            threads.emplace_back(
                static_cast<void (*)(int *, int, int, int, const int)>(&Quicksort::quicksortP),
                liste, mr, rechts, aktuelleEbene + 1, neueThreadsBisEbene);
            for (std::thread &thread : threads) {
                thread.join();
            }
        }
    } else {
        quicksort(liste, links, rechts);
    }
};

void Quicksort::partitioniere(int *liste, int links, int rechts, int &ml, int &mr) {
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
            vertausche(liste, i, j);
            i++;
            j--;
        }
    };
    ml = j;
    mr = i;
};

void Quicksort::vertausche(int *liste, int a, int b) {
    int temp = liste[a];
    liste[a] = liste[b];
    liste[b] = temp;
};
