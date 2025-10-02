#include "generatorius.hpp"
#include <fstream>
#include <iostream>

void generuoti_faila(std::mt19937& rng, const std::string& failo_vardas,
    std::size_t N, std::size_t K, int minP, int maxP) {
    std::ofstream out(failo_vardas);
    if (!out) {
        std::cerr << "Nepavyko sukurti failo: " << failo_vardas << '\n';
        return;
    }

    out << "Pavarde Vardas";
    for (std::size_t i = 1; i <= K; i++) out << " ND" << i;
    out << " Egzaminas\n";

    std::uniform_int_distribution<int> distPaz(minP, maxP);

    for (std::size_t j = 1; j <= N; j++) {
        out << "Pavarde" << j << " Vardas" << j;
        for (std::size_t k = 1; k <= K; k++) out << " " << distPaz(rng);
        out << " " << distPaz(rng) << "\n";
    }
}
