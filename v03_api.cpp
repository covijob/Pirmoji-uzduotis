#include "v03_api.hpp"
#include "skaiciavimas.hpp"
#include "ivestis.hpp"
#include "sort.hpp"
#include "formatas.hpp"
#include <vector>
#include <list>
#include <algorithm>
#include <chrono>
#include <iostream>

static inline double galutinis_by_method(const Studentas& s, int method) {
    if (method == 2) return galutinis_mediana(s);
    return galutinis_vidurkis(s);
}

template<typename Tag>
ContainerT<Tag, Studentas> read_all(const std::string& path, long long* out_read_ms) {
    auto t0 = std::chrono::steady_clock::now();

    if constexpr (std::is_same_v<Tag, VectorTag>) {
        std::vector<Studentas> students;
        students.reserve(10000);
        if (!skaityti_is_failo(path, students)) {
            std::cerr << "Failed to read file: " << path << "\n";
        }
        auto t1 = std::chrono::steady_clock::now();
        if (out_read_ms) *out_read_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        return students;
    }
    else {
        std::list<Studentas> students;
        std::vector<Studentas> tmp;
        if (skaityti_is_failo(path, tmp)) {
            for (auto& s : tmp) students.push_back(std::move(s));
        }
        else {
            std::cerr << "Failed to read file: " << path << "\n";
        }
        auto t1 = std::chrono::steady_clock::now();
        if (out_read_ms) *out_read_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        return students;
    }
}

template<typename Tag>
void split_groups(ContainerT<Tag, Studentas>& all,
    ContainerT<Tag, Studentas>& varg,
    ContainerT<Tag, Studentas>& kiet,
    int method,
    long long* out_split_ms) {
    auto t0 = std::chrono::steady_clock::now();

    varg.clear();
    kiet.clear();

    if constexpr (std::is_same_v<Tag, VectorTag>) {
        varg.reserve(all.size());
        kiet.reserve(all.size());
    }

    auto is_varg = [method](const Studentas& s) {
        return galutinis_by_method(s, method) < 5.0;
        };

    std::partition_copy(all.begin(), all.end(),
        std::back_inserter(varg),
        std::back_inserter(kiet),
        is_varg);

    auto t1 = std::chrono::steady_clock::now();
    if (out_split_ms) *out_split_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
}

template<typename Tag>
void split_groups_remove(ContainerT<Tag, Studentas>& all,
    ContainerT<Tag, Studentas>& varg,
    ContainerT<Tag, Studentas>& kiet,
    int method,
    long long* out_split_ms) {
    auto t0 = std::chrono::steady_clock::now();

    varg.clear();
    kiet.clear();

    if constexpr (std::is_same_v<Tag, VectorTag>) {
        varg.reserve(all.size());
    }

    auto is_varg = [method](const Studentas& s) {
        double g;
        if (method == 1) g = galutinis_vidurkis(s);
        else if (method == 2) g = galutinis_mediana(s);
        else g = galutinis_vidurkis(s);
        return g < 5.0;
        };

    std::copy_if(all.begin(), all.end(), std::back_inserter(varg), is_varg);

    auto it = std::remove_if(all.begin(), all.end(), is_varg);
    all.erase(it, all.end());
    kiet = all;

    auto t1 = std::chrono::steady_clock::now();
    if (out_split_ms) *out_split_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
}

template<typename Tag>
void split_groups_inplace(ContainerT<Tag, Studentas>& all,
    ContainerT<Tag, Studentas>& varg,
    ContainerT<Tag, Studentas>& kiet,
    int method,
    long long* out_split_ms) {
    auto t0 = std::chrono::steady_clock::now();

    varg.clear();
    kiet.clear();

    bool stable = false;
    std::cout << "Naudoti stable partition (1 - taip, 0 - ne): ";
    std::cin >> stable;

    auto is_varg = [method](const Studentas& s) {
        double g;
        if (method == 1) g = galutinis_vidurkis(s);
        else if (method == 2) g = galutinis_mediana(s);
        else g = galutinis_vidurkis(s);
        return g < 5.0;
        };

    auto mid = stable
        ? std::stable_partition(all.begin(), all.end(), is_varg)
        : std::partition(all.begin(), all.end(), is_varg);

    varg.insert(varg.end(), all.begin(), mid);
    kiet.insert(kiet.end(), mid, all.end());

    auto t1 = std::chrono::steady_clock::now();
    if (out_split_ms) *out_split_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
}



