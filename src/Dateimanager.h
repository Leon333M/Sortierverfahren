// Dateimanager.h
#pragma once
#include "Messdaten.h"
#include <algorithm>
#include <functional>
#include <string>
#include <vector>

struct Statistik {
    long long median = 0;
    long long durchschnitt = 0;
    long long standardabweichung = 0;
    long long minimum = 0;
    long long maximum = 0;
};

struct MessdatenStatistik {
    long long mdAnzahl;
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
    void exportMessData(std::string sortieralgorithmus, std::string threadAnzahlVariante);
    void writeMesswerteToFile(const std::string &filename, const std::vector<MessWerte> messWerte1, const std::vector<MessWerte> messWerte2, const std::vector<MessWerte> messWerte3, std::vector<MessWerte> messWerte4, std::vector<MessWerte> messWerte5, std::vector<MessWerte> messWerte6);

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
    static void writeBlock(std::ostream &file, const std::vector<MessWerte> &werte);
    static void sortByThreads(std::vector<MessWerte> &mw);
};
