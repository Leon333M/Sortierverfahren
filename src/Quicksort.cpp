// Quicksort.cpp
#include "Quicksort.h"
#include <chrono>
#include <thread>
#include <vector>

#include <iostream>

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
                Quicksort::partitioniere(liste, links, rechts, ml, mr /*, pool*/);
                pool.addTask({liste, links, ml});
                pool.taskHandler(liste, mr, rechts, pool);
            }
        }
    };

    pool.addTaskWaitUntilDone({liste, links, rechts});
}

void Quicksort::Quickselect(int *liste, int mitte, int bereich) {
    int links = mitte - bereich;
    int rechts = mitte + bereich;
    // Standard Quickselect Loop
    while (links < rechts) {
        int ml, mr;
        partitioniere(liste, links, rechts, ml, mr);
        if (mitte >= ml && mitte <= mr) {
            return;
        } else if (mitte < ml) {
            // Der gesuchte Wert liegt im linken Teil
            rechts = ml - 1;
        } else {
            // Der gesuchte Wert liegt im rechten Teil
            links = mr + 1;
        }
    }
};

void Quicksort::QuickselectW(int *liste, int mitte, int bereich, WorkerPool &pool) {
    int links = mitte - bereich;
    int rechts = mitte + bereich;
    // Standard Quickselect Loop
    while (links < rechts) {
        int ml, mr;
        partitioniereW(liste, links, rechts, ml, mr, pool);
        if (mitte >= ml && mitte <= mr) {
            return;
        } else if (mitte < ml) {
            // Der gesuchte Wert liegt im linken Teil
            rechts = ml - 1;
        } else {
            // Der gesuchte Wert liegt im rechten Teil
            links = mr + 1;
        }
    }
};

