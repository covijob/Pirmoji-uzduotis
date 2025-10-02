#pragma once
#include <string>
#include <random>

void generuoti_faila(std::mt19937& rng, const std::string& failo_vardas,
    std::size_t N, std::size_t K, int minP = 1, int maxP = 10);
