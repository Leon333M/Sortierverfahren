// Messdaten.cpp
#include "Messdaten.h"

// Definition der statischen Member
std::string Messdaten::arrayTyp;
std::string Messdaten::arrayArt;
std::vector<std::vector<std::unique_ptr<Messdaten>>> Messdaten::messDaten;
std::mutex Messdaten::mutex;
