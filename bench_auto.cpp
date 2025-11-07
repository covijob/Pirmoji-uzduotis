#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <system_error>

#include "konteineriu_pasirinkimas.hpp"
#include "skaiciavimas.hpp"
#include "v03_api.hpp"

namespace fs = std::filesystem;
using Steady = std::chrono::steady_clock;

static inline long long ms(Steady::time_point a, Steady::time_point b) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count();
}

static inline double grade_by(int method, const Studentas& s) {
    if (method == 2) return galutinis_mediana(s);
    return galutinis_vidurkis(s);
}

static void try_collect_from(const fs::path& dir, std::vector<std::string>& out) {
    std::error_code ec;
    if (!fs::exists(dir, ec) || !fs::is_directory(dir, ec)) return;
    for (fs::directory_iterator it(dir, ec); !ec && it != fs::directory_iterator(); it.increment(ec)) {
        const auto& p = it->path();
        if (!it->is_regular_file(ec)) continue;
        auto name = p.filename().string();
        if (name.rfind("studentai_", 0) == 0 && p.extension() == ".txt") {
            out.push_back(p.string());
        }
    }
}

static std::vector<std::string> collect_files() {
    std::vector<std::string> files;
    fs::path base = fs::current_path();

    std::vector<fs::path> search_paths = {
        base,
        base.parent_path(),
        base.parent_path().parent_path(),
        base / "Debug",
        base / "x64" / "Debug"
    };

    std::error_code ec;
    for (const auto& dir : search_paths) {
        if (!fs::exists(dir, ec) || !fs::is_directory(dir, ec)) continue;
        for (auto it = fs::directory_iterator(dir, ec); it != fs::directory_iterator(); it.increment(ec)) {
            if (ec) continue;
            const auto& p = it->path();
            auto name = p.filename().string();
            if (name.rfind("studentai_", 0) == 0 && p.extension() == ".txt") {
                files.push_back(p.string());
            }
        }
    }

    std::sort(files.begin(), files.end());
    files.erase(std::unique(files.begin(), files.end()), files.end());

    std::cout << "Rasti failai:\n";
    for (const auto& f : files) std::cout << " - " << f << "\n";
    std::cout.flush();

    return files;
}


template<typename Tag>
void bench_one_file(const std::string& path, int method, int rikiavimas, int strategy) {
    long long t_read = 0, t_split = 0, t_sort = 0, t_write = 0;

    auto all = read_all<Tag>(path, &t_read);

    ContainerT<Tag, Studentas> varg, kiet;

    auto is_varg = [method](const Studentas& s) {
        return grade_by(method, s) < 5.0;
        };

    auto t0 = Steady::now();

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

    auto t1 = Steady::now();
    t_split = ms(t0, t1);

    sort_groups<Tag>(varg, kiet, rikiavimas, &t_sort);
    write_groups<Tag>(varg, kiet, method, &t_write);

    std::cout << "[" << (std::is_same_v<Tag, VectorTag> ? "vector" : "list")
        << "] file=" << path
        << " strategy=" << strategy
        << " read=" << t_read << "ms"
        << " split=" << t_split << "ms"
        << " sort=" << t_sort << "ms"
        << " write=" << t_write << "ms"
        << " total=" << (t_read + t_split + t_sort + t_write) << "ms\n";
    std::cout.flush();
}

int main() {
    std::cout << "Benchmark start\n";
    std::cout.flush();

    auto files = collect_files();
    std::cout << "Found " << files.size() << " files\n";
    std::cout.flush();

    if (files.empty()) {
        std::cout << "No studentai_*.txt files near executable\n";
        return 0;
    }

    int method = 1;
    int rikiavimas = 1;

    for (const auto& f : files) {
        std::cout << "\n=== Analyzing file: " << f << " ===\n";
        std::cout.flush();

        for (int s = 1; s <= 3; ++s) {
            try { bench_one_file<VectorTag>(f, method, rikiavimas, s); }
            catch (const std::exception& e) { std::cout << "vector error: " << e.what() << "\n"; }
            try { bench_one_file<ListTag>(f, method, rikiavimas, s); }
            catch (const std::exception& e) { std::cout << "list error: " << e.what() << "\n"; }
        }
        std::cout << "---- Done file ----\n";
        std::cout.flush();
    }

    std::cout << "\nBenchmark done\n";
    return 0;
}
