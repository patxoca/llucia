// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>
#include <time.h>

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
	Responder rep(CFG_PRODUCTOR);

	std::cout << "Iniciant productor n = " << DIMENSIO << std::endl;

	combinador = new Combinator(NOMBRE_COALICIONS, DIMENSIO);
	t0 = clock();
	while (continuar) {
		switch (rep.get_request_type()) {
		case RQ_ABORT:
			// canviar generador
            avortat = true;
            rep.ack();
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
            rep.data(idpaquet, DIMENSIO, dades);
			break;

		case RQ_REG:
			num_treballadors++;
			rep.registered(num_treballadors);
			break;

		case RQ_UNREG:
			if (num_treballadors > 0) {
				num_treballadors--;
				rep.ack();
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
