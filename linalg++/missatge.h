// -*- coding: utf-8 -*-

// $Id:$

#ifndef _MISSATGE_H_
#define _MISSATGE_H_

#include <zmq.hpp>
#include "combinator.h"
#include "tipus.h"

// Cada treballador té associat un identificador únic.
//
// workflow: el treballador envia una petició i el coordinador retorna una
// resposta (sempre).
//
// Tipus de missatges (i paràmetres):
//
// - ABORT: id_treballador
//   un treballador detecta una condició per la que no cal continuar el
//   càlcul. El productor respon ACK (acknowledgement) al client i a partir
//   d'eixe moment enviarà paquets de finalització en les peticions GET dels
//   clients.
//
// - GET: id_treballador, id_paquet_anterior
//   un treballador sol·licita un paquet de dades i informa de la finalització
//   del càlcul amb el paquet prèvi. Si és la primera petició (no hi ha paquet
//   prèvi) id_paquet_anterior == -1
//
//   El productor respon amb un paquet DATA que conté el id del paquet, el
//   nombre de dades i les dades. Si no hi ha més dades que processar o s'ha
//   avortat el càlcul el productor respondra amb un paquet de finalització
//   (un paquet amb ID -1).
//
// - REG:
//   un treballador demana registrar-se
//
//   Si s'accepta el registre el productor respon amb REGISTERED i envia al
//   treballador el seu ID. Si es rebutja respon amb REJECT.
//
// - UNREG: id_treballador
//   un treballador es desregistra. El productor respont amb ACK (ok) o REJECT
//   (error).
//
//   Quan no queden treballadors registrats el productor finalitza.

typedef enum {
	RQ_ABORT,
	RQ_GAME,
	RQ_GET,
	RQ_REG,
	RQ_UNREG
} request_type_t;

typedef enum {
	RP_ACK,
	RP_DATA,
	RP_GAME,
	RP_REGISTERED,
	RP_REJECT,
} response_type_t;


class Requester {
	zmq::context_t *context;
	zmq::socket_t *socket;
	int worker_id;

  public:

	Requester();
	Requester(const char *address, zmq::context_t *ctx);
	~Requester();

	bool abort();
	bool game(unsigned int *dimension, Fraccio **values);
	bool get(int last_packet_id, int *packet_id, int *size, Combination *buffer);
	bool register_(int *worker_id);
	bool unregister();
};

class Responder {
	zmq::context_t *context;
	zmq::socket_t  *socket;
	zmq::message_t zrequest;

  public:

	Responder();
	Responder(const char *address);

	request_type_t get_request_type();

	bool ack();
	bool data(int packet_id, int size, const Combination *data);
	bool game(unsigned int dimension, const Fraccio *values);
	bool registered(int worker_id);
};

#endif /* _MISSATGE_H_ */
