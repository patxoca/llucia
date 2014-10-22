# -*- coding: utf-8 -*-

# $Id$

import itertools
import logging
import sys
import time

import zmq

from llucia import utils


_logger = logging.getLogger("prod")


def arrancar_productor(generador, adreca, empaquetador,
                       mida_paquet=10000, progres=False):
    """Arranca un productor basat an ZMQ.

    Inicia un servei que consumeix els elements generats per
    GENERADOR, els empaqueta en paquets de mida MIDA_PAQUET i els fica
    a disposició dels treballadors en una cua en l'adreça ADRECA.

    Els elements afegits a la cua son parelles (idpaquet, paquet), on
    IDPAQUET és un valor enter major que zero, diferent per cada
    paquet.

    Si PROGRES val True imprimeix per pantalla un punt cada vegada que
    s'envia un paquet.

    """
    _logger.info(u"Iniciant productor")
    _logger.info(u"  adreça             : %s", adreca)
    _logger.info(u"  ítems/paquet       : %i", mida_paquet)

    context = zmq.Context()
    sender = context.socket(zmq.REP)
    sender.bind(adreca)

    _logger.info(u"Enviant paquets als treballadors")
    generador = enumerate(generador, 1)
    t0 = time.time()
    num_treb = 0
    while True:
        if progres:
            sys.stdout.write(".")
            sys.stdout.flush()
        msg = sender.recv()
        if msg == "GET":
            try:
                idpaquet, paquet = generador.next()
            except StopIteration:
                _logger.info("Dades exhaurides en %.2f segons", time.time() - t0)
                generador = itertools.repeat((-1, None))
                idpaquet, paquet = generador.next()
            sender.send(empaquetador.empaquetar((idpaquet, paquet)))
        elif msg == "ABORT":
            _logger.info(u"Rebut ABORT")
            generador = itertools.repeat((-1, None))
            sender.send(empaquetador.empaquetar(generador.next()))
        elif msg == "REG":
            num_treb += 1
            _logger.info(u"Registrant treballador #%i", num_treb)
            sender.send(str(num_treb))
        elif msg == "UNREG":
            sender.send("OK")
            assert num_treb >= 1
            num_treb -= 1
            _logger.info(u"Desregistrant treballador")
            if num_treb == 0:
                break
        else:
            _logger.warning(u"Missatge desconegut '%s'", msg)
            sender.send(empaquetador.empaquetar((0, None)))

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
            _logger.debug("%r", i)
            yield (valors, i, configuracio.MAX_COMBINACIONS_PAQUET)

    utils.configurar_logging(
        configuracio.SERVIDOR_LOG,
        configuracio.PORT_LOG,
        configuracio.NIVELL_LOG_COORDINADOR,
    )

    arrancar_productor(
        generador(),
        adreca=configuracio.PRODUCTOR,
        empaquetador=utils.Empaquetador(configuracio.NIVELL_COMPRESSIO),
        mida_paquet=1,
        progres=configuracio.VERBOSE,
    )
