#include <iostream>
#include "Torenveelterm.h"
#include <ctime>  // voor clock() en clock_t

int main() {
  std::clock_t t1, t2;
  t1 = std::clock();
	Torenveelterm* T = new Torenveelterm("invoer.in");
	T->nr_permutaties();
  t2 = std::clock();
	std::cout << "Het zoeken van het aantal toewijzingen kostte "
			   << ( (double)(t2-t1) /CLOCKS_PER_SEC ) << " seconden." << std::endl;
	delete T;
	return 0;
}
