# -*- coding: utf-8 -*-

# $Id$


import numpy

import main_client

nombre_bases = 0


def calcul(candidat):
    global nombre_bases

    if numpy.linalg.det(candidat) == 0:
        return None
    nombre_bases += 1
    return "B0"

if __name__ == "__main__":
    main_client.main(calcul)
    print nombre_bases
