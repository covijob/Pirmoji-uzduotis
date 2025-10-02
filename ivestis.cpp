#include "ivestis.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <cctype>

bool try_parse_int(const std::string& tok, int& out) {
    if (tok.empty()) return false;
    size_t i = 0;
    if (tok[0] == '+' || tok[0] == '-') i = 1;
    if (i == tok.size()) return false;
    for (; i < tok.size(); ++i) if (!std::isdigit((unsigned char)tok[i])) return false;
    try {
        long long v = std::stoll(tok);
        if (v < std::numeric_limits<int>::min() || v > std::numeric_limits<int>::max()) return false;
        out = (int)v;
        return true;
    }
    catch (...) { return false; }
}

void ivedimas_is_konsoles(std::vector<Studentas>& grupe) {
    using std::string; using std::getline;

    for (;;) {
        std::cout << "Iveskite: PAVARDE ir VARDA (tuscia eilute - baigti): ";
        string line; getline(std::cin >> std::ws, line);
        if (line.empty()) break;

        std::istringstream pv(line);
        Studentas s;
        if (!(pv >> s.pavarde >> s.vardas)) {
            std::cout << "Nerasta pavarde/vardas. Bandykite dar.\n";
            continue;
        }

        std::cout << "Iveskite ND pazymius VIENOJE eiluteje (pvz.: 10 9 8). Tuscia eilute - pabaiga: ";
        getline(std::cin, line);
        if (!line.empty()) {
            std::istringstream nds(line);
            std::string tok;
            while (nds >> tok) {
                int v;
                if (try_parse_int(tok, v) && v >= 1 && v <= 10) s.nd.push_back(v);
                else std::cout << "Ignoruojama ND reiksme: '" << tok << "'\n";
            }
        }

        for (;;) {
            std::cout << "Egzamino pazymys (1-10): ";
            getline(std::cin, line);
            std::istringstream es(line);
            if (es >> s.egzaminas && s.egzaminas >= 1 && s.egzaminas <= 10) break;
            std::cout << "Neteisinga ivestis. Bandykite dar.\n";
        }

        grupe.push_back(std::move(s));

        std::cout << "Prideta. Enter - prideti kita, arba iveskite 'q' baigti: ";
        getline(std::cin, line);
        if (!line.empty() && (line == "q" || line == "Q")) break;
    }
}

bool skaityti_is_failo(const std::string& path, std::vector<Studentas>& grupe) {
    std::ifstream in(path);
    if (!in) {
        std::cerr << "Klaida atidarant faila: " << path << "\n";
        return false;
    }

    std::string eilute;
    size_t nr = 0;
    while (std::getline(in, eilute)) {
        ++nr;
        if (nr == 1) continue; // praleidžiam antraštę
        if (eilute.find_first_not_of(" \t\r\n") == std::string::npos) continue;

        std::istringstream iss(eilute);
        Studentas s;
        if (!(iss >> s.pavarde >> s.vardas)) continue;

        std::vector<int> visi;
        std::string tok;
        while (iss >> tok) {
            int v;
            if (try_parse_int(tok, v) && v >= 1 && v <= 10) visi.push_back(v);
        }
        if (visi.empty()) continue;

        s.egzaminas = visi.back();
        s.nd.assign(visi.begin(), visi.end() - 1);

        grupe.push_back(std::move(s));
    }
    return true;
}
