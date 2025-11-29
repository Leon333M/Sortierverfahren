// Mergesort.cpp
#include "Mergesort.h"
#include <memory>
#include <thread>
#include <vector>

Mergesort::Mergesort() {};

void Mergesort::sortG(int *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    mergesort(liste, links, rechts);
};

void Mergesort::sortM(int *liste, int lange, int messEbene) {
    int links = 0;
    int rechts = lange - 1;
    mergesort(liste, links, rechts, 1, messEbene);
};

void Mergesort::sortP(int *liste, int lange, int neueThreadsBisEbene) {
    int links = 0;
    int rechts = lange - 1;
    mergesortP(liste, links, rechts, 1, neueThreadsBisEbene);
};

void Mergesort::sortPM(int *liste, int lange, int neueThreadsBisEbene, int messEbene) {
    int links = 0;
    int rechts = lange - 1;
    mergesortP(liste, links, rechts, 1, neueThreadsBisEbene, messEbene);
};

void Mergesort::mergesort(int *liste, const int links, const int rechts) {
    int lange = rechts + 1 - links;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        mergesort(liste, links, mitte);      // A
        mergesort(liste, mitte + 1, rechts); // B
        mischen(liste, links, mitte, rechts, lange);
    }
};

void Mergesort::mergesort(int *liste, const int links, const int rechts, const int aktuelleEbene, const int messEbene) {
    if (aktuelleEbene == messEbene) {
        mergesortM(liste, links, rechts, aktuelleEbene);
    } else {
        int lange = rechts + 1 - links;
        if (lange > 1) {
            int mitte = (links + rechts) / 2;
            mergesort(liste, links, mitte, aktuelleEbene + 1, messEbene);
            mergesort(liste, mitte + 1, rechts, aktuelleEbene + 1, messEbene);
            mischen(liste, links, mitte, rechts, lange);
        }
    }
};

void Mergesort::mergesortM(int *liste, const int links, const int rechts, const int aktuelleEbene) {
    Messdaten *pos = new Messdaten();
    pos->start1 = std::chrono::high_resolution_clock::now();
    int lange = rechts + 1 - links;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        pos->start2 = std::chrono::high_resolution_clock::now();
        mergesort(liste, links, mitte);
        mergesort(liste, mitte + 1, rechts);
        pos->ende2 = std::chrono::high_resolution_clock::now();
        mischen(liste, links, mitte, rechts, lange);
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
    Messdaten::addMessDaten(aktuelleEbene, pos);
};

void Mergesort::mergesortP(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene) {
    if (aktuelleEbene < neueThreadsBisEbene) {
        int lange = rechts + 1 - links;
        if (lange > 1) {
            int mitte = (links + rechts) / 2;
            // mergesort(liste, links, mitte);
            std::thread thread(
                static_cast<void (*)(int *, const int, const int, const int, const int)>(&Mergesort::mergesortP),
                liste, links, mitte, aktuelleEbene + 1, neueThreadsBisEbene);
            // mergesort(liste, mitte + 1, rechts);
            mergesortP(liste, mitte + 1, rechts, aktuelleEbene + 1, neueThreadsBisEbene);
            thread.join();
            mischen(liste, links, mitte, rechts, lange);
        }
    } else {
        mergesort(liste, links, rechts);
    }
};

void Mergesort::mergesortP(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene, const int messEbene) {
    if (aktuelleEbene < neueThreadsBisEbene) {
        if (aktuelleEbene == messEbene) {
            mergesortPM(liste, links, rechts, aktuelleEbene, neueThreadsBisEbene);
        } else {
            int lange = rechts + 1 - links;
            if (lange > 1) {
                int mitte = (links + rechts) / 2;
                // mergesort(liste, links, mitte);
                std::thread thread(
                    static_cast<void (*)(int *, const int, const int, const int, const int, const int)>(&Mergesort::mergesortP),
                    liste, links, mitte, aktuelleEbene + 1, neueThreadsBisEbene, messEbene);
                // mergesort(liste, mitte + 1, rechts);
                mergesortP(liste, mitte + 1, rechts, aktuelleEbene + 1, neueThreadsBisEbene, messEbene);
                thread.join();
                mischen(liste, links, mitte, rechts, lange);
            }
        }
    } else {
        mergesort(liste, links, rechts, aktuelleEbene, messEbene);
    }
};

void Mergesort::mergesortPM(int *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene) {
    Messdaten *pos = new Messdaten();
    pos->start1 = std::chrono::high_resolution_clock::now();
    int lange = rechts + 1 - links;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        pos->start2 = std::chrono::high_resolution_clock::now();
        // mergesort(liste, links, mitte);
        std::thread thread(
            static_cast<void (*)(int *, const int, const int, const int, const int)>(&Mergesort::mergesortP),
            liste, links, mitte, aktuelleEbene + 1, neueThreadsBisEbene);
        // mergesort(liste, mitte + 1, rechts);
        Mergesort::mergesortP(liste, mitte + 1, rechts, aktuelleEbene + 1, neueThreadsBisEbene);
        thread.join();
        pos->ende2 = std::chrono::high_resolution_clock::now();
        mischen(liste, links, mitte, rechts, lange);
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
    Messdaten::addMessDaten(aktuelleEbene, pos);
};

void Mergesort::mischen(int *liste, int links, const int mitte, const int rechts, const int lange) {
    int *listeB = new int[lange];

    // Kopiere nach listeB
    for (int i = links; i < mitte + 1; i++) {
        listeB[i - links] = liste[i];
    }
    for (int i = mitte + 1; i < rechts + 1; i++) {
        listeB[lange - 1 + mitte + 1 - i] = liste[i];
    }

    // Sortiere liste
    int i = 0;         // links
    int j = lange - 1; // rechts
    int k = links;     // links
    while (i < j) {
        if (listeB[i] < listeB[j]) {
            liste[k] = listeB[i];
            i++;
        } else {
            liste[k] = listeB[j];
            j--;
        }
        k++;
    }
    liste[rechts] = listeB[i];

    delete[] listeB;
};
