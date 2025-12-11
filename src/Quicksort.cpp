// Quicksort.cpp
#include "Quicksort.h"
#include "WorkerPool.h"
#include <chrono>
#include <thread>
#include <vector>

Quicksort::Quicksort() {};

void Quicksort::sortG(int *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    quicksort(liste, links, rechts);
};

void Quicksort::sortM(int *liste, int lange, int messEbene) {
    int links = 0;
    int rechts = lange - 1;
    quicksort(liste, links, rechts, 1, messEbene);
};

void Quicksort::sortP(int *liste, int lange, int neueThreadsBisEbene) {
    int links = 0;
    int rechts = lange - 1;
    // int neueThreadsBisEbene = static_cast<int>(std::ceil(std::log2(static_cast<double>(anzahlThreads) + 1.0)));
    quicksortP(liste, links, rechts, 1, neueThreadsBisEbene);
};

void Quicksort::sortPM(int *liste, int lange, int neueThreadsBisEbene, int messEbene) {
    int links = 0;
    int rechts = lange - 1;
    quicksortP(liste, links, rechts, 1, neueThreadsBisEbene, messEbene);
};

void Quicksort::sortW(int *liste, const int lange, const int workerThreads) {
    int links = 0;
    int rechts = lange - 1;
    quicksortW(liste, links, rechts, workerThreads);
};

void Quicksort::quicksort(int *liste, const int links, const int rechts) {
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        quicksort(liste, links, ml);
        quicksort(liste, mr, rechts);
    }
};

void Quicksort::quicksort(int *liste, const int links, const int rechts, const int aktuelleEbene, const int messEbene) {
    if (aktuelleEbene == messEbene) {
        quicksortM(liste, links, rechts, aktuelleEbene);
    } else {
        if (links < rechts) {
            int ml, mr;
            partitioniere(liste, links, rechts, ml, mr);
            quicksort(liste, links, ml, aktuelleEbene + 1, messEbene);
            quicksort(liste, mr, rechts, aktuelleEbene + 1, messEbene);
        }
    }
};

void Quicksort::quicksortM(int *liste, const int links, const int rechts, const int aktuelleEbene) {
    Messdaten *pos = new Messdaten();
    pos->start1 = std::chrono::high_resolution_clock::now();
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        pos->start2 = std::chrono::high_resolution_clock::now();
        quicksort(liste, links, ml);
        quicksort(liste, mr, rechts);
        pos->ende2 = std::chrono::high_resolution_clock::now();
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
    Messdaten::addMessDaten(aktuelleEbene, pos);
};

void Quicksort::quicksortP(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene) {
    if (aktuelleEbene < neueThreadsBisEbene) {
        if (links < rechts) {
            int ml, mr;
            partitioniere(liste, links, rechts, ml, mr);
            // quicksort(liste, links, ml);
            std::thread thread(static_cast<void (*)(int *, const int, const int, const int, const int)>(&Quicksort::quicksortP),
                               liste, links, ml, aktuelleEbene + 1, neueThreadsBisEbene);
            // quicksort(liste, mr, rechts);
            quicksortP(liste, mr, rechts, aktuelleEbene + 1, neueThreadsBisEbene);
            thread.join();
        }
    } else {
        quicksort(liste, links, rechts);
    }
};

void Quicksort::quicksortP(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene, const int messEbene) {
    if (aktuelleEbene < neueThreadsBisEbene) {
        if (aktuelleEbene == messEbene) {
            quicksortPM(liste, links, rechts, aktuelleEbene, neueThreadsBisEbene);
        } else {
            if (links < rechts) {
                int ml, mr;
                partitioniere(liste, links, rechts, ml, mr);
                // quicksort(liste, links, ml);
                std::thread thread(
                    static_cast<void (*)(int *, const int, const int, const int, const int, const int)>(&Quicksort::quicksortP),
                    liste, links, ml, aktuelleEbene + 1, neueThreadsBisEbene, messEbene);
                // quicksort(liste, mr, rechts);
                quicksortP(liste, mr, rechts, aktuelleEbene + 1, neueThreadsBisEbene, messEbene);
                thread.join();
            }
        }
    } else {
        quicksort(liste, links, rechts, aktuelleEbene, messEbene);
    }
};

void Quicksort::quicksortPM(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene) {
    Messdaten *pos = new Messdaten();
    pos->start1 = std::chrono::high_resolution_clock::now();
    if (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        // quicksort(liste, links, ml);
        pos->start2 = std::chrono::high_resolution_clock::now();
        std::thread thread(
            static_cast<void (*)(int *, const int, const int, const int, const int)>(&Quicksort::quicksortP),
            liste, links, ml, aktuelleEbene + 1, neueThreadsBisEbene);
        ;
        // quicksort(liste, mr, rechts);
        quicksortP(liste, mr, rechts, aktuelleEbene + 1, neueThreadsBisEbene);
        thread.join();
        pos->ende2 = std::chrono::high_resolution_clock::now();
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
    Messdaten::addMessDaten(aktuelleEbene, pos);
};

void Quicksort::quicksortW(int *liste, int links, int rechts, int workerThreads) {
    WorkerPool pool(workerThreads);

    pool.taskHandler = [](int *liste, int links, int rechts, WorkerPool &pool) {
        if (links < rechts) {
            if (rechts - links < Sortierverfaren::mindestLange) {
                quicksort(liste, links, rechts);
            } else {
                int ml, mr;
                Quicksort::partitioniere(liste, links, rechts, ml, mr);
                pool.addTask({liste, links, ml});
                pool.taskHandler(liste, mr, rechts, pool);
            }
        }
    };

    pool.addTaskWaitUntilDone({liste, links, rechts});
}

void Quicksort::partitioniere(int *liste, const int links, const int rechts, int &ml, int &mr) {
    int i = links;
    int j = rechts;
    int mitte = links + ((rechts - links) / 2);
    int p = liste[mitte];
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

void Quicksort::vertausche(int *liste, const int a, const int b) {
    int temp = liste[a];
    liste[a] = liste[b];
    liste[b] = temp;
};
