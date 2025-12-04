// ListenerstelerString.h
#pragma once
#include <memory>
#include <string>

class ListenerstelerString {
private:
    std::unique_ptr<std::string[]> liste;
    int listenLange;
    unsigned int seed = 3507613052;

public:
    ListenerstelerString();
    std::string *erstelleListe(char listeVariante, int lange);
    std::string *erstelleZufallsListe(int lange);
    std::string *erstelleSortierteListe(int lange);
    std::string *erstelleInvertierteListe(int lange);
    std::string *erstelleFastSortierteListe(int lange, int swaps = 10);
    std::string *erstelleDuplizierteListe(int lange, int uniqueValues = 100);
    bool istSortiert() const;
};
