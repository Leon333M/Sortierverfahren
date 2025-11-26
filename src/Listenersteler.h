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
    int *erstelleZufallsListe(int lange);
    int *erstelleSortierteListe(int lange);
    int *erstelleInvertierteListe(int lange);
    int *erstelleFastSortierteListe(int lange, int swaps = 10);
    int *erstelleDuplizierteListe(int lange, int uniqueValues = 100);

private:
    // Funktionen
};
