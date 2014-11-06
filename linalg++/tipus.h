// -*- coding: utf-8 -*-

// $Id:$

#ifndef TIPUS_H
#define TIPUS_H

#include <vector>

#include "matrix.h"
#include "rational.h"


typedef unsigned int       Enter;
typedef std::vector<Enter> Coalicio;
typedef Rational<Enter>    Fraccio;
typedef Matrix<Fraccio> Matriu;


#define DIMENSIO 6
#define NOMBRE_MAXIM_JUGADORS ((int)(sizeof(Enter) * 8))
#define NOMBRE_COALICIONS ((1 << DIMENSIO) - 1)


#endif /* TIPUS_H */
