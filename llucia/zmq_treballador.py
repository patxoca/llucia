# -*- coding: utf-8 -*-

# $Id$

import logging
import sys
import time

import numpy
import zmq

from llucia import utils


_logger = logging.getLogger("treb")


def arrancar_treballador(calculador, productor, empaquetador, progres=False):
    """Arranca un treballador basat en ZMQ.

    Connecta amb el productor en l'adreça PRODUCTOR. Descarrega i
    processa paquets fins rebre un paquet de finalització.

    CALCULADOR és una funció que rep un paquet i retorna una llista de
    resultats, un per cada element del paquet, en el mateix ordre.

    Si PROGRES val True imprimeix per pantalla un punt cada vegada que
    es processa un paquet.

    """
    _logger.info(u"Iniciant treballador")
    _logger.info(u"  productor: %s", productor)

    context = zmq.Context()
    receiver = context.socket(zmq.PULL)
    receiver.hwm = 1
    receiver.connect(productor)

    _logger.info(u"Processant paquets")
    num_paquets = 0
    t0 = time.time()
    while True:
        idpaquet, paquet = empaquetador.desempaquetar(receiver.recv())
        if idpaquet == -1:
            break
        if progres:
            sys.stdout.write(".")
            sys.stdout.flush()
        num_paquets += 1
        resultat = calculador(paquet)
    _logger.info(u"%i paquets processats en %.2f segons", num_paquets, time.time() - t0)

    _logger.info(u"Finalitzant treballador")



if __name__ == "__main__":
    from llucia import combinator
    from llucia import configuracio
    from llucia import domini

    total = 0

    def calculador(paquet):
        global total
        resultat = list(numpy.linalg.det(paquet))
        for i in resultat:
            if i:
                total += 1
        return resultat

    def descodificar_paquet(paquet):
        assert len(paquet) == 1
        valors, llavor, mida_lot = paquet[0]
        _logger.debug("%r", llavor)
        l = list(combinator.combinador(valors, llavor, mida_lot))
        return l

    utils.configurar_logging(
        configuracio.SERVIDOR_LOG,
        configuracio.PORT_LOG,
        configuracio.NIVELL_LOG_TREBALLADOR,
    )

    arrancar_treballador(
        calculador=utils.composar_funcions(
            descodificar_paquet,
            calculador
        ),
        productor=configuracio.PRODUCTOR,
        empaquetador=utils.Empaquetador(configuracio.NIVELL_COMPRESSIO),
        progres=configuracio.VERBOSE,
    )
    print "%i bases" % total
