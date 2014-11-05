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
	int mm = mida_mostra;
	int ne = nombre_elements;
	int *idx = indices;
	int v;

	if (!primer && !exhaurit) {
		for (i=mm-1; i>=0 && idx[i]==i+ne-mm; i--) {
		}
		if (i < 0) {
			exhaurit = true;
			return NULL;
		} else {
			v = ++idx[i];
			for (int j=i+1; j < mm; j++) {
				idx[j] = ++v;
			}
			return indices;
		}
	} else {
		return NULL;
	}
}
