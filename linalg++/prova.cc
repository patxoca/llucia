// -*- coding: utf-8 -*-

// $Id$

#include <climits>
#include <iostream>
#include <time.h>

#include "combinator.h"
#include "gameloader.h"
#include "rational.h"
#include "tipus.h"


long max_num = 0;
long max_den = 0;

void update_max(const Matriu & m, const Matriu & r) {
    bool trobat = false;
    for (int i = 0; i < m.get_num_rows(); i++) {
        for (int j = 0; j < m.get_num_cols(); j++) {
            Fraccio f = m.get(i, j);
            if (f.getNumerator() > max_num) {
                std::cout << "  ** " << f << std::endl;
                max_num = f.getNumerator();
                trobat = true;
            }
            if (f.getDenominator() > max_den) {
                max_den = f.getDenominator();
                trobat = true;
            }
        }
    }
    if (trobat) {
        std::cout << "============================================================\n";
        std::cout << "Max num: " << max_num << std::endl;
        std::cout << "Max den: " << max_den << std::endl;
        std::cout << r;
        std::cout << "------------------------------------------------------------\n";
        std::cout << m;
    }
}

int main(int argc, char **argv) {
    std::vector<Coalicio> coalicions;
    Combinator *combinador;
    Fraccio resultat;
    clock_t t0, tf;
    long num_combinacions = 0;
    long num_bases = 0;
    long num_no_det = 0;
    Coalicio or_coalicions;
    GameLoader game;
    int dimension;
    Coalicio COALICIO_TOTAL;


    std::cout << "MAX_INT  " << INT_MAX << std::endl;
    std::cout << "MAX_LONG " << LONG_MAX << std::endl;
    std::cout << "sizeof(long) " << sizeof(long) << std::endl;
    std::cout << "sizeof(long long) " << sizeof(long long) << std::endl;

    try {
        game.load("joc6.txt");
    } catch (GameLoaderException &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    dimension = game.get_dimension();
    COALICIO_TOTAL = (1 << dimension) - 1;
    std::cout << "Iniciant càlcul n = " << dimension << std::endl;
    combinador = new Combinator(game.get_num_coalitions(), dimension);
    Matriu m(dimension, dimension);
    Matriu clon(dimension, dimension);
    Matriu tmpinv(dimension, 2*dimension);
    t0 = clock();
    for (const Combination *c = combinador->first(); c != NULL; c = combinador->next()) {
        or_coalicions = 0;
        num_combinacions++;
        for (int i = 0; i < dimension; i++) {
            or_coalicions |= c[i];
        }
        if (or_coalicions == COALICIO_TOTAL) {
            m.binary_array((unsigned int*)c);
            clon = m;
            Fraccio f = m.det();
            update_max(m, clon);
            if (f != 0) {
                m = clon;
                m.inv(tmpinv);
                update_max(m, clon);
                num_bases++;
            }
        } else {
            num_no_det++;
        }
    }
    tf = clock();

    std::cout << "Num combinacions: " << num_combinacions << std::endl;
    std::cout << "Num bases:        " << num_bases << std::endl;
    std::cout << "Num no det:       " << num_no_det << std::endl;
    std::cout << (tf - t0) / (double)CLOCKS_PER_SEC << "segons\n";
}
