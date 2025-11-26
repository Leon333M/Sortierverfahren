// Sortierverfaren.h
#pragma once
#include <chrono>
#include <mutex>
#include <vector>

struct Position {
    std::chrono::time_point<std::chrono::high_resolution_clock> start1;
    std::chrono::time_point<std::chrono::high_resolution_clock> start2;
    std::chrono::time_point<std::chrono::high_resolution_clock> ende2;
    std::chrono::time_point<std::chrono::high_resolution_clock> ende1;

    // Konstruktor für Startposition
    Position() {}
};

class Sortierverfaren {
public:
    // static
    static std::vector<std::vector<std::unique_ptr<Position>>> messDaten;
    static std::mutex mutex; // Sperre fur Thread-Sicherheit

    // Funktion zum Hinzufugen von Messdaten (Thread-sicher)
    static void addMessDaten(int ebene, Position *daten) {
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

    static void initMessDaten(int arrayGroesse);

protected:
    // Vaibalen

public:
    // Funktionen
    Sortierverfaren() {};
    void sortG(int *liste, int lange);
    virtual void sortM(int *liste, int lange, int messEbene) = 0;
    void sortMA(int lange);
    void sortP(int *liste, int lange, int neueThreadsBisEbene);
    virtual void sortPM(int *liste, int lange, int neueThreadsBisEbene, int messEbene) = 0;
    void sortPMA(int lange, int neueThreadsBisEbene);

protected:
    // Funktionen
};
