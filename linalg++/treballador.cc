// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>
#include <time.h>
#include <zmq.hpp>

#include "cmdline.h"
#include "combinator.h"
#include "missatge.h"
#include "rational.h"
#include "tipus.h"


int main(int argc, char **argv) {
	std::vector<Coalicio> coalicions;
	unsigned int dimensio;
	Fraccio *valors;
	Combinator *combinador;
	Matriu m(DIMENSIO, DIMENSIO);
	Matriu clon(DIMENSIO, DIMENSIO);
	Matriu tmpinv(DIMENSIO, 2*DIMENSIO);
	clock_t t0, tf;
	long num_combinacions = 0;
	long num_bases = 0;
	long num_no_det = 0;
	Coalicio or_coalicions;
	int idtreballador = -1;
	int idpaquet;
	int size;
	Combination buffer[DIMENSIO];
	Requester req;
	WorkerOptions opcions;

	if (opcions.parse_cmd_line(argc, argv)) {
		return 0;
	}

	std::cout << "Connectant amb productor en " << opcions.get_full_address() << std::endl;
	req = Requester(opcions.get_full_address());

	std::cout << "Registrant treballador\n";
	req.register_(&idtreballador);
	if (idtreballador == -1) {
		std::cout << "Treballador rebutjar" << std::endl;
		return 0;
	}

	std::cout << "Descarregant joc\n";
	if (!req.game(&dimensio, &valors)) {
		std::cout << "Error\n";
		return 0;
	}
	std::cout << "Iniciant treballador n = " << dimensio << " Valors = ";
	for (unsigned int i = 1, last = 1 << dimensio; i < last; i++) {
		std::cout << valors[i] << " ";
	}
	std::cout << std::endl;

	t0 = clock();
	while (true) {
		zmq::message_t request (4);
		zmq::message_t reply;

		req.get(-1, &idpaquet, &size, buffer);

		if (idpaquet == -1) {
			std::cout << "Rebut paquet de finalització" << std::endl;
			break;
		}
		// std::cout << "Processant paquet " << idpaquet << std::endl;
		// for (int i = 0; i < size; i++) {
		// 	std::cout << buffer[i] << " ";
		// }
		// std::cout << std::endl;

		combinador = new Combinator(NOMBRE_COALICIONS, DIMENSIO, buffer, MIDA_PAQUET);
		for (const Combination *c = combinador->first(); c != NULL; c = combinador->next()) {
			or_coalicions = 0;
			num_combinacions++;
			for (int i = 0; i < DIMENSIO; i++) {
				or_coalicions |= c[i];
			}
			if (or_coalicions == COALICIO_TOTAL) {
				m.binary_array(c);
				clon = m;
				if (m.det() != 0) {
					num_bases++;
					clon.inv(tmpinv);
				}
			} else {
				num_no_det++;
			}
		}
	}
	tf = clock();
	req.unregister();

	std::cout << "Num combinacions: " << num_combinacions << std::endl;
	std::cout << "Num bases:        " << num_bases << std::endl;
	std::cout << "Num no det:       " << num_no_det << std::endl;
	std::cout << (tf - t0) / (double)CLOCKS_PER_SEC << "segons\n";
}
