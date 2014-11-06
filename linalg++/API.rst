rational.h
==========

Definir tipus::

        typedef Rational<T>    NomNouTipus;

``T`` és un tipus enter (per defecte int) utilitzat pel
numerador i wl denominador::

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

- setNumerator(T):   assigna numerador

- setDenominator(T): assigna denominador

- getNumerator():    retorna el numerador

- getDenominator():  retorna el denominador

- reduce():          simplifica la fracció insitu


Operadors
---------

- = (assignació): accepta un valor de tipus T o Rational.

- +, -, *, /: Rational sumat/restat/multiplicat/dividit amb/per T o
  Rational. Compte, açò no funciona::

        typedef Rational<long> Fraccio;
        ...
        Fraccio f(1, 2);
        Fraccio res;

        res = 1 + f;

    Ja que el tipus enter no te definit l'operador per operar amb
    Rational. Cal fer l'operació al revés (si és possible) o convertir
    el valor enter explícitament a Fraccio::

        typedef Rational<long> Fraccio;

        ...

        Fraccio f(1, 2);
        Fraccio res1, res2;

        res1 = f + 1;
        res2 = Fraccio(1) + f;

- +=, -=, *=, /=: Rational sumat/restat/multiplicat/dividit in-situ
  amb/per T o Rational

- ==, !=, >, <, >=, <= : compara amb T o Rational.

- - (canvi de signe): Rational

- <<: Rational. Com casos especials imprimeix NaN (Not a Number) per
  divisions per zero. Els enters els imprimeix sense denominador. Si
  el numerador és més gran que el denominador imprimeix un número
  mixte.


Funcions
--------

- abs: valor absolut de la fracció


Notes
-----

La simplificació, a més d'eliminar els factors comuns entre el
numerador i el denominador, normalitza certes fraccions (0/n -> 0/1,
n/0 -> 0/0) i, sobretot, garanteix que en les fraccions negatives el
signe aparegui en el numerador. Açò últim és necessari per garantir la
correció de la suma i la resta.

Pel que fa a la simplificació pròpiament dita cal fer-la manualment
quan es consideri adient, per exemple al acabar un càlcul complex. Cal
tindre en compte però que si no es simplifica, depenent dels valors i
les operacions, es poden produir desbordaments, obtenint resultat
errònis.

Com cas especial, al fer E/S amb fraccions utilitzant l'operador << la
fraccio es mostra simplificada. Com que la E/S és costosa de per sí
aquest overhead es considera acceptable.


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
