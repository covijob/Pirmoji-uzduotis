#pragma once
#include <string>
#include <vector>
#include <ostream>
#include "studentai.hpp"

std::size_t u8len(const std::string& s);
void print_col(std::ostream& out, const std::string& text, std::size_t width);

void failo_formatavimas(const std::string& failo_vardas,
    const std::vector<Studentas>& grupe,
    int vartotojo_pasirinkimas);
