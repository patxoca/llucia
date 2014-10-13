# -*- coding: utf-8 -*-

# $Id$

import cPickle
import threading
import time
import zlib

import cherrypy


def _normalitzar_funcions(funcions):
    res = {}
    for f in funcions:
        if callable(f):
            nom = f.__name__
        else:
            f, nom = f
        nom = tuple(nom.split("."))
        res[nom] = f
    return res


class Arrel(object):
    def __init__(self, funcions):
        self._funcions = _normalitzar_funcions(funcions)

    @cherrypy.expose
    def default(self, *path):
        function = self._get_method(tuple(path))
        if function is None:
            raise cherrypy.HTTPError(404)

        args, kw = self._parse_params()
        try:
            res = function(*args, **kw)
        except Exception as e:
            raise cherrypy.HTTPError(500, message=str(e))

        jres = cPickle.dumps(res)
        zres = zlib.compress(jres)

        cpresp = cherrypy.response
        cpresp.status = "200 Ok"
        cpresp.headers["Content-type"] = "application/x-python-pickle-rpc"
        cpresp.headers["Content-length"] = str(len(zres))

        return zres

    def _get_method(self, path):
        return self._funcions.get(path, None)

    def _parse_params(self):
        l = int(cherrypy.request.headers["Content-Length"])
        res = cPickle.loads(zlib.decompress(cherrypy.request.rfile.read(l)))
        return res.get("args", ()), res.get("kwargs", {})

def monitor(continuar):
    while True:
        if not continuar():
            cherrypy.engine.exit()
            break
        time.sleep(1)

def arrancar_servidor_rpc(funcions, continuar=None, host="localhost", port=8000):
    """Arranca el servidor RPC.

    funcions: llista de funciona accesibles des dels clients. Cada
              element pot ser una funció o una parella (funcio, nom).

    continuar: funció que retorna False per aturar del servei.

    """
    if continuar is None:
        continuar = lambda : True
    cherrypy.config.update({
        "server.socket_host": host,
        "server.socket_port": port,
        "request.process_request_body": False,
    })
    t = threading.Thread(target=monitor, args=(continuar, ))
    t.start()
    cherrypy.quickstart(
        Arrel(funcions)
    )
    t.join()



if __name__ == "__main__":
    def sumar(a, b):
        return a + b
    def restar(a, b):
        return a - b
    arrancar_servidor_rpc([sumar, (restar, "math.restar")])
