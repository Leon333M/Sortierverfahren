// Sortierverfaren.h
#pragma once
#include <chrono>

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
protected:
    // Vaibalen

public:
    // Funktionen
    Sortierverfaren() {};
    void sort(int *liste, int lange);
    void sortParallel(int *liste, int lange);

protected:
    // Funktionen
};
