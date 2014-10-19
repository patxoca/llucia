# -*- coding: utf-8 -*-

# $Id$

import logging
import sys
import time

import zmq

from llucia import utils


_logger = logging.getLogger("prod")


def arrancar_productor(generador, adreca, empaquetador, max_clients=10,
                       mida_paquet=10000, progres=False, max_paquets=None):
    """Arranca un productor basat an ZMQ.

    Inicia un servei que consumeix els elements generats per
    GENERADOR, els empaqueta en paquets de mida MIDA_PAQUET i els fica
    a disposició dels treballadors en una cua en l'adreça ADRECA.

    Els elements afegits a la cua son parelles (idpaquet, paquet), on
    IDPAQUET és un valor enter major que zero, diferent per cada
    paquet.

    En exhuarir-se el generador fica en la cua MAX_CLIENTS paquets de
    finalització, parelles (-1, None).

    MAX_PAQUETS permet limitar el nombre de paquets que es generen,
    acotant el consum de memòria. Si no s'especifica cap valor
    s'utilitza el valor de MAX_CLIENTS multiplicat per 3. La
    heurística és que un treballador té un paquet descarregant, un en
    cua i un tercer processant.

    Si PROGRES val True imprimeix per pantalla un punt cada vegada que
    s'envia un paquet.

    """
    if max_paquets is None:
        max_paquets = max_clients * 3
    _logger.info(u"Iniciant productor")
    _logger.info(u"  adreça             : %s", adreca)
    _logger.info(u"  ítems/paquet       : %i", mida_paquet)
    _logger.info(u"  max clients/paquets: %i/%i", max_clients, max_paquets)

    context = zmq.Context()
    sender = context.socket(zmq.PUSH)
    sender.set_hwm(max_paquets)
    sender.bind(adreca)

    _logger.info(u"Enviant paquets als treballadors")
    t0 = time.time()
    for idpaquet, paquet in enumerate(utils.generar_lots(generador, mida_paquet), 1):
        if progres:
            sys.stdout.write(".")
            sys.stdout.flush()
        sender.send(empaquetador.empaquetar((idpaquet, paquet)))
    _logger.info(u"Tots els paquets enviats en %.2f segons", time.time() - t0)

    _logger.info(u"Enviant %i paquets de finalització", max_clients)
    for i in xrange(max_paquets):
        sender.send(empaquetador.empaquetar((-1, None)))

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
        max_clients=configuracio.MAX_TREBALLADORS,
        mida_paquet=1,
        max_paquets=2,
        progres=True,
    )
    # Actualment no es controla que els treballadors hagin finalitzat.
    # Informa l'usuari.
    raw_input("Prem Return quan hagin acabat els treballadors")
