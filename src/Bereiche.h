// Bereiche.h
#pragma once
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
    std::queue<Bereich> unsortierteBereicheLinks;
    std::queue<Bereich> unsortierteBereicheRechts;
    std::mutex mutex;

public:
    Bereiche(int *liste0, int pivo) : liste(liste0), p(pivo) {};

    void partitioniereAlles(int *liste, int links, int rechts, int &ml, int &mr) {
        int lange = rechts - links;
        int bereich = lange / 2;
        int mitte = links + bereich;
        // if (lange > Sortierverfaren::mindestLange) {
        //     Quicksort::Quickselect(liste, mitte, 0.5 * bereich);
        // }
        p = liste[mitte];
        int mindestLange = 4000;
        mindestLange = mindestLange * log2(mindestLange);
        int minBereiche;
        int mainLinks = mitte;
        int mainRechts = mitte;
        bool istFertig = false;
        while (!istFertig) {
            // Erstellen
            erstelleBereiche(links, mindestLange, mitte, mainLinks, rechts, mainRechts, minBereiche);
            // Verarbeiten
            koordiniereParallelePartitionierung(liste, mainLinks, mainRechts, minBereiche, ml, mr);
            // Auswerten
            istFertigUpdate(liste, links, rechts, ml, mr, mitte, istFertig);
        }
    }

private:
    void erstelleBereiche(int links, int mindestLange, int mitte, int &mainLinks, int rechts, int &mainRechts, int &minBereiche) {
        int bNumerL = 0;
        while (true) {
            int bAnfag = links + bNumerL * mindestLange;
            int bEnde = links + (bNumerL + 1) * mindestLange;
            if (bEnde >= mitte) {
                mainLinks = bAnfag;
                break;
            }
            bNumerL++;
            Bereich bereich{bAnfag, bEnde};
            addUnsortierterBereichLinks(bereich);
        }
        int bNumerR = 0;
        while (true) {
            int bEnde = rechts - bNumerR * mindestLange;
            int bAnfag = rechts - (bNumerR + 1) * mindestLange;
            if (bAnfag < mitte) {
                mainRechts = bEnde;
                break;
            }
            bNumerR++;
            Bereich bereich{bAnfag, bEnde};
            addUnsortierterBereichRechts(bereich);
        }
        minBereiche = std::min(bNumerL, bNumerR);
    }

    void koordiniereParallelePartitionierung(int *liste, int mainLinks, int mainRechts, int minBereiche, int &ml, int &mr) {
        // spilt in neuen thads partitioniereBereich();
        partitioniere(liste, mainLinks, mainRechts, ml, mr);
        partitioniereBereich();
        // join / wait auf thads
    }

    void istFertigUpdate(int *liste, int &links, int &rechts, int &ml, int &mr, int &mitte, bool &retFlag) {
        // uberprufe ob (unsortierteBereicheLinks.size() + unsortierteBereicheRechts.size() == 0)
        // Ja fertig !
        retFlag = true;
        int leftQueueSize = unsortierteBereicheLinks.size();
        int rightQueueSize = unsortierteBereicheRechts.size();
        if ((leftQueueSize == 0) && (rightQueueSize == 0)) {
            // fertig !
            return;
        }
        if ((leftQueueSize > 0) && (rightQueueSize > 0)) {
            std::cout << "Error: leftQueueSize: " << leftQueueSize << " rightQueueSize: " << rightQueueSize << std::endl;
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
            int &links = linkerBereich.bereichAnfang;
            int &rechts = rechterBereich.bereichEnde;
            int &i = links;
            int &j = rechts;
            // int p = liste[mitte];
            while (i <= j) {
                while (liste[i] < p) {
                    i++;
                    if (i > linkerBereich.bereichEnde) {
                        // neuen Bereich nehemen
                        std::unique_lock<std::mutex> lock(mutex);
                        linkerBereich = entnehmeUnsortierterBereichLinks();
                        if (linkerBereich.bereichAnfang == -1) {
                            addUnsortierterBereichRechts(rechterBereich);
                            return;
                        }
                    }
                }
                while (liste[j] > p) {
                    j--;
                    if (j < rechterBereich.bereichAnfang) {
                        // neuen Bereich nehemen
                        std::unique_lock<std::mutex> lock(mutex);
                        rechterBereich = entnehmeUnsortierterBereichRechts();
                        if (rechterBereich.bereichAnfang == -1) {
                            addUnsortierterBereichLinks(linkerBereich);
                            return;
                        }
                    }
                }
                if (i <= j) {
                    vertausche(liste, i, j);
                    i++;
                    j--;
                }
            };
            // ml = j;
            // mr = i;
        }
    };

    static void partitioniere(int *liste, const int links, const int rechts, int &ml, int &mr) {
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
