# -*- coding: utf-8 -*-

# $Id$

import client_rpc
import domini
import httplib
import logging
import logging.handlers
import Queue
import time
import threading


# paràmetres del servidor RPC
SERVIDOR_RPC = "localhost"
PORT_RPC = "8000"

# paràmetres del servidor de log
SERVIDOR_LOG = SERVIDOR_RPC
PORT_LOG = 8001

logger = logging.getLogger("client")

cua_entrada = Queue.Queue(maxsize=1)
cua_sortida = Queue.Queue(maxsize=1)



def descarregador():
    """Aquesta funció intenta que sempre hi hagi un paquet disponible en
    la cua d'entrada per evitar que la funció 'calculador()' hagi
    d'esperar a que s'en descarregui un.

    """
    logger.debug("descarregador:creant connexio amb %s:%s" % (SERVIDOR_RPC, PORT_RPC))
    pdown = client_rpc.RPCProxy(SERVIDOR_RPC, PORT_RPC)
    while True:
        logger.debug("descarregador:descarregant")
        try:
            paquet = pdown.descarregar()
        except httplib.HTTPException as e:
            logger.error("descarregador:error de connexio %s", str(e))
        else:
            logger.debug("descarregador:encuant %i", paquet["id"])
            cua_entrada.put(paquet)

def pujador():
    """Aquesta funció intenta pujar el resultat en paralel al
    càlcul/descarrega.

    """
    logger.debug("pujador:creant connexio amb %s:%s" % (SERVIDOR_RPC, PORT_RPC))
    pup   = client_rpc.RPCProxy(SERVIDOR_RPC, PORT_RPC)
    idpaquet = None
    while True:
        if idpaquet is None:
            logger.debug("pujador:desencuant")
            idpaquet, paquet = cua_sortida.get()
        else:
            logger.debug("pujador:reintentant")
        logger.debug("pujador:pujant %i", idpaquet)
        try:
            pup.pujar(idpaquet, paquet)
        except httplib.HTTPException as e:
            logger.error("pujador:error de connexio %s", str(e))
        else:
            idpaquet = None

def calculador(funcio):
    """Aquesta funció coordina el càlcul i recull estadístiques per
    analitzar el rendiment.

    FUNCIO és la funció responsable de fer el càlcul. FUNCIO rebra una
    llista de llistes de vectors (els candidats a base) i ha de
    retorna una llista amb les bases.

    """
    temps_descarrega = 0
    temps_calcul = 0
    temps_pujada = 0
    nombre_paquets = 0
    total_combinacions = 0
    total_bases = 0
    while True:
        t1 = time.time()

        # @@
        dades = cua_entrada.get()

        t2 = time.time()
        idpaquet = dades["id"]
        candidats = dades["dades"]
        nombre_paquets += 1
        total_combinacions += len(candidats)

        # @@
        resultat = funcio(candidats)

        t3 = time.time()
        total_bases += len(resultat)

        # @@
        cua_sortida.put((idpaquet, resultat))

        t4 = time.time()

        temps_descarrega += t2 - t1
        temps_calcul += t3 - t2
        temps_pujada = t4 - t3
        temps_total = temps_descarrega + temps_calcul + temps_pujada

        logger.debug("#paquets: %i    #comb %i (%i)    #bas %i (%i)",
                      nombre_paquets,
                      total_combinacions,
                      len(candidats),
                      total_bases,
                      len(resultat)
        )
        logger.debug("    TT: %9.2f    TD: %9.2f    TC: %9.2f    TP: %9.2f",
                      temps_total,
                      temps_descarrega,
                      temps_calcul,
                      temps_pujada
        )
        logger.debug("                    %%TD:    %6.2f   %%TC:    %6.2f   %%TP:    %6.2f",
                      temps_descarrega / temps_total * 100,
                      temps_calcul / temps_total * 100,
                      temps_pujada / temps_total * 100
        )

def fer_algo(candidats):
    resultats = []
    for i in domini.generar_bases(candidats):
        resultats.append(i)
    return resultats


if __name__ == "__main__":
    logging.basicConfig(
        datefmt="%H:%M:%S",
        format="%(asctime)-9s - %(message)s",
        level=logging.DEBUG
    )
    socketHandler = logging.handlers.SocketHandler(
        SERVIDOR_LOG,
        PORT_LOG
    )
    logger.addHandler(socketHandler)

    fil_down = threading.Thread(target=descarregador)
    fil_up   = threading.Thread(target=pujador)

    fil_down.start()
    fil_up.start()
    calculador(fer_algo)
