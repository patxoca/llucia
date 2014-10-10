# -*- coding: utf-8 -*-

# $Id$


import itertools
import numpy

import client_rpc
import configuracio
import main_client
import utils

nombre_bases = 0
valors = None

def calcul(candidats):
    global nombre_bases

    res = []
    for lot in utils.generar_lots(candidats, 100):
        # Genera un lot de 100 candidats i calcula els 100
        # determinants de cop. Estranyament és possible fer-ho i és
        # més ràpid!!! En aquest cas 'det' retorna una "llista" amb el
        # valor de cada determinant.
        det = list(numpy.linalg.det(lot))
        for c, d in itertools.izip(lot, det):
            # c : candidat
            # d : determinant
            if d != 0:
                nombre_bases += 1
                # calcular la classe a partir de 'c' i afegir-la a
                # 'res'
                res.append("B0")
            else:
                res.append(None)
    return res


if __name__ == "__main__":
    utils.configurar_logging(
        configuracio.SERVIDOR_LOG,
        configuracio.PORT_LOG,
        configuracio.NIVELL_LOG_TREBALLADOR,
    )
    proxy = client_rpc.RPCProxy(
        configuracio.SERVIDOR_RPC,
        configuracio.PORT_RPC
    )
    valors = proxy.valors()
    main_client.main(calcul)
    print nombre_bases
