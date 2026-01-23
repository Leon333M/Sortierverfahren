// MergesortString.h
#pragma once
#include "SortierverfarenString.h"

class MergesortString : public SortierverfarenString {
private:
    // Vaibalen

public:
    // Funktionen
    MergesortString();
    void sortG(std::string *liste, int lange);
    void sortM(std::string *liste, int lange, int messEbene);
    void sortP(std::string *liste, int lange, int neueThreadsBisEbene);
    void sortPM(std::string *liste, int lange, int neueThreadsBisEbene, int messEbene);
    void sortW(std::string *liste, int lange, int workerThreads);

private:
    // Funktionen
    static void mergesortString(std::string *liste, const int links, const int rechts);
    static void mergesortString(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int messEbene);
    static void mergesortStringM(std::string *liste, const int links, const int rechts, const int aktuelleEbene);
    static void mergesortStringP(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene);
    static void mergesortStringP(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene, const int messEbene);
    static void mergesortStringPM(std::string *liste, const int links, const int rechts, const int aktuelleEbene, const int neueThreadsBisEbene);
    static void mergesortStringW(std::string *liste, int links, int rechts, int workerThreads);
    // aus Algorithmen und Datenstrukturen
    static void mischen(std::string *liste, const int links, const int mitte, const int rechts, const int lange);
};
