// Dateimanager.cpp
#include "Dateimanager.h"
#include <fstream>

void Dateimanager::exportMessData() {
    std::ofstream myConfigurationFile(path);

    // myConfigurationFile << i << ":" << getSliderValue(static_cast<SliderId::ids>(i)) << std::endl;

    myConfigurationFile.close();
}
