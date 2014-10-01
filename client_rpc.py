# -*- coding: utf-8 -*-

# $Id$

import cPickle
import httplib
import json
import zlib

class Foo(object):
    def __init__(self, name, proxy):
        self._proxy = proxy
        self._parts = [name]

    def __getattr__(self, name):
        self._parts.append(name)
        return self

    def __call__(self, *args, **kw):
        params = dict(args=args, kwargs=kw)
        jparams = zlib.compress(cPickle.dumps(params))
        self._proxy._connection.request("POST", "/" + "/".join(self._parts), jparams)
        resp = self._proxy._connection.getresponse()
        if resp.status != 200:
            # TODO: tractar millor els codis d'error
            raise ValueError((resp.status, resp.reason))
        data = cPickle.loads(zlib.decompress(resp.read()))
        return data


class RPCProxy(object):
    def __init__(self, host, port):
        self._host = host
        self._port = port
        self._connection = httplib.HTTPConnection(self._host, self._port)

    def __getattr__(self, name):
        return Foo(name, self)


if __name__ == "__main__":
    p = RPCProxy("localhost", "8000")
