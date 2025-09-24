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

int main() {
	const char* pr = "studentai1.txt";

	int pasirinktas_saltinis = 1;
	std::cout << "Pasirinkite duomenu saltini (1 - txt failas, 2 - atsitiktinai generuoti domenys): \n";
	std::cout << "__________________________________________________________\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
	if (!(std::cin >> pasirinktas_saltinis) || (pasirinktas_saltinis != 1 && pasirinktas_saltinis != 2)){
		std::cout << "Prasome pasirinkti 1 arba 2.\n";
		std::cout << "Programa nutraukiama.\n";
		std::exit(1);
	}
	// txt generavimas 
	if (pasirinktas_saltinis == 2) {
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> distN(1, 100000);
		std::uniform_int_distribution<int> distK(5, 25);

		int N = distN(rng); //studentu skaicius
		int K = distK(rng); //pazymiu skaicius studentui

		std::cout<< "Studentu skaicius: " << N << " " << "Bendras pazymiu kiekis: " << K << "\n";
		std::cout << "__________________________________________________________\n";

		int minP = 1;
		int maxP = 10;

		pr = "studentai_gen.txt";

		std::ofstream gen(pr);

		if (!gen) {
			std::cout << "Nepavyko sukurti failo: " << pr << std::endl;
			return 4;
		} // note self:  gen tampa ofstream objektu, eina konversija i bool tipa

		gen << "Pavarde Vardas ";
		for (int j = 1; j <= K; ++j) gen << " ND" << j;
		gen << " Egzaminas\n";

		std::uniform_int_distribution<int> distPaz(minP, maxP);

		for (int i = 1; i <= N; ++i) {
			gen << "Pavarde" << i << " Vardas" << i;
			for (int j = 1; j <= K; ++j) gen << " " << distPaz(rng);
			gen << " " << distPaz(rng) << "\n";
		}
		/*
		for (int i = 1; i <= N; ++i) {
			std::cout << "Pavarde" << i << " Vardas" << i;
			for (int j = 1; j <= K; ++j) std::cout << " " << distPaz(rng);
			std::cout << " " << distPaz(rng) << "\n";
		}
		*/
		gen.close();

	}
	// txt generavimo pabaiga

	
	int vartotojo_pasirinkimas = 1;
	std::cout << "Pasirinkite skaiciavimo buda (1 - vidurkis, 2 - mediana, 3 - vidurkis ir mediana): \n";
	std::cout << "__________________________________________________________\n";

	if (!(std::cin >> vartotojo_pasirinkimas) || (vartotojo_pasirinkimas < 1 || vartotojo_pasirinkimas > 3)) {
		std::cout << "Prasome pasirinkti 1, 2 arba 3.\n";
		std::cout << "Programa nutraukiama.\n";
	std:exit(1);

	}

	//const char* pr = "studentai1.txt";
	const char* rz = "rezultatas.txt";

	std::ifstream in(pr);
	if (!in) {    //in.fail() == true
		std::cout << "Klaida atidarant faila: " << pr << std::endl;
		return 1;
	}

	std::vector<Studentas> grupe;
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

		//	grupe.push_back(s);
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
	if (grupe.empty()) {
		std::cout << "Nera nei vieno studento. Duomenu faile nerasta.\n";
		return 0;
	}
	std::ofstream out(rz);
	if (!out) {
		std::cout << "Nepavyko sukurti failo: " << rz << std::endl;
		return 4;
	}

	out << std::fixed << std::setprecision(2);
	
	print_col(out, "Vardas", 15);
	print_col(out, "Pavarde", 15);
	
	if (vartotojo_pasirinkimas == 1) {
		out << "Galutinis (Vid.)\n";
		out << std::string(15 + 15 + 18, '-') << "\n";
	}
	else if (vartotojo_pasirinkimas == 2) {
		out << "Galurinis (Med.)\n";
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
		else { // 3 – abi reikšmės
			out << std::right << std::setw(8) << gVid;
			out << std::string(10, ' ');           // tarpas, kad tilptų į ~18 pločio stulpelį
			out << std::right << std::setw(8) << gMed << "\n";
		}
	}


	if (vartotojo_pasirinkimas == 1) std::cout << "Galutinis (Vid.)\n";
	else if (vartotojo_pasirinkimas == 2) std::cout << "Galutinis (Med.)\n";
	else std::cout << "Galutinis (Vid.) ir Galutinis (Med.)\n";

	std::cout << "Rezultatai irasyti i faila: " << rz << std::endl;
	return 0;



}