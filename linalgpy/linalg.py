# -*- mode: python, coding: utf-8 -*-
# $Id$

import itertools





class array(object):
    """
    """
    def __init__(self, matriu, dtype=Fraccio):
        """Donat el array (llista de llistes) MATRIU amb enters i/o fraccions
        retorna un array de fraccions.

        """
        matriu = self._valida_geometria(matriu)
        self._num_files = len(matriu)
        self._num_columnes = len(matriu[0])
        self._dtype = dtype
        res = []
        for fila in matriu:
            res_fila = []
            for item in fila:
                try:
                    item = dtype(item) # el converteix a fracció
                except ValueError:
                    # error de conversió
                    raise ValueError("Tipus no suportat per element de la matriu: %r" % (item, ))
                res_fila.append(item)
            res.append(res_fila)
        self._matriu = res

    @classmethod
    def zeros(cls, n, m=None, dtype=Fraccio):
        if m is None:
            m = n
        return cls([[0] * m for i in xrange(n)], dtype)

    @classmethod
    def ones(cls, n, dtype=int):
        return [dtype(1)] * n

    @classmethod
    def identitat(cls, n, dtype=Fraccio):
        res = cls.zeros(n, n, dtype)
        for i in xrange(n):
            res._matriu[i][i] = dtype(1)
        return res

    @property
    def dtype(self):
        """Retorna el tipus del items del array
        """
        return self._dtype

    @property
    def num_files(self):
        """Retorna el nombre de files
        """
        return self._num_files

    @property
    def num_columnes(self):
        """Retorna el nombre de columnes
        """
        return self._num_columnes

    def fila(self, num_fila):
        """Retorna una copia de la fila NUM_FILA.
        """
        if 0 <= num_fila < self._num_files:
            return list(self._matriu[num_fila])
        raise IndexError(num_fila)

    def columna(self, num_columna):
        """Retorna una copia de la columna NUM_COLUMNA.
        """
        if 0 <= num_columna < self._num_columnes:
            return [self._matriu[i][num_columna] for i in xrange(self._num_files)]
        raise IndexError(num_columna)

    def _valida_geometria(self, matriu):
        if len(matriu) == 0:
            raise ValueError("Matriu buida")
        if not isinstance(matriu[0], (list, tuple)):
            matriu = [matriu]
        num_cols = len(matriu[0])
        for fila in matriu:
            if len(fila) != num_cols:
                raise ValueError("Inconsistencia en el nombre de columnes")
        return matriu

    def esten(self, m):
        if self.num_files != m.num_files:
            raise ValueError("Nombre de files no coincideix")
        self._num_columnes += m.num_columnes
        for fs, fm in itertools.izip(self._matriu, m._matriu):
            fs.extend([self.dtype(i) for i in fm])

    def clona(self):
        return self.__class__(self._matriu, self._dtype)

    def submatriu(self, fi, ff, ci, cf):
        assert 0 <= fi <= ff < self.num_files
        assert 0 <= ci <= cf < self.num_columnes
        res = []
        for i in range(fi, ff + 1):
            fila = self._matriu[i]
            res_fila = []
            for j in range(ci, cf + 1):
                res_fila.append(fila[j])
            res.append(res_fila)
        return array(res, self.dtype)

    def _pseudogauss(self, diagonal=False):
        n = self.num_files
        p = self.num_columnes
        signe = 1
        m = self._matriu
        for i in xrange(n - 1): # fila/columna del pivot
            if not m[i][i]:
                for j in xrange(i + 1, n):
                    if m[j][i]:
                        m[i], m[j] = m[j], m[i]
                        signe = -signe
                        break
                else:
                    return 0, None
            for j in xrange(i + 1, n): # fila on es fa zero
                if m[j][i]:
                    factor = m[j][i] / m[i][i]
                    for k in xrange(i, p): # recorre la fila horitzontalment
                        m[j][k] = m[j][k] - factor * m[i][k]
        if diagonal:
            if not self._matriu[n - 1][n - 1]:
                return 0, None
            # zeros en el triangle superior
            for i in xrange(n - 1, 0, -1):
                for j in xrange(i - 1, -1, -1):
                    factor = m[j][i] / m[i][i]
                    for k in xrange(i, p):
                        m[j][k] = m[j][k] - factor * m[i][k]
        return signe, m

    def dot(self, m):
        if isinstance(m, (list, tuple)):
            # per facilitar al programació del codi client les llistes
            # i tuples es tracten de forma especial
            if not isinstance(m[0], (list, tuple)):
                # assumim que és un vector de racionals
                if self.num_columnes > 1:
                    # ens cal un vector columna
                    m = array([[i] for i in m], dtype=self.dtype)
                else:
                    m = array([i for i in m], dtype=self.dtype)
            else:
                m = array(m, dtype=self.dtype)
        if self.num_columnes != m.num_files:
            raise ValueError("Nombre de columnes i files no coincideix")
        res = self.zeros(self.num_files, m.num_columnes, self.dtype)
        for i in xrange(self.num_files):
            for j in xrange(m.num_columnes):
                r = self.dtype(0)
                for k in xrange(self.num_columnes):
                    r = r + self._matriu[i][k] * m._matriu[k][j]
                res._matriu[i][j] = r
        return res

    def det(self):
        if self.num_files != self.num_columnes:
            raise ValueError("Matriu no cuadrada")
        m2 = self.clona()
        signe, matriu = m2._pseudogauss()
        if matriu is None:
            return 0
        res = m2.dtype(1)
        for i in xrange(m2.num_files):
            res *= m2._matriu[i][i]
        return res

    def inv(self):
        m = self.clona()
        m.esten(self.identitat(self.num_files))
        signe, matriu = m._pseudogauss(diagonal=True)
        if not matriu:
            return None
        return m.submatriu(0, self.num_files - 1, self.num_columnes, self.num_columnes * 2 - 1)

    @property
    def T(self):
        res = self.zeros(self.num_columnes, self.num_files, self._dtype)
        for i, fila in enumerate(self._matriu):
            for j, item in enumerate(fila):
                res._matriu[j][i] = self._dtype(item)
        return res

    def __getitem__(self, index):
        if isinstance(index, int):
            return self._matriu[index]
        if isinstance(index, tuple):
            assert len(index) == 2
            return self._matriu[index[0]][index[1]]
        raise IndexError("Tipus d'index no suportat")

    def __repr__(self):
        res = [str(i) for i in self._matriu]
        return "\n".join(res)

    def __iter__(self):
        for fila in self._matriu:
            for item in fila:
                yield item

    def __hash__(self):
        return hash((self.num, self.den))



