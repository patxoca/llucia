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


def arrancar_treballador(calculador, productor, progres=False):
    """Arranca un treballador basat en ZMQ.

    Connecta amb el productor en l'adreça PRODUCTOR. Descarrega i
    processa paquets fins rebre un paquet de finalització.

    CALCULADOR és una funció que rep un paquet i realitza algun càlcul
    amb ell. El resultat és ignorat. Si dispara una excepció
    ``llucia.utils.Abort`` s'envia un missatge d'avortament al
    productor.

    Si PROGRES val True imprimeix per pantalla un punt cada vegada que
    es processa un paquet.

    """
    _logger.info(u"Iniciant treballador")
    _logger.info(u"  productor: %s", productor)

    context = zmq.Context()
    receiver = context.socket(zmq.REQ)
    receiver.connect(productor)

    # registra el treballador
    _logger.info(u"Registrant treballador")
    receiver.send_pyobj((None, "REG", None))
    id_treballador = receiver.recv_pyobj()
    _logger.info(u"Treballador registrat #%i", id_treballador)

    _logger.info(u"Processant paquets")
    num_paquets = 0
    t0 = time.time()
    idpaquet = None
    while True:
        receiver.send_pyobj((id_treballador, "GET", idpaquet))
        idpaquet, paquet = receiver.recv_pyobj()
        if idpaquet == -1:
            _logger.info(u"Rebut paquet de finalització")
            break
        if progres:
            sys.stdout.write(".")
            sys.stdout.flush()
        num_paquets += 1
        try:
            resultat = calculador(paquet)
        except utils.Abort:
            _logger.info(u"Avortant càlcul")
            receiver.send_pyobj((id_treballador, "ABORT", None))
            receiver.recv_pyobj()
            break
    _logger.info(u"%i paquets processats en %.2f segons", num_paquets, time.time() - t0)
    _logger.info(u"Desregistrant treballador.")
    receiver.send_pyobj((id_treballador, "UNREG", None))
    receiver.recv_pyobj()
    _logger.info(u"Treballador desregistrat.")

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
        if random.random() > 0.99:
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
        progres=configuracio.VERBOSE,
    )
    print "%i bases" % total
