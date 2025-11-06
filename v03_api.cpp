#include "v03_api.hpp"
#include "skaiciavimas.hpp"
#include "ivestis.hpp"
#include "sort.hpp"
#include "formatas.hpp"
#include <fstream>
#include <algorithm>
#include <chrono>
#include <iostream>

template<>
ContainerT<VectorTag, Studentas> read_all<VectorTag>(
    const std::string& path, long long* out_read_ms) {

    auto start = std::chrono::steady_clock::now();

    std::vector<Studentas> students;
    students.reserve(10000);

    if (!skaityti_is_failo(path, students)) {
        std::cerr << "Nepavyko nuskaityti failo: " << path << "\n";
    }

    auto end = std::chrono::steady_clock::now();

    if (out_read_ms) {
        *out_read_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    return students;
}

template<>
ContainerT<ListTag, Studentas> read_all<ListTag>(
    const std::string& path, long long* out_read_ms) {

    auto start = std::chrono::steady_clock::now();

    std::list<Studentas> students;

    std::vector<Studentas> temp;
    if (skaityti_is_failo(path, temp)) {
        for (auto& s : temp) {
            students.push_back(std::move(s));
        }
    }
    else {
        std::cerr << "Nepavyko nuskaityti failo: " << path << "\n";
    }

    auto end = std::chrono::steady_clock::now();

    if (out_read_ms) {
        *out_read_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    return students;
}

template<>
void split_groups<VectorTag>(
    std::vector<Studentas>& all,
    std::vector<Studentas>& varg,
    std::vector<Studentas>& kiet,
    int method,
    long long* out_split_ms) {

    auto start = std::chrono::steady_clock::now();

    varg.reserve(all.size());
    kiet.reserve(all.size());

    if (method == 1) {
        for (const auto& s : all) {
            double galutinis = galutinis_vidurkis(s);
            if (galutinis < 5.0) {
                varg.push_back(s);
            }
            else {
                kiet.push_back(s);
            }
        }
    }
    else if (method == 2) {
        auto it = std::remove_if(all.begin(), all.end(),
            [&varg](const Studentas& s) {
                double galutinis = galutinis_vidurkis(s);
                if (galutinis < 5.0) {
                    varg.push_back(s);
                    return true;
                }
                return false;
            });

        kiet.assign(all.begin(), it);
        all.erase(all.begin(), it);
    }
    else if (method == 3) {
        auto partition_point = std::partition(all.begin(), all.end(),
            [](const Studentas& s) {
                return galutinis_vidurkis(s) >= 5.0;
            });

        kiet.assign(all.begin(), partition_point);
        varg.assign(partition_point, all.end());
    }

    auto end = std::chrono::steady_clock::now();

    if (out_split_ms) {
        *out_split_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }
}

template<>
void split_groups<ListTag>(
    std::list<Studentas>& all,
    std::list<Studentas>& varg,
    std::list<Studentas>& kiet,
    int method,
    long long* out_split_ms) {

    auto start = std::chrono::steady_clock::now();

    if (method == 1) {
        for (const auto& s : all) {
            double galutinis = galutinis_vidurkis(s);
            if (galutinis < 5.0) {
                varg.push_back(s);
            }
            else {
                kiet.push_back(s);
            }
        }
    }
    else if (method == 2) {
        auto it = all.begin();
        while (it != all.end()) {
            double galutinis = galutinis_vidurkis(*it);
            if (galutinis < 5.0) {
                varg.push_back(*it);
                it = all.erase(it);
            }
            else {
                ++it;
            }
        }
        kiet = all;
    }
    else if (method == 3) {
        auto partition_point = std::partition(all.begin(), all.end(),
            [](const Studentas& s) {
                return galutinis_vidurkis(s) >= 5.0;
            });

        kiet.assign(all.begin(), partition_point);
        varg.assign(partition_point, all.end());
    }

    auto end = std::chrono::steady_clock::now();

    if (out_split_ms) {
        *out_split_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }
}

template<>
void sort_groups<VectorTag>(
    std::vector<Studentas>& varg,
    std::vector<Studentas>& kiet,
    int rikiavimas,
    long long* out_sort_ms) {

    auto start = std::chrono::steady_clock::now();

    if (rikiavimas == 1) {
        std::sort(varg.begin(), varg.end(), less_vardas_pavarde);
        std::sort(kiet.begin(), kiet.end(), less_vardas_pavarde);
    }
    else {
        std::sort(varg.begin(), varg.end(), less_pavarde_vardas);
        std::sort(kiet.begin(), kiet.end(), less_pavarde_vardas);
    }

    auto end = std::chrono::steady_clock::now();

    if (out_sort_ms) {
        *out_sort_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }
}

template<>
void sort_groups<ListTag>(
    std::list<Studentas>& varg,
    std::list<Studentas>& kiet,
    int rikiavimas,
    long long* out_sort_ms) {

    auto start = std::chrono::steady_clock::now();

    if (rikiavimas == 1) {
        varg.sort(less_vardas_pavarde);
        kiet.sort(less_vardas_pavarde);
    }
    else {
        varg.sort(less_pavarde_vardas);
        kiet.sort(less_pavarde_vardas);
    }

    auto end = std::chrono::steady_clock::now();

    if (out_sort_ms) {
        *out_sort_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }
}

template<>
void write_groups<VectorTag>(
    const std::vector<Studentas>& varg,
    const std::vector<Studentas>& kiet,
    int method,
    long long* out_write_ms) {

    auto start = std::chrono::steady_clock::now();

    failo_formatavimas("vargsiukai.txt", varg, method);
    failo_formatavimas("kietiakiai.txt", kiet, method);

    auto end = std::chrono::steady_clock::now();

    if (out_write_ms) {
        *out_write_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    std::cout << "Rezultatai issaugoti: vargsiukai.txt (" << varg.size()
        << "), kietiakiai.txt (" << kiet.size() << ")\n";
}

template<>
void write_groups<ListTag>(
    const std::list<Studentas>& varg,
    const std::list<Studentas>& kiet,
    int method,
    long long* out_write_ms) {

    auto start = std::chrono::steady_clock::now();

    std::vector<Studentas> varg_vec(varg.begin(), varg.end());
    std::vector<Studentas> kiet_vec(kiet.begin(), kiet.end());

    failo_formatavimas("vargsiukai.txt", varg_vec, method);
    failo_formatavimas("kietiakiai.txt", kiet_vec, method);

    auto end = std::chrono::steady_clock::now();

    if (out_write_ms) {
        *out_write_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    std::cout << "Rezultatai issaugoti: vargsiukai.txt (" << varg.size()
        << "), kietiakiai.txt (" << kiet.size() << ")\n";
}