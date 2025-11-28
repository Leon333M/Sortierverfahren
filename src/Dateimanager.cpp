// Dateimanager.cpp
#include "Dateimanager.h"
#include "Messdaten.h"
#include <fstream>

void Dateimanager::exportMessData() {
    std::ofstream myConfigurationFile(path);
    path += " " + Messdaten::arrayArt;
    path += " " + Messdaten::arrayTyp;
    path += " " + Messdaten::arrayLange;

    // myConfigurationFile << i << ":" << getSliderValue(static_cast<SliderId::ids>(i)) << std::endl;

    myConfigurationFile.close();
}
