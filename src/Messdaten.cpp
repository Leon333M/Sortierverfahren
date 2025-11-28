// Messdaten.cpp
#include "Messdaten.h"

// Definition der statischen Member
std::vector<std::vector<std::unique_ptr<Messdaten>>> Messdaten::messDaten;
std::mutex Messdaten::mutex;