#ifndef COMBINATOR_H
#define COMBINATOR_H


typedef unsigned int  Combination;

class Combinator {
    bool  exhaurit;
    bool  primer;
    int   nombre_elements;
    int   mida_mostra;
    int   mida_bloc;
    Combination  *indices;

public:
    Combinator(int n, int m);
    Combinator(int n, int m, const Combination *seed, int block_size);
    ~Combinator();
    const Combination *first();
    const Combination *next(int block_size = 1);
};

#endif /* COMBINATOR_H */
