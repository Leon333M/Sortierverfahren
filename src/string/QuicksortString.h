// QuicksortString.h
#pragma once
#include "SortierverfarenString.h"

class QuicksortString : public SortierverfarenString {
private:
    // Vaibalen

public:
    // Funktionen
    QuicksortString();
    void sortG(std::string *liste, int lange);
    void sortM(std::string *liste, int lange, int messEbene);
    void sortP(std::string *liste, int lange, int neueThreadsBisEbene);
    void sortPM(std::string *liste, int lange, int neueThreadsBisEbene, int messEbene);
    void sortW(std::string *liste, int lange, int workerThreads);

    // public Static

private:
    // Funktionen
    static void quicksortString(std::string *liste, const int links, const int rechts);
    static void quicksortString(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int messEbene);
    static void quicksortStringM(std::string *liste, const int links, const int rechts, const int aktuelleEbene);
    static void quicksortStringP(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene);
    static void quicksortStringP(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene, const int messEbene);
    static void quicksortStringPM(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene);
    static void quicksortStringW(std::string *liste, const int links, const int rechts, const int workerThreads);
    // aus Algorithmen und Datenstrukturen
    static void partitioniere(std::string *liste, const int links, const int rechts, int &ml, int &mr);
    static void vertausche(std::string *liste, const int a, const int b);
};
