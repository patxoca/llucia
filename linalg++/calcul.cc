// -*- coding: utf-8 -*-

// $Id:$

#include <iostream>


#include "calcul.h"


// ací implementes els métodes

Calcul::Calcul() {
	dimensio = 0;
	valors = NULL;
	Bmes_no_buida = false;
	num_bases = 0;
}

Calcul::Calcul(int d, Fraccio *v) {
	dimensio = d;
	valors = v;
	Bmes_no_buida = false;
	num_bases = 0;

	matriu = Matriu(d, d);
	tmp_inv = Matriu(d, 2*d);
	clon = Matriu(d, d);
}

void Calcul::calcular(const Combination *c) {
	matriu.binary_array(c);
	clon = matriu;
	if (matriu.det() != 0) {
		num_bases++;
		clon.inv(tmp_inv);
	}
}

bool Calcul::final_calcul() {
	std::cout << "Nombre bases: " << num_bases << std::endl;
	return Bmes_no_buida;
}
