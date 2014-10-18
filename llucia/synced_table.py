# -*- coding: utf-8 -*-

# $Id$

import collections
import threading


class SyncedTable(collections.OrderedDict):
    """Taula per emmagatzema elements (res novedos). Cada element te
    associat un identificador únic definit per la taula. Aquest
    identificador es pot utilitzar a posteriori per recuperar
    l'element.

    Aquesta estructura permet afegir (put) i recuperar (get) elements.
    També permet comprovar si un identificador correspon a un element
    (operador in).

    Aquesta estructura és thread-safe.

    """
    def __init__(self, maxsize=0):
        self._maxsize = maxsize if maxsize >= 0 else 0
        self._id_paquet = 0
        self._dict = collections.OrderedDict()
        self._lock = threading.Lock()

    def put(self, paquet):
        """Afegir un paquet a la llista.

        Retorna l'identificador.
        """
        with self._lock:
            if self._maxsize and len(self._dict) == self._maxsize:
                raise ValueError("plena")
            self._id_paquet += 1
            self._dict[self._id_paquet] = paquet
            return self._id_paquet

    def get(self, id_, rm=True):
        """Donat un identificador recupera l'element associat.

        Si RM és True (valor per defecte) l'element és eliminat de la
        taula.

        Dispara una excepció KeyError si el identificador no correspon
        a cap element.

        """
        with self._lock:
            res = self._dict[id_]
            if rm:
                del self._dict[id_]
        return res

    def __contains__(self, id_):
        return id_ in self._dict

    def __nonzero__(self):
        return bool(self._dict)
