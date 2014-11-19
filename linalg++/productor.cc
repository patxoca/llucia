// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>
#include <time.h>
#include <zmq.hpp>

#include "combinator.h"
#include "config.h"
#include "missatge.h"
#include "tipus.h"

#define MIDA_COMBINACIO  (DIMENSIO * sizeof(int))

const int PAQUET_FINALITZACIO[DIMENSIO] = {-1};


int main(int argc, char **argv) {
	Combinator *combinador;
	clock_t t0, tf;
	long num_paquets = 0;
	int num_treballadors = 0;
	bool avortat = false;
    bool primer = true;
	bool continuar = true;
	zmq::message_t zrequest;
	zmq::context_t context(1);
	zmq::socket_t socket(context, ZMQ_REP);
	request_t *request;

    socket.bind (CFG_PRODUCTOR);

	std::cout << "Iniciant productor n = " << DIMENSIO << std::endl;

	combinador = new Combinator(NOMBRE_COALICIONS, DIMENSIO);
	t0 = clock();
	while (continuar) {
		socket.recv(&zrequest);
		request = (request_t*)(zrequest.data());
		switch (request->message) {
		case RQ_ABORT:
			// canviar generador
            avortat = true;
            msg_reply_ack(socket);
			break;

		case RQ_GET:
			// generar paquet
            int idpaquet;
            const Combination *dades;
            if (avortat) {
                idpaquet = -1;
                dades = (Combination*)PAQUET_FINALITZACIO;
            } else {
                if (primer) {
                    dades = combinador->first();
                    primer = false;
                } else {
                    dades = combinador->next(MIDA_PAQUET);
                }
                if (dades == NULL) {
                    idpaquet = -1;
                    dades = (Combination*)PAQUET_FINALITZACIO;
                    avortat = true;
                } else {
                    idpaquet = ++num_paquets;
                }
            }
            std::cout << "Enviant paquet " << idpaquet << std::endl;
            msg_reply_data(socket, idpaquet, DIMENSIO, (int*)dades);
			break;

		case RQ_REG:
			num_treballadors++;
			msg_reply_registered(socket, num_treballadors);
			break;

		case RQ_UNREG:
			if (num_treballadors > 0) {
				num_treballadors--;
				msg_reply_ack(socket);
			}
			if (!num_treballadors) {
				continuar = false;
			}
			break;

		default:
			std::cout << "Missatge desconegut" << std::endl;
		}
	}
	tf = clock();

	std::cout << "Num paquets: " << num_paquets << std::endl;
	std::cout << (tf - t0) / (double)CLOCKS_PER_SEC << "segons\n";
}
