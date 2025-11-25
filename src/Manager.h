// Manager.h
#pragma once
#include "Listenersteler.h"

class Manager {
private:
    // Vaibalen
    int lange = 10;
    Listenersteler listenersteler;

public:
    // Funktionen
    Manager(int argc, char *argv[]);

private:
    // Funktionen
    void grundzeiten();
    void parallelzeiten();
    void messeSortierzeiten();
    void messeSortierzeitenP();
};
