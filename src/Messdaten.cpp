// Messdaten.cpp
#include "Messdaten.h"
#include <algorithm>

// Definition der statischen Member
int Messdaten::arrayLange;
std::string Messdaten::arrayTyp;
std::string Messdaten::arrayArt;
std::vector<std::vector<std::unique_ptr<Messdaten>>> Messdaten::messDaten;
std::mutex Messdaten::mutex;

int Messdaten::berechneDurchschnitt(const std::vector<long long> &werte) {
    if (werte.empty())
        return 0;
    long long sum = 0;
    for (auto v : werte)
        sum += v;
    return static_cast<int>(sum / werte.size());
};

int Messdaten::berechneMedian(std::vector<long long> &werte) {
    if (werte.empty())
        return 0;
    std::sort(werte.begin(), werte.end());
    size_t n = werte.size();
    if (n % 2 == 0)
        return static_cast<int>((werte[n / 2 - 1] + werte[n / 2]) / 2);
    else
        return static_cast<int>(werte[n / 2]);
};

int Messdaten::berechneStandardabweichung(const std::vector<long long> &werte) {
    if (werte.empty())
        return 0;
    int mittelwert = berechneDurchschnitt(werte);

    double sumVar = 0.0;
    for (auto v : werte) {
        double diff = v - mittelwert;
        sumVar += diff * diff;
    }

    double varianz = sumVar / werte.size();
    return static_cast<int>(std::sqrt(varianz));
};
