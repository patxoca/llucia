# -*- coding: utf-8 -*-

# $Id$

import sys
import time

import zmq

import domini
import utils

MAX_CLIENTS = 10


def arrancar_productor(generador, address="tcp://*:5555", consumidor="tcp://localhost:5556"):
    context = zmq.Context()

    # Socket to send messages on
    sender = context.socket(zmq.PUSH)
    sender.bind(address)

    # Socket with direct access to the sink: used to syncronize start of batch
    # sink = context.socket(zmq.PUSH)
    # sink.connect(consumidor)

    print("Prem Return quen els treballadors estiguin preparats: ")
    raw_input()
    print("Sending tasks to workers...")

    t0 = time.time()
    for idpaquet, paquet in enumerate(utils.generar_lots(generador, 250000), 1):
        sys.stdout.write(".")
        sys.stdout.flush()
        sender.send_pyobj(utils.empaquetar((idpaquet, paquet)))
    tf = time.time()
    for i in xrange(MAX_CLIENTS):
        sender.send_pyobj(utils.empaquetar((-1, None)))

    print "%9.2f seg" % (tf - t0)

    # Give 0MQ time to deliver
    raw_input("Prem Return quan hagin acabat els treballadors")



if __name__ == "__main__":
    def generador():
        return domini.generar_combinacions(6)
    arrancar_productor(generador())
