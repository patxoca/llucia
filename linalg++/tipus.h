// -*- coding: utf-8 -*-

// $Id$

#ifndef TIPUS_H
#define TIPUS_H

#include <climits>
#include <time.h>
#include <vector>

#include "array.h"
#include "rational.h"


typedef unsigned int             Coalicio; // màxim 32 o 64 jugadors
typedef Rational<long, INT_MAX>  Fraccio;
typedef Array<Fraccio>           Matriu;

struct ResultatCalcul {
    // dades afegides pel calcul
    bool bmes_no_buida;

    // estadistiques afegides pel treballador
    long   num_combinacions;
    long   num_no_det;
    time_t hora_inici;
    time_t hora_final;
};


/* nombre de combinacions per paquet */
#define MIDA_PAQUET 1000000
#define NOMBRE_MAXIM_JUGADORS ((int)(sizeof(Enter) * 8))


#endif /* TIPUS_H */
