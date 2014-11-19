// -*- coding: utf-8 -*-

// $Id:$

#include "missatge.h"


typedef struct {
	request_type_t message;
	int            worker_id;
} request_t;

typedef struct : request_t {
	int   last_packet_id;
} request_get_t;


typedef struct {
	response_type_t message;
} response_t;

typedef struct : response_t {
	int   packet_id;         // ID del paquet, assignat pel productor, -1 pels
							 // paquets de finalització
	int   size;              // nombre d'elements en el paquet
	int   packet[DIMENSIO];  // paquet
} response_data_t;

typedef struct : response_t {
	int   worker_id;         // ID del treballador, -1 si rebutjat
} response_registered_t;



Requester::Requester(zmq::socket_t *s) {
	socket = s;
	worker_id = -1;
}

bool Requester::abort() {
	zmq::message_t message(sizeof(request_t));
	zmq::message_t reply;
	request_t      request;

	request.message = RQ_ABORT;
	request.worker_id = worker_id;

	memcpy((void*)message.data(), &request, sizeof(request));
	if (!socket->send(message)) {
		// error imagino
	}
	if (!socket->recv(&reply)) {
		// error ?
	}
	// la resposta hauria de ser ACK, no importa realment
	return true;
}

bool Requester::get(int last_packet_id, int *packet_id, int *size, Combination *buffer) {
	zmq::message_t   message(sizeof(request_get_t));
	zmq::message_t   reply;
	request_get_t    request;
	response_data_t *response;

	request.message = RQ_GET;
	request.worker_id = worker_id;
	request.last_packet_id = last_packet_id;

	memcpy((void*)message.data(), &request, sizeof(request));
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
	memcpy(buffer, response->packet, response->size * sizeof(int));

	return true;
}

bool Requester::register_(int *wid) {
	zmq::message_t         message(sizeof(request_t));
	zmq::message_t         reply;
	request_t              request;
	response_registered_t *response;

	request.message = RQ_REG;
	request.worker_id = -1;

	memcpy((void*)message.data(), &request, sizeof(request));
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
	request_t       request;

	request.message = RQ_UNREG;
	request.worker_id = worker_id;

	memcpy((void*)message.data(), &request, sizeof(request));
	if (!socket->send(message)) {
		// error imagino
	}
	if (!socket->recv(&reply)) {
		// error ?
	}
	// la resposta hauria de ser ACK, no importa realment
	return true;
}

Responder::Responder(zmq::socket_t *s) {
	socket = s;
}

bool Responder::ack() {
	zmq::message_t message(sizeof(response_t));
	response_t     response;

	response.message = RP_ACK;
	memcpy((void*)message.data(), &response, sizeof(response));
	if (!socket->send(message)) {
		// error?
	}
	return true;
}

bool Responder::data(int packet_id, int size, const Combination *data) {
	zmq::message_t   message(sizeof(response_data_t));
	response_data_t  response;

	response.message = RP_DATA;
	response.packet_id = packet_id;
	response.size = size;
	assert(size <= DIMENSIO);
	memcpy(&response.packet, data, size * (sizeof(Combination)));
	memcpy((void*)message.data(), &response, sizeof(response));
	if (!socket->send(message)) {
		// error?
	}
	return true;
}

bool Responder::registered(int worker_id) {
	zmq::message_t         message(sizeof(response_registered_t));
	response_registered_t  response;

	response.message = RP_REGISTERED;
	response.worker_id = worker_id;
	memcpy((void*)message.data(), &response, sizeof(response));
	if (!socket->send(message)) {
		// error?
	}
	return true;
}
