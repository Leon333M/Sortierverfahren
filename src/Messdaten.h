// Messdaten.h
#pragma once
#include <chrono>
#include <mutex>
#include <vector>

struct MessWerte {
    long long threads;
    long long dauer;
    long long lange;
};

class Messdaten {
private:
    // Vaibalen

public:
    // Vaibalen
    std::chrono::time_point<std::chrono::high_resolution_clock> start1;
    std::chrono::time_point<std::chrono::high_resolution_clock> start2;
    std::chrono::time_point<std::chrono::high_resolution_clock> ende2;
    std::chrono::time_point<std::chrono::high_resolution_clock> ende1;

    // static
    static long long arrayLange;
    static std::string arrayTyp; // Zufall, Sortiert, InvertiertSortiert, FastSortiert, Dupliziert
    static std::string arrayArt; // int, std::string
    static std::vector<std::vector<std::unique_ptr<Messdaten>>> messDaten;
    static std::mutex mutex; // Sperre fur Thread-Sicherheit

    // Funktion zum Hinzufugen von Messdaten (Thread-sicher)
    static void addMessDaten(int ebene, Messdaten *daten) {
        std::lock_guard<std::mutex> lock(mutex);

        if (ebene >= messDaten.size()) {
            messDaten.resize(ebene + 1);
        }

        messDaten[ebene].emplace_back(daten);
    }

    static void resetMessDaten() {
        std::lock_guard<std::mutex> lock(mutex);
        messDaten.clear();
    }

    static void initMessDaten(int arrayGroesse) {
        // Berechne maximale Tiefe eines binaren Baums für die gegebene Array-Lange
        long long maxEbene = static_cast<int>(std::ceil(std::log2(arrayGroesse))) + 1;

        Messdaten::messDaten.resize(maxEbene);

        // Berechne ungefahre Anzahl Positionen pro Ebene (z.B. 2^ebene)
        for (long long ebene = 0; ebene < maxEbene; ebene++) {
            long long anzahlPositionen = static_cast<long long>(std::pow(2, ebene - 1));
            Messdaten::messDaten[ebene].reserve(anzahlPositionen);
        }
    };

    static long long berechneDurchschnitt(const std::vector<long long> &werte);
    static long long berechneMedian(std::vector<long long> &werte);
    static long long berechneStandardabweichung(const std::vector<long long> &werte);

    // Funktionen
    Messdaten() {};
    long long dauer1() const;
    long long dauer2() const;
    long long dauer1m2() const;

private:
    // Funktionen
};
