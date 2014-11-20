// -*- coding: utf-8 -*-

// $Id:$

#ifndef _CMDLINE_H_
#define _CMDLINE_H_

#include <string>

typedef struct {
	int         num_workers;
	std::string producer_address;
	int         producer_port;
} worker_options_t;

typedef struct {
	std::string data_file;
} producer_options_t;


int parse_worker_cmd_line(int argc, char *argv[], worker_options_t & options);
int parse_producer_cmd_line(int argc, char *argv[], producer_options_t & options);


#endif /* _CMDLINE_H_ */