void Quicksort::partitioniere(int *liste, const int links, const int rechts, int &ml, int &mr) {
    int i = links;
    int j = rechts;
    int lange = rechts - links;
    int bereich = lange / 2;
    int mitte = links + bereich;
    // if (lange > Sortierverfaren::mindestLange) {
    //     Quicksort::Quickselect(liste, mitte, 0.5 * bereich);
    // }
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

struct Bereich {
    int bereichAnfang;
    int lange;
};

void Quicksort::partitioniereW(int *liste, const int links, const int rechts, int &ml, int &mr, WorkerPool &pool) {
    int lange = rechts - links;
    int lange2 = lange / 2;
    int maxWThrads = lange / (10000000);
    int freieThreads = pool.getFreieThreads();
    int useThreads = std::min(freieThreads, maxWThrads);
    if (useThreads == 0) {
        partitioniere(liste, links, rechts, ml, mr);
        return;
    }
    useThreads = std::max(useThreads, 1);
    int bereich = lange / (useThreads * 2);

    int mitte = links + lange2;
    // Quicksort::Quickselect(liste, mitte, 0.5 * lange2);
    // std::cout << "Quickselect" << std::endl;
    int pivo = liste[mitte];

    std::vector<int> offsets(useThreads);
    // std::vector<std::thread> threads;
    PartitionWorkerPool partitionWorkerPool(useThreads - 1);
    std::vector<PartitionWorkerPool::TaskHandle> handles;

    for (int i = 0; i < useThreads - 1; i++) {
        // von linker bereich ausen
        int lba = links + i * bereich;
        // bis linker bereich innen
        int lbi = links + (i + 1) * bereich;
        // bis rechter bereich innen
        int rbi = rechts - (i + 1) * bereich;
        // von rechter bereich ausen
        int rba = rechts - i * bereich;
        // partitioniereBereich(liste, lba, lbi, rbi, rba, pivo, offsets[i]);
        // threads.emplace_back(&Quicksort::partitioniereBereich, liste, lba, lbi, rbi, rba, pivo, std::ref(offsets[i]));
        handles.push_back(
            partitionWorkerPool.addTask([=, &offsets]() {
                partitioniereBereich(liste, lba, lbi, rbi, rba, pivo, offsets[i]);
            }));
    }
    int lba = links + (useThreads - 1) * bereich;
    int rba = rechts - (useThreads - 1) * bereich;
    int ml0, mr0;
    // partitioniereBereich(liste, lba, mitte, mitte, rba, pivo, offsets[useThreads - 1]);
    partitioniere(liste, lba, rba, ml, mr);
    // for (int i = 0; i < threads.size(); i++) {
    //     threads[i].join();
    // }
    for (auto &h : handles) {
        h.wait();
    }

    // Aufraumen
    // int size = useThreads - 1;
    // int zuKleineElemnte = 0;
    // int zuGroseElemte = 0;
    // std::vector<Bereich> kleinerP;
    // std::vector<Bereich> groserP;
    // for (int i = 0; i < size; i++) {
    //     int offset = offsets[i];
    //     if (offset < 0) {
    //         zuKleineElemnte += abs(offset);
    //         int lbi = links + (i + 1) * bereich;
    //         // offset++;
    //         kleinerP.push_back(Bereich(lbi, offset));
    //     } else if (offset > 0) {
    //         zuGroseElemte += abs(offset);
    //         int rbi = rechts - (i + 1) * bereich;
    //         // offset--;
    //         groserP.push_back(Bereich(rbi, offset));
    //     } else if (offset == 0) {
    //         // std::cout << "keine Arbiet" << std::endl;
    //     }
    // }
    // std::string out = "";
    // out += "zuKleineElemnte: ";
    // out += std::to_string(zuKleineElemnte);
    // out += " zuGroseElemte: ";
    // out += std::to_string(zuGroseElemte);
    // out += " useThreads: ";
    // out += std::to_string(useThreads);
    // out += " ";
    // std::cout << out << std::endl;
    // // Aufremen wenn Beich links != bericeh rechts
    // // Aufremen wenn (links == 0) != (rechts == 0)
    // if (zuKleineElemnte == 0 && zuGroseElemte != 0) {
    //     int l = ml;
    //     int groserPIndex = groserP.size() - 1;
    //     int r = groserP[groserPIndex].bereichAnfang;
    //     int rIndex = 0;
    //     while (true) {
    //         vertausche(liste, l, r + rIndex);
    //         l--;
    //         if (rIndex == groserP[groserPIndex].lange) {
    //             groserP[groserPIndex].lange = 0;
    //             groserPIndex--;
    //             if (groserPIndex < 0) {
    //                 groserPIndex++;
    //                 break;
    //             }
    //             rIndex = 0;
    //             r = groserP[groserPIndex].bereichAnfang;
    //         } else {
    //             rIndex++;
    //         }
    //     }
    //     int posPivo = ml + 1;
    //     int gPosPivo = l + 1;
    //     vertausche(liste, posPivo, gPosPivo);
    // }
    // zuGroseElemte = 0;
    // for (Bereich b : groserP) {
    //     zuGroseElemte += b.lange;
    // }
    // zuKleineElemnte = 0;
    // for (Bereich b : kleinerP) {
    //     zuGroseElemte += abs(b.lange);
    // }
    // out = "";
    // out += "zuKleineElemnte: ";
    // out += std::to_string(zuKleineElemnte);
    // out += " zuGroseElemte: ";
    // out += std::to_string(zuGroseElemte);
    // out += " useThreads: ";
    // out += std::to_string(useThreads);
    // out += " ";
    // std::cout << out << std::endl;

    // Aufraumen Platzhalter
    partitioniere(liste, links, rechts, ml, mr);
};

void Quicksort::partitioniereBereich(int *liste, int lba, int lbi, int rbi, int rba, int pivo, int &offset) {

    int i = lba;
    int j = rba;

    while (i <= j) {
        // Suche links (wenn i ueber lbi geht, springe zu rbi)
        while (i <= j && liste[i] < pivo) {
            i++;
            if (i == lbi) {
                i = rbi; // Der Sprung nach innen/rechts
            }
        }
        // Suche rechts (wenn j unter rbi geht, springe zu lbi)
        while (i <= j && liste[j] > pivo) {
            j--;
            if (j == rbi - 1) {
                j = lbi - 1; // Der Sprung nach innen/links
            }
        }

        if (i <= j) {
            vertausche(liste, i, j);
            // Nach dem Tausch auch hier die Spruenge pruefen!
            i++;
            if (i == lbi) {
                i = rbi;
            }
            j--;
            if (j == rbi - 1) {
                j = lbi - 1;
            }
        }
    }

    int ml = j;
    int mr = i;

    // Berechnung des Offsets:
    if (ml >= rbi) {
        // ml ist im rechten Teil gelandet
        offset = (ml - rbi) + 1;
    } else {
        // ml ist im linken Teil stehen geblieben
        offset = -(ml - lbi) - 1;
    }
};
