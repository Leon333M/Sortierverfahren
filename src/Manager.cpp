// Manager.cpp
#include "Manager.h"
#include "Mergesort.h"
#include "Quicksort.h"
#include <chrono>
#include <iostream>
#include <string>

const int t[] = {1, 2, 4, 8, 15, 16, 17, 18, 32, 64, 128, 256, 1024, 4096, 8192, 16384};
const int te[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
const char listeVarianten[] = {'z', 's', 'i', 'f', 'd'};

Manager::Manager(int argc, char *argv[]) {
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
    if (variante == 'i') {
        incArray();
    }

    // Ende
    std::cout << "Ende" << std::endl;
};

void Manager::grundzeiten() {
    Messdaten::resetMessDaten();

    // Mergesort
    int *liste = listenersteler.erstelleListe(listeVariante, lange);
    Mergesort mergesort;
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    mergesort.sortG(liste, lange);
    std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
    long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << " Mergesort" << std::endl;
    istSortiert();

    Messdaten *md = new Messdaten();
    md->start1 = start;
    md->ende1 = stop;
    Messdaten::addMessDaten(1, md);
    dateimanager.exportMessData("Mergesort", "g");
    Messdaten::resetMessDaten();

    // Quicksort
    liste = listenersteler.erstelleListe(listeVariante, lange);
    Quicksort quicksort;
    start = std::chrono::high_resolution_clock::now();
    quicksort.sortG(liste, lange);
    stop = std::chrono::high_resolution_clock::now();
    dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Laufzeit: " << dauer << " ms" << " Quicksort" << std::endl;
    istSortiert();

    md = new Messdaten();
    md->start1 = start;
    md->ende1 = stop;
    Messdaten::addMessDaten(1, md);
    dateimanager.exportMessData("Quicksort", "g");
    Messdaten::resetMessDaten();
};

void Manager::messeSortierzeiten() {
    // init
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    std::cout << "maxEbene : " << maxEbene << std::endl;
    Messdaten::resetMessDaten();
    Messdaten::initMessDaten(lange);
    int *liste = listenersteler.erstelleListe(listeVariante, lange);

    // Mergesort
    Mergesort mergesort;
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
    mergesort.sortMA(listeVariante, lange);
    std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
    long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Gesamtlaufzeit: " << dauer << " ms" << " Mergesort" << std::endl;
    dateimanager.exportMessData("Mergesort", "m");

    // gebe Speicher frei da 10Gb bei Aray von 40'000'000
    Messdaten::resetMessDaten();
    Messdaten::initMessDaten(lange);
    liste = listenersteler.erstelleListe(listeVariante, lange);

    // Quicksort
    Quicksort quicksort;
    start = std::chrono::high_resolution_clock::now();
    quicksort.sortMA(listeVariante, lange);
    stop = std::chrono::high_resolution_clock::now();
    dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "Gesamtlaufzeit: " << dauer << " ms" << " Quicksort" << std::endl;
    dateimanager.exportMessData("Quicksort", "m");
};

void Manager::parallelzeiten() {
    // init
    Messdaten::resetMessDaten();
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    std::cout << "maxEbene : " << maxEbene << std::endl;

    // Mergesort
    std::cout << "Mergesort :" << std::endl;
    for (int i : te) {
        if (i > maxEbene) {
            break;
        }
        Mergesort mergesort;
        int *liste = listenersteler.erstelleListe(listeVariante, lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        mergesort.sortP(liste, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int t = static_cast<int>(std::pow(2, i - 1));
        std::cout << "Laufzeit: " << dauer << " ms" << " neue threads bis Ebene: " << i << " rund Threads: " << t << std::endl;
        istSortiert();
        Messdaten *md = new Messdaten();
        md->start1 = start;
        md->ende1 = stop;
        Messdaten::addMessDaten(1, md);
        dateimanager.exportMessData("Mergesort", "p" + std::to_string(i));
        Messdaten::resetMessDaten();
    }

    // Quicksort
    std::cout << "Quicksort :" << std::endl;
    for (int i : te) {
        if (i > maxEbene) {
            break;
        }
        Quicksort quicksort;
        int *liste = listenersteler.erstelleListe(listeVariante, lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        quicksort.sortP(liste, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int t = static_cast<int>(std::pow(2, i - 1));
        std::cout << "Laufzeit: " << dauer << " ms" << " neue threads bis Ebene: " << i << " rund Threads: " << t << std::endl;
        istSortiert();
        Messdaten *md = new Messdaten();
        md->start1 = start;
        md->ende1 = stop;
        Messdaten::addMessDaten(1, md);
        dateimanager.exportMessData("Quicksort", "p" + std::to_string(i));
        Messdaten::resetMessDaten();
    }
};

void Manager::workerZeiten() {
    // init
    Messdaten::resetMessDaten();
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    std::cout << "maxEbene : " << maxEbene << std::endl;

    // Mergesort
    std::cout << "Mergesort :" << std::endl;
    for (int i : t) {
        Mergesort mergesort;
        int *liste = listenersteler.erstelleListe(listeVariante, lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        mergesort.sortW(liste, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int t = static_cast<int>(std::pow(2, i - 1));
        std::cout << "Laufzeit: " << dauer << " ms" << " workerThreads: " << i << std::endl;
        istSortiert();
        Messdaten *md = new Messdaten();
        md->start1 = start;
        md->ende1 = stop;
        Messdaten::addMessDaten(1, md);
        dateimanager.exportMessData("Mergesort", "w" + std::to_string(i));
        Messdaten::resetMessDaten();
    }

    // Quicksort
    std::cout << "Quicksort :" << std::endl;
    for (int i : t) {
        Quicksort quicksort;
        int *liste = listenersteler.erstelleListe(listeVariante, lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        quicksort.sortW(liste, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        std::cout << "Laufzeit: " << dauer << " ms" << " workerThreads: " << i << std::endl;
        istSortiert();
        Messdaten *md = new Messdaten();
        md->start1 = start;
        md->ende1 = stop;
        Messdaten::addMessDaten(1, md);
        dateimanager.exportMessData("Quicksort", "w" + std::to_string(i));
        Messdaten::resetMessDaten();
    }
};

void Manager::messeSortierzeitenP() {
    // init
    int maxEbene = static_cast<int>(std::ceil(std::log2(lange))) + 1;
    std::cout << "maxEbene : " << maxEbene << std::endl;
    Messdaten::resetMessDaten();
    Messdaten::initMessDaten(lange);

    // Mergesort
    std::cout << "Mergesort :" << std::endl;
    for (int i : te) {
        if (i > maxEbene) {
            break;
        }
        Mergesort mergesort;
        int *liste = listenersteler.erstelleListe(listeVariante, lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        mergesort.sortPMA(listeVariante, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int t = static_cast<int>(std::pow(2, i - 1));
        std::cout << "Gesamtlaufzeit: " << dauer << " ms" << " neue threads bis Ebene: " << i << " rund Threads: " << t << std::endl;
        dateimanager.exportMessData("Mergesort", "nTbE" + std::to_string(i));
        // gebe Speicher frei da 10Gb bei Aray von 40'000'000
        Messdaten::resetMessDaten();
        Messdaten::initMessDaten(lange);
    }

    // Quicksort
    std::cout << "Quicksort :" << std::endl;
    for (int i : te) {
        if (i > maxEbene) {
            break;
        }
        Quicksort quicksort;
        int *liste = listenersteler.erstelleListe(listeVariante, lange);
        std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
        quicksort.sortPMA(listeVariante, lange, i);
        std::chrono::time_point<std::chrono::high_resolution_clock> stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
        int t = static_cast<int>(std::pow(2, i - 1));
        std::cout << "Gesamtlaufzeit: " << dauer << " ms" << " neue threads bis Ebene: " << i << " rund Threads: " << t << std::endl;
        dateimanager.exportMessData("Quicksort", "nTbE" + std::to_string(i));
        // gebe Speicher frei da 10Gb bei Aray von 40'000'000
        Messdaten::resetMessDaten();
        Messdaten::initMessDaten(lange);
    }
};

void Manager::istSortiert() const {
    if (!listenersteler.istSortiert()) {
        std::cout << "ist nicht Sortiert" << std::endl;
    }
};

void Manager::messeAlles() {
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
    // Gesamtlaufzeit: 989 s aller Messungen zusammen. 10 - 800000
    // Gesamtlaufzeit: 808 s aller Messungen zusammen. 10 - 80000
    // Gesamtlaufzeit: 3520 s aller Messungen zusammen. 200000 - 400000000
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

void Manager::messeZeiten() {
    grundzeiten();
    parallelzeiten();
    workerZeiten();
    if (lange <= 80000) {
        messeSortierzeiten();
        messeSortierzeitenP();
    }
};

void Manager::incArray() {
    int lange = 1;
    bool fin = false;
    std::vector<int> thrads;
    std::vector<MessWerte> messWerte;
    std::vector<MessWerte> messWerte2;
    std::vector<MessWerte> messWerte3;
    while (!fin) {
        benchmarkIncThreads(lange, thrads, messWerte, false);
        lange = lange * 2;
        if (lange < 1) {
            fin = true;
        }
    }
    int maxLange = 1;
    for (MessWerte mw : messWerte) {
        if (maxLange < mw.lange) {
            maxLange = mw.lange;
        }
    }
    thrads.clear();
    benchmarkIncThreads(maxLange, thrads, messWerte2, true);
    thrads.clear();
    const int maxIntLange = std::numeric_limits<int>::max();
    benchmarkIncThreads(maxIntLange, thrads, messWerte3, true);
    dateimanager.writeMesswerteToFile("incArray", messWerte, messWerte2, messWerte3);
};

void Manager::incArrayMT(volatile int *liste, long long lange, int threadCount) {
    std::vector<std::thread> threads;
    threads.reserve(threadCount - 1);
    long long chunk = lange / threadCount;

    for (int t = 0; t < threadCount; t++) {
        long long start = t * chunk;
        long long lambdaStart = start;
        if (t == threadCount - 1) {
            // letzter Thread
            long long lambdaEnd = lange;
            for (long long i = lambdaStart; i < lambdaEnd; i++) {
                liste[i] = liste[i] + 1;
            }
        } else {
            long long lambdaEnd = start + chunk;
            threads.emplace_back([=]() {
                for (long long i = lambdaStart; i < lambdaEnd; i++) {
                    liste[i] = liste[i] + 1;
                }
            });
        }
    }
    for (auto &t : threads) {
        t.join();
    }
};

void Manager::benchmarkIncThreads(int lange, std::vector<int> &thrads, std::vector<MessWerte> &messWerte, bool messeAlles) {
    long long orginlDauer;
    // Single-Thread Benchmark
    {
        std::unique_ptr<int[]> liste(new int[lange]);
        volatile int *vptr = liste.get();

        auto start = std::chrono::high_resolution_clock::now();
        for (long long i = 0; i < lange; i++) {
            vptr[i] = vptr[i]++;
        }
        auto stop = std::chrono::high_resolution_clock::now();
        long long dauer = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

        // std::cout << "Single-Thread Dauer: " << dauer << " ns, lange: " << lange << std::endl;
        orginlDauer = dauer;
    }
    // Multi-Thread Benchmark
    for (int t = 1; t <= 16; t++) {
        if (std::find(thrads.begin(), thrads.end(), t) == thrads.end() || messeAlles) {
            std::unique_ptr<int[]> liste(new int[lange]);
            volatile int *vptr = liste.get();

            auto start = std::chrono::high_resolution_clock::now();
            incArrayMT(vptr, lange, t);
            auto stop = std::chrono::high_resolution_clock::now();
            long long dauer = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();

            if (dauer <= orginlDauer / 2 || messeAlles) {
                // schneller als 50% der Single-Thread-Laufzeit
                // std::cout << "50% schneller erreicht! threads: " << t << std::endl;
                thrads.push_back(t);
                messWerte.push_back({t, dauer, lange});
            }
        }
    }
};
