// Sortierverfaren.cpp
#include "Sortierverfaren.h"
#include "Listenersteler.h"

// Definition der statischen Member
template <typename T>
std::vector<std::vector<std::unique_ptr<Position>>> Sortierverfaren<T>::messDaten;

template <typename T>
std::mutex Sortierverfaren<T>::mutex;

template <typename T>
void Sortierverfaren<T>::initMessDaten(int arrayGroesse) {
    // Berechne maximale Tiefe eines binaren Baums für die gegebene Array-Lange
    int maxEbene = static_cast<int>(std::ceil(std::log2(arrayGroesse))) + 1;

    Sortierverfaren<T>::messDaten.resize(maxEbene);

    // Berechne ungefahre Anzahl Positionen pro Ebene (z.B. 2^ebene)
    for (int ebene = 0; ebene < maxEbene; ebene++) {
        int anzahlPositionen = static_cast<int>(std::pow(2, ebene - 1));
        Sortierverfaren<T>::messDaten[ebene].resize(anzahlPositionen);
    }
};

template <typename T>
void Sortierverfaren<T>::sortMA(int lange) {
    Listenersteler listenersteler;
    int *liste;
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    for (int ebene = 1; ebene < (maxEbene + 1); ebene++) {
        liste = listenersteler.erstelleListe(lange);
        sortM(liste, lange, ebene);
    }
};

template <typename T>
void Sortierverfaren<T>::sortPMA(int lange, int neueThreadsBisEbene) {
    Listenersteler listenersteler;
    int *liste;
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    for (int ebene = 1; ebene < (maxEbene + 1); ebene++) {
        liste = listenersteler.erstelleListe(lange);
        sortPM(liste, lange, ebene, neueThreadsBisEbene);
    }
};

// explizite Template-Instanziierungen
template class Sortierverfaren<int>;
template class Sortierverfaren<std::string>;
