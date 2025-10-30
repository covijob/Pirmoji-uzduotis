#pragma once
#include <iostream>
#include <type_traits>

#include "konteineriu_pasirinkimas.hpp" 
#include "streaming.hpp"                

template<typename Tag>
void run_v03(const std::string& path, int method, int rikiavimas) {
    long long t_read = 0, t_split = 0, t_write = 0;

    split_streaming(path, method, rikiavimas, &t_read, &t_split, &t_write);

    std::cout << "=== v0.3 ("
        << (std::is_same_v<Tag, VectorTag> ? "vector" : "list")
        << ") – laikinai naudoja v0.2 streaming kelią ===\n";
    std::cout << "Read:  " << t_read << " ms\n";
    std::cout << "Split: " << t_split << " ms\n";
    std::cout << "Write: " << t_write << " ms\n";
    std::cout << "Total: " << (t_read + t_split + t_write) << " ms\n";
}
