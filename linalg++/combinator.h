#ifndef COMBINATOR_H
#define COMBINATOR_H


class Combinator {
	bool  exhaurit;
	bool  primer;
	int   nombre_elements;
	int   mida_mostra;
	int  *indices;

public:
	Combinator(int n, int m);
	~Combinator();
	const int * first();
	const int * next();
};

#endif /* COMBINATOR_H */
