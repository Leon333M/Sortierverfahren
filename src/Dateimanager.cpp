// Dateimanager.cpp
#include "Dateimanager.h"
#include "Messdaten.h"
#include <fstream>

void Dateimanager::exportMessData() {
    std::ofstream myConfigurationFile(path);
    path += " " + Messdaten::arrayArt;
    path += " " + Messdaten::arrayTyp;
    path += " " + Messdaten::arrayLange;

    std::vector<std::vector<std::unique_ptr<Messdaten>>> &messDaten = Messdaten::messDaten;
    for (int i = 1; i < messDaten.size(); i++) {
        std::vector<std::unique_ptr<Messdaten>> &md = messDaten[i];
        int mdAnzahl = md.size();
        int mdMed;
        int mdDurchschnitt;
        int mdStandardabweichung;
    }
    // myConfigurationFile << i << ":" << getSliderValue(static_cast<SliderId::ids>(i)) << std::endl;

    myConfigurationFile.close();
}
