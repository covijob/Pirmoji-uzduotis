#pragma once
#include <string>
#include "studentai.hpp"
#include "konteineriu_pasirinkimas.hpp"

template<typename Tag>
ContainerT<Tag, Studentas> read_all(const std::string& path, long long* out_read_ms);

template<typename Tag>
void split_groups(ContainerT<Tag, Studentas>& all,
    ContainerT<Tag, Studentas>& varg,
    ContainerT<Tag, Studentas>& kiet,
    int method,    
    long long* out_split_ms);

template<typename Tag>
void sort_groups(ContainerT<Tag, Studentas>& varg,
    ContainerT<Tag, Studentas>& kiet,
    int rikiavimas,
    long long* out_sort_ms);

template<typename Tag>
void write_groups(const ContainerT<Tag, Studentas>& varg,
    const ContainerT<Tag, Studentas>& kiet,
    int method,
    long long* out_write_ms);
