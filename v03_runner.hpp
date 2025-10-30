#pragma once
#include <iostream>
#include "v03_api.hpp"

template<typename Tag>
void run_v03(const std::string& path, int method, int rikiavimas) {
    long long t_read = 0, t_split = 0, t_sort = 0, t_write = 0;

    auto all = read_all<Tag>(path, &t_read);

    ContainerT<Tag, Studentas> varg, kiet;
    split_groups<Tag>(all, varg, kiet, method, &t_split);
    sort_groups<Tag>(varg, kiet, rikiavimas, &t_sort);
    write_groups<Tag>(varg, kiet, method, &t_write);

    std::cout << "=== v0.3 (" << (std::is_same_v<Tag, VectorTag> ? "vector" : "list") << ") ===\n";
    std::cout << "Read:  " << t_read << " ms\n";
    std::cout << "Split: " << t_split << " ms\n";
    std::cout << "Sort:  " << t_sort << " ms\n";
    std::cout << "Write: " << t_write << " ms\n";
    std::cout << "Total: " << (t_read + t_split + t_sort + t_write) << " ms\n";
}
