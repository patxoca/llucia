// -*- coding: utf-8 -*-

// $Id$

#ifndef _ACUMULADOR_H_
#define _ACUMULADOR_H_

#include <time.h>
#include "tipus.h"


class Acumulador {
    long num_combinacions;
    long num_no_det;
    time_t t0;

  public:
    Acumulador();
    void processar_resultat(const ResultatCalcul & r);
    void final_processament();
};

#endif /* _ACUMULADOR_H_ */
