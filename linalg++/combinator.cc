#include "combinator.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>


Combinator::Combinator(int n, int m) {
    nombre_elements = n;
    mida_mostra = m;
    mida_bloc = -1;
    exhaurit = (nombre_elements < mida_mostra);
    primer = true;
    indices  = (Combination*)malloc(m * sizeof(Combination));
    for (int i = 0; i < m; i++) {
        indices[i] = i + 1;
    }
}

Combinator::Combinator(int n, int m, const Combination *seed, int block_size) {
    nombre_elements = n;
    mida_mostra = m;
    mida_bloc = block_size;
    exhaurit = (nombre_elements < mida_mostra);
    primer = true;
    indices  = (Combination*)malloc(m * sizeof(Combination));
    for (int i = 0; i < m; i++) {
        indices[i] = seed[i];
    }
}

Combinator::~Combinator() {
    free(indices);
}

const Combination * Combinator::first() {
    if (!primer || exhaurit) {
        return NULL; // throw
    }
    primer = false;
    return indices;
}

const Combination * Combinator::next(int block_size) {
    int i;
    int mm = mida_mostra;
    int ne = nombre_elements;
    unsigned int llindar = ne - mm + 1; // evita recalcular-ho cada iteració
    Combination *idx = indices;
    int v;

    if (mida_bloc > 0) {
        mida_bloc--;
        if (mida_bloc == 0) {
            exhaurit = true;
        }
    }
    if (!primer && !exhaurit) {
        while (block_size--) {
            for (i=mm-1; i>=0 && idx[i]==i+llindar; i--) {
            }
            if (i < 0) {
                exhaurit = true;
                return NULL;
            } else {
                v = ++idx[i];
                for (int j=i+1; j < mm; j++) {
                    idx[j] = ++v;
                }
            }
        }
        return indices;
    } else {
        return NULL;
    }
}