# per compatibilitat amb numpy

def _es_multiarray(m):
    return isinstance(m[0][0], (list, tuple))

def _garanteix_array(m):
    if not isinstance(m, array):
        m = array(m, Fraccio)
    return m

def det(m):
    """Calcula el determinant.

    M pot ser una matriu python (llista de files), un array o una
    llista de matrius python i/o arrays. Retorna un fracció o una
    llista de fraccions.

    Nota: els multiarray s'implementen per compatibilitat, en la
    pràctica no suposa cap avantatge.

    """
    if _es_multiarray(m):
        return [_garanteix_array(i).det() for i in m]
    else:
        return _garanteix_array(m).det()

def inv(m):
    """Calcula la inversa.

    M pot ser una matriu python (llista de files), un array o una
    llista de matrius python i/o arrays. Retorna un array o una llista
    de arrays.

    Si una matriu no te inversa retorna None, a diferència de numpy
    que avorta el càlcul. Açò possibilitat emparellar (zip) la llista
    de matrius amb les inverses.

    Nota: els multiarray s'implementen per compatibilitat, en la
    pràctica no suposa cap avantatge.

    """
    if _es_multiarray(m):
        return [_garanteix_array(i).inv() for i in m]
    else:
        return _garanteix_array(m).inv()

def dot(m, n):
    """Calcula el producte de dos matrius.

    A diferència de numpy retorna una matriu.
    """
    return _garanteix_array(m).dot(n)

def vdot(v1, v2, dtype=Fraccio):
    """Producte escalar de dos vectors.
    """
    res = dtype(0)
    for i, j in zip(v1, v2):
        res = res + dtype(i * j)
    return res

def ones(n):
    return array.ones(n)
