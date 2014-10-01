# -*- coding: utf-8 -*-

# $Id$

import client_rpc
import domini
import logging
import Queue
import time
import threading


cua_entrada = Queue.Queue(maxsize=1)
cua_sortida = Queue.Queue(maxsize=1)



def descarregador():
    """Aquesta funció intenta que sempre hi hagi un paquet disponible en
    la cua d'entrada per evitar que la funció 'calculador()' hagi
    d'esperar a que s'en descarregui un.

    """
    pdown = client_rpc.RPCProxy("localhost", "8000")
    while True:
        logging.debug("descarregador:descarregant")
        paquet = pdown.descarregar()
        logging.debug("descarregador:encuant %i", paquet["id"])
        cua_entrada.put(paquet)

def pujador():
    """Aquesta funció intenta pujar el resultat en paralel al
    càlcul/descarrega.

    """
    pup   = client_rpc.RPCProxy("localhost", "8000")
    while True:
        logging.debug("pujador:desencuant")
        id, paquet = cua_sortida.get()
        logging.debug("pujador:pujant %i", id)
        pup.pujar(id, paquet)

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

        logging.debug("#paquets: %i    #comb %i (%i)    #bas %i (%i)",
                      nombre_paquets,
                      total_combinacions,
                      len(candidats),
                      total_bases,
                      len(resultat)
        )
        logging.debug("    TT: %9.2f    TD: %9.2f    TC: %9.2f    TP: %9.2f",
                      temps_total,
                      temps_descarrega,
                      temps_calcul,
                      temps_pujada
        )
        logging.debug("                    %%TD:    %6.2f   %%TC:    %6.2f   %%TP:    %6.2f",
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
    fil_down = threading.Thread(target=descarregador)
    fil_up   = threading.Thread(target=pujador)

    fil_down.start()
    fil_up.start()
    calculador(fer_algo)
