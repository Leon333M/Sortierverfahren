// ManagerString.h
#pragma once
#include "Dateimanager.h"
#include "ListenerstelerString.h"

class ManagerString {
private:
    // Vaibalen
    int lange = 10;
    char listeVariante;
    ListenerstelerString listenerstelerString;
    Dateimanager dateimanager;

public:
    // Funktionen
    ManagerString(int argc, char *argv[]);

private:
    // Funktionen
    void grundzeiten();
    void parallelzeiten();
    void workerZeiten();
    void messeSortierzeiten();
    void messeSortierzeitenP();
    void istSortiert() const;
    void messeAlles();
    void messeZeiten();
};
