// LET OP: deze code wordt niet meegenomen in de Makefile, omdat het geen echt onderdeel is van de opdracht, maar alleen dient ter verder onderzoek


// TODO Testen met gdb
// Verandert het bord in een ander bord waarvoor de veelterm hetzelfde is
Torenveelterm& Torenveelterm::maakequivalent () {
	int rijen[hoogte] = {}, kolommen[breedte] = {}; // TODO member voor hergebruik
	for (int i = 0; i < hoogte; i++) {
		for (int j = 0; j < breedte; j++) {
			if (!bord[i][j]) {
				rijen[i]++;
				kolommen[j]++;
			}
		}
	}
	std::sort( rijen, rijen + hoogte );
	std::sort( kolommen, kolommen + breedte );
	clear();
	for (int i = 0; i < hoogte; i++) {
		for (int j = 0; j < breedte; j++) {
			if (kolommen[j] != 0 && rijen[i] != 0) {
				bord[i][j] = true;
				kolommen[j]--;
				rijen[i]--;
			}
		}
	}
	return *this; // Let op: 'Torenveelterm D = maakequivalent(C)' roept ook operator= aan
}
