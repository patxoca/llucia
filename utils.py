# -*- coding: utf-8 -*-

# $Id$

import cPickle
import functools
import logging
import logging.handlers
import thread
import zlib

def trace(logger):
    """Decorador per traçar l'execució de les funcions i facilitar la
    depuració d'errors.

    """
    def decorator(f):
        name = f.__name__
        @functools.wraps(f)
        def wrapper(*args, **kw):
            thread_id = thread.get_ident()
            logger.debug(">>> %s[%i]", name, thread_id)
            res = f(*args, **kw)
            logger.debug("<<< %s[%i]", name, thread_id)
            return res
        return wrapper
    return decorator


def configurar_logging(servidor="localhost", port=8001, nivell=logging.INFO):
    """Configura el subsistema de logging.

    Defineix el format dels missatges, el nivel mínim per registra un
    missatge i configura el sistema per enviar els missatges a un
    servidor central.

    """
    logging.basicConfig(
        datefmt="%H:%M:%S",
        format="%(asctime)-9s - %(message)s",
        level=nivell
    )
    socketHandler = logging.handlers.SocketHandler(
        servidor,
        port
    )
    logger = logging.getLogger()
    logger.addHandler(socketHandler)
    logger.info("Logging inicialitzat.")

def generar_lots(elements, mida_lot):
    """Genera lots de mida MIDA_LOT a partir de ELEMENTS.

    """
    lot = []
    for i in elements:
        lot.append(i)
        if len(lot) == mida_lot:
            yield lot
            lot = []
    if lot:
        yield lot

def empaquetar(dades):
    return zlib.compress(cPickle.dumps(dades))

def desempaquetar(dades):
    return cPickle.loads(zlib.decompress(dades))
