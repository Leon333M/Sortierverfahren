// Quicksort.cpp
#include "Quicksort.h"
#include <chrono>
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
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    quicksortParallel(liste, links, rechts, &start);
};

void Quicksort::quicksort(int *liste, int links, int rechts) {
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        quicksort(liste, links, ml);
        quicksort(liste, mr, rechts);
    }
};

void Quicksort::quicksortParallel(int *liste, const int links, const int rechts, std::chrono::time_point<std::chrono::high_resolution_clock> *zeitpunkt) {
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    int lange = rechts + 1 - links;
    long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(start - *zeitpunkt).count();
    std::cout << "lange : " << lange << " init Zeit : " << dauer << " ms" << std::endl;
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        std::vector<std::thread> threads = std::vector<std::thread>();
        std::chrono::time_point<std::chrono::high_resolution_clock> start1 = std::chrono::high_resolution_clock::now();
        threads.emplace_back(&Quicksort::quicksortParallel, liste, links, ml, &start1);
        std::chrono::time_point<std::chrono::high_resolution_clock> start2 = std::chrono::high_resolution_clock::now();
        threads.emplace_back(&Quicksort::quicksortParallel, liste, mr, rechts, &start2);
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
