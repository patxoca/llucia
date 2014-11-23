// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>
#include <time.h>

#include "combinator.h"
#include "gameloader.h"
#include "rational.h"
#include "tipus.h"


int main(int argc, char **argv) {
	std::vector<Coalicio> coalicions;
	Combinator *combinador;
	Fraccio resultat;
	clock_t t0, tf;
	long num_combinacions = 0;
	long num_bases = 0;
	long num_no_det = 0;
	Coalicio or_coalicions;
	GameLoader game;
	int dimension;
	Coalicio COALICIO_TOTAL;


	try {
		game.load("joc6.txt");
	} catch (GameLoaderException &e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	dimension = game.get_dimension();
	COALICIO_TOTAL = (1 << dimension) - 1;
	std::cout << "Iniciant càlcul n = " << dimension << std::endl;
	combinador = new Combinator(game.get_num_coalitions(), dimension);
	Matriu m(dimension, dimension);
	Matriu clon(dimension, dimension);
	Matriu tmpinv(dimension, 2*dimension);
	t0 = clock();
	for (const Combination *c = combinador->first(); c != NULL; c = combinador->next()) {
		or_coalicions = 0;
		num_combinacions++;
		for (int i = 0; i < dimension; i++) {
			or_coalicions |= c[i];
		}
		if (or_coalicions == COALICIO_TOTAL) {
			m.binary_array((unsigned int*)c);
			clon = m;
		    if (m.det() != 0) {
				//m = clon;
				m.inv(tmpinv);
			    num_bases++;
			}
		} else {
			num_no_det++;
		}
	}
	tf = clock();

	std::cout << "Num combinacions: " << num_combinacions << std::endl;
	std::cout << "Num bases:        " << num_bases << std::endl;
	std::cout << "Num no det:       " << num_no_det << std::endl;
	std::cout << (tf - t0) / (double)CLOCKS_PER_SEC << "segons\n";
}
