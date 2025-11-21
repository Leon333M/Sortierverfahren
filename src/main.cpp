// main.cpp
#include "Listenersteler.h"
#include "Mergesort.h"
#include "Quicksort.h"
#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    int lange = 10;
    // bei 40000000 merge 9.2s quick 2.9s
    if (argc == 2) {
        lange = std::stoi(argv[1]);
    }
    Listenersteler listenersteler;
    int *liste = listenersteler.erstelleListe(lange);
    if (lange <= 75) {
        for (int i = 0; i < lange; i++) {
            std::cout << liste[i] << " ";
        }
        std::cout << std::endl;
    }

    // Mergesort
    Mergesort mergesort;
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    mergesort.sort(liste, lange);
    std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
    long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << std::endl;
    if (lange <= 75) {
        for (int i = 0; i < lange; i++) {
            std::cout << liste[i] << " ";
        }
        std::cout << std::endl;
    }

    // Quicksort
    liste = listenersteler.erstelleListe(lange);
    Quicksort quicksort;
    start = std::chrono::high_resolution_clock::now();
    quicksort.sort(liste, lange);
    stop = std::chrono::high_resolution_clock::now();
    dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << std::endl;
    if (lange <= 75) {
        for (int i = 0; i < lange; i++) {
            std::cout << liste[i] << " ";
        }
        std::cout << std::endl;
    }

    // QuicksorttParallel
    liste = listenersteler.erstelleListe(lange);
    Quicksort quicksortParallel;
    start = std::chrono::high_resolution_clock::now();
    quicksortParallel.sortParallel(liste, lange);
    stop = std::chrono::high_resolution_clock::now();
    dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << std::endl;
    if (lange <= 75) {
        for (int i = 0; i < lange; i++) {
            std::cout << liste[i] << " ";
        }
        std::cout << std::endl;
    }

    // Ende
    std::cout << "Ende" << std::endl;
    return 0;
}
