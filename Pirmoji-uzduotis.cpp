#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <list>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <chrono>
#include <thread>

#include "studentai.hpp"
#include "skaiciavimas.hpp"
#include "ivestis.hpp"
#include "sort.hpp"
#include "generatorius.hpp"
#include "formatas.hpp"
#include "streaming.hpp"
#include "konteineriu_pasirinkimas.hpp"
#include "v03_runner.hpp"

int main() {
    using clock = std::chrono::steady_clock;
    auto ms = [](auto dt) { return std::chrono::duration_cast<std::chrono::milliseconds>(dt).count(); };

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
        std::uniform_int_distribution<int> distK(6, 7);
        int K = distK(rng);

        std::vector<std::size_t> N_list = { 1000, 10000, 100000 };

        std::cout << "Generavimas (K=" << K << "):\n";
        long long gen_total_ms = 0;
        for (auto N : N_list) {
            std::string vardas = "studentai_" + std::to_string(N) + "_K" + std::to_string(K) + ".txt";

            auto t0 = clock::now();
            generuoti_faila(rng, vardas, N, static_cast<std::size_t>(K));
            auto t1 = clock::now();

            long long took = ms(t1 - t0);
            gen_total_ms += took;
            sugeneruoti.push_back(vardas);
            std::cout << "  - Sugeneruotas: " << vardas << " (" << took << " ms)\n";
        }

        std::cout << "Visu " << sugeneruoti.size() << " failu generavimas: " << gen_total_ms << " ms\n";

        std::cout << "Pasirinkite kuri sugeneruota faila naudoti:\n";
        for (size_t i = 0; i < sugeneruoti.size(); i++) {
            std::cout << i + 1 << " - " << sugeneruoti[i] << "\n";
        }
        int pasirinktas_failas = 0;
        while (!(std::cin >> pasirinktas_failas) ||
            pasirinktas_failas < 1 || pasirinktas_failas >(int)sugeneruoti.size()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Blogas pasirinkimas. Bandykite dar karta: ";
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

    int konteineris = 1;
    std::cout << "Pasirinkite konteineri (1 - std::vector, 2 - std::list): \n";
    std::cout << "__________________________________________________________\n";
    while (!(std::cin >> konteineris) || (konteineris != 1 && konteineris != 2)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Prasome pasirinkti 1 arba 2: ";
    }

    std::vector<Studentas> grupe;

    if (pasirinktas_saltinis != 3) {
        int versija = 0;
        std::cout << "Pasirinkite versija (1 - v0.2 streaming, 2 - v0.3 konteineriai): \n";
        std::cout << "__________________________________________________________\n";
        while (!(std::cin >> versija) || (versija != 1 && versija != 2)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Prasome pasirinkti 1 arba 2: ";
        }

        if (versija == 1) {
            long long t_read_ms = 0, t_split_ms = 0, t_write_ms = 0;
            split_streaming(pr, vartotojo_pasirinkimas, rikiavimo_pasirinkimas,
                &t_read_ms, &t_split_ms, &t_write_ms);

            std::cout << "--------------------------------------------------\n";
            std::cout << "Santrauka (v0.2 streaming):\n";
            std::cout << "  Nuskaitymas: " << t_read_ms << " ms\n";
            std::cout << "  Skirstymas:  " << t_split_ms << " ms\n";
            std::cout << "  Isvedimas:   " << t_write_ms << " ms\n";
            std::cout << "  Bendra:      " << (t_read_ms + t_split_ms + t_write_ms) << " ms\n";
        }
        else {
            if (pasirinktas_saltinis == 2 && !sugeneruoti.empty()) {
                for (const auto& failas : sugeneruoti) {
                    std::cout << "\n=== Testas su failu: " << failas << " ===\n";
                    run_v03<VectorTag>(failas, vartotojo_pasirinkimas, rikiavimo_pasirinkimas);
                    run_v03<ListTag>(failas, vartotojo_pasirinkimas, rikiavimo_pasirinkimas);
                }
            }
            else {
                if (konteineris == 1) {
                    run_v03<VectorTag>(pr, vartotojo_pasirinkimas, rikiavimo_pasirinkimas);
                }
                else {
                    run_v03<ListTag>(pr, vartotojo_pasirinkimas, rikiavimo_pasirinkimas);
                }
            }

            std::cout << "\n--- Atminties adresu diagnostika ---\n";
            Studentas s1, s2;
            std::vector<Studentas> v;
            std::list<Studentas> l;
            v.push_back(s1);
            v.push_back(s2);
            l.push_back(s1);
            l.push_back(s2);
            std::cout << "Vector elementas 1: " << &v[0] << "\n";
            std::cout << "Vector elementas 2: " << &v[1] << "\n";
            std::cout << "List elementas 1: " << &(*l.begin()) << "\n";
            auto it = l.begin();
            ++it;
            std::cout << "List elementas 2: " << &(*it) << "\n";
        }

        return 0;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    ivedimas_is_konsoles(grupe);

    if (grupe.empty()) {
        std::cout << " Nera nei vieno studento.\n";
        return 0;
    }

    if (rikiavimo_pasirinkimas == 1) merge_sort(grupe, less_vardas_pavarde);
    else merge_sort(grupe, less_pavarde_vardas);

    auto t_split0 = clock::now();
    std::vector<Studentas> vargsiukai;
    std::vector<Studentas> kietiakiai;
    vargsiukai.reserve(grupe.size());
    kietiakiai.reserve(grupe.size());

    for (const auto& s : grupe) {
        double galutinis;
        if (vartotojo_pasirinkimas == 1) galutinis = galutinis_vidurkis(s);
        else if (vartotojo_pasirinkimas == 2) galutinis = galutinis_mediana(s);
        else                                   galutinis = galutinis_vidurkis(s);
        if (galutinis < 5.0) vargsiukai.push_back(s);
        else                 kietiakiai.push_back(s);
    }
    auto t_split1 = clock::now();
    std::cout << "Skirstymas i dvi kategorijas: "
        << ms(t_split1 - t_split0) << " ms.  (Vargsiukai: "
        << vargsiukai.size() << ", Kietiakiai: " << kietiakiai.size() << ")\n";

    std::cout << ">>> Kuriami failai...\n";
    long long write_total_ms = 0;

    auto t_w1_0 = clock::now();
    failo_formatavimas("vargsiukai.txt", vargsiukai, vartotojo_pasirinkimas);
    auto t_w1_1 = clock::now();
    long long w_vargsiukai = ms(t_w1_1 - t_w1_0);
    write_total_ms += w_vargsiukai;

    auto t_w2_0 = clock::now();
    failo_formatavimas("kietiakiai.txt", kietiakiai, vartotojo_pasirinkimas);
    auto t_w2_1 = clock::now();
    long long w_kietiakiai = ms(t_w2_1 - t_w2_0);
    write_total_ms += w_kietiakiai;

    failo_formatavimas("rezultatas.txt", grupe, vartotojo_pasirinkimas);

    std::cout << "  - vargsiukai.txt: " << w_vargsiukai << " ms\n";
    std::cout << "  - kietiakiai.txt: " << w_kietiakiai << " ms\n";
    std::cout << "Isvedimo i du failus suma: " << write_total_ms << " ms\n";
    std::cout << ">>> Viskas baigta.\n";

    return 0;
}
