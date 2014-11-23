// -*- coding: utf-8 -*-

// $Id:$

#ifndef TIPUS_H
#define TIPUS_H

#include <vector>

#include "array.h"
#include "rational.h"


typedef unsigned int       Coalicio; // màxim 32 o 64 jugadors
typedef Rational<long>     Fraccio;
typedef Array<Fraccio>     Matriu;


#define DIMENSIO 7
/* nombre de combinacions per paquet */
#define MIDA_PAQUET 1000000
#define NOMBRE_MAXIM_JUGADORS ((int)(sizeof(Enter) * 8))
#define NOMBRE_COALICIONS ((1 << DIMENSIO) - 1)
#define COALICIO_TOTAL ((1 << DIMENSIO) - 1)

#endif /* TIPUS_H */
