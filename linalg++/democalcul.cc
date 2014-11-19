// -*- coding: utf-8 -*-

// $Id:$

#include "calcul.h"
#include <iostream>

// aquest codi es per a que puguis fer proves. Ve a ser una simplificació del
// meu treballador.cc eliminat la comunicació amb el productor i la generació
// de combinacions per valors fixes.
//
// Il·lustra com el meu codi es "coodina" amb el teu codi i et permet fer
// proves partint d'un input conegut i validant el output obtingut.

int main(int argc, char *argv[]) {
	Calcul calcul;

	// en aquest punt el meu codi haurà esbrinat els paràmetres del joc, li
	// haurà demanat al productor, haurà llegit un arxiu de configuració, el
	// que sigui.

	int     dim      = 3; // 7 coalicions
	Fraccio valors[] = {0, 1, 2, 3, 4, 5, 6, 7}; // recorda l'index zero no
												 // s'utilitza però cal que
												 // estigui present

	// inicialitza el teu calculador

	calcul = Calcul(dim, (Fraccio*)valors);

	// ací el treballador demana blocs de combinacions al producto i les
	// genera

	// açò estaria dins el bucle que itera sobre les combinacions, correspon
	// al processament d'una combinació

	Combination combinacio[] = {1, 3, 5};

	// delega en el teu codi per fer el càlcul

	try {
		calcul.calcular((Combination*)combinacio);
	} catch (AbortCalculationException e) {
		std::cout << "Interceptada peticio d'avortament" << std::cout;
		// notificar al productor etc.
	}
	// el bucle acaba ací

	// eventualment les combinacions s'exhauriran i es donarà per finalitzat
	// el càlcul

	bool resultat = calcul.final_calcul();

	// fer algo amb el resultat

	std::cout << "Resultat = " << resultat << std::endl;
}
