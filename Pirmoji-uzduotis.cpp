// v0.01 failu skaitymas

#include <iostream>
#include <fstream>

int main() {
	const char* pr = "studentai_v1.txt";
	const char* rz = "rezultatas.txt";

	std::ifstream in(pr, std::ios::binary);
	if (!in) {    //in.fail() == true
		std::cout << "Klaida atidarant faila: " << pr << std::endl;
		return 1;
	}

	std::ofstream out(rz, std::ios::binary);
	if (!out) {
		std::cout << "Nepavyko sukurti failo: " << rz << std::endl;
		return 2;
	}
	


}