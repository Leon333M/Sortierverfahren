// ManagerStringString.cpp
#include "ManagerString.h"
#include "MergesortString.h"
#include "QuicksortString.h"
#include <chrono>
#include <iostream>
#include <string>

const int t[] = {1, 2, 4, 8, 15, 16, 17, 18, 32, 64, 128, 256, 1024, 4096, 8192, 16384};
const int te[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
const char listeVarianten[] = {'z', 's', 'i', 'f', 'd'};

ManagerString::ManagerString(int argc, char *argv[]) {
    Messdaten::arrayArt = "string";
    char variante = 'a';
    listeVariante = 'z';
    std::vector<char> lvn;

    if (argc >= 2) {
        lange = std::stoi(argv[1]);
    }
    if (argc >= 3) {
        variante = argv[2][0];
    }
    if (argc >= 4) {
        listeVariante = argv[3][0];
        if (listeVariante == 'a') {
            for (char lv : listeVarianten) {
                lvn.push_back(lv);
            }
        } else {
            lvn.push_back(listeVariante);
        }
    } else {
        lvn.push_back(listeVariante);
    }
    if (argc >= 6) {
        std::cout << "messe Alles" << std::endl;
        messeAlles();
        variante = 'i';
    }

    for (char lv : lvn) {
        listeVariante = lv;
        std::cout << "listeVariante :" << listeVariante << std::endl;
        if (variante == 'g' || variante == 'a') {
            grundzeiten();
        }
        if (variante == 'm' || variante == 'a') {
            messeSortierzeiten();
        }
        if (variante == 'p' || variante == 'a') {
            parallelzeiten();
        }
        if (variante == 'o' || variante == 'a') {
            messeSortierzeitenP();
        }
        if (variante == 'w' || variante == 'a') {
            workerZeiten();
        }
    }

    // Ende
    std::cout << "Ende" << std::endl;
};

void ManagerString::grundzeiten() {
    Messdaten::resetMessDaten();

    // MergesortString
    std::string *liste = listenerstelerString.erstelleListe(listeVariante, lange);
    MergesortString mergesortString;
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    mergesortString.sortG(liste, lange);
    std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
    long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << " MergesortString" << std::endl;
    istSortiert();

    Messdaten *md = new Messdaten();
    md->start1 = start;
    md->ende1 = stop;
    Messdaten::addMessDaten(1, md);
    dateimanager.exportMessData("MergesortString", "g");
    Messdaten::resetMessDaten();

    // QuicksortString
    liste = listenerstelerString.erstelleListe(listeVariante, lange);
    QuicksortString quicksortString;
    start = std::chrono::high_resolution_clock::now();
    quicksortString.sortG(liste, lange);
    stop = std::chrono::high_resolution_clock::now();
    dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << " QuicksortString" << std::endl;
    istSortiert();

    md = new Messdaten();
    md->start1 = start;
    md->ende1 = stop;
    Messdaten::addMessDaten(1, md);
    dateimanager.exportMessData("QuicksortString", "g");
    Messdaten::resetMessDaten();
};

void ManagerString::messeSortierzeiten() {
    // init
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    std::cout << "maxEbene : " << maxEbene << std::endl;
    Messdaten::resetMessDaten();
    Messdaten::initMessDaten(lange);
    std::string *liste = listenerstelerString.erstelleListe(listeVariante, lange);

    // MergesortString
    MergesortString mergesortString;
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    mergesortString.sortMA(listeVariante, lange);
    std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
    long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Gesamtlaufzeit: " << dauer << " ms" << " MergesortString" << std::endl;
    dateimanager.exportMessData("MergesortString", "m");

    // gebe Speicher frei da 10Gb bei Aray von 40'000'000
    Messdaten::resetMessDaten();
    Messdaten::initMessDaten(lange);
    liste = listenerstelerString.erstelleListe(listeVariante, lange);

    // QuicksortString
    QuicksortString quicksortString;
    start = std::chrono::high_resolution_clock::now();
    quicksortString.sortMA(listeVariante, lange);
    stop = std::chrono::high_resolution_clock::now();
    dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Gesamtlaufzeit: " << dauer << " ms" << " QuicksortString" << std::endl;
    dateimanager.exportMessData("QuicksortString", "m");
};

void ManagerString::parallelzeiten() {
    // init
    Messdaten::resetMessDaten();
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    std::cout << "maxEbene : " << maxEbene << std::endl;

    // MergesortString
    std::cout << "MergesortString :" << std::endl;
    for (int i : te) {
        if (i > maxEbene) {
            break;
        }
        MergesortString mergesortString;
        std::string *liste = listenerstelerString.erstelleListe(listeVariante, lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        mergesortString.sortP(liste, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int t = static_cast<int>(std::pow(2, i - 1));
        std::cout << "Laufzeit: " << dauer << " ms" << " neue threads bis Ebene: " << i << " rund Threads: " << t << std::endl;
        istSortiert();
        Messdaten *md = new Messdaten();
        md->start1 = start;
        md->ende1 = stop;
        Messdaten::addMessDaten(1, md);
        dateimanager.exportMessData("MergesortString", "p" + std::to_string(i));
        Messdaten::resetMessDaten();
    }

    // QuicksortString
    std::cout << "QuicksortString :" << std::endl;
    for (int i : te) {
        if (i > maxEbene) {
            break;
        }
        QuicksortString quicksortString;
        std::string *liste = listenerstelerString.erstelleListe(listeVariante, lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        quicksortString.sortP(liste, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int t = static_cast<int>(std::pow(2, i - 1));
        std::cout << "Laufzeit: " << dauer << " ms" << " neue threads bis Ebene: " << i << " rund Threads: " << t << std::endl;
        istSortiert();
        Messdaten *md = new Messdaten();
        md->start1 = start;
        md->ende1 = stop;
        Messdaten::addMessDaten(1, md);
        dateimanager.exportMessData("QuicksortString", "p" + std::to_string(i));
        Messdaten::resetMessDaten();
    }
};

void ManagerString::workerZeiten() {
    // init
    Messdaten::resetMessDaten();
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    std::cout << "maxEbene : " << maxEbene << std::endl;

    // MergesortString
    std::cout << "MergesortString :" << std::endl;
    for (int i : t) {
        MergesortString mergesortString;
        std::string *liste = listenerstelerString.erstelleListe(listeVariante, lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        mergesortString.sortW(liste, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int t = static_cast<int>(std::pow(2, i - 1));
        std::cout << "Laufzeit: " << dauer << " ms" << " workerThreads: " << i << std::endl;
        istSortiert();
        Messdaten *md = new Messdaten();
        md->start1 = start;
        md->ende1 = stop;
        Messdaten::addMessDaten(1, md);
        dateimanager.exportMessData("MergesortString", "w" + std::to_string(i));
        Messdaten::resetMessDaten();
    }

    // QuicksortString
    std::cout << "QuicksortString :" << std::endl;
    for (int i : t) {
        QuicksortString quicksortString;
        std::string *liste = listenerstelerString.erstelleListe(listeVariante, lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        quicksortString.sortW(liste, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        std::cout << "Laufzeit: " << dauer << " ms" << " workerThreads: " << i << std::endl;
        istSortiert();
        Messdaten *md = new Messdaten();
        md->start1 = start;
        md->ende1 = stop;
        Messdaten::addMessDaten(1, md);
        dateimanager.exportMessData("QuicksortString", "w" + std::to_string(i));
        Messdaten::resetMessDaten();
    }
};

void ManagerString::messeSortierzeitenP() {
    // init
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    std::cout << "maxEbene : " << maxEbene << std::endl;
    Messdaten::resetMessDaten();
    Messdaten::initMessDaten(lange);

    // MergesortString
    std::cout << "MergesortString :" << std::endl;
    for (int i : te) {
        if (i > maxEbene) {
            break;
        }
        MergesortString mergesortString;
        std::string *liste = listenerstelerString.erstelleListe(listeVariante, lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        mergesortString.sortPMA(listeVariante, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int t = static_cast<int>(std::pow(2, i - 1));
        std::cout << "Gesamtlaufzeit: " << dauer << " ms" << " neue threads bis Ebene: " << i << " rund Threads: " << t << std::endl;
        dateimanager.exportMessData("MergesortString", "nTbE" + std::to_string(i));
        // gebe Speicher frei da 10Gb bei Aray von 40'000'000
        Messdaten::resetMessDaten();
        Messdaten::initMessDaten(lange);
    }

    // QuicksortString
    std::cout << "QuicksortString :" << std::endl;
    for (int i : te) {
        if (i > maxEbene) {
            break;
        }
        QuicksortString quicksortString;
        std::string *liste = listenerstelerString.erstelleListe(listeVariante, lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        quicksortString.sortPMA(listeVariante, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int t = static_cast<int>(std::pow(2, i - 1));
        std::cout << "Gesamtlaufzeit: " << dauer << " ms" << " neue threads bis Ebene: " << i << " rund Threads: " << t << std::endl;
        dateimanager.exportMessData("QuicksortString", "nTbE" + std::to_string(i));
        // gebe Speicher frei da 10Gb bei Aray von 40'000'000
        Messdaten::resetMessDaten();
        Messdaten::initMessDaten(lange);
    }
};

void ManagerString::istSortiert() const {
    if (!listenerstelerString.istSortiert()) {
        std::cout << "ist nicht Sortiert" << std::endl;
    }
};

void ManagerString::messeAlles() {
    auto start = std::chrono::high_resolution_clock::now();
    // 10 - 40'000'000
    int langen[] = {
        10,
        100,
        1000,
        1200,
        2000,
        4000,
        8000,
        20000,
        40000,
        80000,
        // 200000,
        // 400000,
        // 800000,
        // 2000000,
        // 4000000,
        // 8000000,
        // 40000000,
        // 400000000,
    };
    // Gesamtlaufzeit: 786 s aller Messungen zusammen. 10 - 80000
    for (int l : langen) {
        lange = l;
        std::cout << "lange :" << lange << std::endl;
        for (char lv : listeVarianten) {
            listeVariante = lv;
            std::cout << "listeVariante :" << listeVariante << std::endl;
            messeZeiten();
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();
    long long dauer = std::chrono::duration_cast<std::chrono::seconds>(stop - start).count();
    std::cout << "Gesamtlaufzeit: " << dauer << " s" << " aller Messungen zusammen." << std::endl;
};

void ManagerString::messeZeiten() {
    grundzeiten();
    parallelzeiten();
    workerZeiten();
    if (lange <= 80000) {
        messeSortierzeiten();
        messeSortierzeitenP();
    }
};
