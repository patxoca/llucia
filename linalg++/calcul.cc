// -*- coding: utf-8 -*-

// $Id$

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

void Calcul::final_calcul(ResultatCalcul &r) {
	std::cout << "Nombre bases: " << num_bases << std::endl;
	r.bmes_no_buida = Bmes_no_buida;
}
