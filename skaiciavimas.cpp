#include "skaiciavimas.hpp"
#include <algorithm>

double vidurkis(const std::vector<int>& a) {
    if (a.empty()) return 0.0;
    long long s = 0;
    for (int x : a) s += x;
    return static_cast<double>(s) / a.size();
}

double mediana(std::vector<int> a) {
    if (a.empty()) return 0.0;
    std::sort(a.begin(), a.end());
    size_t n = a.size();
    if (n % 2 == 0) return (a[n / 2 - 1] + a[n / 2]) / 2.0;
    else return a[n / 2];
}

double galutinis_vidurkis(const Studentas& s) {
    return 0.4 * vidurkis(s.nd) + 0.6 * s.egzaminas;
}

double galutinis_mediana(const Studentas& s) {
    return 0.4 * mediana(s.nd) + 0.6 * s.egzaminas;
}
