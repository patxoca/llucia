// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>
#include <time.h>
#include <zmq.hpp>

#include "cmdline.h"
#include "combinator.h"
#include "config.h"
#include "missatge.h"
#include "rational.h"
#include "tipus.h"


int main(int argc, char **argv) {
	std::vector<Coalicio> coalicions;
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
	Requester req(CFG_PRODUCTOR);
	worker_options_t opcions;

	if (parse_worker_cmd_line(argc, argv, opcions)) {
		return 0;
	}

	std::cout << "Iniciant treballador n = " << DIMENSIO << std::endl;

	req.register_(&idtreballador);
	if (idtreballador == -1) {
		std::cout << "Treballador rebutjar" << std::endl;
		return 0;
	}
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
