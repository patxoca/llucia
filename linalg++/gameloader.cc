// -*- coding: utf-8 -*-

// $Id:$

//#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>

#include <fstream>
#include <string>
#include <vector>

#include "gameloader.h"
#include "tipus.h"


namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;


struct Value {
	Coalicio coalition;
	int      numerator;
	int      denominator;

	Value() {
		coalition = 0;
		numerator = 0;
		denominator = 1;
	}
};

struct Game {
	unsigned int       dimension;
	std::vector<Value> values;

	Game() {
		dimension = 0;
	}
};

BOOST_FUSION_ADAPT_STRUCT(
    Value,
    (Coalicio, coalition)
    (int, numerator)
    (int, denominator)
)

BOOST_FUSION_ADAPT_STRUCT(
	Game,
	(unsigned int, dimension)
	(std::vector<Value>, values)
)


template <typename Iterator>
struct game_parser : qi::grammar<Iterator, Game(), ascii::space_type> {
	game_parser() : game_parser::base_type(start) {
		using qi::int_;
		using qi::uint_;
		using qi::lit;

		value %=
			uint_ >> int_ >> -('/' >> int_)
			;

		start %=
			lit("dimensio")
			>> '='
			>> uint_
			>> ';'
			>> lit("joc")
			>> '='
			>> value % ','
			>> ';'
			;
	}

	qi::rule<Iterator, Value(), ascii::space_type> value;
	qi::rule<Iterator, Game(), ascii::space_type> start;
};

static void parse_game_file(const char *path, Game &game);
static unsigned int recode_coalition(Coalicio c, int dimension);


GameLoader::GameLoader() {
	dimension = 0;
	values = NULL;
}

void GameLoader::load(const char *path) {
	Game game;
	bool *seen;

	parse_game_file(path, game);

	if (game.dimension <= 0) {
		boost::format fmt("Dimensió no vàlida: %i");
		throw GameLoaderException(fmt % game.dimension);
	}
	dimension = game.dimension;
	nombre_coalicions = (1 << dimension) - 1;
	if (nombre_coalicions != game.values.size()) {
		boost::format fmt("Discrepància en nombre de coalicions: esperat %i, trobat %i");
		throw GameLoaderException(fmt % nombre_coalicions % game.values.size());
	}
	values = new Fraccio[nombre_coalicions + 1];
	seen = new bool[nombre_coalicions + 1];
	for (unsigned int i = 0; i < nombre_coalicions; i++) {
		seen[i] = false;
	}
	for(unsigned int i = 0; i < nombre_coalicions; i++) {
		Value v = game.values[i];
		Coalicio coalition = recode_coalition(v.coalition, dimension);
		if (seen[coalition]) {
			boost::format fmt("Coalicio duplicada: %i");
			throw GameLoaderException(fmt % v.coalition);
		}
		seen[coalition] = true;
		values[coalition] = Fraccio(v.numerator, v.denominator);
	}
	delete[] seen;

	std::cout << "Dimensio: " << dimension << std::endl;
	std::cout << "Valors: " << std::endl;
	for (unsigned int i = 1; i <= nombre_coalicions; i++) {
		std::cout << "  " << i << " = " << values[i] << std::endl;
	}
}

unsigned int GameLoader::get_dimension() {
	return dimension;
}

unsigned int GameLoader::get_num_coalitions() {
	return nombre_coalicions;
}

const Fraccio *GameLoader::get_values() {
	return values;
}

static void parse_game_file(const char *path, Game &game) {
	std::ifstream joc(path);
	std::string contents;
	game_parser<std::string::iterator> parser;

	if (joc) {
		joc.seekg(0, std::ios::end);
		contents.resize(joc.tellg());
		joc.seekg(0, std::ios::beg);
		joc.read(&contents[0], contents.size());
		joc.close();

		if (qi::phrase_parse(contents.begin(), contents.end(), parser, ascii::space, game)) {
			std::cout << "Dimension = " << game.dimension << std::endl;
			std::cout << "Valors\n  ";
			for(unsigned int i = 0; i < game.values.size(); i++) {
				Value v = game.values[i];
				std::cout << "(" << v.coalition << ", " << v.numerator << "/" << v.denominator << ")";
			}
			std::cout << std::endl;
		} else {
			throw GameLoaderException("Error analitzant l'arxiu");
		}
	} else {
		boost::format fmt("Arxiu no trobat: '%s'");
		throw GameLoaderException(fmt % path);
	}
}

// transforma la coalicio 123 (enter) en el enter 7, en binari 00111

static unsigned int recode_coalition(Coalicio coalition, int dimension) {
	unsigned int res = 0;
	Coalicio c = coalition;
	int mask;

	if (coalition <= 0) {
		boost::format fmt("Coalició no vàlida: %i");
		throw GameLoaderException(fmt % coalition);
	}
	while (c > 0) {
		int r = c % 10;
		if (r == 0) {
			r = 10;
		}
		if (r > dimension) {
			boost::format fmt("Element fora de rang: %i en coalicio %i");
			throw GameLoaderException(fmt % r % coalition);
		}
		mask = (1 << (r - 1));
		if (res & mask) {
			boost::format fmt("Digit duplicat: %i en coalicio %i ");
			throw GameLoaderException(fmt % r % coalition);
		}
		res |= mask;
		c /= 10;
	}
	return res;
}
