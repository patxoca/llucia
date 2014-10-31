# -*- coding: utf-8 -*-

# $Id$

import itertools
import logging
import sys
import time

import zmq

from llucia import utils

PAQUET_FINALITZACIO = (-1, None)

_logger = logging.getLogger("prod")

def arrancar_productor(generador, adreca, progres=False):
    """Arranca un productor basat an ZMQ.

    Inicia un servei que consumeix els elements generats per GENERADOR
    i els serveix als treballadors en l'adreça ADRECA.

    Els elements enviats als treballadors son tuples (idpaquet, paquet),
    on IDPAQUET és un valor enter major que zero, diferent per cada
    paquet, i PAQUET son les dades a processar.

    Si PROGRES val True imprimeix per pantalla un punt cada vegada que
    s'envia un paquet.

    """
    _logger.info(u"Iniciant productor")
    _logger.info(u"  adreça             : %s", adreca)

    context = zmq.Context()
    socket = context.socket(zmq.REP)
    socket.bind(adreca)

    _logger.info(u"Esperant peticions dels treballadors")
    generador = enumerate(generador, 1)
    t0 = time.time()
    num_treb = 0
    continuar = True
    paquets_pendents = {}
    while continuar:
        if progres:
            sys.stdout.write(".")
            sys.stdout.flush()
        idtreb, msg, params = socket.recv_pyobj()
        reposta = None
        if msg == "GET":
            if params is not None:
                # és el id del paquet anterior
                if params in paquets_pendents:
                    del paquets_pendents[params]
                else:
                    _logger.warning("ID de paquet desconegut %i (client %i)", params, idtreb)
            try:
                resposta = idpaquet, paquet = generador.next()
            except StopIteration:
                _logger.info("Dades exhaurides en %.2f segons", time.time() - t0)
                generador = itertools.repeat(PAQUET_FINALITZACIO)
                resposta = PAQUET_FINALITZACIO
            else:
                if idpaquet > 0:
                    paquets_pendents[idpaquet] = (idtreb, paquet)
        elif msg == "ABORT":
            _logger.info(u"Rebut ABORT del treballador #%i", idtreb)
            generador = itertools.repeat(PAQUET_FINALITZACIO)
        elif msg == "REG":
            num_treb += 1
            _logger.info(u"Registrant treballador #%i", num_treb)
            resposta = num_treb
        elif msg == "UNREG":
            assert num_treb >= 1
            num_treb -= 1
            _logger.info(u"Desregistrant treballador #%i", idtreb)
            if num_treb == 0:
                continuar = False
        else:
            _logger.warning(u"Missatge desconegut '%s'", msg)

        socket.send_pyobj(resposta)

    _logger.info(u"Finalitzant productor")



if __name__ == "__main__":
    from llucia import combinator
    from llucia import configuracio
    from llucia import domini

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
            yield (valors, i, configuracio.MAX_COMBINACIONS_PAQUET)

    utils.configurar_logging(
        configuracio.SERVIDOR_LOG,
        configuracio.PORT_LOG,
        configuracio.NIVELL_LOG_COORDINADOR,
    )

    arrancar_productor(
        generador(),
        adreca=configuracio.PRODUCTOR,
        progres=configuracio.VERBOSE,
    )
