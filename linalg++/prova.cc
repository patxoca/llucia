// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>
#include <time.h>

#include "cmdline.h"
#include "combinator.h"
#include "rational.h"
#include "tipus.h"


int main(int argc, char **argv) {
	std::vector<Coalicio> coalicions;
	Combinator *combinador;
	Matriu m(DIMENSIO, DIMENSIO);
	Matriu clon(DIMENSIO, DIMENSIO);
	Matriu tmpinv(DIMENSIO, 2*DIMENSIO);
	Fraccio resultat;
	clock_t t0, tf;
	long num_combinacions = 0;
	long num_bases = 0;
	long num_no_det = 0;
	Coalicio or_coalicions;
	options_t opcions;


	if (parse_cmd_line(argc, argv, opcions)) {
		return 0;
	}

	std::cout << "Iniciant càlcul n = " << DIMENSIO << std::endl;

	combinador = new Combinator(NOMBRE_COALICIONS, DIMENSIO);
	t0 = clock();
	for (const Combination *c = combinador->first(); c != NULL; c = combinador->next()) {
		or_coalicions = 0;
		num_combinacions++;
		for (int i = 0; i < DIMENSIO; i++) {
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
