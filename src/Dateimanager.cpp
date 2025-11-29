// Dateimanager.cpp
#include "Dateimanager.h"
#include <algorithm>
#include <fstream>
#include <iostream>

MessdatenStatistik::MessdatenStatistik(const std::vector<std::unique_ptr<Messdaten>> &md) {
    mdAnzahl = md.size();
    dauer1 = Dateimanager::berechneDauerStatistik1(md);
    dauer2 = Dateimanager::berechneDauerStatistik2(md);
    dauer1m2 = Dateimanager::berechneDauerStatistik1m2(md);
};

void Dateimanager::exportMessData(std::string sortieralgorithmus, int threadAnzahl) {
    std::string path = originalPath;
    path += sortieralgorithmus;
    path += " " + Messdaten::arrayArt;
    path += " " + Messdaten::arrayTyp;
    path += " " + std::to_string(Messdaten::arrayLange);
    path += " " + std::to_string(threadAnzahl);
    path += dateiendung;

    // werte Messdaten aus
    std::vector<std::vector<std::unique_ptr<Messdaten>>> &messDaten = Messdaten::messDaten;
    std::vector<MessdatenStatistik> mdsv;
    mdsv.reserve(messDaten.size());
    for (int i = 1; i < messDaten.size(); i++) {
        std::vector<std::unique_ptr<Messdaten>> &md = messDaten[i];
        MessdatenStatistik mds(md);
        mdsv.push_back(mds);
    }

    std::ofstream outFile(path);
    if (!outFile) {
        std::cerr << "Fehler beim Offnen der Datei: " << path << std::endl;
        return;
    }
    schreibeMessdatenInDatei(outFile, mdsv);
    // schreibeMessdatenInDatei(std::cout, mdsv);
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
                       if (!m) {
                           return 0LL;
                       }
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

void Dateimanager::schreibeMessdatenInDatei(std::ostream &outFile, const std::vector<MessdatenStatistik> &mdsv) {

    for (size_t i = 0; i < mdsv.size(); ++i) {
        const auto &mds = mdsv[i];
        outFile << "Messebene: " << i + 1 << std::endl;
        outFile << "Anzahl Messungen: " << mds.mdAnzahl << std::endl;
        outFile << std::endl;

        schreibeStatistik(outFile, "Dauer1", mds.dauer1);
        schreibeStatistik(outFile, "Dauer2", mds.dauer2);
        schreibeStatistik(outFile, "Dauer1-2", mds.dauer1m2);
        outFile << std::endl;
    }
}

void Dateimanager::schreibeStatistik(std::ostream &outFile, const std::string &name, const Statistik &s) {
    outFile << name << ":" << std::endl
            << "Median: " << s.median << std::endl
            << "Durchschnitt: " << s.durchschnitt << std::endl
            << "StdAbw: " << s.standardabweichung << std::endl
            << "Min: " << s.minimum << std::endl
            << "Max: " << s.maximum << std::endl
            << std::endl;
}
