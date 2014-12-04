// -*- coding: utf-8 -*-

// $Id$

#ifndef _CMDLINE_H_
#define _CMDLINE_H_

#include <string>


class WorkerOptions {
    std::string full_address;
    int         num_workers;
    std::string producer_address;
    int         producer_port;

  public:

    WorkerOptions();

    int parse_cmd_line(int argc, char *argv[]);
    const char *get_full_address();
    int get_num_workers();
};


class ProducerOptions {
    std::string full_address;
    std::string data_file;
    int         producer_port;

  public:

    ProducerOptions();

    int parse_cmd_line(int argc, char *argv[]);
    const char *get_full_address();
    const char *get_data_file_path();
};




#endif /* _CMDLINE_H_ */
