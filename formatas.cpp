#include "formatas.hpp"
#include "skaiciavimas.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

std::size_t u8len(const std::string& s) {
    std::size_t n = 0;
    for (unsigned char c : s) if ((c & 0xC0) != 0x80) ++n;
    return n;
}

void print_col(std::ostream& out, const std::string& text, std::size_t width) {
    out << text;
    std::size_t len = u8len(text);
    if (len < width) out << std::string(width - len, ' ');
}

void failo_formatavimas(const std::string& failo_vardas,
    const std::vector<Studentas>& grupe,
    int vartotojo_pasirinkimas) {
    std::ofstream out(failo_vardas);
    if (!out) {
        std::cerr << "Nepavyko sukurti failo " << failo_vardas << "\n";
        return;
    }

    out << std::fixed << std::setprecision(2);

    print_col(out, "Pavarde", 15);
    print_col(out, "Vardas", 15);

    if (vartotojo_pasirinkimas == 1) {
        out << "Galutinis (Vid.)\n";
        out << std::string(15 + 15 + 18, '-') << "\n";
    }
    else if (vartotojo_pasirinkimas == 2) {
        out << "Galutinis (Med.)\n";
        out << std::string(15 + 15 + 18, '-') << "\n";
    }
    else {
        print_col(out, "Galutinis (Vid.)", 18);
        print_col(out, "Galutinis (Med.)", 18);
        out << "\n";
        out << std::string(15 + 15 + 18 + 18, '-') << "\n";
    }

    for (const auto& s : grupe) {
        const double gVid = galutinis_vidurkis(s);
        const double gMed = galutinis_mediana(s);

        print_col(out, s.pavarde, 15);
        print_col(out, s.vardas, 15);

        if (vartotojo_pasirinkimas == 1) {
            out << std::right << std::setw(8) << gVid << "\n";
        }
        else if (vartotojo_pasirinkimas == 2) {
            out << std::right << std::setw(8) << gMed << "\n";
        }
        else {
            out << std::right << std::setw(8) << gVid;
            out << std::string(10, ' ');
            out << std::right << std::setw(8) << gMed << "\n";
        }
    }
    std::cout << "Rezultatai issaugoti faile: " << failo_vardas << "\n";
}
