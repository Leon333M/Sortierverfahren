// ListenerstelerString.cpp
#include "ListenerstelerString.h"
#include <algorithm>
#include <random>

ListenerstelerString::ListenerstelerString() {}

std::string *ListenerstelerString::erstelleListe(char listeVariante, int lange) {
    switch (listeVariante) {
        case 'z':
            return erstelleZufallsListe(lange);
        case 's':
            return erstelleSortierteListe(lange);
        case 'i':
            return erstelleInvertierteListe(lange);
        case 'f':
            return erstelleFastSortierteListe(lange);
        case 'd':
            return erstelleDuplizierteListe(lange);
        default:
            return erstelleZufallsListe(lange);
    }
}

std::string *ListenerstelerString::erstelleZufallsListe(int lange) {
    listenLange = lange;
    liste = std::make_unique<std::string[]>(listenLange);

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, 25); // Buchstaben A-Z

    for (int i = 0; i < lange; i++) {
        int len = 5 + (dis(gen) % 10); // 5–14 Zeichen
        std::string s;
        s.reserve(len);
        for (int j = 0; j < len; j++)
            s.push_back('A' + dis(gen));
        liste[i] = s;
    }

    return liste.get();
}

std::string *ListenerstelerString::erstelleSortierteListe(int lange) {
    listenLange = lange;
    liste = std::make_unique<std::string[]>(listenLange);

    for (int i = 0; i < lange; i++)
        liste[i] = "STR_" + std::to_string(i);

    return liste.get();
}

std::string *ListenerstelerString::erstelleInvertierteListe(int lange) {
    listenLange = lange;
    liste = std::make_unique<std::string[]>(listenLange);

    for (int i = 0; i < lange; i++)
        liste[i] = "STR_" + std::to_string(lange - i);

    return liste.get();
}

std::string *ListenerstelerString::erstelleFastSortierteListe(int lange, int swaps) {
    listenLange = lange;
    liste = std::make_unique<std::string[]>(listenLange);

    for (int i = 0; i < lange; i++)
        liste[i] = "STR_" + std::to_string(i);

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, lange - 1);

    for (int i = 0; i < swaps; i++)
        std::swap(liste[dis(gen)], liste[dis(gen)]);

    return liste.get();
}

std::string *ListenerstelerString::erstelleDuplizierteListe(int lange, int uniqueValues) {
    listenLange = lange;
    liste = std::make_unique<std::string[]>(listenLange);

    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(0, uniqueValues - 1);

    for (int i = 0; i < lange; i++)
        liste[i] = "STR_" + std::to_string(dis(gen));

    return liste.get();
}

bool ListenerstelerString::istSortiert() const {
    return std::is_sorted(liste.get(), liste.get() + listenLange);
}
