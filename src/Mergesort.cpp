// Mergesort.cpp
#include "Mergesort.h"
#include <memory>
#include <thread>
#include <vector>

template <typename T>
Mergesort<T>::Mergesort(){};

template <typename T>
void Mergesort<T>::sortG(T *liste, int lange) {
    int links = 0;
    int rechts = lange - 1;
    mergesort(liste, links, rechts);
};

template <typename T>
void Mergesort<T>::sortM(T *liste, int lange, int messEbene) {
    int links = 0;
    int rechts = lange - 1;
    mergesort(liste, links, rechts, 1, messEbene);
};

template <typename T>
void Mergesort<T>::sortP(T *liste, int lange, int neueThreadsBisEbene) {
    int links = 0;
    int rechts = lange - 1;
    mergesortP(liste, links, rechts, 1, neueThreadsBisEbene);
};

template <typename T>
void Mergesort<T>::sortPM(T *liste, int lange, int neueThreadsBisEbene, int messEbene) {
    int links = 0;
    int rechts = lange - 1;
    mergesortP(liste, links, rechts, 1, neueThreadsBisEbene, messEbene);
};

template <typename T>
void Mergesort<T>::mergesort(T *liste, const int links, const int rechts) {
    int lange = rechts + 1 - links;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        mergesort(liste, links, mitte);
        mergesort(liste, mitte + 1, rechts);
        mischen(liste, links, mitte, rechts, lange);
    }
};

template <typename T>
void Mergesort<T>::mergesort(T *liste, const int links, const int rechts, const int aktuelleEbene, const int messEbene) {
    if (aktuelleEbene == messEbene) {
        mergesortM(liste, links, rechts, aktuelleEbene + 1);
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

template <typename T>
void Mergesort<T>::mergesortM(T *liste, const int links, const int rechts, const int aktuelleEbene) {
    Position *pos = new Position();
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
    Sortierverfaren<T>::addMessDaten(aktuelleEbene, pos);
};

template <typename T>
void Mergesort<T>::mergesortP(T *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene) {
    if (aktuelleEbene < neueThreadsBisEbene) {
        int lange = rechts + 1 - links;
        if (lange > 1) {
            int mitte = (links + rechts) / 2;
            std::vector<std::thread> threads;
            // mergesort(liste, links, mitte);
            threads.emplace_back(
                static_cast<void (*)(T *, const int, const int, const int, const int)>(&Mergesort<T>::mergesortP),
                liste, links, mitte, aktuelleEbene + 1, neueThreadsBisEbene);
            // mergesort(liste, mitte + 1, rechts);
            threads.emplace_back(
                static_cast<void (*)(T *, const int, const int, const int, const int)>(&Mergesort<T>::mergesortP),
                liste, mitte + 1, rechts, aktuelleEbene + 1, neueThreadsBisEbene);
            for (std::thread &thread : threads) {
                thread.join();
            }
            mischen(liste, links, mitte, rechts, lange);
        }
    } else {
        mergesort(liste, links, rechts);
    }
};

template <typename T>
void Mergesort<T>::mergesortP(T *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene, const int messEbene) {
    if (aktuelleEbene < neueThreadsBisEbene) {
        if (aktuelleEbene == messEbene) {
            mergesortPM(liste, links, rechts, aktuelleEbene + 1, neueThreadsBisEbene);
        } else {
            int lange = rechts + 1 - links;
            if (lange > 1) {
                int mitte = (links + rechts) / 2;
                std::vector<std::thread> threads;
                // mergesort(liste, links, mitte);
                threads.emplace_back(
                    static_cast<void (*)(T *, const int, const int, const int, const int, const int)>(&Mergesort<T>::mergesortP),
                    liste, links, mitte, aktuelleEbene + 1, neueThreadsBisEbene, messEbene);
                // mergesort(liste, mitte + 1, rechts);
                threads.emplace_back(
                    static_cast<void (*)(T *, const int, const int, const int, const int, const int)>(&Mergesort<T>::mergesortP),
                    liste, mitte + 1, rechts, aktuelleEbene + 1, neueThreadsBisEbene, messEbene);
                for (std::thread &thread : threads) {
                    thread.join();
                }
                mischen(liste, links, mitte, rechts, lange);
            }
        }
    } else {
        mergesort(liste, links, rechts, aktuelleEbene, messEbene);
    }
};

template <typename T>
void Mergesort<T>::mergesortPM(T *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene) {
    Position *pos = new Position();
    pos->start1 = std::chrono::high_resolution_clock::now();
    int lange = rechts + 1 - links;
    if (lange > 1) {
        int mitte = (links + rechts) / 2;
        std::vector<std::thread> threads;
        pos->start2 = std::chrono::high_resolution_clock::now();
        // mergesort(liste, links, mitte);
        threads.emplace_back(
            static_cast<void (*)(T *, const int, const int, const int, const int)>(&Mergesort<T>::mergesortP),
            liste, links, mitte, aktuelleEbene + 1, neueThreadsBisEbene);
        // mergesort(liste, mitte + 1, rechts);
        threads.emplace_back(
            static_cast<void (*)(T *, const int, const int, const int, const int)>(&Mergesort<T>::mergesortP),
            liste, mitte + 1, rechts, aktuelleEbene + 1, neueThreadsBisEbene);
        for (std::thread &thread : threads) {
            thread.join();
        }
        pos->ende2 = std::chrono::high_resolution_clock::now();
        mischen(liste, links, mitte, rechts, lange);
    }
    pos->ende1 = std::chrono::high_resolution_clock::now();
    Sortierverfaren<T>::addMessDaten(aktuelleEbene, pos);
};

template <typename T>
void Mergesort<T>::mischen(T *liste, int links, const int mitte, const int rechts, const int lange) {
    T *listeB = new T[lange];

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

// explizite Template-Instanziierungen
template class Mergesort<int>;
template class Mergesort<std::string>;