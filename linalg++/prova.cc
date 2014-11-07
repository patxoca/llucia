// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>
#include <time.h>

#include "combinator.h"
#include "matrix.h"
#include "rational.h"
#include "utils.h"


int main(int argc, char **argv) {
	std::vector<Coalicio> coalicions;
	Combinator *combinador;
	Matriu m(DIMENSIO, DIMENSIO);
	Fraccio resultat;
	Matriu inversa;
	clock_t t0, tf;
	int k = 0;

	coalicions = generar_coalicions(DIMENSIO);
	combinador = new Combinator(coalicions.size(), DIMENSIO);
	t0 = clock();
	for (const int *c = combinador->first(); c != NULL; c = combinador->next()) {
		for (int i = 0; i < DIMENSIO; i++) {
			Coalicio coalicio = coalicions[c[i]];
			for (int j = 0; j < DIMENSIO; j++) {
				m.put(i, j, coalicio[j]);
			}
		}
		inversa = m.getInverse();
		k++;
	}
	tf = clock();

	std::cout << k << std::endl << std::endl;
	std::cout << (tf - t0) / (double)CLOCKS_PER_SEC << "segons\n";
}
