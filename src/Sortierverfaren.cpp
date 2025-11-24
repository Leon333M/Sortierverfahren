// Sortierverfaren.cpp
#include "Sortierverfaren.h"

// Definition der statischen Member
std::vector<std::vector<std::unique_ptr<Position>>> Sortierverfaren::messDaten;
std::mutex Sortierverfaren::mutex;

void Sortierverfaren::initMessDaten(int arrayGroesse) {
    // Berechne maximale Tiefe eines binären Baums für die gegebene Array-Länge
    int maxEbene = static_cast<int>(std::ceil(std::log2(arrayGroesse))) + 1;

    Sortierverfaren::messDaten.resize(maxEbene);

    // Berechne ungefähre Anzahl Positionen pro Ebene (z.B. 2^ebene)
    for (int ebene = 0; ebene < maxEbene; ++ebene) {
        int anzahlPositionen = static_cast<int>(std::pow(2, ebene));
        Sortierverfaren::messDaten[ebene].resize(anzahlPositionen);
    }
}
