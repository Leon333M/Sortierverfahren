// SortierverfarenString.h
#pragma once
#include "Messdaten.h"

class SortierverfarenString {
public:
    static const int mindestLange = 4000;

protected:
    // Vaibalen

public:
    // Funktionen
    SortierverfarenString() {};
    void sortG(std::string *liste, int lange);
    virtual void sortM(std::string *liste, int lange, int messEbene) = 0;
    void sortMA(char listeVariante, int lange);
    void sortP(std::string *liste, int lange, int neueThreadsBisEbene);
    virtual void sortPM(std::string *liste, int lange, int neueThreadsBisEbene, int messEbene) = 0;
    void sortPMA(char listeVariante, int lange, int neueThreadsBisEbene);
    void sortW(std::string *liste, int lange, int workerThreads);

protected:
    // Funktionen
};
