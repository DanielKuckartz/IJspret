#include "Torenveelterm.h"

#include <fstream> // in 'Torenveelterm::leesin()'
#include <vector> // in 'voortbrenger::voortbrenger()'
#include <iostream> // std::cout
#include <iomanip> // std::setprecision()

// Vermenigvuldig een voortbrenger met een andere voortbrenger
voortbrenger& voortbrenger::operator*= (const voortbrenger& rhs) {
	voortbrenger hulp = *this;
	coefficienten.clear();
	coefficienten.resize(hulp.size() + rhs.size() - 1, 0);
	for (int i = 0; i < hulp.size(); i++)
		for (int j = 0; j < rhs.size(); j++)
			(*this)[i+j] += hulp[i] * rhs[j];
	return *this;
}

// Zet een voortbrenger gelijk aan een andere voortbrenger
voortbrenger& voortbrenger::operator=(const voortbrenger& rhs) {
	if (this == &rhs)
		return *this;
	coefficienten = rhs.coefficienten;
	return *this;
}

// Tel een andere voortbrenger bij een voortbrenger op
voortbrenger& voortbrenger::operator+=(const voortbrenger& rhs) {
	coefficienten.resize( rhs.size() );
	for (int i = 0; i < rhs.size(); i++)
		(*this)[i] += rhs[i];
	return *this;
}

// Tel twee voortbrengers bij elkaar op
voortbrenger operator+(voortbrenger lhs, const voortbrenger& rhs) {
	lhs += rhs;
	return lhs;
}

// Stel een veld gelijk aan een ander veld
veld& veld::operator=(const veld& rhs) {
	if (this == &rhs)
		return *this; 
	x = rhs.x;
	y = rhs.y; 
	return *this;
}

// Constructor vanuit een tekstbestand
Torenveelterm::Torenveelterm(const char filename[]) {
	leesin(filename);
	veelterm = {{}};
}

void Torenveelterm::clear() {
	for (int i = 0; i < maxN; i++)
		for (int j = 0; j < maxN; j++)
			bord[i][j] = 1;
}

// Gegeven een bestand met een integer op de eerste rij dat n voorsteld en 
//   0'en en 1'en in tabelvorm die samen het te initialiseren bord representeren
void Torenveelterm::leesin(const char filename[]) {
	char verboden;
	std::ifstream file;
	file.open(filename);
	if (file.fail())
		throw "Openen van bestand mislukt";
	clear();
	file >> hoogte;
	breedte = hoogte;
	for (int i = 0; i < hoogte; i++) { // lees te gebruiken velden in
		for (int j = 0; j < breedte; j++) {
			file >> verboden;
			bord[i][j] = (verboden == '1');
		}
	}
	file.close();
}

Torenveelterm::Torenveelterm(bool rhsbord[maxN][maxN], int h, int b) {
	hoogte = min(h, maxN);
	hoogte = max(0, hoogte);
	breedte = min(b, maxN);
	hoogte = max(0, breedte);
	for (int i = 0; i < maxN; i++)
		for (int j = 0; j < maxN; j++)
			bord[i][j] = rhsbord[i][j];
	veelterm = {{}};
}

// assignment operator vanuit een reeds bestaande Torenveelterm instantie
Torenveelterm& Torenveelterm::operator= (const Torenveelterm& rhs) {
	if (this == &rhs)
		return *this;
	hoogte = rhs.hoogte;
	breedte = rhs.breedte;
	for (int i = 0; i < maxN; i++)
		for (int j = 0; j < maxN; j++)
			bord[i][j] = rhs.bord[i][j];
	veelterm = rhs.veelterm; 
	return *this;
}

// Retourneert of het bord het nulelement is, dat wil zeggen: of er GEEN 
//   verboden velden zijn.
bool Torenveelterm::nulelement() {
	for (int i = 0; i < breedte; i++)
		for (int j = 0; j < hoogte; j++)
			if ( bord[i][j] == false)
				return false;
	return true;
}

