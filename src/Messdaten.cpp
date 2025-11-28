// Messdaten.cpp
#include "Messdaten.h"

// Definition der statischen Member
int Messdaten::arrayLange;
std::string Messdaten::arrayTyp;
std::string Messdaten::arrayArt;
std::vector<std::vector<std::unique_ptr<Messdaten>>> Messdaten::messDaten;
std::mutex Messdaten::mutex;
