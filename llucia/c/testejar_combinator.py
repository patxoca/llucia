# -*- coding: utf-8 -*-

# $Id$

# Aquest script exercita el mòdul 'combinator' en condicions
# (aproximadament) reals.


import logging

from llucia import combinator
from llucia import configuracio
from llucia import domini
from llucia import utils


_logger = logging.getLogger("core")

def generador():
    """Genera tripletes (valor, llavor, mida)
    """
    valors = tuple(domini.generar_vectors(configuracio.DIMENSIO))
    g = combinator.lot_combinacions(
        len(valors),
        configuracio.DIMENSIO,
        configuracio.MAX_COMBINACIONS_PAQUET
    )
    for i in g:
        #_logger.debug("gen:%r", i)
        yield (valors, i, configuracio.MAX_COMBINACIONS_PAQUET)

def expandir_paquet(paquet):
    assert len(paquet) == 1
    valors, llavor, mida_lot = paquet[0]
    #_logger.debug("des:%r", llavor)
    l = list(combinator.combinador(valors, llavor, mida_lot))
    return l



if __name__ == "__main__":
    utils.configurar_logging(
        configuracio.SERVIDOR_LOG,
        configuracio.PORT_LOG,
        configuracio.NIVELL_LOG_TREBALLADOR,
    )

    total = 0
    for i in generador():
        l = expandir_paquet([i])
        total += len(l)

    _logger.info("finalitzat; %i combinacions", total)
