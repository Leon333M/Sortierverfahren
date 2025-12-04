// QuicksortString.cpp
#include "QuicksortString.h"
#include "WorkerPoolString.h"
#include <chrono>
#include <thread>
#include <vector>

QuicksortString::QuicksortString() {};

void QuicksortString::sortG(std::string *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    quicksortString(liste, links, rechts);
};

void QuicksortString::sortM(std::string *liste, int lange, int messEbene) {
    int links = 0;
    int rechts = lange - 1;
    quicksortString(liste, links, rechts, 1, messEbene);
};

void QuicksortString::sortP(std::string *liste, int lange, int neueThreadsBisEbene) {
    int links = 0;
    int rechts = lange - 1;
    // int neueThreadsBisEbene = static_cast<int>(std::ceil(std::log2(static_cast<double>(anzahlThreads) + 1.0)));
    quicksortStringP(liste, links, rechts, 1, neueThreadsBisEbene);
};

void QuicksortString::sortPM(std::string *liste, int lange, int neueThreadsBisEbene, int messEbene) {
    int links = 0;
    int rechts = lange - 1;
    quicksortStringP(liste, links, rechts, 1, neueThreadsBisEbene, messEbene);
};

void QuicksortString::sortW(std::string *liste, const int lange, const int workerThreads) {
    int links = 0;
    int rechts = lange - 1;
    quicksortStringW(liste, links, rechts, workerThreads);
};

void QuicksortString::quicksortString(std::string *liste, const int links, const int rechts) {
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        quicksortString(liste, links, ml);
        quicksortString(liste, mr, rechts);
    }
};

void QuicksortString::quicksortString(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int messEbene) {
    if (aktuelleEbene == messEbene) {
        quicksortStringM(liste, links, rechts, aktuelleEbene);
    } else {
        if (links < rechts) {
            int ml, mr;
            partitioniere(liste, links, rechts, ml, mr);
            quicksortString(liste, links, ml, aktuelleEbene + 1, messEbene);
            quicksortString(liste, mr, rechts, aktuelleEbene + 1, messEbene);
        }
    }
};

void QuicksortString::quicksortStringM(std::string *liste, const int links, const int rechts, const int aktuelleEbene) {
    Messdaten *pos = new Messdaten();
    pos->start1 = std::chrono::high_resolution_clock::now();
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        pos->start2 = std::chrono::high_resolution_clock::now();
        quicksortString(liste, links, ml);
        quicksortString(liste, mr, rechts);
        pos->ende2 = std::chrono::high_resolution_clock::now();
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
    Messdaten::addMessDaten(aktuelleEbene, pos);
};

void QuicksortString::quicksortStringP(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene) {
    if (aktuelleEbene < neueThreadsBisEbene) {
        if (links < rechts) {
            int ml, mr;
            partitioniere(liste, links, rechts, ml, mr);
            // quicksortString(liste, links, ml);
            std::thread thread(static_cast<void (*)(std::string *, const int, const int, const int, const int)>(&QuicksortString::quicksortStringP),
                               liste, links, ml, aktuelleEbene + 1, neueThreadsBisEbene);
            // quicksortString(liste, mr, rechts);
            quicksortStringP(liste, mr, rechts, aktuelleEbene + 1, neueThreadsBisEbene);
            thread.join();
        }
    } else {
        quicksortString(liste, links, rechts);
    }
};

void QuicksortString::quicksortStringP(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene, const int messEbene) {
    if (aktuelleEbene < neueThreadsBisEbene) {
        if (aktuelleEbene == messEbene) {
            quicksortStringPM(liste, links, rechts, aktuelleEbene, neueThreadsBisEbene);
        } else {
            if (links < rechts) {
                int ml, mr;
                partitioniere(liste, links, rechts, ml, mr);
                // quicksortString(liste, links, ml);
                std::thread thread(
                    static_cast<void (*)(std::string *, const int, const int, const int, const int, const int)>(&QuicksortString::quicksortStringP),
                    liste, links, ml, aktuelleEbene + 1, neueThreadsBisEbene, messEbene);
                // quicksortString(liste, mr, rechts);
                quicksortStringP(liste, mr, rechts, aktuelleEbene + 1, neueThreadsBisEbene, messEbene);
                thread.join();
            }
        }
    } else {
        quicksortString(liste, links, rechts, aktuelleEbene, messEbene);
    }
};

void QuicksortString::quicksortStringPM(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene) {
    Messdaten *pos = new Messdaten();
    pos->start1 = std::chrono::high_resolution_clock::now();
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        // quicksortString(liste, links, ml);
        pos->start2 = std::chrono::high_resolution_clock::now();
        std::thread thread(
            static_cast<void (*)(std::string *, const int, const int, const int, const int)>(&QuicksortString::quicksortStringP),
            liste, links, ml, aktuelleEbene + 1, neueThreadsBisEbene);
        ;
        // quicksortString(liste, mr, rechts);
        quicksortStringP(liste, mr, rechts, aktuelleEbene + 1, neueThreadsBisEbene);
        thread.join();
        pos->ende2 = std::chrono::high_resolution_clock::now();
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
    Messdaten::addMessDaten(aktuelleEbene, pos);
};

void QuicksortString::quicksortStringW(std::string *liste, int links, int rechts, int workerThreads) {
    WorkerPoolString pool(workerThreads);

    pool.taskHandler = [](std::string *liste, int links, int rechts, WorkerPoolString &pool) {
        if (links < rechts) {
            if (rechts - links < SortierverfarenString::mindestLange) {
                quicksortString(liste, links, rechts);
            } else {
                int ml, mr;
                QuicksortString::partitioniere(liste, links, rechts, ml, mr);
                pool.addTask({liste, links, ml});
                pool.taskHandler(liste, mr, rechts, pool);
            }
        }
    };

    pool.addTask({liste, links, rechts});
    pool.waitUntilDone();
}

void QuicksortString::partitioniere(std::string *liste, const int links, const int rechts, int &ml, int &mr) {
    int i = links;
    int j = rechts;
    int mitte = links + ((rechts - links) / 2);
    std::string p = liste[mitte];
    while (i <= j) {
        while (liste[i] < p) {
            i++;
        }
        while (liste[j] > p) {
            j--;
        }
        if (i <= j) {
            vertausche(liste, i, j);
            i++;
            j--;
        }
    };
    ml = j;
    mr = i;
};

void QuicksortString::vertausche(std::string *liste, const int a, const int b) {
    std::string temp = liste[a];
    liste[a] = liste[b];
    liste[b] = temp;
};
