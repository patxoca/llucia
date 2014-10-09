# -*- coding: utf-8 -*-

# $Id$

import domini
import main_servidor

# nombre de jugadors
DIMENSIO = 5


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
        self._arxiu.tancar()


def generador():
    return domini.generar_combinacions(DIMENSIO)

processador = Processador("/tmp/resultat.txt")


if __name__ == "__main__":
    main_servidor.main(
        generador(),
        processador
    )
    processador.tancar()
