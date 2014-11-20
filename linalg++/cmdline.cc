// -*- coding: utf-8 -*-

// $Id:$

#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>

#include "cmdline.h"


namespace po = boost::program_options;


int parse_worker_cmd_line(int argc, char *argv[], worker_options_t & opts) {
	po::options_description generic("Generic options");
	po::options_description config("Configuration");
	po::options_description hidden("Hidden options");
	po::options_description cmdline_options;
	po::options_description config_file_options;
	po::options_description visible_options("Allowed options");
	po::variables_map vm;
	std::string config_file;

	generic.add_options()
		("help,h", "mostra aquest missatge d'ajuda")
		("version,v", "mostra versió del programa")
		;
	config.add_options()
		("config-file,c",
		 po::value<std::string>(&config_file)->default_value("foo.ini"),
		 "arxiu de configuracio")
		("num-workers,n",
		 po::value<int>(&opts.num_workers)->default_value(-1),
		 "nombre de treballadors, -1 automàtic")
		;

	cmdline_options.add(generic).add(config);
	config_file_options.add(config);

	po::store(
		po::command_line_parser(argc, argv).options(cmdline_options).run(),
		vm
		);
	po::notify(vm);

	std::ifstream ifs(config_file.c_str());
	if (!ifs) {
		std::cout << "can not open config file: " << config_file << std::endl;
		return 0;
	} else {
		po::store(po::parse_config_file(ifs, config_file_options), vm);
		notify(vm);
	}

	if (vm.count("help")) {
		std::cout << visible_options << std::endl;
		return 1;
	}
	return 0;
}

int parse_producer_cmd_line(int argc, char *argv[], producer_options_t & opts) {
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
		;
	config.add_options()
		("config-file,c",
		 po::value<std::string>(&config_file)->default_value("foo.ini"),
		 "arxiu de configuracio")
		;
	hidden.add_options()
		("game-file,g",
		 po::value<std::string>(&opts.data_file)->default_value("game.txt"),
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

	std::ifstream ifs(config_file.c_str());
	if (!ifs) {
		std::cout << "can not open config file: " << config_file << std::endl;
		return 0;
	} else {
		po::store(po::parse_config_file(ifs, config_file_options), vm);
		notify(vm);
	}

	if (vm.count("help")) {
		std::cout << visible_options << std::endl;
		return 1;
	}
	return 0;
}
