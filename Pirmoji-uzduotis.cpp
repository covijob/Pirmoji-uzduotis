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
#include <chrono>
#include <thread>
#include <filesystem>  

#include "studentai.hpp"
#include "skaiciavimas.hpp"
#include "ivestis.hpp"
#include "sort.hpp"
#include "generatorius.hpp"
#include "formatas.hpp"
#include "streaming.hpp"

namespace fs = std::filesystem;

int main() {
    using clock = std::chrono::steady_clock;
    auto ms = [](auto dt) { return std::chrono::duration_cast<std::chrono::milliseconds>(dt).count(); };

    std::ios::sync_with_stdio(false);

    std::string pr = "studentai1.txt";

    int pasirinktas_saltinis = 1;

    std::cout << "Pasirinkite veiksma:\n";
    std::cout << "  1 - naudoti jau sugeneruotus / esamus studentu failus\n";
    std::cout << "  2 - sugeneruoti naujus atsitiktinius failus (be tolimesnio apdorojimo)\n";
    std::cout << "  3 - rankinis ivedimas is klaviaturos\n";
    std::cout << "__________________________________________________________\n";

    if (!(std::cin >> pasirinktas_saltinis) || (pasirinktas_saltinis < 1 || pasirinktas_saltinis > 3)) {
        std::cout << "Prasome pasirinkti 1, 2 arba 3.\n";
        return 1;
    }

    if (pasirinktas_saltinis == 1) {
        std::vector<std::string> rasti_failai;

        for (const auto& entry : fs::directory_iterator(fs::current_path())) {
            if (!entry.is_regular_file()) continue;
            auto path = entry.path();

            if (path.extension() == ".txt") {
                std::string fname = path.filename().string();

                if (fname.rfind("studentai_", 0) == 0) {
                    rasti_failai.push_back(fname);
                }
            }
        }

        if (rasti_failai.empty()) {
            std::cout << "Nerasta nei vieno failo, prasidedancio 'studentai_' dabartiniame kataloge.\n";
            std::cout << "Pirmiau sugeneruokite failus (pasirinkimas 2).\n";
            return 0;
        }

        std::sort(rasti_failai.begin(), rasti_failai.end());

        std::cout << "Rasti failai:\n";
        for (std::size_t i = 0; i < rasti_failai.size(); ++i) {
            std::cout << "  " << (i + 1) << " - " << rasti_failai[i] << "\n";
        }

        std::cout << "Pasirinkite faila pagal numeri: ";
        int failo_indeksas = 0;
        while (!(std::cin >> failo_indeksas) ||
            failo_indeksas < 1 ||
            failo_indeksas > static_cast<int>(rasti_failai.size())) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Blogas pasirinkimas. Bandykite dar karta: ";
        }

        pr = rasti_failai[failo_indeksas - 1];
        std::cout << ">> Pasirinktas failas: " << pr << "\n";
    }

    if (pasirinktas_saltinis == 2) {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> distK(6, 7);
        int K = distK(rng);

        std::vector<std::size_t> N_list = { 1000, 10000, 100000, 1000000, 10000000 };

        std::cout << "Generavimas (K=" << K << "):\n";
        long long gen_total_ms = 0;
        std::vector<std::string> sugeneruoti;

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
        std::cout << "Visu 5 failu generavimas: " << gen_total_ms << " ms\n";

        std::cout << ">>> Generavimas baigtas. Siuos failus veliau galesite naudoti pasirinkdami 1 varianta.\n";
        return 0;
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

    if (pasirinktas_saltinis == 1) {
        long long t_read_ms = 0, t_split_ms = 0, t_write_ms = 0;
        split_streaming(pr, vartotojo_pasirinkimas, rikiavimo_pasirinkimas,
            &t_read_ms, &t_split_ms, &t_write_ms);

        std::cout << "--------------------------------------------------\n";
        std::cout << "Santrauka:\n";
        std::cout << "  Nuskaitymas: " << t_read_ms << " ms\n";
        std::cout << "  Skirstymas:  " << t_split_ms << " ms\n";
        std::cout << "  Isvedimas:   " << t_write_ms << " ms\n";
        std::cout << "  Bendra:      " << (t_read_ms + t_split_ms + t_write_ms) << " ms\n";
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
        if (vartotojo_pasirinkimas == 1)      galutinis = galutinis_vidurkis(s);
        else if (vartotojo_pasirinkimas == 2) galutinis = galutinis_mediana(s);
        else                                  galutinis = galutinis_vidurkis(s);

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
