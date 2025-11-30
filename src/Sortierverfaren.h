// Sortierverfaren.h
#pragma once
#include "Messdaten.h"

class Sortierverfaren {
protected:
    // Vaibalen

public:
    // Funktionen
    Sortierverfaren() {};
    void sortG(int *liste, int lange);
    virtual void sortM(int *liste, int lange, int messEbene) = 0;
    void sortMA(char listeVariante, int lange);
    void sortP(int *liste, int lange, int neueThreadsBisEbene);
    virtual void sortPM(int *liste, int lange, int neueThreadsBisEbene, int messEbene) = 0;
    void sortPMA(char listeVariante, int lange, int neueThreadsBisEbene);

protected:
    // Funktionen
};
