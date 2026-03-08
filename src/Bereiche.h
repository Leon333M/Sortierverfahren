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

#include <iostream>

struct Bereich {
    int bereichAnfang;
    int bereichEnde;
};

class Bereiche {
private:
    int *liste;
    int p;
    const int mindestLange = 500000;
    std::queue<Bereich> unsortierteBereicheLinks;
    std::queue<Bereich> unsortierteBereicheRechts;
    std::mutex mutex;
    WorkerPool &workerPool;

public:
    Bereiche(int *liste0, WorkerPool &pool) : liste(liste0), workerPool(pool) {};

    void partitioniereAlles(int *liste, int links, int rechts, int &ml, int &mr) {
        int startLinks = links;
        int startRechts = rechts;
        ml = startLinks;
        mr = startRechts;
        int lange = rechts - links;
        int bereich = lange / 2;
        int mitte = links + bereich;
        // if (lange > Sortierverfaren::mindestLange) {
        //     Quicksort::Quickselect(liste, mitte, 0.5 * bereich);
        // }
        p = liste[mitte];
        // return partitioniere(liste, links, rechts, ml, mr);
        int minBereiche;
        bool istFertig = false;
        while (!istFertig) {
            if (rechts - links + 1 <= mindestLange) {
                partitioniere(liste, links, rechts, ml, mr);
                istFertig = true;
                // zeigeStatus(mr, links, mitte, liste, rechts, ml);
                // std::cout << " partit ";
                // zeigeStatusEnde(istFertig);
            } else {
                // Erstellen
                erstelleBereiche(links, mindestLange, mitte, rechts, minBereiche);
                // Verarbeiten
                koordiniereParallelePartitionierung(liste, links, rechts, minBereiche, ml, mr);
                // Auswerten
                ml = mitte - 1;
                mr = mitte + 1;
                // zeigeStatus(mr, links, mitte, liste, rechts, ml);
                istFertigUpdate(liste, links, rechts, ml, mr, mitte, istFertig);
                // zeigeStatusEnde(istFertig);
            }
        }
    }

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

    void koordiniereParallelePartitionierung(int *liste, int links, int rechts, int minBereiche, int &ml, int &mr) {
        int leftQueueSize = unsortierteBereicheLinks.size();
        int rightQueueSize = unsortierteBereicheRechts.size();
        if (leftQueueSize == 0 || rightQueueSize == 0) {
            std::cout << "Error!: leftQueueSize: " << leftQueueSize << " rightQueueSize: " << rightQueueSize << std::endl;
        }
        std::vector<WorkerPool::TaskHandle> handles;
        {
            std::mutex &sperre = workerPool.getSperre();
            std::unique_lock<std::mutex> lock(sperre);
            int freieThreads = workerPool.getFreieThreads();
            int useThreads = std::min(freieThreads, minBereiche);
            handles.reserve(useThreads);
            // spilt in neuen thads partitioniereBereich();
            for (int i = 0; i < useThreads; i++) {
                handles.push_back(workerPool.addLambdaTask([=]() {
                    partitioniereBereich();
                }));
            }
        }
        partitioniereBereich();
        // join / wait auf thads
        for (const auto &handle : handles) {
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
            // std::cout << " Mitte  ";
            return;
        }
        if ((leftQueueSize > 0) && (rightQueueSize > 0)) {
            std::cout << "Error: leftQueueSize: " << leftQueueSize << " rightQueueSize: " << rightQueueSize << std::endl;
            return partitioniere(liste, links, rechts, ml, mr);
        }
        if (rightQueueSize > 0) {
            // std::cout << " Rechts ";
            links = mitte; // nicht Partitionieren
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
            // std::cout << " Links  ";
            rechts = mitte; // nicht Partitionieren
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
        // fur neuen bereich widerholen (begin:) bis fertig !
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
                    bool linkerBereichUngueltig = false;
                    bool rechterBereichUngueltig = false;
                    if (linkerBereich.bereichAnfang > linkerBereich.bereichEnde) {
                        linkerBereichUngueltig = true;
                    }
                    if (rechterBereich.bereichEnde < rechterBereich.bereichAnfang) {
                        rechterBereichUngueltig = true;
                    }
                    if (linkerBereichUngueltig && rechterBereichUngueltig) {
                        // std::cout << "sodnerfall" << std::endl;
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
            // ml = rechterBereich.bereichEnde;
            // mr = linkerBereich.bereichAnfang;
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
        // if (lange > Sortierverfaren::mindestLange) {
        //     Quicksort::Quickselect(liste, mitte, 0.5 * bereich);
        // }
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

    void zeigeStatusEnde(bool istFertig) {
        std::cout << " istFertig: " << istFertig;
        std::cout << std::endl;
    }

    void zeigeStatus(int &mr, int links, int mitte, int *liste, int rechts, int &ml) {
        std::cout << "pivo: " << p << " Liste: ";
        for (int i = 0; i < 21; i++) {
            if ((i == mr)) {
                std::cout << " -| ";
            }
            if (i == links) {
                std::cout << " L ";
            }
            if (i == mitte) {
                std::cout << " |M ";
            }
            std::cout << liste[i] << " ";
            if (i == mitte) {
                std::cout << " M| ";
            }
            if (i == rechts) {
                std::cout << " R ";
            }
            if ((i == ml)) {
                std::cout << " |- ";
            }
        }
    }

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
