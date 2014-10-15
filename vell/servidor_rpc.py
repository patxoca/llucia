# -*- coding: utf-8 -*-

# $Id$


import BaseHTTPServer
import cPickle
import json
import zlib

from SocketServer import ThreadingMixIn

import utils


class ThreadedHTTPServer(ThreadingMixIn, BaseHTTPServer.HTTPServer):
    """Handle requests in a separate thread."""


class RPCHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    """Gestiona les peticions rebudes des dels consumidors.

    Només accepta peticions POST.

    El contingut de la petició ha de ser un diccionari amb les claus
    "args" i "kwargs", comprimit amb zlib i serialitzat amb cPickle.

    """
    def do_POST(self):
        function = self._get_method(self.path)
        if function is None:
            self.send_error(404)
            self.end_headers()
            return

        args, kw = self._parse_params()
        try:
            res = function(*args, **kw)
        except Exception as e:
            self.send_error(500, str(e))
            self.end_headers()
            return

        jres = cPickle.dumps(res)
        zres = zlib.compress(jres)

        self.send_response(200)
        self.send_header("Content-type", "application/x-json-rpc")
        self.send_header("Content-length", str(len(zres)))
        self.end_headers()
        self.wfile.write(zres)

    def _get_method(self, path):
        return self.server._funcions.get(path, None)

    def _parse_params(self):
        l = int(self.headers.getheader('content-length'))
        res = cPickle.loads(zlib.decompress(self.rfile.read(l)))
        return res.get("args", ()), res.get("kwargs", {})

    def log_message(self, format, *args):
        pass




def arrancar_servidor_rpc(funcions, continuar=None, host="localhost", port=8000):
    """Arranca el servidor RPC.

    funcions: llista de funciona accesibles des dels clients. Cada
              element pot ser una funció o una parella (funcio, nom).

    continuar: funció que retorna False per aturar del servei.

    """
    if continuar is None:
        continuar = lambda : True
    server_address = (host, port)
    httpd = ThreadedHTTPServer(server_address, RPCHandler)
    httpd._funcions = utils.normalitzar_taula_funcions(funcions)
    httpd.timeout = 1
    while continuar():
        httpd.handle_request()



if __name__ == "__main__":
    def sumar(a, b):
        return a + b
    def restar(a, b):
        return a - b
    arrancar_servidor_rpc([sumar, (restar, "math.restar")])
