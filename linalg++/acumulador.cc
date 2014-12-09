// -*- coding: utf-8 -*-

// $Id$

#include <iostream>

#include "acumulador.h"


Acumulador::Acumulador() {
    num_combinacions = 0;
    num_no_det = 0;
    t0 = time(NULL);
}

void Acumulador::processar_resultat(const ResultatCalcul & r) {
    num_combinacions += r.num_combinacions;
    num_no_det += r.num_no_det;

    std::cout << "==============================\n";
    std::cout << "Nombre combinacions  : " << r.num_combinacions << std::endl;
    std::cout << "Nombre no determinant: " << r.num_no_det << std::endl;
    std::cout << "Temps total (segons) : " << r.hora_final - r.hora_inici << std::endl;
}

void Acumulador::final_processament() {
    std::cout << "==============================\n";
    std::cout << "Nombre total combinacions  : " << num_combinacions << std::endl;
    std::cout << "Nombre total no determinant: " << num_no_det << std::endl;
    std::cout << "Temps total (segons) : " << time(NULL) - t0 << std::endl;
}
