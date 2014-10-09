# -*- coding: utf-8 -*-

# $Id$

import itertools
import logging
import threading
import Queue

import configuracio
import synced_table
import servidor_rpc
import utils


MAX_CLIENTS = configuracio.MAX_TREBALLADORS
MAX_COMBINACIONS_PAQUET = configuracio.MAX_COMBINACIONS_PAQUET


logger = logging.getLogger("srv")

cua_sortida = Queue.Queue(maxsize=MAX_CLIENTS)
cua_entrada = Queue.Queue(maxsize=MAX_CLIENTS)

# Taula pels paquets pendents: aquesta taula emmagatzema els paquets
# que s'han enviat als treballadors però pels que encara no s'ha rebut
# el resultat del càlcul. Per poder distingir-los cada paquet te
# associat un ID únic.
#
# La idea (no està implementat) es ser resistent davant treballadors
# que deixen de funcionar, permetent reenviar paquets pendents a
# treballadors ociosos.

paquets_pendents = synced_table.SyncedTable()

taula_clients = synced_table.SyncedTable(maxsize=MAX_CLIENTS)

estat_productor = 0 # 0 per arrancar
                    # 1 produint
                    # 2 finalitzat

# variables on s'emmagatzemen els threads del productor i del
# consumidor respectivament.
prod_thread = None
cons_thread = None

def productor_finalitzat():
    return estat_productor == 2

@utils.trace(logger)
def productor(generador, mida_paquet):
    """El productor intenta que sempre hi hagi paquets disponibles en la
    cua de sortida de forma que la funció rpc_servir_paquet() pugui
    enviar dades als treballadors minimitzant l'espera.

    En finalitzar la producció encuarà MAX_CLIENTS paquets de
    finalització (amb valor None) per informar als clients del final
    del càlcul.

    COMPTE: si hi ha menys de MAX_CLIENTS actius no es consumiran tots
    els paquets de finalització i la cua no quedarà buida, no és un
    problema però impedeix utilitzar el test 'cua_sortida.empty()' per
    determinar si queden paquets per processar. Per una altra banda
    cal que s'hagin consumit *tots* els paquets de dades per tal de
    que el productor pugui emplenar la cua de sortida amb paquets de
    finalització. Que el productor hagi finalitzat sí implica que la
    cua de sortida no te paquets de dades per processar.

    """
    global estat_productor
    estat_productor = 1
    paquet = []
    for item in generador:
        paquet.append(item)
        if len(paquet) == mida_paquet:
            cua_sortida.put(paquet)
            paquet = []
    if paquet:
        cua_sortida.put(paquet)
    for i in xrange(MAX_CLIENTS):
        logger.debug("encuant paquet de finalitzacio per client %i", i + 1)
        cua_sortida.put(None)
    estat_productor = 2
    logger.info("FINALITZANT PRODUCTOR")

def consumidor(processador):
    """El consumidor processa els paquets de resultats enviats pels
    treballadors.

    Per cada resultat crida a la funcio 'processador' passant-li el
    ítem que ha originat el càlcul i el corresponent resultat.

    """
    while True:
        cua_buida = False
        try:
            # TODO: no m'agrada utilitzar un timeout
            paquet, resultats = cua_entrada.get(True, 1)
        except Queue.Empty:
            cua_buida = True
        else:
            for i, j in itertools.izip(paquet, resultats):
                processador(i, j)
        if cua_buida and productor_finalitzat() and not paquets_pendents:
            break
    logger.info("FINALITZANT CONSUMIDOR")

def check_client_registrat(idclient):
    """Verifica que el client IDCLIENT està registrat.

    Retorna l'objecte associat al client.

    Si el client no està registrat loggeja l'error i dispara una
    excepció ValueError.

    """
    if idclient not in taula_clients:
        logger.info("client no registrat: %i", idclient)
        raise ValueError("client no registrat")
    return taula_clients.get(idclient, rm=False)

@utils.trace(logger)
def rpc_registrar_client():
    """Registra un client.

    Registra un client i li retorna un identificador únic. Si s'ha
    assolit el màxim de clients retorna None.

    Actualment no es fa cap verificació de la identitat del client
    (falsos clients o clients duplicats).

    """
    estadistiques = {}
    try:
        idclient = taula_clients.put(estadistiques)
    except ValueError:
        idclient = None
    logger.debug("Registrant client: %s", idclient)
    return idclient

@utils.trace(logger)
def rpc_servir_paquet(idclient):
    """Funció cridada per un treballador per obtindre un paquet de dades.

    Retorna un diccionari amb les claus:

    * id:    identificador del paquet

    * dades: llista de ítems sobre els que fer el càlcul

    """
    client = check_client_registrat(idclient)
    # TODO: açò pot bloquejar indefinidament i no té en compte els
    # paquets pendents. No hauria de ser un problema si els
    # treballadors funcionen correctament. En tot cas, seria millor
    # comprovar si el productor ha finalitzat i llavors tirar dels
    # paquets pendents o retornar None.
    paquet = cua_sortida.get()
    if paquet is None:
        id_paquet = -1
        logger.info("No mes paquets, enviant paquet de finalitzacio")
    else:
        id_paquet = paquets_pendents.put(paquet)
        logger.debug("Servint paquet #%06i", id_paquet)
    return {
        "id"    : id_paquet,
        "dades" : paquet
    }

@utils.trace(logger)
def rpc_recepcionar_resultat(idclient, idpaquet, resultats):
    """Funció cridada pel client per pujar un paquet de resultats.

    Paràmetres:

    * idpaquet:  identificador del paquet al que correspon el resultat

    * resultats: llista amb els resultats d'aplicar el càlcul a cada
                 item del paquet original.

    """
    client = check_client_registrat(idclient)
    if idpaquet not in paquets_pendents:
        raise ValueError("paquet #%i no esta pendent")
    logger.debug("Recepcionant resultats #%06i", idpaquet)
    paquet = paquets_pendents.get(idpaquet)
    logger.debug("Encuant resultat")
    cua_entrada.put((paquet, resultats))
    logger.debug("Paquet recepcionat     #%06i" , idpaquet)



def main(generador, processador, funcions=()):
    """Inicia el coordinador.

    Paràmetres:

    - generador: una funció que retorna (yield) un element cada vegada
      que es cridada. Aquest element és el que rebrà el treballador
      per realitzar el càlcul.

    - processador: una funció que rep l'element generat per
      'generador' i el resultat del càlcul retornat pel treballador.

    - funcions: llista/tupla de parelles (funcio, nom) addicionals que
      es registren amb el coordinador per ficar-les a disposició dels
      treballadors.

    """

    prod_thread = threading.Thread(
        target=productor,
        args=(generador, MAX_COMBINACIONS_PAQUET)
    )
    cons_thread = threading.Thread(
        target=consumidor,
        args=(processador, )
    )

    logger.info("Iniciant productor")
    prod_thread.start()
    logger.info("Iniciant consumidor")
    cons_thread.start()
    logger.info("Arrancant coodinador")
    servidor_rpc.arrancar_servidor_rpc(
        (
            (rpc_servir_paquet, "descarregar"),
            (rpc_recepcionar_resultat, "pujar"),
            (rpc_registrar_client, "registrar"),
        ) + tuple(funcions)
    )
