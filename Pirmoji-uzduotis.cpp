#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>

struct Studentas {
	std::string vardas;
	std::string pavarde;
	std::vector<int> nd;
	int egzaminas = 0;
	double galutinis = 0.0;
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
//------------
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
//-----------------

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
			int x;
			while (nds >> x) s.nd.push_back(x);
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

static const std::vector<std::string> VARD = {
	"Jonas","Mantas","Lukas","Dominykas","Tomas","Nojus","Arnas","Kajus",
	"Paulius","Dovydas","Andrius","Rokas","Benas","Adomas","Martynas",
	"Ignas","Vilius","Titas","Laurynas","Deividas"
};
static const std::vector<std::string> PAV = {
	"Kazlauskas","Jankauskas","Petrauskas","Stankevičius","Vasiliauskas",
	"Žukauskas","Šimkus","Paulauskas","Mikalauskas","Pocius",
	"Grigaitis","Sadauskas","Kavaliauskas","Giedraitis","Noreika",
	"Valantinas","Bacevičius","Mažeika","Kairys","Čepas"
};
//----------------------- rikiavimas
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
//-----------------------

int main() {
	const char* pr = "studentai1.txt";

	int pasirinktas_saltinis = 1;
	std::cout << "Pasirinkite duomenu saltini (1 - txt failas, 2 - atsitiktinai generuoti duomenys, 3 - rankinis ivedimas): \n";
	std::cout << "__________________________________________________________\n";
	if (!(std::cin >> pasirinktas_saltinis) || (pasirinktas_saltinis < 1 || pasirinktas_saltinis > 3)) {
		std::cout << "Prasome pasirinkti 1, 2 arba 3.\n";
		std::cout << "Programa nutraukiama.\n";
		std::exit(1);
	}

	int mstudentu_kiekis;
	if (pasirinktas_saltinis == 2) {
		std::cout << "Pasirinkite didziausia leistina studentu kieki.\n";
		std::cout << "__________________________________________________________\n";
		std::cin >> mstudentu_kiekis;
	}

	if (pasirinktas_saltinis == 2) {
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> distN(1, mstudentu_kiekis);
		std::uniform_int_distribution<int> distK(5, 25);

		int N = distN(rng);
		int K = distK(rng);

		std::cout << "Studentu skaicius: " << N << " " << "Bendras pazymiu kiekis: " << K << "\n";
		std::cout << "__________________________________________________________\n";

		int minP = 1;
		int maxP = 10;

		pr = "studentai_gen.txt";

		std::ofstream gen(pr);
		std::uniform_int_distribution<size_t> iv(0, VARD.size() - 1);
		std::uniform_int_distribution<size_t> ip(0, PAV.size() - 1);

		if (!gen) {
			std::cout << "Nepavyko sukurti failo: " << pr << std::endl;
			return 4;
		}

		gen << "Pavarde Vardas ";
		for (int j = 1; j <= K; ++j) gen << " ND" << j;
		gen << " Egzaminas\n";

		std::uniform_int_distribution<int> distPaz(minP, maxP);

		for (int i = 1; i <= N; ++i) {
			const std::string& vardas = VARD[iv(rng)];
			const std::string& pavarde = PAV[ip(rng)];
			gen << pavarde << ' ' << vardas;
			for (int j = 1; j <= K; ++j) gen << " " << distPaz(rng);
			gen << " " << distPaz(rng) << "\n";
		}
		gen.close();
	}

	int vartotojo_pasirinkimas = 1;
	std::cout << "Pasirinkite skaiciavimo buda (1 - vidurkis, 2 - mediana, 3 - vidurkis ir mediana): \n";
	std::cout << "__________________________________________________________\n";

	if (!(std::cin >> vartotojo_pasirinkimas) || (vartotojo_pasirinkimas < 1 || vartotojo_pasirinkimas > 3)) {
		std::cout << "Prasome pasirinkti 1, 2 arba 3.\n";
		std::cout << "Programa nutraukiama.\n";
		std::exit(1);
	}

	int rikiavimo_pasirinkimas = 1;
	std::cout << "Pasirinkite rikiavimo parametra (1 - studento vardas, 2 - studento pavarde)\n";
	std::cout << "__________________________________________________________\n";

	if (!(std::cin >> rikiavimo_pasirinkimas) || (rikiavimo_pasirinkimas != 1 && rikiavimo_pasirinkimas != 2)) {
		std::cout << "Prasome pasirinkti 1 arba 2.\n";
		std::cout << "Programa nutraukiama.\n";
		std::exit(1);
	}

	const char* rz = "rezultatas.txt";

	std::vector<Studentas> grupe;

	if (pasirinktas_saltinis == 3) {
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		ivedimas_is_konsoles(grupe);
	}
	else {
		std::ifstream in(pr);
		if (!in) {
			std::cout << "Klaida atidarant faila: " << pr << std::endl;
			return 1;
		}

		std::string eilute;
		size_t nr = 0;

		while (std::getline(in, eilute)) {
			++nr;
			if (nr == 1) continue;
			if (eilute.find_first_not_of(" \t\n\r") == std::string::npos) continue;

			std::istringstream iss(eilute);
			Studentas s;
			if (!(iss >> s.pavarde >> s.vardas)) {
				std::cout << nr << " eiluteje nerasta pavarde/vardas – studentas praleidziamas.\n";
				continue;
			}

			std::vector<int> visi_skaiciai;
			int x;
			while (iss >> x) visi_skaiciai.push_back(x);

			if (visi_skaiciai.empty()) {
				std::cout << nr << " eiluteje nerasta nei vieno pazymio - studentas praleidziamas.\n";
				continue;
			}

			s.egzaminas = visi_skaiciai.back();
			s.nd.assign(visi_skaiciai.begin(), visi_skaiciai.end() - 1);
			grupe.push_back(std::move(s));
		}

		if (in.fail() && !in.eof()) {
			std::cout << "Failo skaityme kilo klaida.\n";
			return 3;
		}
	}

	if (grupe.empty()) {
		std::cout << "Nera nei vieno studento.\n";
		return 0;
	}

	std::ofstream out(rz);
	if (!out) {
		std::cout << "Nepavyko sukurti failo: " << rz << std::endl;
		return 4;
	}

	if (rikiavimo_pasirinkimas == 1) merge_sort(grupe, less_vardas_pavarde);
	else merge_sort(grupe, less_pavarde_vardas);

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

	if (vartotojo_pasirinkimas == 1) std::cout << "Galutinis (Vid.)\n";
	else if (vartotojo_pasirinkimas == 2) std::cout << "Galutinis (Med.)\n";
	else std::cout << "Galutinis (Vid.) ir Galutinis (Med.)\n";

	std::cout << "Rezultatai irasyti i faila: " << rz << std::endl;
	return 0;
}
