// Bereiche.h
#pragma once
#include "Sortierverfaren.h"
#include "WorkerPool.h"
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

struct Bereich {
    int bereichAnfang;
    int bereichEnde;
};

class Bereiche {
public:
    static const int mindestLange = 500000;

private:
    int *liste;
    int p;
    std::queue<Bereich> unsortierteBereicheLinks;
    std::queue<Bereich> unsortierteBereicheRechts;
    std::queue<std::shared_ptr<std::atomic<bool>>> ThreadFertigStatusListe;
    std::mutex mutex;
    WorkerPool &workerPool;

public:
    Bereiche(int *liste0, WorkerPool &pool) : liste(liste0), workerPool(pool) {};

    void partitioniereAlles(int *liste, int links, int rechts, int &ml, int &mr) {
        int lange = rechts - links;
        int bereich = lange / 2;
        int mitte = links + bereich;
        // if (lange > Sortierverfaren::mindestLange) {
        //     Quicksort::Quickselect(liste, mitte, 0.5 * bereich);
        // }
        p = liste[mitte];
        int minBereiche;
        bool istFertig = false;
        while (!istFertig) {
            if (rechts - links + 1 <= mindestLange) {
                partitioniere(liste, links, rechts, ml, mr);
                istFertig = true;
            } else {
                // Erstellen
                erstelleBereiche(links, mindestLange, mitte, rechts, minBereiche);
                // Verarbeiten
                koordiniereParallelePartitionierung(liste, minBereiche);
                // Auswerten
                istFertigUpdate(liste, links, rechts, ml, mr, mitte, istFertig);
            }
        }
    };

    // void meldeThreadAnUndArbeite() {
    //     std::unique_lock<std::mutex> lock(mutex);
    //     std::shared_ptr<std::atomic_bool> fetig = false;
    //     ThreadFertigStatusListe.push(fetig);
    //     partitioniereBereich();
    //     fetig.get()->store(true);
    // };

private:
    void erstelleBereiche(int links, int mindestLange, int mitte, int rechts, int &minBereiche) {
        int bNumerL = 0;
        while (true) {
            int bAnfag = links + bNumerL * mindestLange;
            int bEnde = links + (bNumerL + 1) * mindestLange - 1;
            if (bEnde + 1 >= mitte) {
                bEnde = mitte - 1;
                Bereich bereich{bAnfag, bEnde};
                addUnsortierterBereichLinks(bereich);
                break;
            }
            Bereich bereich{bAnfag, bEnde};
            addUnsortierterBereichLinks(bereich);
            bNumerL++;
        }
        int bNumerR = 0;
        while (true) {
            int bEnde = rechts - bNumerR * mindestLange;
            int bAnfag = rechts - (bNumerR + 1) * mindestLange + 1;
            if (bAnfag - 1 <= mitte) {
                bAnfag = mitte + 1;
                Bereich bereich{bAnfag, bEnde};
                addUnsortierterBereichRechts(bereich);
                break;
            }
            Bereich bereich{bAnfag, bEnde};
            addUnsortierterBereichRechts(bereich);
            bNumerR++;
        }
        minBereiche = std::min(bNumerL, bNumerR);
    }

    void koordiniereParallelePartitionierung(int *liste, int minBereiche) {
        int leftQueueSize = unsortierteBereicheLinks.size();
        int rightQueueSize = unsortierteBereicheRechts.size();
        if (leftQueueSize == 0 || rightQueueSize == 0) {
            // std::cout << "Error!: leftQueueSize: " << leftQueueSize << " rightQueueSize: " << rightQueueSize << std::endl;
        }
        std::vector<WorkerPool::TaskHandle> handles;
        {
            std::mutex &sperre = workerPool.getSperre();
            std::unique_lock<std::mutex> lock(sperre);
            int freieThreads = workerPool.getFreieThreads();
            freieThreads = freieThreads - workerPool.getTaskQueueSize();
            freieThreads = std::max(freieThreads, 0);
            int useThreads = std::min(freieThreads, minBereiche);
            handles.reserve(useThreads);
            // split in neuen Threads partitioniereBereich();
            for (int i = 0; i < useThreads; i++) {
                handles.push_back(workerPool.addLambdaTask([=]() {
                    partitioniereBereich();
                }));
            }
        }
        partitioniereBereich();
        // join / wait auf Threads
        for (auto &handle : handles) {
            handle.wait();
        }
    }

    void istFertigUpdate(int *liste, int &links, int &rechts, int &ml, int &mr, int &mitte, bool &retFlag) {
        // uberprufe ob (unsortierteBereicheLinks.size() + unsortierteBereicheRechts.size() == 0)
        // Ja fertig !
        retFlag = true;
        int leftQueueSize = unsortierteBereicheLinks.size();
        int rightQueueSize = unsortierteBereicheRechts.size();
        if ((leftQueueSize == 0) && (rightQueueSize == 0)) {
            // fertig !
            ml = mitte - 1;
            mr = mitte + 1;
            return;
        }
        if ((leftQueueSize > 0) && (rightQueueSize > 0)) {
            // std::cout << "Error: leftQueueSize: " << leftQueueSize << " rightQueueSize: " << rightQueueSize << std::endl;
            return partitioniere(liste, links, rechts, ml, mr);
        }
        if (rightQueueSize > 0) {
            links = mitte;
            // rechts = max unsortierteBereicheRechts.bereichEnde
            rechts = entnehmeUnsortierterBereichRechts().bereichEnde;
            for (int i = 0; i < rightQueueSize - 1; i++) {
                Bereich bereich = entnehmeUnsortierterBereichRechts();
                if (rechts < bereich.bereichEnde) {
                    rechts = bereich.bereichEnde;
                }
            }
            mitte = mitte + (rechts - mitte) / 2;
            vertausche(liste, links, mitte);
        }
        if (leftQueueSize > 0) {
            rechts = mitte;
            // links = min unsortierteBereicheLinks.bereichAnfang
            links = entnehmeUnsortierterBereichLinks().bereichAnfang;
            for (int i = 0; i < leftQueueSize - 1; i++) {
                Bereich bereich = entnehmeUnsortierterBereichLinks();
                if (links > bereich.bereichAnfang) {
                    links = bereich.bereichAnfang;
                }
            }
            mitte = mitte - (mitte - links) / 2;
            vertausche(liste, mitte, rechts);
        }
        // fur neuen bereich widerholen bis fertig !
        retFlag = false;
    }

