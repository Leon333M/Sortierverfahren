// Sortierverfaren.h
#pragma once
#include <chrono>
#include <mutex>
#include <vector>

struct Position {
    int abschnitt;
    Position *pos1 = nullptr;
    Position *pos2 = nullptr;
    std::chrono::time_point<std::chrono::high_resolution_clock> start1;
    std::chrono::time_point<std::chrono::high_resolution_clock> start2;
    std::chrono::time_point<std::chrono::high_resolution_clock> ende2;
    std::chrono::time_point<std::chrono::high_resolution_clock> ende1;

    // Konstruktor für Startposition
    Position(int neuerabschnitt) : abschnitt(neuerabschnitt) {}

    // Konstruktor, der eine neue Position aus einer bestehenden ableitet
    Position(Position *parent, int neuerAbschnitt) {
        abschnitt = neuerAbschnitt;
        if (neuerAbschnitt > 1) {
            parent->pos1 = this;
        } else {
            parent->pos2 = this;
        }
    }

    // Dekonstruktor
    ~Position() {
        delete pos1;
        delete pos2;
    }
};

class Sortierverfaren {
public:
    // static
    static std::vector<std::vector<std::unique_ptr<Position>>> messDaten;
    static std::mutex mutex; // Sperre für Thread-Sicherheit

    // Funktion zum Hinzufugen von Messdaten (Thread-sicher)
    static void addMessDaten(int ebene, Position *daten) {
        std::lock_guard<std::mutex> lock(mutex);

        if (ebene >= messDaten.size()) {
            messDaten.resize(ebene + 1);
        }

        messDaten[ebene].emplace_back(daten);
    }

protected:
    // Vaibalen

public:
    // Funktionen
    Sortierverfaren() {};
    void sortG(int *liste, int lange);
    void sortO(int *liste, int lange, Position *pos);
    // void sortParallel(int *liste, int lange);

protected:
    // Funktionen
};
