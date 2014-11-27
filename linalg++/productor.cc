// -*- coding: utf-8 -*-

// $Id$

#include <iostream>
#include <time.h>

#include "cmdline.h"
#include "combinator.h"
#include "gameloader.h"
#include "missatge.h"
#include "tipus.h"



int main(int argc, char **argv) {
	Combinator *combinador;
	time_t t0, tf;
	int dimensio;
	long num_paquets = 0;
	int num_treballadors = 0;
	bool avortat = false;
    bool primer = true;
	bool continuar = true;
	ProducerOptions opcions;
	GameLoader game;
	Combination *PAQUET_FINALITZACIO;

	if (opcions.parse_cmd_line(argc, argv)) {
		return 0;
	}

	std::cout << "Carregant joc " << opcions.get_data_file_path() << std::endl;
	try {
		game.load(opcions.get_data_file_path());
	} catch (GameLoaderException e) {
		std::cout << e.what() << std::endl;
		return 0;
	}
	dimensio = game.get_dimension();
	PAQUET_FINALITZACIO = new Combination[dimensio];
	for (int i = 0; i < dimensio; i++) {
		PAQUET_FINALITZACIO[i] = 0;
	}

	std::cout << "Iniciant productor n = " << dimensio << std::endl;
	std::cout << "Escoltant en " << opcions.get_full_address() << std::endl;

	Responder rep(opcions.get_full_address());

	combinador = new Combinator((1 << dimensio) - 1, dimensio);
	t0 = time(NULL);
	while (continuar) {
		switch (rep.get_request_type()) {
		case RQ_ABORT:
			// canviar generador
            avortat = true;
            rep.ack();
			break;

		case RQ_GAME:
			rep.game(dimensio, game.get_values());
			break;

		case RQ_GET:
			// generar paquet
            int idpaquet;
            const Combination *dades;
            if (avortat) {
                idpaquet = -1;
                dades = PAQUET_FINALITZACIO;
            } else {
                if (primer) {
                    dades = combinador->first();
                    primer = false;
                } else {
                    dades = combinador->next(MIDA_PAQUET);
                }
                if (dades == NULL) {
                    idpaquet = -1;
                    dades = PAQUET_FINALITZACIO;
                    avortat = true;
                } else {
                    idpaquet = ++num_paquets;
                }
            }
            // std::cout << "Enviant paquet " << idpaquet << std::endl;
            rep.data(idpaquet, dimensio, dades);
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
	tf = time(NULL);

	std::cout << "Num paquets: " << num_paquets << std::endl;
	std::cout << tf - t0 << "segons\n";
}
