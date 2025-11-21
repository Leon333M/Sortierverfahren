// main.cpp
#include "Listenersteler.h"
#include "Mergesort.h"
#include "Quiksort.h"
#include <iostream>

int main() {
    std::cout << "Hallo Welt!" << std::endl;
    Listenersteler listenersteler;
    int lange = 10;
    int *liste = listenersteler.erstelleListe(lange);
    for (int i = 0; i < lange; ++i) {
        std::cout << liste[i] << " ";
    }
    std::cout << std::endl;

    // Mergesort
    Mergesort mergesort;
    mergesort.sort(liste, lange);
    for (int i = 0; i < lange; ++i) {
        std::cout << liste[i] << " ";
    }
    std::cout << std::endl;

    // Quiksort
    Quiksort quiksort;
    quiksort.sort(liste, lange);
    for (int i = 0; i < lange; ++i) {
        std::cout << liste[i] << " ";
    }
    std::cout << std::endl;

    // Ende
    std::cout << "Ende" << std::endl;
    return 0;
}
