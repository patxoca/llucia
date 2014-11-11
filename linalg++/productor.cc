// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>
#include <time.h>
#include <zmq.hpp>

#include "combinator.h"
#include "tipus.h"

#define MIDA_COMBINACIO  (DIMENSIO * sizeof(int))

const int PAQUET_FINALITZACIO[DIMENSIO] = {-1};


int main(int argc, char **argv) {
	Combinator *combinador;
	clock_t t0, tf;
	long num_paquets = 0;
	zmq::context_t context (1);
	zmq::socket_t socket (context, ZMQ_REP);
	socket.bind ("tcp://*:5555");

	std::cout << "Iniciant productor n = " << DIMENSIO << std::endl;

	combinador = new Combinator(NOMBRE_COALICIONS, DIMENSIO);
	t0 = clock();
	for (const int *c = combinador->first(); c != NULL; c = combinador->next(MIDA_PAQUET)) {
		num_paquets++;
		zmq::message_t request;
		socket.recv (&request);
		zmq::message_t reply (MIDA_COMBINACIO);
		memcpy ((void *) reply.data (), c, MIDA_COMBINACIO);
		socket.send (reply);
	}
	zmq::message_t request;
	zmq::message_t reply(MIDA_COMBINACIO);
	socket.recv (&request);
	memcpy ((void *) reply.data (), PAQUET_FINALITZACIO, MIDA_COMBINACIO);
	socket.send (reply);
	tf = clock();

	std::cout << "Num paquets: " << num_paquets << std::endl;
	std::cout << (tf - t0) / (double)CLOCKS_PER_SEC << "segons\n";
}
