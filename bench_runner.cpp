#include <iostream>
#include <filesystem>
#include <vector>
#include <chrono>
#include "konteineriu_pasirinkimas.hpp"
#include "v03_runner.hpp"

namespace fs = std::filesystem;

int main() {
    std::vector<std::string> failai;
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".txt" && entry.path().string().find("studentai_") != std::string::npos)
            failai.push_back(entry.path().string());
    }

    if (failai.empty()) {
        std::cout << "Nerasta studentu failu kataloge.\n";
        return 0;
    }

    std::cout << "Rasti failai:\n";
    for (auto& f : failai) std::cout << " - " << f << "\n";

    int skaiciavimo_budas = 1;
    int rikiavimas = 1;

    for (const auto& failas : failai) {
        std::cout << "\n=== Testuojamas failas: " << failas << " ===\n";
        run_v03<VectorTag>(failas, skaiciavimo_budas, rikiavimas);
        run_v03<ListTag>(failas, skaiciavimo_budas, rikiavimas);
    }

    std::cout << "\nTestai baigti.\n";
    return 0;
}
