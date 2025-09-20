#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>


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

int main() {

	int vartotojo_pasirinkimas = 1;
	std::cout << "Pasirinkite skaiciavimo buda (1 - vidurkis, 2 - mediana): ";
	if (!(std::cin >> vartotojo_pasirinkimas) || (vartotojo_pasirinkimas != 1 && vartotojo_pasirinkimas != 2)) {
		std::cout << "Neteisingas pasirinkimas. Naudojamas vidurkis.\n";
		vartotojo_pasirinkimas = 1;
	}

	const char* pr = "studentai1.txt";
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
			std::cout << nr << " eiluteje nerasta neivieno pazymio - studentas praleidziamas.\n";
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

	out << std::left << std::setw(15) << "Pavarde"
		<< std::left << std::setw(15) << "Vardas"
		<< (vartotojo_pasirinkimas == 1 ? "Galutinis (Vid.)" : "Galutinis (Med.)") << "\n";

	out << std::string(15 + 15 + 18, '-') << "\n";

	for (const auto& s : grupe) {
		double pasirinkimas = (vartotojo_pasirinkimas == 1)
			? galutinis_vidurkis(s)
			: galutinis_mediana(s);

		out << std::left << std::setw(15) << s.pavarde
			<< std::left << std::setw(15) << s.vardas
			<< std::right << std::setw(8) << std::fixed << std::setprecision(2)
			<< pasirinkimas << "\n";
	}


	
	std::cout << (vartotojo_pasirinkimas == 1 ? "Galutinis (Vid.)" : "Galutinis (Med.)") << "\n";
	std::cout << "Rezultatai irasyti i faila: " << rz << std::endl;
	return 0;



}