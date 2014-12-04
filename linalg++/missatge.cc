// -*- coding: utf-8 -*-

// $Id$

#include "missatge.h"


typedef struct {
	request_type_t message;
	int            worker_id;
} request_t;

typedef struct : request_t {
	int   last_packet_id;
} request_get_t;

typedef struct : request_t {
	int size;
	char data;
} request_bulk_t;

typedef struct {
	response_type_t message;
} response_t;

typedef struct : response_t {
	unsigned int dimension;
	Fraccio      values;  // array amb 2^dimensio items
} response_game_t;

typedef struct : response_t {
	int         packet_id;   // ID del paquet, assignat pel productor, -1 pels
							 // paquets de finalització
	int         size;        // nombre d'elements en el paquet
	Combination packet;      // paquet amb size elements
} response_data_t;

typedef struct : response_t {
	int   worker_id;         // ID del treballador, -1 si rebutjat
} response_registered_t;

/*
 *                                 _
 *  _ __ ___  __ _ _   _  ___  ___| |_ ___ _ __
 * | '__/ _ \/ _` | | | |/ _ \/ __| __/ _ \ '__|
 * | | |  __/ (_| | |_| |  __/\__ \ ||  __/ |
 * |_|  \___|\__, |\__,_|\___||___/\__\___|_|
 *              |_|
 */

Requester::Requester() {
	context = NULL;
	socket = NULL;
	worker_id = -1;
}

Requester::Requester(const char *address, zmq::context_t *ctx) {
	context = ctx;
	socket = new zmq::socket_t(*context, ZMQ_REQ);
	socket->connect(address);
	worker_id = -1;
}

void Requester::close() {
	context = NULL;
	if (socket != NULL) {
		socket->close();
		socket = NULL;
	}
	std::cout << "fin close\n";
}

bool Requester::abort() {
	zmq::message_t message(sizeof(request_t));
	zmq::message_t reply;
	request_t      *request;

	request = (request_t*)message.data();
	request->message = RQ_ABORT;
	request->worker_id = worker_id;

	if (!socket->send(message)) {
		// error imagino
	}
	if (!socket->recv(&reply)) {
		// error ?
	}
	// la resposta hauria de ser ACK, no importa realment
	return true;
}

bool Requester::game(unsigned int *dimension, Fraccio **values) {
	zmq::message_t  message(sizeof(request_t));
	zmq::message_t  reply;
	request_t       *request;
	unsigned int    nc;
	response_game_t *response;

	request = (request_t*)message.data();
	request->message = RQ_GAME;
	request->worker_id = worker_id;
	if (!socket->send(message)) {
		// error imagino
	}
	if (!socket->recv(&reply)) {
		// error ?
	}
	response = (response_game_t*)reply.data();
	if (response->message != RP_GAME) {
		// error!!
	}
	*dimension = response->dimension;
	nc = 1 << *dimension; // 1 + nombre coalicions
	*values = new Fraccio[nc];
	memcpy(*values, &response->values, nc * sizeof(Fraccio));
	return true;
}

bool Requester::get(int last_packet_id, int *packet_id, int *size, Combination *buffer) {
	zmq::message_t   message(sizeof(request_get_t));
	zmq::message_t   reply;
	request_get_t   *request;
	response_data_t *response;

	request = (request_get_t*)message.data();
	request->message = RQ_GET;
	request->worker_id = worker_id;
	request->last_packet_id = last_packet_id;

	if (!socket->send(message)) {
		// error imagino
	}
	if (!socket->recv(&reply)) {
		// error ?
	}
	response = (response_data_t*)reply.data();
	if (response->message != RP_DATA) {
		// error!!
	}
	*packet_id = response->packet_id;
	*size = response->size;
	memcpy(buffer, &response->packet, response->size * sizeof(Combination));

	return true;
}

