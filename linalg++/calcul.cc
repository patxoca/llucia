// -*- coding: utf-8 -*-

// $Id:$

// calcul.h inclou tipus.h etc, no cal tornar a incloure'ls.

#include "calcul.h"


// ací implementes els métodes

Calcul::Calcul() {
	dimensio = 0;
	valors = NULL;
	Bmes_no_buida = false;
}

Calcul::Calcul(int d, Fraccio *v) {
	dimensio = d;
	valors = v;
	Bmes_no_buida = false;

	matriu = Matriu(d, d);
	tmp_inv = Matriu(d, 2*d);
}

void Calcul::calcular(Combination *c) {
	// fer calculs variats
	matriu.binary_array(c);

	// des d'aquest mètode pots utilitzar la resta de mètodes que hagis
	// definit

	Coalicio coalicio = 3; // 011
	Fraccio punt[] = {2, 3, 4};
	Fraccio resultat = valorar_coalicio(coalicio, (Fraccio*)punt);

	// i accedir als atributs de l'objecte:

	std::cout << "B+ " << Bmes_no_buida << std::endl;
}

bool Calcul::final_calcul() {
	return Bmes_no_buida;
}


// mètodes privats

// tria un nom millor
Fraccio Calcul::valorar_coalicio(Coalicio c, Fraccio *punt) {
	Fraccio resultat = 0;

	while (c) { // c != 0
		if (c & 1) {
			resultat += *punt;
		}
		c >>= 1;
		punt++;
	}
	return resultat;
}
