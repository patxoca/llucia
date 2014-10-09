# -*- coding: utf-8 -*-

# $Id$

import configuracio
import domini
import main_servidor
import utils



class Processador(object):
    def __init__(self, nom_arxiu):
        self._nom_arxiu = nom_arxiu
        self._arxiu = open(nom_arxiu, "w")
        self._comptador = 0

    def __call__(self, candidat, classe):
        if classe is None:
            return
        self._comptador += 1
        domini.codificar_linia_base_arxiu(
            self._arxiu,
            candidat,
            classe,
            self._comptador
        )

    def tancar(self):
        self._arxiu.close()


def generador():
    return domini.generar_combinacions(configuracio.DIMENSIO)

processador = Processador("/tmp/resultat.txt")


def descarregar_alguna_cosa():
    # aquesta funció es registrada amb el coordinador i ficada a
    # disposició dels treballadors.
    return {
        "a" : 1234,
        "b" : 5678,
        "c" : 9012
    }

if __name__ == "__main__":
    utils.configurar_logging(
        configuracio.SERVIDOR_LOG,
        configuracio.PORT_LOG,
        configuracio.NIVELL_LOG_COORDINADOR,
    )
    main_servidor.main(
        generador(),
        processador,
        funcions=(
            (descarregar_alguna_cosa, "valors"), # tupla amb un únic
                                                 # element, cal una
                                                 # coma al final
        )
    )
    processador.tancar()
