// Listenersteler.h
#pragma once
#include <memory>

class Listenersteler {
private:
    // Vaibalen
    std::unique_ptr<int[]> liste;
    int listenLange;
    unsigned int seed = 3507613052;

public:
    // Funktionen
    Listenersteler();
    int *erstelleListe(int lange);

private:
    // Funktionen
};
