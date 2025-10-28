#include "streaming.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <string>

#include "ivestis.hpp"     // try_parse_int
#include "formatas.hpp"    // print_col

namespace {
    using clock = std::chrono::steady_clock;
    inline long long ms(clock::time_point a, clock::time_point b) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count();
    }

    struct Row {
        std::string pavarde;
        std::string vardas;
        double gVid = 0.0;
        double gMed = 0.0;
    };

    inline void write_header(std::ostream& out, int budas) {
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
    }

    inline void write_row(std::ostream& out, const Row& r, int budas) {
        print_col(out, r.pavarde, 15);
        print_col(out, r.vardas, 15);
        if (budas == 1) {
            out << std::right << std::setw(8) << r.gVid << "\n";
        }
        else if (budas == 2) {
            out << std::right << std::setw(8) << r.gMed << "\n";
        }
        else {
            out << std::right << std::setw(8) << r.gVid
                << std::string(10, ' ')
                << std::right << std::setw(8) << r.gMed << "\n";
        }
    }
}


void split_streaming(
    const std::string& pr,
    int budas,
    int rikiavimo_pasirinkimas,
    long long* out_read_ms,
    long long* out_split_ms,
    long long* out_write_ms
) {
    std::ifstream in(pr);
    if (!in) {
        std::cerr << "Klaida atidarant faila: " << pr << "\n";
        if (out_read_ms)  *out_read_ms = 0;
        if (out_split_ms) *out_split_ms = 0;
        if (out_write_ms) *out_write_ms = 0;
        return;
    }

 
    long long read_ms = 0;  
    long long split_ms = 0;  
    long long write_ms = 0;  

    std::vector<Row> vargsiukai;
    std::vector<Row> kietiakiai;
    vargsiukai.reserve(1024);
    kietiakiai.reserve(1024);

    std::string line;
    std::size_t nr = 0;

    for (;;) {
        auto tr0 = clock::now();
        if (!std::getline(in, line)) break;
        ++nr;
        if (nr == 1) { 
            continue;
        }
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        std::istringstream iss(line);

        Row row;
        if (!(iss >> row.pavarde >> row.vardas)) {
            auto tr1_bad = clock::now();
            read_ms += ms(tr0, tr1_bad);
            continue;
        }

        std::array<int, 8> vals{};  
        int m = 0;
        std::string tok;
        while (iss >> tok) {
            int v;
            if (try_parse_int(tok, v) && v >= 1 && v <= 10) {
                if (m < (int)vals.size()) vals[m++] = v;
            }
        }
        auto tr1 = clock::now();
        read_ms += ms(tr0, tr1);

        if (m < 1) continue;    
        int egz = vals[m - 1];
        int nd_cnt = m - 1;
        if (nd_cnt <= 0) continue;

        auto ts0 = clock::now();

        long long sum = 0;
        for (int i = 0; i < nd_cnt; ++i) sum += vals[i];
        double nd_avg = static_cast<double>(sum) / nd_cnt;
        row.gVid = 0.4 * nd_avg + 0.6 * egz;

        if (budas == 1) {
            row.gMed = row.gVid;
        }
        else {
            std::array<int, 7> tmp{};
            for (int i = 0; i < nd_cnt; ++i) tmp[i] = vals[i];
            std::sort(tmp.begin(), tmp.begin() + nd_cnt);
            double medND = (nd_cnt % 2)
                ? tmp[nd_cnt / 2]
                : (tmp[nd_cnt / 2 - 1] + tmp[nd_cnt / 2]) / 2.0;
            row.gMed = 0.4 * medND + 0.6 * egz;
        }

        const double gal = (budas == 2) ? row.gMed : row.gVid;
        if (gal < 5.0) vargsiukai.push_back(row);
        else           kietiakiai.push_back(row);

        auto ts1 = clock::now();
        split_ms += ms(ts0, ts1);
    }

    {
        auto ts0 = clock::now();
        auto cmp_vardas = [](const Row& a, const Row& b) {
            if (a.vardas == b.vardas) return a.pavarde < b.pavarde;
            return a.vardas < b.vardas;
            };
        auto cmp_pavarde = [](const Row& a, const Row& b) {
            if (a.pavarde == b.pavarde) return a.vardas < b.vardas;
            return a.pavarde < b.pavarde;
            };
        if (rikiavimo_pasirinkimas == 1) {
            std::sort(vargsiukai.begin(), vargsiukai.end(), cmp_vardas);
            std::sort(kietiakiai.begin(), kietiakiai.end(), cmp_vardas);
        }
        else if (rikiavimo_pasirinkimas == 2) {
            std::sort(vargsiukai.begin(), vargsiukai.end(), cmp_pavarde);
            std::sort(kietiakiai.begin(), kietiakiai.end(), cmp_pavarde);
        }
        auto ts1 = clock::now();
        split_ms += ms(ts0, ts1);
    }

    auto tw0 = clock::now();
    {
        std::ofstream foutV("vargsiukai.txt");
        std::ofstream foutK("kietiakiai.txt");
        if (!foutV || !foutK) {
            std::cerr << "Nepavyko sukurti isvedimo failu (vargsiukai.txt/kietiakiai.txt)\n";
            if (out_read_ms)  *out_read_ms = read_ms;
            if (out_split_ms) *out_split_ms = split_ms;
            if (out_write_ms) *out_write_ms = 0;
            return;
        }
        write_header(foutV, budas);
        write_header(foutK, budas);

        for (const auto& r : vargsiukai) write_row(foutV, r, budas);
        for (const auto& r : kietiakiai) write_row(foutK, r, budas);
    }
    auto tw1 = clock::now();
    write_ms += ms(tw0, tw1);

    std::cout << "Streaming skaidymas baigtas. Sukurti: vargsiukai.txt, kietiakiai.txt\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "Duomenu nuskaitymas: " << read_ms << " ms\n";
    std::cout << "Skirstymas i dvi grupes"
        " (skaiciavimai + priskyrimas"
        << (rikiavimo_pasirinkimas == 1 || rikiavimo_pasirinkimas == 2 ? " + rikiavimas" : "")
        << "): " << split_ms << " ms "
        << "(Vargsiukai: " << vargsiukai.size()
        << ", Kietiakiai: " << kietiakiai.size() << ")\n";
    std::cout << "Isvedimas i du failus (rasymas): " << write_ms << " ms\n";
    std::cout << "Bendra: " << (read_ms + split_ms + write_ms) << " ms\n";

    if (out_read_ms)  *out_read_ms = read_ms;
    if (out_split_ms) *out_split_ms = split_ms;
    if (out_write_ms) *out_write_ms = write_ms;
}
