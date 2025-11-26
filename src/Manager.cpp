// Manager.cpp
#include "Manager.h"
#include "Mergesort.h"
#include "Quicksort.h"
#include <chrono>
#include <iostream>
#include <string>

Manager::Manager(int argc, char *argv[]) {
    char variante = 'a';
    if (argc >= 2) {
        lange = std::stoi(argv[1]);
    }
    if (argc >= 3) {
        variante = argv[2][0];
    }

    if (variante == 'g' || variante == 'a') {
        grundzeiten();
    }
    if (variante == 'm' /*|| variante == 'a'*/) {
        messeSortierzeiten();
    }
    if (variante == 'p' || variante == 'a') {
        parallelzeiten();
    }
    if (variante == 'o' /*|| variante == 'a'*/) {
        messeSortierzeitenP();
    }

    // Ende
    std::cout << "Ende" << std::endl;
};

void Manager::grundzeiten() {

    // Mergesort
    int *liste = listenersteler.erstelleListe(lange);
    Mergesort mergesort;
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    mergesort.sortG(liste, lange);
    std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
    long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << " Mergesort" << std::endl;

    // Quicksort
    liste = listenersteler.erstelleListe(lange);
    Quicksort quicksort;
    start = std::chrono::high_resolution_clock::now();
    quicksort.sortG(liste, lange);
    stop = std::chrono::high_resolution_clock::now();
    dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << " Quicksort" << std::endl;
};

void Manager::messeSortierzeiten() {
    // init
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    std::cout << "maxEbene : " << maxEbene << std::endl;
    Sortierverfaren::initMessDaten(lange);
    int *liste = listenersteler.erstelleListe(lange);

    // Mergesort
    Mergesort mergesort;
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    mergesort.sortMA(lange);
    std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
    long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << " Mergesort" << std::endl;

    // gebe Speicher frei da 10Gb bei Aray von 40'000'000
    Sortierverfaren::resetMessDaten();
    Sortierverfaren::initMessDaten(lange);
    liste = listenersteler.erstelleListe(lange);

    // Quicksort
    Quicksort quicksort;
    start = std::chrono::high_resolution_clock::now();
    quicksort.sortMA(lange);
    stop = std::chrono::high_resolution_clock::now();
    dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << " Quicksort" << std::endl;
};

void Manager::parallelzeiten() {
    // init
    int t[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 1024, 4096};
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    std::cout << "maxEbene : " << maxEbene << std::endl;

    // Mergesort
    std::cout << "Mergesort :" << std::endl;
    for (int i : t) {
        Mergesort mergesort;
        int *liste = listenersteler.erstelleListe(lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        mergesort.sortP(liste, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        std::cout << "Laufzeit: " << dauer << " ms" << " threads : " << i << std::endl;
    }

    // Quicksort
    std::cout << "Quicksort :" << std::endl;
    for (int i : t) {
        Quicksort quicksort;
        int *liste = listenersteler.erstelleListe(lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        quicksort.sortP(liste, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        std::cout << "Laufzeit: " << dauer << " ms" << " threads : " << i << std::endl;
    }
};

void Manager::messeSortierzeitenP() {
    // init
    int t[] = {1, 2, 4, 8, 16, 32, 64, 128, 256};
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    std::cout << "maxEbene : " << maxEbene << std::endl;
    Sortierverfaren::resetMessDaten();
    Sortierverfaren::initMessDaten(lange);

    // Mergesort
    std::cout << "Mergesort :" << std::endl;
    for (int i : t) {
        Mergesort mergesort;
        int *liste = listenersteler.erstelleListe(lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        mergesort.sortPMA(lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        std::cout << "Laufzeit: " << dauer << " ms" << " threads : " << i << std::endl;
        // gebe Speicher frei da 10Gb bei Aray von 40'000'000
        Sortierverfaren::resetMessDaten();
        Sortierverfaren::initMessDaten(lange);
    }

    // // gebe Speicher frei da 10Gb bei Aray von 40'000'000
    // Sortierverfaren::resetMessDaten();
    // Sortierverfaren::initMessDaten(lange);
    // liste = listenersteler.erstelleListe(lange);

    // // Quicksort
    // Quicksort quicksort;
    // start = std::chrono::high_resolution_clock::now();
    // quicksort.sortMA(lange);
    // stop = std::chrono::high_resolution_clock::now();
    // dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    // std::cout << "Laufzeit: " << dauer << " ms" << " Quicksort" << std::endl;
};
