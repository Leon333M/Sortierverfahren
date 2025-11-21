// Quicksort.cpp
#include "Quicksort.h"
#include <thread>
#include <vector>

#include <iostream>

Quicksort::Quicksort() {};

void Quicksort::sort(int *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    quicksort(liste, links, rechts);
};

void Quicksort::sortParallel(int *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    quicksortParallel(liste, links, rechts);
};

void Quicksort::quicksort(int *liste, int links, int rechts) {
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        quicksort(liste, links, ml);
        quicksort(liste, mr, rechts);
    }
};

void Quicksort::quicksortParallel(int *liste, int links, int rechts) {
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        std::vector<std::thread> threads = std::vector<std::thread>();
        threads.emplace_back(&Quicksort::quicksortParallel, liste, links, ml);
        threads.emplace_back(&Quicksort::quicksortParallel, liste, mr, rechts);
        for (std::thread &thread : threads) {
            thread.join();
        }
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
            vertausche(liste, i, j);
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

void Quicksort::vertausche(int *liste, int a, int b) {
    int temp = liste[a];
    liste[a] = liste[b];
    liste[b] = temp;
};
