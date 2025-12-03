// Manager.h
#pragma once
#include "Dateimanager.h"
#include "Listenersteler.h"

class Manager {
private:
    // Vaibalen
    int lange = 10;
    char listeVariante;
    Listenersteler listenersteler;
    Dateimanager dateimanager;

public:
    // Funktionen
    Manager(int argc, char *argv[]);

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
    void incArray();
    void incArrayMT(volatile int *liste, long long lange, int threadCount);
    void benchmarkIncThreads(int lange, std::vector<int> &thrads, std::vector<MessWerte> &messWerte, bool messeAlles);
};
