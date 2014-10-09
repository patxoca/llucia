# -*- coding: utf-8 -*-

# $Id$


import numpy

import client_rpc
import configuracio
import main_client
import utils

nombre_bases = 0
valors = None

def calcul(candidat):
    global nombre_bases

    # Des d'ací es pot fer referència al valor de la variable
    # "valors". Com que només accedirem al seu valor no cal
    # declarar-la com global.

    if numpy.linalg.det(candidat) == 0:
        return None
    nombre_bases += 1
    return "B0"

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
