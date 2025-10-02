#pragma once
#include "studentai.hpp"

inline bool less_pavarde_vardas(const Studentas& a, const Studentas& b) {
    if (a.pavarde != b.pavarde) return a.pavarde < b.pavarde;
    return a.vardas < b.vardas;
}
inline bool less_vardas_pavarde(const Studentas& a, const Studentas& b) {
    if (a.vardas != b.vardas) return a.vardas < b.vardas;
    return a.pavarde < b.pavarde;
}

template <class T, class Less>
void merge_sort(std::vector<T>& a, Less less) {
    const std::size_t n = a.size();
    if (n <= 1) return;
    std::vector<T> tmp(n);
    for (std::size_t width = 1; width < n; width *= 2) {
        for (std::size_t i = 0; i < n; i += 2 * width) {
            std::size_t l = i, m = std::min(i + width, n), r = std::min(i + 2 * width, n);
            std::size_t p = l, q = m, k = l;
            while (p < m && q < r) {
                if (!less(a[q], a[p])) tmp[k++] = std::move(a[p++]);
                else tmp[k++] = std::move(a[q++]);
            }
            while (p < m) tmp[k++] = std::move(a[p++]);
            while (q < r) tmp[k++] = std::move(a[q++]);
            for (std::size_t t = l; t < r; ++t) a[t] = std::move(tmp[t]);
        }
    }
}
