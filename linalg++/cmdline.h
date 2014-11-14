// -*- coding: utf-8 -*-

// $Id:$

#ifndef _CMDLINE_H_
#define _CMDLINE_H_

#include <string>

typedef struct {
	std::string data_file;
	int         num_workers;
} options_t;


int parse_cmd_line(int argc, char *argv[], options_t & options);


#endif /* _CMDLINE_H_ */
