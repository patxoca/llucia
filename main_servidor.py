# -*- coding: utf-8 -*-

# $Id$

import threading
import Queue

import synced_table
import domini
import servidor_rpc

# nombre de jugadors
DIMENSIO = 5

# nombre de combinacions en cada paquet enviat als treballadors
MAX_COMBINACIONS_PAQUET = 25000


cua_sortida = Queue.Queue(maxsize=10)
cua_entrada = Queue.Queue(maxsize=20)

# Taula pels paquets pendents: aquesta taula emmagatzema els paquets
# que s'han enviat als treballadors però pels que encara no s'ha rebut
# el resultat del càlcul. Per poder distingir-los cada paquet te
# associat un ID únic.
#
# La idea (no està implementat) es ser resistent davant treballadors
# que deixen de funcionar, permetent reenviar paquets pendents a
# treballadors ociosos.

paquets_pendents = synced_table.SyncedTable()


def productor(dimensio, mida_paquet):
    """El productor intenta que sempre hi hagi paquets disponibles en la
    cua de sortida de forma que la funció rpc_servir_paquet() pugui
    enviar dades als treballadors minimitzant l'espera.

    """
    paquet = []
    for combinacio in domini.generar_combinacions(dimensio):
        paquet.append(combinacio)
        if len(paquet) == mida_paquet:
            cua_sortida.put(paquet)
            paquet = []
    if paquet:
        cua_sortida.put(paquet)

def consumidor():
    """El consumidor processa els resultats enviats pels treballadors.

    """
    total = 0
    while True:
        paquet = cua_entrada.get()
        total += len(paquet)
        print "**** #bases %i" % total


def rpc_servir_paquet():
    """Funció cridada per un treballador per obtindre un paquet de dades.

    Retorna un diccionari amb les claus:

    * id:    identificador del paquet

    * dades: llista de llistes de N vectors de dimensió N (candidats a
             bases)

    """
    paquet = cua_sortida.get()
    id_paquet = paquets_pendents.put(paquet)
    print "**** Servint paquet #%06i" % id_paquet
    return {
        "id"    : id_paquet,
        "dades" : paquet
    }

def rpc_recepcionar_resultat(idpaquet, bases):
    """Funció cridada pel client per pujar un paquet de resultats.

    Paràmetres:

    * idpaquet: identificador del paquet al que correspon el resultat

    * bases:    subconjunt de les combinacions que formen una base

    """
    if idpaquet not in paquets_pendents:
        raise ValueError("paquet #%i no esta pendent")
    print "**** Recepcionant resultats #%06i" % idpaquet
    paquets_pendents.get(idpaquet)
    print "**** Encuant resultat"
    cua_entrada.put(bases)
    print "**** Paquet recepcionat     #%06i" % idpaquet



if __name__ == "__main__":
    prod_thread = threading.Thread(target=productor,
                                   args=(DIMENSIO, MAX_COMBINACIONS_PAQUET))
    cons_thread = threading.Thread(target=consumidor)

    print "Iniciant productor"
    prod_thread.start()
    print "Iniciant consumidor"
    cons_thread.start()
    print "Arrancant coodinador"
    servidor_rpc.arrancar_servidor_rpc(
        [
            (rpc_servir_paquet, "descarregar"),
            (rpc_recepcionar_resultat, "pujar")
        ]
    )
