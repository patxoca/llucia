// -*- coding: utf-8 -*-

// $Id:$

#include <fstream>
#include <iostream>
#include <unistd.h>

#include <boost/program_options.hpp>

#include "cmdline.h"



namespace po = boost::program_options;

static int get_num_cores() {
    // http://stackoverflow.com/questions/150355/
	return sysconf( _SC_NPROCESSORS_ONLN );
}


WorkerOptions::WorkerOptions() {
	full_address = "";
}

const char *WorkerOptions::get_full_address() {
	if (full_address == "") {
		std::ostringstream str;
		str << "tcp://" << producer_address << ":" << producer_port;
		full_address = str.str();
	}
	return full_address.c_str();
}

int WorkerOptions::get_num_workers() {
	int num_cores = get_num_cores();

	if (num_workers == 0) {
		return num_cores;
	}
	if (num_workers < 0) {
		return num_workers + num_cores;
	}
	return num_workers;
}

int WorkerOptions::parse_cmd_line(int argc, char *argv[]) {
	po::options_description generic("Generic options");
	po::options_description config("Configuration");
	po::options_description cmdline_options;
	po::options_description config_file_options;
	po::variables_map vm;
	std::string config_file;

	generic.add_options()
		("help,h", "mostra aquest missatge d'ajuda")
		("version,v", "mostra versió del programa")
		("config-file,c",
		 po::value<std::string>(&config_file)->default_value("worker.ini"),
		 "arxiu de configuracio")
		;
	config.add_options()
		("producer-ip,i",
		 po::value<std::string>(&producer_address)->default_value("127.0.0.1"),
		 "IP del productor")
		("producer-port,p",
		 po::value<int>(&producer_port)->default_value(31415),
		 "port del productor")
		("num-workers,n",
		 po::value<int>(&num_workers)->default_value(0),
		 "nombre de treballadors, automàtic")
		;

	cmdline_options.add(generic).add(config);
	config_file_options.add(config);

	po::store(
		po::command_line_parser(argc, argv).options(cmdline_options).run(),
		vm
		);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << cmdline_options << std::endl;
		return 1;
	}

	std::ifstream ifs(config_file.c_str());
	if (!ifs) {
		std::cout << "can not open config file: " << config_file << std::endl;
		return 0;
	} else {
		po::store(po::parse_config_file(ifs, config_file_options), vm);
		notify(vm);
	}

	return 0;
}

ProducerOptions::ProducerOptions() {
	full_address = "";
}

const char *ProducerOptions::get_full_address() {
	if (full_address == "") {
		std::ostringstream str;
		str << "tcp://*:" << producer_port;
		full_address = str.str();
		// full_address = (char*)malloc(str.str().length() + 1);
		// memcpy(full_address, str.str().c_str(), str.str().length() + 1);
	}
	return full_address.c_str();
}

const char *ProducerOptions::get_data_file_path() {
	return data_file.c_str();
}

int ProducerOptions::parse_cmd_line(int argc, char *argv[]) {
	po::options_description generic("Generic options");
	po::options_description config("Configuration");
	po::options_description hidden("Hidden options");
	po::options_description cmdline_options;
	po::options_description config_file_options;
	po::options_description visible_options("Allowed options");
	po::variables_map vm;
	po::positional_options_description pos;
	std::string config_file;

	generic.add_options()
		("help,h", "mostra aquest missatge d'ajuda")
		("version,v", "mostra versió del programa")
		("config-file,c",
		 po::value<std::string>(&config_file)->default_value("producer.ini"),
		 "arxiu de configuracio")
		;
	config.add_options()
		("producer-port,p",
		 po::value<int>(&producer_port)->default_value(31415),
		 "port del productor")
		;
	hidden.add_options()
		("game-file,g",
		 po::value<std::string>(&data_file)->default_value("game.txt"),
		 "arxiu de dades")
		;

	cmdline_options.add(generic).add(config).add(hidden);
	config_file_options.add(config).add(hidden);
	visible_options.add(generic).add(config);

	pos.add("game-file", -1);

	po::store(
		po::command_line_parser(argc, argv).options(cmdline_options).positional(pos).run(),
		vm
		);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << visible_options << std::endl;
		return 1;
	}

	std::ifstream ifs(config_file.c_str());
	if (!ifs) {
		std::cout << "can not open config file: " << config_file << std::endl;
		return 0;
	} else {
		po::store(po::parse_config_file(ifs, config_file_options), vm);
		notify(vm);
	}

	return 0;
}
