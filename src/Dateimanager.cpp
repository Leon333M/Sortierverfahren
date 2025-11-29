// Dateimanager.cpp
#include "Dateimanager.h"
#include <algorithm>
#include <fstream>

MessdatenStatistik::MessdatenStatistik(const std::vector<std::unique_ptr<Messdaten>> &md) {
    mdAnzahl = md.size();
    dauer1 = Dateimanager::berechneDauerStatistik1(md);
    dauer2 = Dateimanager::berechneDauerStatistik2(md);
    dauer1m2 = Dateimanager::berechneDauerStatistik1m2(md);
};

void Dateimanager::exportMessData(int threadAnzahl) {
    std::string path = originalPath;
    path += " " + Messdaten::arrayArt;
    path += " " + Messdaten::arrayTyp;
    path += " " + Messdaten::arrayLange;
    path += " " + threadAnzahl;

    // werte Messdaten aus
    std::vector<std::vector<std::unique_ptr<Messdaten>>> &messDaten = Messdaten::messDaten;
    std::vector<MessdatenStatistik> mdsv;
    mdsv.reserve(messDaten.size());
    for (int i = 1; i < messDaten.size(); i++) {
        std::vector<std::unique_ptr<Messdaten>> &md = messDaten[i];
        MessdatenStatistik mds(md);
        mdsv.push_back(mds);
    }

    // std::ofstream myConfigurationFile(path);
    // myConfigurationFile << i << ":" << getSliderValue(static_cast<SliderId::ids>(i)) << std::endl;
    // myConfigurationFile.close();
}

// private Funktionen

Statistik Dateimanager::berechneStatistik(std::vector<long long> &werte) {
    Statistik s;

    if (werte.empty()) {
        return s; // alles bleibt 0
    }

    // Min & Max
    auto [minIt, maxIt] = std::minmax_element(werte.begin(), werte.end());
    s.minimum = static_cast<int>(*minIt);
    s.maximum = static_cast<int>(*maxIt);

    // Durchschnitt
    s.durchschnitt = Messdaten::berechneDurchschnitt(werte);

    // Median (sortiert werte)
    s.median = Messdaten::berechneMedian(werte);

    // Standardabweichung
    s.standardabweichung = Messdaten::berechneStandardabweichung(werte);

    return s;
}

Statistik Dateimanager::berechneDauerStatistik(
    const std::vector<std::unique_ptr<Messdaten>> &md,
    std::function<long long(const Messdaten &)> dauerFunktion) {
    std::vector<long long> werte(md.size());

    std::transform(md.begin(), md.end(), werte.begin(),
                   [&](const std::unique_ptr<Messdaten> &m) {
                       return dauerFunktion(*m);
                   });

    return berechneStatistik(werte);
}

Statistik Dateimanager::berechneDauerStatistik1(const std::vector<std::unique_ptr<Messdaten>> &md) {
    return berechneDauerStatistik(md, [](const Messdaten &m) { return m.dauer1(); });
}

Statistik Dateimanager::berechneDauerStatistik2(const std::vector<std::unique_ptr<Messdaten>> &md) {
    return berechneDauerStatistik(md, [](const Messdaten &m) { return m.dauer2(); });
}

Statistik Dateimanager::berechneDauerStatistik1m2(const std::vector<std::unique_ptr<Messdaten>> &md) {
    return berechneDauerStatistik(md, [](const Messdaten &m) { return m.dauer1m2(); });
}
