// -*- coding: utf-8 -*-

// $Id:$

#ifndef _CALCUL_H_
#define _CALCUL_H_

#include "combinator.h" // Combination
#include "tipus.h"      // Fraccio i Matriu


// Excepció utilitzada per indicar al codi de gestió del treballador que cal
// avortar el càlcul. Probablement se li podria passar un string per incloure
// detalls sobre el motiu (p.e. ha fallat tal condicio), hauria de mirar-ho.

struct AbortCalculationException : public std::exception {
};


class Calcul {
	// Aci es declaren les variables que et calen per varis càlculs, per
	// exemple la matriu candidat, la matriu temporal per la inversa, la taula
	// de valors associats a les coalicions, estadístiques etc.
	//
	// En certa forma son com variables globals, pero restringides a la
	// classe.

	int      dimensio;
	Fraccio *valors;
	bool     Bmes_no_buida;
	Matriu   matriu;
	Matriu   tmp_inv;

  public:
	// La classe defineix dos constructors (per raons tècniques):
	//
	// Un que no fa res (inicialitza els atributs amb valors per defecte
	// raonables):

	Calcul();

	// I un que rep paràmetres i inicialitza l'objecte per poder operar amb
	// ell:
	//
	// Paràmetres:
	//
	// - dim: dimensió
	//
	// - valors: array (vector) de fraccions, valor associat a cada coalició
	//
	// El nombre de paràmetres podrà variar si veiem que falta alguna cosa.
	//
	// A més d'emmagatzemar els paràmetres pot inicialitzar les variables
	// temporals (matrius auxiliars etc.)

	Calcul(int dim, Fraccio *valors);

	// i dos mètodes públics:

	// Aquest fa el càlcul. Es cridat per cada candidat.
	//
	// Fixat que no cal passar-li ni la dimensió ni els valors. No retorna
	// res. Si li cal pot actualitzar els atributs de l'objecte, en particular
	// el valor de Bmes_no_buida;
	//
	// També podria rebre una estructura de dades per actualitzar
	// estadistiques que serien enviades al productor per anar informant a
	// l'usuari etc.

	void calcular(Combination *candidats);

	// Aquest es cridat al finalitzar el calcul. Normalment s'utilitza per
	// alliberar recursos que s'hagin pogut crear però per aquest cas no te
	// sentit.
	//
	// Podria retornar el valor de Bmes_no_buida per exemple o rebre una
	// estructura de dades en la que guarda el "resultat" del càlcul
	// (Bmes_no_buida + estadístiques ...)

	bool final_calcul();

  protected:

	// pots afegir tants mètodes protegits (o privats, en aquest context no hi
	// ha diferència) com et calguin.

	Fraccio valorar_coalicio(Coalicio c, Fraccio *punt);
};

#endif /* _CALCUL_H_ */
