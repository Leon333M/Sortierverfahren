// Dateimanager.cpp
#include "Dateimanager.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

MessdatenStatistik::MessdatenStatistik(const std::vector<std::unique_ptr<Messdaten>> &md) {
    mdAnzahl = md.size();
    dauer1 = Dateimanager::berechneDauerStatistik1(md);
    dauer2 = Dateimanager::berechneDauerStatistik2(md);
    dauer1m2 = Dateimanager::berechneDauerStatistik1m2(md);
};

void Dateimanager::exportMessData(std::string sortieralgorithmus, std::string threadAnzahlVariante) {
    std::string path = originalPath;
    path += Messdaten::arrayArt + "/";
    path += std::to_string(Messdaten::arrayLange) + "/";
    path += Messdaten::arrayTyp + "/";
    std::filesystem::create_directories(path);
    path += sortieralgorithmus;
    path += " " + threadAnzahlVariante;
    path += dateiendung;

    // werte Messdaten aus
    std::vector<std::vector<std::unique_ptr<Messdaten>>> &messDaten = Messdaten::messDaten;
    std::vector<MessdatenStatistik> mdsv;
    mdsv.reserve(messDaten.size());
    for (long long i = 1; i < messDaten.size(); i++) {
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
};

void Dateimanager::writeMesswerteToFile(
    const std::string &filename,
    std::vector<MessWerte> messWerte1,
    std::vector<MessWerte> messWerte2,
    std::vector<MessWerte> messWerte3,
    std::vector<MessWerte> messWerte4,
    std::vector<MessWerte> messWerte5,
    std::vector<MessWerte> messWerte6) {
    std::string path = originalPath;
    path += filename;
    path += dateiendung;
    std::ofstream file(path);

    // Sortiere nach Thread anzahl
    sortByThreads(messWerte1);
    sortByThreads(messWerte2);

    if (!file.is_open()) {
        std::cerr << "Fehler: Datei konnte nicht geoeffnet werden!" << std::endl;
    } else {
        file << "Messwerte (Zeitpunkt, an dem Threads erstmals >50% schneller sind):" << std::endl;
        writeBlock(file, messWerte1);
        file << std::endl;
        file << "Messwerte (Messwerte bei maximaler Laenge aus Block 1):" << std::endl;
        writeBlock(file, messWerte2);
        file << std::endl;
        file << "Messwerte (Messwerte bei maximaler int Laenge):" << std::endl;
        writeBlock(file, messWerte3);
        file << std::endl;
        file << "Messwerte (Messwerte bei " << messWerte4[0].lange << " Laenge):" << std::endl;
        writeBlock(file, messWerte4);
        file << std::endl;
        file << "Messwerte und Threads verdoppeln (Messwerte ab " << messWerte5[0].lange << " Laenge):" << std::endl;
        writeBlock(file, messWerte5);
        file << std::endl;
        file << "Messwerte verdoppeln und Threads konstant (Messwerte ab " << messWerte6[0].lange << " Laenge):" << std::endl;
        writeBlock(file, messWerte6);
    }

    file.close();
};

// private Funktionen

Statistik Dateimanager::berechneStatistik(std::vector<long long> &werte) {
    Statistik s;

    if (werte.empty()) {
        return s; // alles bleibt 0
    }

    // Min & Max
    auto [minIt, maxIt] = std::minmax_element(werte.begin(), werte.end());
    s.minimum = static_cast<long long>(*minIt);
    s.maximum = static_cast<long long>(*maxIt);

    // Durchschnitt
    s.durchschnitt = Messdaten::berechneDurchschnitt(werte);

    // Median (sortiert werte)
    s.median = Messdaten::berechneMedian(werte);

    // Standardabweichung
    s.standardabweichung = Messdaten::berechneStandardabweichung(werte);

    return s;
};

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
};

Statistik Dateimanager::berechneDauerStatistik1(const std::vector<std::unique_ptr<Messdaten>> &md) {
    return berechneDauerStatistik(md, [](const Messdaten &m) { return m.dauer1(); });
};

Statistik Dateimanager::berechneDauerStatistik2(const std::vector<std::unique_ptr<Messdaten>> &md) {
    return berechneDauerStatistik(md, [](const Messdaten &m) { return m.dauer2(); });
};

