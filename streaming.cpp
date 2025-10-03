#include "streaming.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <array>
#include <iomanip>
#include <iostream>

#include "studentai.hpp"
#include "ivestis.hpp"     // try_parse_int
#include "formatas.hpp"    // print_col

void split_streaming(const std::string& pr, int budas) {
    std::ifstream in(pr);
    if (!in) {
        std::cerr << "Klaida atidarant faila: " << pr << "\n";
        return;
    }

    std::ofstream foutV("vargsiukai.txt");
    std::ofstream foutK("kietiakiai.txt");
    if (!foutV || !foutK) {
        std::cerr << "Nepavyko sukurti isvedimo failu (vargsiukai.txt/kietiakiai.txt)\n";
        return;
    }

    auto write_header = [&](std::ostream& out) {
        out << std::fixed << std::setprecision(2);
        print_col(out, "Pavarde", 15);
        print_col(out, "Vardas", 15);
        if (budas == 1) {
            out << "Galutinis (Vid.)\n";
            out << std::string(15 + 15 + 18, '-') << "\n";
        }
        else if (budas == 2) {
            out << "Galutinis (Med.)\n";
            out << std::string(15 + 15 + 18, '-') << "\n";
        }
        else {
            print_col(out, "Galutinis (Vid.)", 18);
            print_col(out, "Galutinis (Med.)", 18);
            out << "\n";
            out << std::string(15 + 15 + 18 + 18, '-') << "\n";
        }
        };
    write_header(foutV);
    write_header(foutK);

    std::string line;
    std::size_t nr = 0;

    while (std::getline(in, line)) {
        ++nr;
        if (nr == 1) continue; 
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;

        std::istringstream iss(line);

        Studentas stub; 
        if (!(iss >> stub.pavarde >> stub.vardas)) continue;

        std::array<int, 8> vals{}; 
        int m = 0;
        std::string tok;
        while (iss >> tok) {
            int v;
            if (try_parse_int(tok, v) && v >= 1 && v <= 10) {
                if (m < (int)vals.size()) vals[m++] = v; 
            }
        }
        if (m < 1) continue; 
        int egz = vals[m - 1];
        int nd_cnt = m - 1;
        if (nd_cnt <= 0) continue;

        long long sum = 0;
        for (int i = 0; i < nd_cnt; ++i) sum += vals[i];
        double nd_avg = static_cast<double>(sum) / nd_cnt;
        double gVid = 0.4 * nd_avg + 0.6 * egz;

        double gMed = gVid;
        if (budas != 1) {
            std::array<int, 7> tmp{};
            for (int i = 0; i < nd_cnt; ++i) tmp[i] = vals[i];
            std::sort(tmp.begin(), tmp.begin() + nd_cnt);
            double medND = (nd_cnt % 2)
                ? tmp[nd_cnt / 2]
                : (tmp[nd_cnt / 2 - 1] + tmp[nd_cnt / 2]) / 2.0;
            gMed = 0.4 * medND + 0.6 * egz;
        }

        auto write_row = [&](std::ostream& out) {
            print_col(out, stub.pavarde, 15);
            print_col(out, stub.vardas, 15);
            if (budas == 1) {
                out << std::right << std::setw(8) << gVid << "\n";
            }
            else if (budas == 2) {
                out << std::right << std::setw(8) << gMed << "\n";
            }
            else {
                out << std::right << std::setw(8) << gVid
                    << std::string(10, ' ')
                    << std::right << std::setw(8) << gMed << "\n";
            }
            };

        const double gal_ribai = (budas == 2) ? gMed : gVid; 
        if (gal_ribai < 5.0) write_row(foutV);
        else                 write_row(foutK);
    }

    std::cout << "Streaming skaidymas baigtas. Sukurti: vargsiukai.txt, kietiakiai.txt\n";
}
