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

struct Studentas {
    std::string vardas;
    std::string pavarde;
    std::vector<int> nd;
    int egzaminas = 0;
};

double vidurkis(const std::vector<int>& a) {
    if (a.empty()) return 0.0;
    long long s = 0;
    for (int x : a) s += x;
    return static_cast<double>(s) / a.size();
}

double mediana(std::vector<int> a) {
    if (a.empty()) return 0.0;
    std::sort(a.begin(), a.end());
    size_t n = a.size();
    if (n % 2 == 0) return (a[n / 2 - 1] + a[n / 2]) / 2.0;
    else return a[n / 2];
}

double galutinis_vidurkis(const Studentas& s) {
    return 0.4 * vidurkis(s.nd) + 0.6 * s.egzaminas;
}

double galutinis_mediana(const Studentas& s) {
    return 0.4 * mediana(s.nd) + 0.6 * s.egzaminas;
}

std::size_t u8len(const std::string& s) {
    std::size_t n = 0;
    for (unsigned char c : s) {
        if ((c & 0xC0) != 0x80) ++n;
    }
    return n;
}

void print_col(std::ostream& out, const std::string& text, std::size_t width) {
    out << text;
    std::size_t len = u8len(text);
    if (len < width) out << std::string(width - len, ' ');
}

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
    using std::string;
    using std::getline;

    for (;;) {
        std::cout << "Iveskite: PAVARDE ir VARDA (tuscia eilute - baigti): ";
        string line;
        getline(std::cin >> std::ws, line);
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

inline bool less_pavarde_vardas(const Studentas& a, const Studentas& b) {
    if (a.pavarde != b.pavarde) return a.pavarde < b.pavarde;
    return a.vardas < b.vardas;
}
inline bool less_vardas_pavarde(const Studentas& a, const Studentas& b) {
    if (a.vardas != b.vardas) return a.vardas < b.vardas;
    return a.pavarde < b.pavarde;
}

template <class T, class Less>
void merge_sort(std::vector<T>& a, Less less) {
    const std::size_t n = a.size();
    if (n <= 1) return;
    std::vector<T> tmp(n);
    for (std::size_t width = 1; width < n; width *= 2) {
        for (std::size_t i = 0; i < n; i += 2 * width) {
            std::size_t l = i;
            std::size_t m = std::min(i + width, n);
            std::size_t r = std::min(i + 2 * width, n);
            std::size_t p = l, q = m, k = l;
            while (p < m && q < r) {
                if (!less(a[q], a[p])) tmp[k++] = std::move(a[p++]);
                else                    tmp[k++] = std::move(a[q++]);
            }
            while (p < m) tmp[k++] = std::move(a[p++]);
            while (q < r) tmp[k++] = std::move(a[q++]);
            for (std::size_t t = l; t < r; ++t) a[t] = std::move(tmp[t]);
        }
    }
}

void generuoti_faila(std::mt19937& rng, const std::string& failo_vardas,
    std::size_t N, std::size_t K, int minP = 1, int maxP = 10) {
    std::ofstream out(failo_vardas);
    if (!out) {
        std::cerr << "Nepavyko sukurti failo: " << failo_vardas << '\n';
        return;
    }

    out << "Pavarde Vardas";
    for (std::size_t i = 1; i <= K; i++) out << " ND" << i;
    out << " Egzaminas\n";

    std::uniform_int_distribution<int> distPaz(minP, maxP);

    for (std::size_t j = 1; j <= N; j++) {
        out << "Pavarde" << j << " Vardas" << j;
        for (std::size_t k = 1; k <= K; k++) out << " " << distPaz(rng);
        out << " " << distPaz(rng) << "\n";
    }
}

void failo_formatavimas(const std::string& failo_vardas,
    const std::vector<Studentas>& grupe, int vartotojo_pasirinkimas) {
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
