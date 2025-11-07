#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <chrono>
#include <filesystem>

#include "konteineriu_pasirinkimas.hpp"
#include "skaiciavimas.hpp"
#include "v03_api.hpp"

namespace fs = std::filesystem;
using clock_t = std::chrono::steady_clock;

static inline long long ms(std::chrono::steady_clock::time_point a, std::chrono::steady_clock::time_point b) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count();
}

static inline double grade_by(int method, const Studentas& s) {
    if (method == 2) return galutinis_mediana(s);
    return galutinis_vidurkis(s);
}

template<typename Tag>
void bench_one_file(const std::string& path, int method, int rikiavimas, int strategy) {
    long long t_read = 0, t_split = 0, t_sort = 0, t_write = 0;

    auto all = read_all<Tag>(path, &t_read);

    ContainerT<Tag, Studentas> varg, kiet;

    auto t0 = clock_t::now();

    auto is_varg = [method](const Studentas& s) {
        return grade_by(method, s) < 5.0;
        };

    if (strategy == 1) {
        varg.clear(); kiet.clear();
        if constexpr (std::is_same_v<Tag, VectorTag>) {
            varg.reserve(all.size());
            kiet.reserve(all.size());
        }
        std::partition_copy(all.begin(), all.end(),
            std::back_inserter(varg),
            std::back_inserter(kiet),
            is_varg);
    }
    else if (strategy == 2) {
        varg.clear(); kiet.clear();
        if constexpr (std::is_same_v<Tag, VectorTag>) varg.reserve(all.size());
        std::copy_if(all.begin(), all.end(), std::back_inserter(varg), is_varg);
        auto it = std::remove_if(all.begin(), all.end(), is_varg);
        all.erase(it, all.end());
        kiet = all;
    }
    else {
        varg.clear(); kiet.clear();
        auto mid = std::partition(all.begin(), all.end(), is_varg);
        varg.insert(varg.end(), all.begin(), mid);
        kiet.insert(kiet.end(), mid, all.end());
    }

    auto t1 = clock_t::now();
    t_split = ms(t0, t1);

    sort_groups<Tag>(varg, kiet, rikiavimas, &t_sort);
    write_groups<Tag>(varg, kiet, method, &t_write);

    std::cout << "["
        << (std::is_same_v<Tag, VectorTag> ? "vector" : "list")
        << "] file=" << path
        << " strategy=" << strategy
        << " read=" << t_read << "ms"
        << " split=" << t_split << "ms"
        << " sort=" << t_sort << "ms"
        << " write=" << t_write << "ms"
        << " total=" << (t_read + t_split + t_sort + t_write) << "ms\n";
}

int main() {
    std::vector<std::string> files;
    for (const auto& e : fs::directory_iterator(".")) {
        if (!e.is_regular_file()) continue;
        auto p = e.path().string();
        if (p.find("studentai_") != std::string::npos && p.rfind(".txt") == p.size() - 4) {
            files.push_back(p);
        }
    }
    if (files.empty()) {
        std::cout << "No studentai_*.txt files found.\n";
        return 0;
    }
    std::sort(files.begin(), files.end());

    int method = 1;
    int rikiavimas = 1;

    std::cout << "Benchmark start\n";
    for (const auto& f : files) {
        for (int s = 1; s <= 3; ++s) {
            bench_one_file<VectorTag>(f, method, rikiavimas, s);
            bench_one_file<ListTag>(f, method, rikiavimas, s);
        }
        std::cout << "----\n";
    }
    std::cout << "Benchmark done\n";
    return 0;
}
