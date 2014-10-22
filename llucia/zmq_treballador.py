# -*- coding: utf-8 -*-

# $Id$

import logging
import os
import random
import sys
import time

import numpy
import zmq

from llucia import utils


_logger = logging.getLogger("W%s%05i" % (os.environ["HOSTNAME"], os.getpid()))


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
    receiver = context.socket(zmq.REQ)
    receiver.hwm = 1
    receiver.connect(productor)

    # registra el treballador
    _logger.info(u"Registrant client")
    receiver.send("REG")
    id_treballador = int(receiver.recv())
    _logger.info(u"Client registrat #%i", id_treballador)

    _logger.info(u"Processant paquets")
    num_paquets = 0
    t0 = time.time()
    while True:
        receiver.send("GET")
        idpaquet, paquet = empaquetador.desempaquetar(receiver.recv())
        if idpaquet == -1:
            break
        if progres:
            sys.stdout.write(".")
            sys.stdout.flush()
        num_paquets += 1
        try:
            resultat = calculador(paquet)
        except utils.Abort:
            _logger.info(u"Avortant càlcul")
            receiver.send("ABORT")
            receiver.recv()
            break
    _logger.info(u"%i paquets processats en %.2f segons", num_paquets, time.time() - t0)
    _logger.info(u"Desregistrant client.")
    receiver.send("UNREG")
    receiver.recv()
    _logger.info(u"Client desregistrat.")

    _logger.info(u"Finalitzant treballador")



if __name__ == "__main__":
    from llucia import combinator
    from llucia import configuracio
    from llucia import domini

    total = 0
    limit = 1000000000
    llindar = 10000

    def calculador(paquet):
        global total
        global llindar
        resultat = list(numpy.linalg.det(paquet))
        if random.random() > 0.95:
            raise utils.Abort()
        for i in resultat:
            if i:
                total += 1
        if total >= llindar:
            if total >= limit:
                llindar += limit
            else:
                llindar *= 10
            _logger.info("#bases: %i", total)
        return resultat

    def descodificar_paquet(paquet):
        valors, llavor, mida_lot = paquet
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