bool Requester::register_(int *wid) {
	zmq::message_t         message(sizeof(request_t));
	zmq::message_t         reply;
	request_t             *request;
	response_registered_t *response;

	request = (request_t*)message.data();
	request->message = RQ_REG;
	request->worker_id = -1;

	if (!socket->send(message)) {
		// error imagino
	}
	if (!socket->recv(&reply)) {
		// error ?
	}
	// la resposta hauria de ser ACK, no importa realment
	response = (response_registered_t*)reply.data();
	if (response->message != RP_REGISTERED) {
		*wid = -1;
	} else {
		worker_id = *wid = response->worker_id;
	}
	return true;
}

bool Requester::unregister() {
	zmq::message_t  message(sizeof(request_t));
	zmq::message_t  reply;
	request_t      *request;

	request = (request_t*)message.data();
	request->message = RQ_UNREG;
	request->worker_id = worker_id;

	if (!socket->send(message)) {
		// error imagino
	}
	if (!socket->recv(&reply)) {
		// error ?
	}
	// la resposta hauria de ser ACK, no importa realment
	return true;
}

bool Requester::result(void *data, int size) {
	zmq::message_t  message(sizeof(request_bulk_t) + size);
	zmq::message_t  reply;
	request_bulk_t *request;

	request = (request_bulk_t*)message.data();
	request->message = RQ_RESULT;
	request->worker_id = worker_id;
	request->size = size;
	memcpy(&(request->data), data, size);

	if (!socket->send(message)) {
		// error imagino
	}
	if (!socket->recv(&reply)) {
		// error ?
	}
	// la resposta hauria de ser ACK, no importa realment
	return true;
}

/*
 *                                      _
 *  _ __ ___  ___ _ __   ___  _ __   __| | ___ _ __
 * | '__/ _ \/ __| '_ \ / _ \| '_ \ / _` |/ _ \ '__|
 * | | |  __/\__ \ |_) | (_) | | | | (_| |  __/ |
 * |_|  \___||___/ .__/ \___/|_| |_|\__,_|\___|_|
 *               |_|
*/


Responder::Responder() {
	context = NULL;
	socket = NULL;
}

Responder::Responder(const char *address) {
	context = new zmq::context_t(1);
	socket = new zmq::socket_t(*context, ZMQ_REP);
	socket->bind(address);
}

request_type_t Responder::get_request_type() {
	socket->recv(&zrequest);
	return *(request_type_t*)(zrequest.data());
}

void Responder::get_request_payload(void *dst) {
	request_bulk_t *request = (request_bulk_t*)zrequest.data();
	memcpy(dst, &(request->data), request->size);
}

bool Responder::ack() {
	zmq::message_t message(sizeof(response_t));
	response_t     *response;

	response = (response_t*)message.data();
	response->message = RP_ACK;
	if (!socket->send(message)) {
		// error?
	}
	return true;
}

bool Responder::data(int packet_id, int size, const Combination *data) {
	int              response_size = sizeof(response_data_t) + size * sizeof(Combination);
	zmq::message_t   message(response_size);
	response_data_t *response;

	response = (response_data_t*)message.data();
	response->message = RP_DATA;
	response->packet_id = packet_id;
	response->size = size;
	memcpy(&response->packet, data, size * (sizeof(Combination)));
	if (!socket->send(message)) {
		// error?
	}
	return true;
}

bool Responder::game(unsigned int dimension, const Fraccio *values) {
	int nc = (1 << dimension);
	int response_size = sizeof(response_game_t) + nc * sizeof(Fraccio);
	zmq::message_t   message(response_size);
	response_game_t *response;

	response = (response_game_t*)message.data();
	response->message = RP_GAME;
	response->dimension = dimension;
	memcpy(&response->values, values, nc * sizeof(Fraccio));
	if (!socket->send(message)) {
		// error?
	}
	return true;
}

bool Responder::registered(int worker_id) {
	zmq::message_t         message(sizeof(response_registered_t));
	response_registered_t *response;

	response = (response_registered_t*)message.data();
	response->message = RP_REGISTERED;
	response->worker_id = worker_id;
	if (!socket->send(message)) {
		// error?
	}
	return true;
}
