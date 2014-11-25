// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>
#include <time.h>
#include <zmq.hpp>

#include <boost/chrono.hpp>
#include <boost/thread.hpp>

#include "calcul.h"
#include "cmdline.h"
#include "combinator.h"
#include "missatge.h"
#include "tipus.h"




void worker_thread(zmq::context_t *context, WorkerOptions *opcions);


int main(int argc, char **argv) {
	zmq::context_t context(1);
	WorkerOptions opcions;
	int num_workers;
	boost::thread_group fils;

	if (opcions.parse_cmd_line(argc, argv)) {
		return 0;
	}

	num_workers = opcions.get_num_workers();
	std::cout << "Iniciant treballador amb " << num_workers << " workers\n";

	for (int i = 0; i < num_workers; i++) {
		fils.add_thread(new boost::thread(worker_thread, &context, &opcions));
		boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
	}
	fils.join_all();

	std::cout << "Final\n";
	return 0;
}

void worker_thread(zmq::context_t *context, WorkerOptions *opcions) {
	unsigned int dimensio;
	Fraccio *valors;
	Combinator *combinador;
	time_t t0, tf;
	long num_combinacions = 0;
	long num_no_det = 0;
	Coalicio or_coalicions;
	int idtreballador = -1;
	int idpaquet;
	int size;
	Combination *buffer;
	Requester req;
	Coalicio COALICIO_TOTAL;


	std::cout << "Connectant amb productor en " << opcions->get_full_address() << std::endl;
	req = Requester(opcions->get_full_address(), context);

	std::cout << "Registrant treballador\n";
	req.register_(&idtreballador);
	if (idtreballador == -1) {
		std::cout << "Treballador rebutjar" << std::endl;
		return;
	}

	std::cout << "Descarregant joc\n";
	if (!req.game(&dimensio, &valors)) {
		std::cout << "Error\n";
		return;
	}
	std::cout << "Iniciant treballador n = " << dimensio << " Valors = ";
	for (unsigned int i = 1, last = 1 << dimensio; i < last; i++) {
		std::cout << valors[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "Inicialitzant calcul." << std::endl;
	Calcul calcul(dimensio, valors);
	buffer = (Combination*)malloc(dimensio * sizeof(Combination));
	COALICIO_TOTAL = (1 << dimensio) - 1; // 111...11
	t0 = time(NULL);
	while (true) {
		req.get(-1, &idpaquet, &size, buffer);

		if (idpaquet == -1) {
			std::cout << "Rebut paquet de finalització" << std::endl;
			goto abort_calculation;
		}

		combinador = new Combinator((1 << dimensio) - 1, dimensio, buffer, MIDA_PAQUET);
		for (const Combination *c = combinador->first(); c != NULL; c = combinador->next()) {
			or_coalicions = 0;
			num_combinacions++;
			for (unsigned int i = 0; i < dimensio; i++) {
				or_coalicions |= c[i];
			}
			if (or_coalicions == COALICIO_TOTAL) {
				try {
					calcul.calcular(c);
				} catch (AbortCalculationException e) {
					goto abort_calculation;
				}
			} else {
				num_no_det++;
			}
		}
	}
  abort_calculation:
	calcul.final_calcul();
	tf = time(NULL);
	req.unregister();
	req.close();

	std::cout << "Num combinacions: " << num_combinacions << std::endl;
	std::cout << "Num no det:       " << num_no_det << std::endl;
	std::cout << tf - t0 << "segons\n";

	return;
}