    void partitioniereBereich() {
        while (true) {
            // Erstellen
            Bereich linkerBereich;
            Bereich rechterBereich;
            {
                // neuen Bereich nehemen
                std::unique_lock<std::mutex> lock(mutex);
                linkerBereich = entnehmeUnsortierterBereichLinks();
                if (linkerBereich.bereichAnfang == -1) {
                    return;
                }
                // neuen Bereich nehemen
                rechterBereich = entnehmeUnsortierterBereichRechts();
                if (rechterBereich.bereichAnfang == -1) {
                    addUnsortierterBereichLinks(linkerBereich);
                    return;
                }
            }
            // Verarbeiten
            // partitioniereBereich
            // partitioniere(int *liste, const int links, const int rechts, int &ml, int &mr) {
            // int p = liste[mitte];
            while (linkerBereich.bereichAnfang <= rechterBereich.bereichEnde) {
                while (liste[linkerBereich.bereichAnfang] < p) {
                    linkerBereich.bereichAnfang++;
                    if (aktualisiereLinkenBereich(linkerBereich, rechterBereich)) {
                        return;
                    }
                }
                while (liste[rechterBereich.bereichEnde] > p) {
                    rechterBereich.bereichEnde--;
                    if (aktualisiereRechtenBereich(rechterBereich, linkerBereich)) {
                        return;
                    }
                }
                if (linkerBereich.bereichAnfang <= rechterBereich.bereichEnde) {
                    vertausche(liste, linkerBereich.bereichAnfang, rechterBereich.bereichEnde);
                    linkerBereich.bereichAnfang++;
                    rechterBereich.bereichEnde--;
                    bool linkerBereichFertig = false;
                    bool rechterBereichFertig = false;
                    if (linkerBereich.bereichAnfang > linkerBereich.bereichEnde) {
                        linkerBereichFertig = true;
                    }
                    if (rechterBereich.bereichEnde < rechterBereich.bereichAnfang) {
                        rechterBereichFertig = true;
                    }
                    if (linkerBereichFertig && rechterBereichFertig) {
                        break;
                    }
                    if (aktualisiereLinkenBereich(linkerBereich, rechterBereich)) {
                        return;
                    }
                    if (aktualisiereRechtenBereich(rechterBereich, linkerBereich)) {
                        return;
                    }
                }
            };
        }
    }

    bool aktualisiereLinkenBereich(Bereich &linkerBereich, const Bereich &rechterBereich) {
        if (linkerBereich.bereichAnfang > linkerBereich.bereichEnde) {
            // neuen Bereich nehemen
            std::unique_lock<std::mutex> lock(mutex);
            linkerBereich = entnehmeUnsortierterBereichLinks();
            if (linkerBereich.bereichAnfang == -1) {
                addUnsortierterBereichRechts(rechterBereich);
                return true;
            }
        }
        return false;
    }

    bool aktualisiereRechtenBereich(Bereich &rechterBereich, const Bereich &linkerBereich) {
        if (rechterBereich.bereichEnde < rechterBereich.bereichAnfang) {
            // neuen Bereich nehemen
            std::unique_lock<std::mutex> lock(mutex);
            rechterBereich = entnehmeUnsortierterBereichRechts();
            if (rechterBereich.bereichAnfang == -1) {
                addUnsortierterBereichLinks(linkerBereich);
                return true;
            }
        }
        return false;
    };

    void partitioniere(int *liste, const int links, const int rechts, int &ml, int &mr) {
        int i = links;
        int j = rechts;
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

    static void vertausche(int *liste, const int a, const int b) {
        int temp = liste[a];
        liste[a] = liste[b];
        liste[b] = temp;
    };

    void addUnsortierterBereichLinks(Bereich bereich) {
        // std::unique_lock<std::mutex> lock(mutex);
        unsortierteBereicheLinks.push(bereich);
    };

    void addUnsortierterBereichRechts(Bereich bereich) {
        // std::unique_lock<std::mutex> lock(mutex);
        unsortierteBereicheRechts.push(bereich);
    };

    Bereich entnehmeUnsortierterBereichLinks() {
        // std::unique_lock<std::mutex> lock(mutex);
        if (unsortierteBereicheLinks.empty()) {
            return Bereich{-1, -1};
        }
        Bereich bereich = unsortierteBereicheLinks.front();
        unsortierteBereicheLinks.pop();
        return bereich;
    };

    Bereich entnehmeUnsortierterBereichRechts() {
        // std::unique_lock<std::mutex> lock(mutex);
        if (unsortierteBereicheRechts.empty()) {
            return Bereich{-1, -1};
        }
        Bereich bereich = unsortierteBereicheRechts.front();
        unsortierteBereicheRechts.pop();
        return bereich;
    };
};
