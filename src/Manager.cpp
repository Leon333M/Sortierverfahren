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
        char variante = argv[2][0];
    }

    if (variante == 'g' || variante == 'a') {
        grundzeiten();
    }
    if (variante == 'o' || variante == 'a') {
        overheadzeiten();
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
    std::cout << "Laufzeit: " << dauer << " ms" << std::endl;

    // Quicksort
    liste = listenersteler.erstelleListe(lange);
    Quicksort quicksort;
    start = std::chrono::high_resolution_clock::now();
    quicksort.sortG(liste, lange);
    stop = std::chrono::high_resolution_clock::now();
    dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << std::endl;
};

void Manager::overheadzeiten() {

    // Mergesort
    Position pos = Position(1);
    int *liste = listenersteler.erstelleListe(lange);
    Mergesort mergesort;
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    mergesort.sortO(liste, lange, &pos);
    std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
    long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << std::endl;

    // gebe Speicher frei da 10Gb bei Aray von 40'000'000
    pos.~Position();

    // Quicksort
    pos = Position(1);
    liste = listenersteler.erstelleListe(lange);
    Quicksort quicksort;
    start = std::chrono::high_resolution_clock::now();
    quicksort.sortO(liste, lange, &pos);
    stop = std::chrono::high_resolution_clock::now();
    dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << std::endl;
};
