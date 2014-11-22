// -*- coding: utf-8 -*-

// $Id:$

#ifndef _GAME_LOADER_H_
#define _GAME_LOADER_H_

#include <exception>
#include <boost/format.hpp>
#include <ostream>

#include "tipus.h"

class GameLoaderException : public std::exception {
	const char *message;
  public:
	GameLoaderException(const char* msg) { message = msg; }
	GameLoaderException(const boost::format &msg) { message = msg.str().c_str(); }
	virtual const char *what() { return message; }
};


class GameLoader {
	unsigned int dimension;
	unsigned int nombre_coalicions; // per conveniencia
	Fraccio *values;

  public:

	GameLoader();

	void load(const char *path);

	unsigned int get_dimension() const;
	unsigned int get_num_coalitions() const;
	const Fraccio *get_values() const;
};

std::ostream & operator << (std::ostream & stream, GameLoader const & game);


#endif /* _GAME_LOADER_H_ */
