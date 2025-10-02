#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>
#include <random>
#include <cstdlib>

#include "studentai.hpp"
#include "skaiciavimas.hpp"
#include "ivestis.hpp"
#include "sort.hpp"
#include "generatorius.hpp"
#include "formatas.hpp"


int main() {
    std::ios::sync_with_stdio(false);

    std::string pr = "studentai1.txt";

    int pasirinktas_saltinis = 1;
    std::cout << "Pasirinkite duomenu saltini (1 - txt failas, 2 - atsitiktinai generuoti duomenys, 3 - rankinis ivedimas): \n";
    std::cout << "__________________________________________________________\n";
    if (!(std::cin >> pasirinktas_saltinis) || (pasirinktas_saltinis < 1 || pasirinktas_saltinis > 3)) {
        std::cout << "Prasome pasirinkti 1, 2 arba 3.\n";
        return 1;
    }

    std::vector<std::string> sugeneruoti;

    if (pasirinktas_saltinis == 2) {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> distK(5, 15);
        int K = distK(rng);

        std::vector<std::size_t> N_list = { 1000, 10000, 100000, 1000000}; //10000000

        std::cout << "Generavimas (K=" << K << "):\n";
        for (auto N : N_list) {
            std::string vardas = "studentai_" + std::to_string(N) + "_K" + std::to_string(K) + ".txt";
            generuoti_faila(rng, vardas, N, static_cast<std::size_t>(K));
            sugeneruoti.push_back(vardas);
            std::cout << "  - Sugeneruotas: " << vardas << "\n";
        }

        std::cout << "Pasirinkite kuri sugeneruota faila naudoti:\n";
        for (size_t i = 0; i < sugeneruoti.size(); i++) {
            std::cout << i + 1 << " - " << sugeneruoti[i] << "\n";
        }
        int pasirinktas_failas = 0;
        while (!(std::cin >> pasirinktas_failas) ||
            pasirinktas_failas < 1 || pasirinktas_failas >(int)sugeneruoti.size()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Blogas pasirinkimas. Bandykite dar kartą: ";
        }
        pr = sugeneruoti[pasirinktas_failas - 1];
        std::cout << ">> Pasirinktas failas: " << pr << "\n";
    }

    int vartotojo_pasirinkimas = 1;
    std::cout << "Pasirinkite skaiciavimo buda (1 - vidurkis, 2 - mediana, 3 - vidurkis ir mediana): \n";
    std::cout << "__________________________________________________________\n";
    while (!(std::cin >> vartotojo_pasirinkimas) || (vartotojo_pasirinkimas < 1 || vartotojo_pasirinkimas > 3)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Prasome pasirinkti 1, 2 arba 3: ";
    }

    int rikiavimo_pasirinkimas = 1;
    std::cout << "Pasirinkite rikiavimo parametra (1 - studento vardas, 2 - studento pavarde)\n";
    std::cout << "__________________________________________________________\n";
    while (!(std::cin >> rikiavimo_pasirinkimas) || (rikiavimo_pasirinkimas != 1 && rikiavimo_pasirinkimas != 2)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Prasome pasirinkti 1 arba 2: ";
    }

    std::vector<Studentas> grupe;

    if (pasirinktas_saltinis == 3) {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        ivedimas_is_konsoles(grupe);
    }
    else {
        std::cout << ">>> nuskaitomas txt: " << pr << " ...\n";
        std::ifstream in(pr);
        if (!in) {
            std::cout << "Klaida atidarant faila: " << pr << std::endl;
            return 1;
        }

        std::string eilute;
        size_t nr = 0;
        std::size_t irasu = 0;

        while (std::getline(in, eilute)) {
            ++nr;
            if (nr == 1) continue; 
            if (eilute.find_first_not_of(" \t\n\r") == std::string::npos) continue;

            std::istringstream iss(eilute);
            Studentas s;
            if (!(iss >> s.pavarde >> s.vardas)) continue;

            std::vector<int> visi_skaiciai;
            std::string tok;
            while (iss >> tok) {
                int v;
                if (try_parse_int(tok, v) && v >= 1 && v <= 10) visi_skaiciai.push_back(v);
            }
            if (visi_skaiciai.empty()) continue;

            s.egzaminas = visi_skaiciai.back();
            s.nd.assign(visi_skaiciai.begin(), visi_skaiciai.end() - 1);

            grupe.push_back(std::move(s));
            ++irasu;

            if (irasu % 100000 == 0) {
                std::cout << "  - Nuskaityta " << irasu << " irasu...\n";
            }
        }
        std::cout << ">>> NUSKAITYTA: " << irasu << " irasu.\n";
    }

    if (grupe.empty()) {
        std::cout << "Nera nei vieno studento.\n";
        return 0;
    }

    std::cout << ">>> Rikiuojama (" << (rikiavimo_pasirinkimas == 1 ? "vardas" : "pavarde") << ")...\n";
    if (rikiavimo_pasirinkimas == 1) merge_sort(grupe, less_vardas_pavarde);
    else merge_sort(grupe, less_pavarde_vardas);
    std::cout << ">>> Rikiavimas baigtas.\n";

    std::cout << ">>> Skirstau i vargsiukus / kietiakius...\n";
    std::vector<Studentas> vargsiukai;
    std::vector<Studentas> kietiakiai;
    vargsiukai.reserve(grupe.size());
    kietiakiai.reserve(grupe.size());

    for (const auto& s : grupe) {
        double galutinis;
        if (vartotojo_pasirinkimas == 1) galutinis = galutinis_vidurkis(s);
        else if (vartotojo_pasirinkimas == 2) galutinis = galutinis_mediana(s);
        else galutinis = galutinis_vidurkis(s);
        if (galutinis < 5.0) vargsiukai.push_back(s);
        else kietiakiai.push_back(s);
    }
    std::cout << ">>> Skirstymas baigtas. Vargsiukai: " << vargsiukai.size()
        << ", Kietiakai: " << kietiakiai.size() << "\n";

    std::cout << ">>> formatuojami rezultatu txt...\n";
    failo_formatavimas("rezultatas.txt", grupe, vartotojo_pasirinkimas);
    failo_formatavimas("vargsiukai.txt", vargsiukai, vartotojo_pasirinkimas);
    failo_formatavimas("kietiakiai.txt", kietiakiai, vartotojo_pasirinkimas);
    std::cout << ">>> Viskas baigta.\n";

    return 0;
}