// Retourneer het verboden veld, waarvan de som van het aantal verboden velden 
//   in zijn kolom en rij minimaal is.
veld Torenveelterm::vindkoning () {
	int rijen[hoogte] = {}, kolommen[breedte] = {};
	veld koning;
	int aantal = hoogte + breedte; // oneindig of groot genoeg
	for (int i = 0; i < hoogte; i++) {
		for (int j = 0; j < breedte; j++) {
			if (!bord[i][j]) {
				rijen[i]++;
				kolommen[j]++;
			}
		}
	}
	for (int i = 0; i < hoogte; i++) {
		for (int j = 0; j < breedte; j++) {
			if (!bord[i][j] && rijen[i] + kolommen[j] - 1 < aantal) {
				aantal = rijen[i] + kolommen[j] - 1;
				koning.x = j;
				koning.y = i;
			}
		}
	}
	return koning;
}

// Retourneer het eerstvolgende veld in leesrichting dat slaags staat met de
//   koning.
veld Torenveelterm::vinddame (veld dame, veld koning) {
	if (dame.x != koning.x) {
		if (dame.y != koning.y) { // dame is niet-interfererend met koning
			dame.y = koning.y;
			dame.x = 0;
		}
		while (dame.x < breedte) {
			if ( !bord[dame.y][dame.x] && dame != koning )
				return dame;
			dame.x++;
		}
		// gevraagd veld nog niet gevonden
		dame.x = koning.x; 
		dame.y = 0;
	}
	while (dame.y < hoogte) {
		if ( !bord[dame.y][dame.x] && dame != koning )
			return dame;
		dame.y++;
	}
	return koning;
}

// Op moment van aanroepen is de koning niet-interfererend met de rest van het bord.
voortbrenger Torenveelterm::eigenschap1 (veld dame) {
	voortbrenger koningsveld{{1, 1}};
	Torenveelterm E(bord, hoogte, breedte);
	E.bord[dame.y][dame.x] = true; // verwijder verboden veld
	veelterm = E.berekenveelterm(); // de positie van de nieuwe koning is onbekend
	veelterm *= koningsveld;
	return veelterm;
}

voortbrenger Torenveelterm::eigenschap2 (veld dame, veld koning) {
	std::vector<long long>::iterator it;
	Torenveelterm D(bord, hoogte, breedte);
	Torenveelterm E(bord, hoogte, breedte);
	E.bord[dame.y][dame.x] = true; // verwijder verboden veld
	for (int j = 0; j < breedte; j++)
		D.bord[dame.y][j] = true;
	for (int i = 0; i < hoogte; i++)
		D.bord[i][dame.x] = true;
	E.berekenveelterm(dame, koning); // koning blijft onveranderd
	veelterm = D.berekenveelterm(); 
	it = veelterm.coefficienten.begin();
	veelterm.coefficienten.insert(it, 0); // vermenigvuldiging met x
	veelterm += E.veelterm;
	return veelterm;
}

voortbrenger Torenveelterm::berekenveelterm (veld dame, veld koning) {
	dame = vinddame(dame, koning);
	if (dame == koning)
		veelterm = eigenschap1 (dame);
	else
		veelterm = eigenschap2 (dame, koning);
	return veelterm;
}

// Als koning en dame nog niet bekend zijn
voortbrenger Torenveelterm::berekenveelterm () {
	if ( nulelement() )
		veelterm = {{1}};
	else {
		veld koning = vindkoning();
		veld dame = {-1, -1};
		berekenveelterm(dame, koning);
	}
	return veelterm;
}

// Stelling 1.9
long long Torenveelterm::nr_permutaties () {
	long long result = 0;
	double kans;
	bool positief = (hoogte % 2 == 0);
	long long f = 1; // faculteit
	if (hoogte != breedte)
		return 0;
	berekenveelterm();
	veelterm.coefficienten.resize(hoogte+1); // tegen out-of-bound
for (int i = 0; i < veelterm.size(); i++)
	std::cout << veelterm[i] << ' ';
std::cout << std::endl;

	result += teken(positief) * f * veelterm[hoogte];
	for (int k = hoogte-1; k >= 0; --k) {
		positief = !positief;
		f *= (hoogte - k);
		result += teken(positief) * f * veelterm[k];
	}
	kans = (double) result / (double) f;
	std::cout << "Er zijn " << result << " geldige toewijzingen." << std::endl
						<< "En de kans is op zo'n toewijzing is " << std::setprecision(4) 
						<< kans << std::endl;
	return result;
}
