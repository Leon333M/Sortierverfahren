// Dateimanager.h
#pragma once
#include "Messdaten.h"
#include <algorithm>
#include <functional>
#include <string>
#include <vector>

struct Statistik {
    int median = 0;
    int durchschnitt = 0;
    int standardabweichung = 0;
    int minimum = 0;
    int maximum = 0;
};

struct MessdatenStatistik {
    int mdAnzahl;
    Statistik dauer1;
    Statistik dauer2;
    Statistik dauer1m2;
    MessdatenStatistik(const std::vector<std::unique_ptr<Messdaten>> &md);
};

class Dateimanager {

private:
    // Vaibalen
    std::string originalPath = "E:/Code/Sortierverfahren/Messdaten/";
    std::string dateiendung = ".txt";

public:
    // Funktionen
    Dateimanager() {};
    void exportMessData(int threadAnzahl);

    static Statistik berechneDauerStatistik1(const std::vector<std::unique_ptr<Messdaten>> &md);
    static Statistik berechneDauerStatistik2(const std::vector<std::unique_ptr<Messdaten>> &md);
    static Statistik berechneDauerStatistik1m2(const std::vector<std::unique_ptr<Messdaten>> &md);

private:
    // Funktionen
    static Statistik berechneStatistik(std::vector<long long> &werte);
    static Statistik berechneDauerStatistik(
        const std::vector<std::unique_ptr<Messdaten>> &md,
        std::function<long long(const Messdaten &)> dauerFunktion);
    static void schreibeMessdatenInDatei(std::ostream &outFile, const std::vector<MessdatenStatistik> &mdsv);
    static void schreibeStatistik(std::ostream &outFile, const std::string &name, const Statistik &s);
};
