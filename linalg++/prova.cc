// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>
#include <time.h>

#include "combinator.h"
#include "rational.h"
#include "tipus.h"
#include "utils.h"


int main(int argc, char **argv) {
	std::vector<Coalicio> coalicions;
	Combinator *combinador;
	Matriu m(DIMENSIO, DIMENSIO);
	Fraccio resultat;
	clock_t t0, tf;
	int num_combinacions = 0;
	int num_bases = 0;

	coalicions = generar_coalicions(DIMENSIO);
	combinador = new Combinator(coalicions.size(), DIMENSIO);
	t0 = clock();
	for (const int *c = combinador->first(); c != NULL; c = combinador->next()) {
		num_combinacions++;
		for (int i = 0; i < DIMENSIO; i++) {
			Coalicio coalicio = coalicions[c[i]];
			for (int j = 0; j < DIMENSIO; j++) {
				m.put(i, j, coalicio[j]);
			}
		}
		if (m.det() != 0) {
			num_bases++;
		}
	}
	tf = clock();

	std::cout << "Num combinacions: " << num_combinacions << std::endl << std::endl;
	std::cout << "Num bases:        " << num_bases << std::endl << std::endl;
	std::cout << (tf - t0) / (double)CLOCKS_PER_SEC << "segons\n";
}
