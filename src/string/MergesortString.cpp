// MergesortString.cpp
#include "MergesortString.h"
#include "MergeWorkerPoolString.h"
#include <memory>
#include <thread>
#include <vector>

MergesortString::MergesortString() {};

void MergesortString::sortG(std::string *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    mergesortString(liste, links, rechts);
};

void MergesortString::sortM(std::string *liste, int lange, int messEbene) {
    int links = 0;
    int rechts = lange - 1;
    mergesortString(liste, links, rechts, 1, messEbene);
};

void MergesortString::sortP(std::string *liste, int lange, int neueThreadsBisEbene) {
    int links = 0;
    int rechts = lange - 1;
    mergesortStringP(liste, links, rechts, 1, neueThreadsBisEbene);
};

void MergesortString::sortPM(std::string *liste, int lange, int neueThreadsBisEbene, int messEbene) {
    int links = 0;
    int rechts = lange - 1;
    mergesortStringP(liste, links, rechts, 1, neueThreadsBisEbene, messEbene);
};

void MergesortString::sortW(std::string *liste, int lange, int workerThreads) {
    int links = 0;
    int rechts = lange - 1;
    mergesortStringW(liste, links, rechts, workerThreads);
};

void MergesortString::mergesortString(std::string *liste, const int links, const int rechts) {
    int lange = rechts - links + 1;
    if (lange > 1) {
        int mitte = links + ((rechts - links) / 2);
        mergesortString(liste, links, mitte);      // A
        mergesortString(liste, mitte + 1, rechts); // B
        mischen(liste, links, mitte, rechts, lange);
    }
};

void MergesortString::mergesortString(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int messEbene) {
    if (aktuelleEbene == messEbene) {
        mergesortStringM(liste, links, rechts, aktuelleEbene);
    } else {
        int lange = rechts - links + 1;
        if (lange > 1) {
            int mitte = links + ((rechts - links) / 2);
            mergesortString(liste, links, mitte, aktuelleEbene + 1, messEbene);
            mergesortString(liste, mitte + 1, rechts, aktuelleEbene + 1, messEbene);
            mischen(liste, links, mitte, rechts, lange);
        }
    }
};

void MergesortString::mergesortStringM(std::string *liste, const int links, const int rechts, const int aktuelleEbene) {
    Messdaten *pos = new Messdaten();
    pos->start1 = std::chrono::high_resolution_clock::now();
    int lange = rechts - links + 1;
    if (lange > 1) {
        int mitte = links + ((rechts - links) / 2);
        pos->start2 = std::chrono::high_resolution_clock::now();
        mergesortString(liste, links, mitte);
        mergesortString(liste, mitte + 1, rechts);
        pos->ende2 = std::chrono::high_resolution_clock::now();
        mischen(liste, links, mitte, rechts, lange);
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
    Messdaten::addMessDaten(aktuelleEbene, pos);
};

void MergesortString::mergesortStringP(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene) {
    if (aktuelleEbene < neueThreadsBisEbene) {
        int lange = rechts - links + 1;
        if (lange > 1) {
            int mitte = links + ((rechts - links) / 2);
            // mergesortString(liste, links, mitte);
            std::thread thread(
                static_cast<void (*)(std::string *, const int, const int, const int, const int)>(&MergesortString::mergesortStringP),
                liste, links, mitte, aktuelleEbene + 1, neueThreadsBisEbene);
            // mergesortString(liste, mitte + 1, rechts);
            mergesortStringP(liste, mitte + 1, rechts, aktuelleEbene + 1, neueThreadsBisEbene);
            thread.join();
            mischen(liste, links, mitte, rechts, lange);
        }
    } else {
        mergesortString(liste, links, rechts);
    }
};

void MergesortString::mergesortStringP(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene, const int messEbene) {
    if (aktuelleEbene < neueThreadsBisEbene) {
        if (aktuelleEbene == messEbene) {
            mergesortStringPM(liste, links, rechts, aktuelleEbene, neueThreadsBisEbene);
        } else {
            int lange = rechts - links + 1;
            if (lange > 1) {
                int mitte = links + ((rechts - links) / 2);
                // mergesortString(liste, links, mitte);
                std::thread thread(
                    static_cast<void (*)(std::string *, const int, const int, const int, const int, const int)>(&MergesortString::mergesortStringP),
                    liste, links, mitte, aktuelleEbene + 1, neueThreadsBisEbene, messEbene);
                // mergesortString(liste, mitte + 1, rechts);
                mergesortStringP(liste, mitte + 1, rechts, aktuelleEbene + 1, neueThreadsBisEbene, messEbene);
                thread.join();
                mischen(liste, links, mitte, rechts, lange);
            }
        }
    } else {
        mergesortString(liste, links, rechts, aktuelleEbene, messEbene);
    }
};

void MergesortString::mergesortStringPM(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene) {
    Messdaten *pos = new Messdaten();
    pos->start1 = std::chrono::high_resolution_clock::now();
    int lange = rechts - links + 1;
    if (lange > 1) {
        int mitte = links + ((rechts - links) / 2);
        pos->start2 = std::chrono::high_resolution_clock::now();
        // mergesortString(liste, links, mitte);
        std::thread thread(
            static_cast<void (*)(std::string *, const int, const int, const int, const int)>(&MergesortString::mergesortStringP),
            liste, links, mitte, aktuelleEbene + 1, neueThreadsBisEbene);
        // mergesortString(liste, mitte + 1, rechts);
        MergesortString::mergesortStringP(liste, mitte + 1, rechts, aktuelleEbene + 1, neueThreadsBisEbene);
        thread.join();
        pos->ende2 = std::chrono::high_resolution_clock::now();
        mischen(liste, links, mitte, rechts, lange);
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
    Messdaten::addMessDaten(aktuelleEbene, pos);
};

void MergesortString::mergesortStringW(std::string *liste, int links, int rechts, int workerThreads) {
    MergeWorkerPoolString pool(workerThreads);

    pool.taskHandler = [&](std::string *liste, int links, int rechts, MergeWorkerPoolString &pool) {
        if (links < rechts) {
            int lange = rechts - links + 1;
            if (lange < SortierverfarenString::mindestLange) {
                mergesortString(liste, links, rechts);
            } else {
                int mitte = links + ((rechts - links) / 2);
                auto leftHandle = pool.addTaskSmart({liste, links, mitte});
                pool.taskHandler(liste, mitte + 1, rechts, pool);
                leftHandle.wait();
                mischen(liste, links, mitte, rechts, lange);
            }
        }
    };

    // Starttask
    pool.taskHandler(liste, links, rechts, pool);
}

void MergesortString::mischen(std::string *liste, int links, const int mitte, const int rechts, const int lange) {
    std::string *listeB = new std::string[lange];

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
