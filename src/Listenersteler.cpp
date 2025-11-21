// Listenersteler.cpp
#include "Listenersteler.h"
#include <algorithm>
#include <random>

Listenersteler::Listenersteler() {};

int *Listenersteler::erstelleListe(int lange) {
    listenLange = lange;
    liste = std::make_unique<int[]>(listenLange);

    // Zufallsgenerator vorbereiten
    // std::random_device rd;
    // std::mt19937 gen(rd());
    std::mt19937 gen(seed);
    // std::uniform_int_distribution<> dis(0, 9);
    std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    std::generate_n(liste.get(), lange, [&]() { return dis(gen); });

    return liste.get();
};
