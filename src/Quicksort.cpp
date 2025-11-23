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
    Position pos(1);
    pos.start1 = std::chrono::high_resolution_clock::now();
    quicksort(liste, links, rechts, &pos);
    pos.ende2 = std::chrono::high_resolution_clock::now();
};

void Quicksort::sortParallel(int *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    quicksortParallel(liste, links, rechts);
};

void Quicksort::quicksort(int *liste, int links, int rechts, Position *pos) {
    // also ein gesamt erhohte Laufzeit von 80/12
    pos->start1 = std::chrono::high_resolution_clock::now();
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        pos->start2 = std::chrono::high_resolution_clock::now();
        quicksort(liste, links, ml, new Position(pos, 1));
        quicksort(liste, mr, rechts, new Position(pos, 2));
        pos->ende2 = std::chrono::high_resolution_clock::now();
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
};

void Quicksort::quicksortParallel(int *liste, const int links, const int rechts) {
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    int lange = rechts + 1 - links;
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        std::vector<std::thread> threads = std::vector<std::thread>();
        std::chrono::time_point<std::chrono::high_resolution_clock> start1 = std::chrono::high_resolution_clock::now();
        threads.emplace_back(&Quicksort::quicksortParallel, liste, links, ml);
        std::chrono::time_point<std::chrono::high_resolution_clock> start2 = std::chrono::high_resolution_clock::now();
        threads.emplace_back(&Quicksort::quicksortParallel, liste, mr, rechts);
        for (std::thread &thread : threads) {
            thread.join();
        }
    }
    std::chrono::time_point<std::chrono::high_resolution_clock> ende = std::chrono::high_resolution_clock::now();
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
