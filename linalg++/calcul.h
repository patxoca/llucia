// -*- coding: utf-8 -*-

// $Id$

#ifndef _CALCUL_H_
#define _CALCUL_H_

#include "combinator.h" // Combination
#include "tipus.h"      // Fraccio i Matriu


struct AbortCalculationException : public std::exception {
};


class Calcul {

	int      dimensio;
	Fraccio *valors;
	bool     Bmes_no_buida;
	long     num_bases;

	Matriu   matriu;
	Matriu   tmp_inv;
	Matriu   clon;

  public:

	Calcul();
	Calcul(int dim, Fraccio *valors);

	void calcular(const Combination *candidats);
	void final_calcul(ResultatCalcul &r);

};

#endif /* _CALCUL_H_ */