template<typename Tag>
void sort_groups(ContainerT<Tag, Studentas>& varg,
    ContainerT<Tag, Studentas>& kiet,
    int rikiavimas,
    long long* out_sort_ms) {
    auto t0 = std::chrono::steady_clock::now();

    if constexpr (std::is_same_v<Tag, VectorTag>) {
        if (rikiavimas == 1) {
            std::sort(varg.begin(), varg.end(), less_vardas_pavarde);
            std::sort(kiet.begin(), kiet.end(), less_vardas_pavarde);
        }
        else {
            std::sort(varg.begin(), varg.end(), less_pavarde_vardas);
            std::sort(kiet.begin(), kiet.end(), less_pavarde_vardas);
        }
    }
    else {
        if (rikiavimas == 1) {
            varg.sort(less_vardas_pavarde);
            kiet.sort(less_vardas_pavarde);
        }
        else {
            varg.sort(less_pavarde_vardas);
            kiet.sort(less_pavarde_vardas);
        }
    }

    auto t1 = std::chrono::steady_clock::now();
    if (out_sort_ms) *out_sort_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
}

template<typename Tag>
void write_groups(const ContainerT<Tag, Studentas>& varg,
    const ContainerT<Tag, Studentas>& kiet,
    int method,
    long long* out_write_ms) {
    auto t0 = std::chrono::steady_clock::now();

    if constexpr (std::is_same_v<Tag, VectorTag>) {
        failo_formatavimas("vargsiukai.txt", varg, method);
        failo_formatavimas("kietiakiai.txt", kiet, method);
    }
    else {
        std::vector<Studentas> v1(varg.begin(), varg.end());
        std::vector<Studentas> v2(kiet.begin(), kiet.end());
        failo_formatavimas("vargsiukai.txt", v1, method);
        failo_formatavimas("kietiakiai.txt", v2, method);
    }

    auto t1 = std::chrono::steady_clock::now();
    if (out_write_ms) *out_write_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    std::cout << "Saved: vargsiukai.txt (" << std::distance(varg.begin(), varg.end())
        << "), kietiakiai.txt (" << std::distance(kiet.begin(), kiet.end()) << ")\n";
}

template ContainerT<VectorTag, Studentas> read_all<VectorTag>(const std::string&, long long*);
template ContainerT<ListTag, Studentas>   read_all<ListTag>(const std::string&, long long*);
template void split_groups<VectorTag>(ContainerT<VectorTag, Studentas>&, ContainerT<VectorTag, Studentas>&, ContainerT<VectorTag, Studentas>&, int, long long*);
template void split_groups<ListTag>(ContainerT<ListTag, Studentas>&, ContainerT<ListTag, Studentas>&, ContainerT<ListTag, Studentas>&, int, long long*);
template void sort_groups<VectorTag>(ContainerT<VectorTag, Studentas>&, ContainerT<VectorTag, Studentas>&, int, long long*);
template void sort_groups<ListTag>(ContainerT<ListTag, Studentas>&, ContainerT<ListTag, Studentas>&, int, long long*);
template void write_groups<VectorTag>(const ContainerT<VectorTag, Studentas>&, const ContainerT<VectorTag, Studentas>&, int, long long*);
template void write_groups<ListTag>(const ContainerT<ListTag, Studentas>&, const ContainerT<ListTag, Studentas>&, int, long long*);
template void split_groups_remove<VectorTag>(ContainerT<VectorTag, Studentas>&, ContainerT<VectorTag, Studentas>&, ContainerT<VectorTag, Studentas>&, int, long long*);
template void split_groups_remove<ListTag>(ContainerT<ListTag, Studentas>&, ContainerT<ListTag, Studentas>&, ContainerT<ListTag, Studentas>&, int, long long*);
template void split_groups_inplace<VectorTag>(ContainerT<VectorTag, Studentas>&, ContainerT<VectorTag, Studentas>&, ContainerT<VectorTag, Studentas>&, int, long long*);
template void split_groups_inplace<ListTag>(ContainerT<ListTag, Studentas>&, ContainerT<ListTag, Studentas>&, ContainerT<ListTag, Studentas>&, int, long long*);

template<typename Tag>
void split_groups_remove(ContainerT<Tag, Studentas>& all,
ContainerT<Tag, Studentas>& varg,
ContainerT<Tag, Studentas>& kiet,
int method,
long long* out_split_ms);

template<typename Tag>
void split_groups_inplace(ContainerT<Tag, Studentas>& all,
ContainerT<Tag, Studentas>& varg,
ContainerT<Tag, Studentas>& kiet,
int method,
long long* out_split_ms);