Statistik Dateimanager::berechneDauerStatistik1m2(const std::vector<std::unique_ptr<Messdaten>> &md) {
    return berechneDauerStatistik(md, [](const Messdaten &m) { return m.dauer1m2(); });
};

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
};

void Dateimanager::schreibeStatistik(std::ostream &outFile, const std::string &name, const Statistik &s) {
    outFile << name << ":" << std::endl
            << "Median: " << s.median << std::endl
            << "Durchschnitt: " << s.durchschnitt << std::endl
            << "StdAbw: " << s.standardabweichung << std::endl
            << "Min: " << s.minimum << std::endl
            << "Max: " << s.maximum << std::endl
            << std::endl;
};

void Dateimanager::writeBlock(std::ostream &file, const std::vector<MessWerte> &werte) {
    for (const auto &m : werte) {
        file << "threads: " << m.threads << std::endl
             << "dauer: " << m.dauer << std::endl
             << "lange: " << m.lange << std::endl
             << std::endl;
    }
};

void Dateimanager::sortByThreads(std::vector<MessWerte> &mw) {
    std::sort(mw.begin(), mw.end(),
              [](const MessWerte &a, const MessWerte &b) {
                  return a.threads < b.threads;
              });
};

void Dateimanager::printAllMessWerte(std::string variante, std::string targetFile) {
    auto pfade = getAllMessWerte(variante, targetFile);
    auto werte = leseArraygroesseUndMedian(pfade);
    for (const auto &[n, t] : werte) {
        std::cout << "(" << n << "," << t << ")," << std::endl;
    }
};

static long long extractArraySize(const std::filesystem::path &p) {
    bool nextIsNumber = false;

    for (const auto &part : p) {
        if (nextIsNumber) {
            return std::stoll(part.string());
        }
        if (part == "int") {
            nextIsNumber = true;
        }
    }
    return -1; // sollte nie passieren
}

std::vector<std::string> Dateimanager::getAllMessWerte(std::string variante, std::string targetFile) {
    std::vector<std::filesystem::path> pfadeF;

    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(originalPath)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        if (entry.path().filename() != targetFile) {
            continue;
        }
        for (const auto &part : entry.path()) {
            if (part == variante) {
                pfadeF.push_back(entry.path());
                break;
            }
        }
    }

    // Sortieren
    std::sort(pfadeF.begin(), pfadeF.end(),
              [](const auto &a, const auto &b) {
                  return extractArraySize(a) < extractArraySize(b);
              });

    // Umwandlung in string
    std::vector<std::string> pfade;
    pfade.reserve(pfadeF.size());
    for (const auto &p : pfadeF) {
        pfade.push_back(p.string());
    }

    return pfade;
}

std::vector<long long> Dateimanager::leseMedianWerte(
    const std::vector<std::string> &pfade) {

    std::vector<long long> werte;
    werte.reserve(pfade.size());

    for (const auto &pfad : pfade) {
        std::ifstream file(pfad);
        if (!file)
            continue;

        std::string line;
        for (int i = 0; i < 5; ++i) {
            if (!std::getline(file, line))
                break;
        }

        // erwartet: "Median: 1000"
        const std::string prefix = "Median:";
        if (line.starts_with(prefix)) {
            long long value =
                std::stoll(line.substr(prefix.size()));
            werte.push_back(value);
        }
    }

    return werte;
}

std::vector<std::pair<long long, long long>> Dateimanager::leseArraygroesseUndMedian(const std::vector<std::string> &pfade) {

    std::vector<std::pair<long long, long long>> werte;
    werte.reserve(pfade.size());

    for (const auto &pfadStr : pfade) {
        std::filesystem::path pfad(pfadStr);

        long long arrayGroesse = extractArraySize(pfad);

        std::ifstream file(pfadStr);
        if (!file)
            continue;

        std::string line;
        for (int i = 0; i < 5; ++i) {
            if (!std::getline(file, line))
                break;
        }

        // erwartet: "Median: 312100"
        long long median = std::stoll(line.substr(line.find(':') + 1));

        werte.emplace_back(arrayGroesse, median);
    }
    return werte;
}
