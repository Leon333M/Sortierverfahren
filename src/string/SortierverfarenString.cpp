// SortierverfarenString.cpp
#include "SortierverfarenString.h"
#include "ListenerstelerString.h"

void SortierverfarenString::sortMA(char listeVariante, int lange) {
    ListenerstelerString listenerstelerString;
    std::string *liste;
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    for (int ebene = 1; ebene < (maxEbene + 1); ebene++) {
        liste = listenerstelerString.erstelleListe(listeVariante, lange);
        sortM(liste, lange, ebene);
    }
};

void SortierverfarenString::sortPMA(char listeVariante, int lange, int neueThreadsBisEbene) {
    ListenerstelerString listenerstelerString;
    std::string *liste;
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    for (int ebene = 1; ebene < (maxEbene + 1); ebene++) {
        liste = listenerstelerString.erstelleListe(listeVariante, lange);
        sortPM(liste, lange, neueThreadsBisEbene, ebene);
    }
};
