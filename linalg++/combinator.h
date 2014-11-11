#ifndef COMBINATOR_H
#define COMBINATOR_H


class Combinator {
	bool  exhaurit;
	bool  primer;
	int   nombre_elements;
	int   mida_mostra;
	int   mida_bloc;
	int  *indices;

public:
	Combinator(int n, int m);
	Combinator(int n, int m, const int *seed, int block_size);
	~Combinator();
	const int * first();
	const int * next(int block_size = 1);
};

#endif /* COMBINATOR_H */
