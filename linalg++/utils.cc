// -*- coding: utf-8 -*-

// $Id:$

#include "utils.h"


Coalicio enter_a_coalicio(Enter valor, int dimensio) {
	Coalicio v = Coalicio(dimensio, 0);

	for (int i = 0; valor != 0 && i < NOMBRE_MAXIM_JUGADORS; i++, valor >>= 1) {
		v[i] = valor & 1;
	}
	return v;
}

std::vector<Coalicio> generar_coalicions(int dimensio) {
	int nombre_coalicions = (1 << dimensio) - 1;
	std::vector<Coalicio> c = std::vector<Coalicio>(nombre_coalicions);

	for (int i = 1; i <= nombre_coalicions; i++) {
		c[i - 1] = enter_a_coalicio(i);
	}
	return c;
}
