// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>
#include <time.h>
#include <zmq.hpp>

#include "combinator.h"
#include "config.h"
#include "missatge.h"
#include "rational.h"
#include "tipus.h"


int main(int argc, char **argv) {
	std::vector<Coalicio> coalicions;
	Combinator *combinador;
	Matriu m(DIMENSIO, DIMENSIO);
	clock_t t0, tf;
	long num_combinacions = 0;
	long num_bases = 0;
	long num_no_det = 0;
	Coalicio or_coalicions;
	int idtreballador = -1;
	int idpaquet;
	int size;
	Combination buffer[DIMENSIO];
	zmq::context_t context (1);
	zmq::socket_t socket (context, ZMQ_REQ);

	std::cout << "Iniciant treballador n = " << DIMENSIO << std::endl;
	socket.connect (CFG_PRODUCTOR);

	msg_request_register(socket, &idtreballador);
	if (idtreballador == -1) {
		std::cout << "Treballador rebutjar" << std::endl;
		return 0;
	}
	t0 = clock();
	while (true) {
		zmq::message_t request (4);
		zmq::message_t reply;

		msg_request_get(socket, idtreballador, -1, &idpaquet, &size, buffer);

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
				Coalicio coalicio = c[i] + 1;
				or_coalicions |= coalicio;
				for (int j = 0; j < DIMENSIO; j++) {
					m.put(i, j,  coalicio & 1);
					coalicio >>= 1;
				}
			}
			if (or_coalicions == COALICIO_TOTAL) {
				if (m.det() != 0) {
					num_bases++;
				}
			} else {
				num_no_det++;
			}
		}
	}
	tf = clock();
	msg_request_unregister(socket, idtreballador);

	std::cout << "Num combinacions: " << num_combinacions << std::endl;
	std::cout << "Num bases:        " << num_bases << std::endl;
	std::cout << "Num no det:       " << num_no_det << std::endl;
	std::cout << (tf - t0) / (double)CLOCKS_PER_SEC << "segons\n";
}
