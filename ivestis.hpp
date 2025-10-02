#pragma once
#include <string>
#include <vector>
#include "studentai.hpp"

bool try_parse_int(const std::string& tok, int& out);
void ivedimas_is_konsoles(std::vector<Studentas>& grupe);
bool skaityti_is_failo(const std::string& path, std::vector<Studentas>& grupe);
