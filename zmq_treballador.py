# -*- coding: utf-8 -*-

# $Id$


import sys

import numpy
import zmq

import utils


def arrancar_treballador(calculador, coordinador="tcp://localhost:5555", consumidor="tcp:localhost:5556"):
    context = zmq.Context()
    # Socket to receive messages on
    receiver = context.socket(zmq.PULL)
    receiver.connect(coordinador)

    # Socket to send messages to
    # sender = context.socket(zmq.PUSH)
    # sender.connect(consumidor)

    # Process tasks forever
    while True:
        idpaquet, paquet = utils.desempaquetar(receiver.recv_pyobj())
        if idpaquet == -1:
            break
        resultat = calculador(paquet)
        # Send results to sink
        #sender.send(utils.empaquetar((idpaquet, resultat)))

if __name__ == "__main__":
    total = 0

    def calculador(paquet):
        global total
        sys.stdout.write(".")
        sys.stdout.flush()
        resultat = list(numpy.linalg.det(paquet))
        for i in resultat:
            if i:
                total += 1
        return resultat

    arrancar_treballador(calculador)
    print "%i bases" % total
