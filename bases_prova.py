# -*- coding: utf-8 -*-

# $Id$

import itertools

import bases_treballador as treballador
import bases_coordinador as coordinador
import configuracio
import utils


lots = utils.generar_lots(
    coordinador.generador(),
    configuracio.MAX_COMBINACIONS_PAQUET
)

for lot in lots:
    resultat = treballador.calcul(lot)
    for c, r in itertools.izip(lot, resultat):
        coordinador.processador(c, r)
coordinador.processador.tancar()
