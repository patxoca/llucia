#include "combinator.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

Combinator::Combinator(int n, int m) {
	nombre_elements = n;
	mida_mostra = m;
	exhaurit = (nombre_elements < mida_mostra);
	primer = true;
	indices  = (int*)malloc(m * sizeof(int));
	for (int i = 0; i < m; i++) {
		indices[i] = i;
	}
}

Combinator::~Combinator() {
	free(indices);
}

const int * Combinator::first() {
	if (!primer || exhaurit) {
		return NULL; // throw
	}
	primer = false;
	return indices;
}

const int * Combinator::next() {
	int i;

	if (!primer && !exhaurit) {
		for (i=mida_mostra-1; i>=0 && indices[i]==i+nombre_elements-mida_mostra; i--) {
		}
		if (i < 0) {
			exhaurit = true;
			return NULL;
		} else {
			indices[i]++;
			for (int j=i+1; j < mida_mostra; j++) {
				indices[j] = indices[j - 1] + 1;
			}
			return indices;
		}
	} else {
		return NULL;
	}
}

#define NOMBRE_ELEMENTS 127
#define MIDA_MOSTRA 7

int main(int argc, char **argv) {
	Combinator c(NOMBRE_ELEMENTS, MIDA_MOSTRA);
	clock_t t0, tf;
	long k = 0;

	t0 = clock();
	for (const int *i=c.first(); i != NULL; i = c.next()) {
		k += 1;
		// for (Vector::const_iterator j = i->begin(); j != i-> end(); ++j) {
		// 	std::cout << *j;
		// }
		// std::cout << std::endl;
	}
	tf = clock();
	std::cout << (tf - t0) / (double)CLOCKS_PER_SEC << "segons\n";
}
