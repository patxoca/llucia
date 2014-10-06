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

# si es produeix un error de connexió amb el servidor s'introdueix un
# temps d'espera per tornar a intertar-ho. Inicialment l'espera és de
# 1 segon i es va doblant en cada error succesiu fins arribar o
# superar el valor de MAX_ESPERA. Una vegada establida la connexió el
# valor de l'espera es torna a "no-espera" (None, no zero).
MAX_ESPERA = 64

logger = logging.getLogger("cli")
logger_down = logger
logger_up = logger

cua_entrada = Queue.Queue(maxsize=1)
cua_sortida = Queue.Queue(maxsize=1)

# variable que emmagatzema el ID assignat al treballador pel
# coordinador
IDCLIENT = None


def get_temps_espera(temps):
    if temps is None:
        return 1
    if temps >= MAX_ESPERA:
        return temps
    return temps * 2

def descarregador(idclient):
    """Aquesta funció intenta que sempre hi hagi un paquet disponible en
    la cua d'entrada per evitar que la funció 'calculador()' hagi
    d'esperar a que s'en descarregui un.

    """
    logger_down.debug("creant connexio amb %s:%s" % (SERVIDOR_RPC, PORT_RPC))
    pdown = client_rpc.RPCProxy(SERVIDOR_RPC, PORT_RPC)
    espera = None
    while True:
        logger_down.debug("descarregant")
        try:
            paquet = pdown.descarregar(idclient)
        except httplib.HTTPException as e:
            logger_down.error("error de connexio %s", str(e))
            espera = get_temps_espera(espera)
            logger_down.info("esperant %is per reintentar", espera)
            time.sleep(espera)
        else:
            espera = None
            logger_down.debug("encuant %i", paquet["id"])
            cua_entrada.put(paquet)
            if paquet["id"] == -1:
                logger_down.info("rebut paquet de finalitzacio")
                break
    logger_down.info("finalitzant descarregador")

def pujador(idclient):
    """Aquesta funció intenta pujar el resultat en paralel al
    càlcul/descarrega.

    """
    logger_up.debug("creant connexio amb %s:%s" % (SERVIDOR_RPC, PORT_RPC))
    pup = client_rpc.RPCProxy(SERVIDOR_RPC, PORT_RPC)
    idpaquet = None
    espera = None
    while True:
        if idpaquet is None:
            logger_up.debug("desencuant")
            idpaquet, paquet = cua_sortida.get()
        else:
            logger_up.debug("reintentant")
        if idpaquet == -1:
            logger_up.info("rebut paquet de finalitzacio")
            break
        logger_up.debug("pujant %i", idpaquet)
        try:
            pup.pujar(idclient, idpaquet, paquet)
        except httplib.HTTPException as e:
            logger_up.error("error de connexio %s", str(e))
            espera = get_temps_espera(espera)
            logger_up.info("esperant %is per reintentar", espera)
            time.sleep(espera)
        else:
            idpaquet = None
    logger_up.info("finalitzant pujador")

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
        if idpaquet == -1:
            logger.info("rebut paquet de finalitzacio")
            cua_sortida.put((-1, None))
            break

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

        logger.info("#paquets: %i    #comb %i (%i)    #bas %i (%i)",
                    nombre_paquets,
                    total_combinacions,
                    len(candidats),
                    total_bases,
                    len(resultat)
        )
        logger.info("    TT: %9.2f    TD: %9.2f    TC: %9.2f    TP: %9.2f",
                    temps_total,
                    temps_descarrega,
                    temps_calcul,
                    temps_pujada
        )
        logger.info("                    %%TD:    %6.2f   %%TC:    %6.2f   %%TP:    %6.2f",
                    temps_descarrega / temps_total * 100,
                    temps_calcul / temps_total * 100,
                    temps_pujada / temps_total * 100
        )
    logger.info("finalitzant calculador")


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

    proxy = client_rpc.RPCProxy(SERVIDOR_RPC, PORT_RPC)
    logger.debug("registrant client")
    IDCLIENT = proxy.registrar()
    if IDCLIENT is not None:
        logger.debug("client registrat: %i", IDCLIENT)
        logger = logging.getLogger("cli.%03i" % IDCLIENT)
        logger_down = logging.getLogger("cli.%03i.down" % IDCLIENT)
        logger_up = logging.getLogger("cli.%03i.up" % IDCLIENT)

        fil_down = threading.Thread(target=descarregador, args=(IDCLIENT, ))
        fil_up   = threading.Thread(target=pujador, args=(IDCLIENT, ))

        fil_down.start()
        fil_up.start()
        calculador(fer_algo)
        fil_down.join()
        fil_up.join()
    else:
        logger.error("client no registrat")
