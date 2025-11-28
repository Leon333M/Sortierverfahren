// Listenersteler.cpp
#include "Listenersteler.h"
#include "Messdaten.h"
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

    Messdaten::arrayTyp = "Zufall";
    Messdaten::arrayArt = "int";
    Messdaten::arrayLange = lange;

    return liste.get();
};

int *Listenersteler::erstelleZufallsListe(int lange) {
    listenLange = lange;
    liste = std::make_unique<int[]>(listenLange);

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    std::generate_n(liste.get(), lange, [&]() { return dis(gen); });

    Messdaten::arrayTyp = "Zufall";
    Messdaten::arrayArt = "int";
    Messdaten::arrayLange = lange;

    return liste.get();
}

int *Listenersteler::erstelleSortierteListe(int lange) {
    listenLange = lange;
    liste = std::make_unique<int[]>(listenLange);

    for (int i = 0; i < lange; i++) {
        liste[i] = i;
    }

    Messdaten::arrayTyp = "Sortiert";
    Messdaten::arrayArt = "int";
    Messdaten::arrayLange = lange;

    return liste.get();
}

int *Listenersteler::erstelleInvertierteListe(int lange) {
    listenLange = lange;
    liste = std::make_unique<int[]>(listenLange);

    for (int i = 0; i < lange; i++) {
        liste[i] = lange - i;
    }

    Messdaten::arrayTyp = "InvertSortiert";
    Messdaten::arrayArt = "int";
    Messdaten::arrayLange = lange;

    return liste.get();
}

int *Listenersteler::erstelleFastSortierteListe(int lange, int swaps) {
    listenLange = lange;
    liste = std::make_unique<int[]>(listenLange);

    // Erst sortierte Liste erzeugen
    for (int i = 0; i < lange; i++) {
        liste[i] = i;
    }

    // Reproduzierbare Zufallsvertauschungen
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, lange - 1);

    for (int i = 0; i < swaps; i++) {
        int a = dis(gen);
        int b = dis(gen);
        std::swap(liste[a], liste[b]);
    }

    Messdaten::arrayTyp = "FastSortiert";
    Messdaten::arrayArt = "int";
    Messdaten::arrayLange = lange;

    return liste.get();
}

int *Listenersteler::erstelleDuplizierteListe(int lange, int uniqueValues) {
    listenLange = lange;
    liste = std::make_unique<int[]>(listenLange);

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, uniqueValues - 1);

    std::generate_n(liste.get(), lange, [&]() { return dis(gen); });

    Messdaten::arrayTyp = "Dupliziert";
    Messdaten::arrayArt = "int";
    Messdaten::arrayLange = lange;

    return liste.get();
}

bool Listenersteler::istSortiert() const {
    return std::is_sorted(liste.get(), liste.get() + listenLange);
}
