// Sortierverfaren.cpp
#include "Sortierverfaren.h"
#include "Listenersteler.h"

void Sortierverfaren::sortMA(char listeVariante, int lange) {
    Listenersteler listenersteler;
    int *liste;
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    for (int ebene = 1; ebene < (maxEbene + 1); ebene++) {
        liste = listenersteler.erstelleListe(listeVariante, lange);
        sortM(liste, lange, ebene);
    }
};

void Sortierverfaren::sortPMA(char listeVariante, int lange, int neueThreadsBisEbene) {
    Listenersteler listenersteler;
    int *liste;
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    for (int ebene = 1; ebene < (maxEbene + 1); ebene++) {
        liste = listenersteler.erstelleListe(listeVariante, lange);
        sortPM(liste, lange, neueThreadsBisEbene, ebene);
    }
};
