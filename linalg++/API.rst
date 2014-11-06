rational.h
==========

Definir tipus::

        typedef Rational<E, F>    NomNouTipus;

E és un tipus enter (per defecte int) i F real (per defecte double). E
s'utilitza pel numerador/denominador i F per conversions a/des de
"real"::

        typedef Rational<int>  FraccioEnter;
        typedef Rational<long> FraccioEnterLlarg;

Constructors
------------

- Rational(T n): n / 1

- Rational(T n, T d): n / d

- Rational(Rational f): f (copia de f)

- Rational(): 0 (en realitat 0 / 1)


Mètodes
-------

- setNumerator(T):   assigna numerador i simplifica

- setDenominator(T): assigna denominador i simplifica

- getNumerator():    retorna el numerador

- getDenominator():  retorna el denominador

- getAsDouble():     retorna la fracció convertida a real

Operadors
---------

- = (assignació): accepta un valor de tipus T, F (no recomanat) o
  Rational. Compte amb l'assignació de números reals.

- +, -, *, /: Rational sumat/restat/multiplicat/dividit amb/per T o
  Rational. Compte, açò no funcionarà::

        typedef Rational<long> Fraccio;
        ...
        Fraccio f(1, 2);
        Fraccio res;

        res = 1 + f;

    Ja que el tipus enter no te definit l'operador per Rational. Cal
    fer la suma al revés o convertir el valor enter explícitament a
    Fraccio::

        typedef Rational<long> Fraccio;

        ...

        Fraccio f(1, 2);
        Fraccio res1, res2;

        res1 = f + 1;
        res2 = Fraccio(1) + f;

- +=, -=, *=, /=: Rational sumat/restat/multiplicat/dividit in-situ
  amb/per T o Rational

- ==, !=, >, <, >=, <= : compara amb T o Rational. Assumeix que la
  fracció està simplificada (cosa que hauria d'ocorrer automàticament)
  al operar.

- - (canvi de signe): Rational

- <<: Rational. Com casos especials imprimeix NaN (Not a Number) per
  divisions per zero. Els enters els imprimeix sense denominador. Si
  el numerador és més gran que el denominador imprimeix un número
  mixte.

Funcions
--------

- abs: valor absolut de la fracció


matrix.h
========

Definir tipus::

        typedef Matrix<T>    NomNouTipus;

        typedef Rational<int>   Fraccio;
        typedef Matrix<Fraccio> MatriuQ;

Constructors
------------



Mètodes
-------


Operadors
---------
