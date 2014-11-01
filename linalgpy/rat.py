# -*- mode: python, coding: utf-8 -*-
# $Id$


def mcd(a, b):
    if a < b:
        a, b = b, a
    while b:
        a, b = b, a % b
    return a


class Fraccio(object):
    """Classe que representa una fracció.

    Fraccio(num)      -> num / 1
    Fraccio(num, den) -> num / den
    Fraccio(fraccio)  -> copia de fraccio

    Cal que tant NUM com DEN siguin enters o que NUM sigui una Fraccio
    i DEN 1.

    En cas de que la fracció sigui negativa el signe s'associa al
    numerador, es a dir, el denominador sempre serà positiu.

    """
    # _tipus_int és una llista amb els tipus acceptats pel numerador i
    # el denominador.
    _tipus_int = (int, long)
    # _tipus_float és el tipus emprat per convertir la fracció a un
    # número en punt flotant
    _tipus_float= float

    def __init__(self, num, den=1):
        if isinstance(num, Fraccio):
            assert den == 1
            num, den = num.num, num.den
        if not isinstance(num, self._tipus_int):
            raise ValueError("Tipus erroni numerador")
        if not isinstance(den, self._tipus_int):
            raise ValueError("Tipus erroni denominador")
        if den < 0:
            num = -num
            den = -den
        self.num = num
        self.den = den

    def __add__(self, f):
        res = self.__class__(self)
        res += f
        return res

    __radd__ = __add__

    def __iadd__(self, f):
        if isinstance(f, self._tipus_int):
            f = self.__class__(f)
        self.num = self.num * f.den + self.den * f.num
        self.den = self.den * f.den
        return self

    def __sub__(self, f):
        res = self.__class__(self)
        res -= f
        return res

    def __rsub__(self, f):
        res = self.__class__(f)
        res -= self
        return res

    def __isub__(self, f):
        if isinstance(f, self._tipus_int):
            f = self.__class__(f)
        self.num = self.num * f.den - self.den * f.num
        self.den = self.den * f.den
        return self

    def __mul__(self, f):
        res = self.__class__(self)
        res *= f
        return res

    __rmul__ = __mul__

    def __imul__(self, f):
        if isinstance(f, self._tipus_int):
            f = self.__class__(f)
        self.num = self.num * f.num
        self.den = self.den * f.den
        return self

    def __div__(self, f):
        res = self.__class__(self)
        res /= f
        return res

    def __rdiv__(self, f):
        res = self.__class__(f)
        res /= self
        return res

    def __idiv__(self, f):
        if isinstance(f, self._tipus_int):
            f = self.__class__(f)
        self.num = self.num * f.den
        self.den = self.den * f.num
        if self.den == 0:
            raise ZeroDivisionError()
        if self.den < 0:
            self.num = -self.num
            self.den = -self.den
        return self

    def __nonzero__(self):
        return self.num != 0

    def __cmp__(self, f):
        if isinstance(f, self._tipus_int):
            return cmp(self.num, self.den * f)
        if isinstance(f, Fraccio):
            return cmp(self.num * f.den, self.den * f.num)
        raise ValueError(f)

    def __repr__(self):
        if self.den == 1:
            return str(self.num)
        if self.num == 0:
            return "0"
        return "%i/%i" % (self.num, self.den)

    def simplificar(self):
        m = mcd(self.num, self.den)
        self.num = self.num / m
        self.den = self.den / m
        if self.den < 0:
            self.num = -self.num
            self.den = -self.den
        return self
