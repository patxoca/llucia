// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>
#include <time.h>
#include <zmq.hpp>

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
	long num_combinacions = 0;
	long num_bases = 0;
	long num_no_det = 0;
	Coalicio or_coalicions;
	bool continuar = true;
	zmq::context_t context (1);
	zmq::socket_t socket (context, ZMQ_REQ);

	std::cout << "Iniciant treballador n = " << DIMENSIO << std::endl;
	socket.connect ("tcp://localhost:5555");

	combinador = new Combinator(NOMBRE_COALICIONS, DIMENSIO);
	t0 = clock();
	while (continuar) {
		// for (const int *c = combinador->first(); c != NULL; c = combinador->next()) {
		zmq::message_t request (4);
		zmq::message_t reply;
		int *c;

		memcpy ((void *) request.data (), "GET", 3);
		socket.send (request);
		socket.recv (&reply);
		c = (int*)reply.data();

		if (c[0] == -1) {
			continuar = false;
		}
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
	tf = clock();

	std::cout << "Num combinacions: " << num_combinacions << std::endl;
	std::cout << "Num bases:        " << num_bases << std::endl;
	std::cout << "Num no det:       " << num_no_det << std::endl;
	std::cout << (tf - t0) / (double)CLOCKS_PER_SEC << "segons\n";
}
