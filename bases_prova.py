# -*- coding: utf-8 -*-

# $Id$

import bases_treballador as treballador
import bases_coordinador as coordinador

for c in coordinador.generador():
    resultat = treballador.calcul(c)
    coordinador.processador(c, resultat)
coordinador.processador.tancar()
