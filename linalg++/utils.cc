// -*- coding: utf-8 -*-

// $Id:$

#include "utils.h"


Coalicio enter_a_coalicio(Enter valor) {
	Coalicio v = Coalicio(DIMENSIO, 0);

	for (int i = 0; valor != 0 && i < NOMBRE_MAXIM_JUGADORS; i++, valor >>= 1) {
		v[i] = valor & 1;
	}
	return v;
}

std::vector<Coalicio> generar_coalicions() {
	std::vector<Coalicio> c = std::vector<Coalicio>(NOMBRE_COALICIONS);

	for (int i = 1; i <= NOMBRE_COALICIONS; i++) {
		c[i - 1] = enter_a_coalicio(i);
	}
	return c;
}
