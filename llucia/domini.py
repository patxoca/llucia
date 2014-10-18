# -*- coding: utf-8 -*-

# $Id$

import itertools

import numpy


##############################################################################
#
# Generar vectors i bases
#

def decimal_a_binari(n, mida):
    """
    n=1, mida=3 -> (1, 0, 0)
    n=3, mida=3 -> (1, 1, 0)
    n=7, mida=2 -> ERROR
    """
    r = [ 0 ] * mida
    i = 0
    while n:
        r[i] = n % 2
        n = n / 2
        i += 1
    return tuple(r)


def generar_vectors(mida):
    for i in xrange(1, 2**mida):
        yield decimal_a_binari(i, mida)


def generar_combinacions(mida):
    """Genera totes les possibles combinacions de coalicions de fins MIDA
    jugadors.

    Cada combinació és una llista de coalicions i cada coalició és
    representa mitjançant un valor enter que codifica en binari la
    presència dels jugadors en la coalició:

      1 -> (1, 0, 0)
      2 -> (0, 1, 0)
      3 -> (1, 1, 0)
      etc.

    """
    vectors = range(1, 2**mida)
    for i in itertools.combinations(vectors, mida):
        yield i

def construir_descodificador_coalicions(mida):
    """Retorna una funció per descodificar coalicions de fins MIDA
    jugadors.

    """
    taula = [None] # la posició zero no s'utilitza
    for i in range(1, 2**mida):
        taula.append(decimal_a_binari(i, mida))
    def descodificador(coalicio):
        return taula[coalicio]
    return descodificador

def descodificar_combinacio(combinacio, descodificador):
    """Descodifica una combinació de coalicions.

    DESCODIFICADOR és una funció que rep un enter i retorna un vector
    binari.

    Retorna una llista de vectors binaris.

    """
    return map(descodificador, combinacio)

def generar_bases(iterador):
    for candidat in iterador:
        if numpy.linalg.det(candidat) != 0:
            yield candidat



##############################################################################
#
# Guardar/llegir matrius en/des d'arxius
#

def guardar_matrius(nom_arxiu, dimensio, iterable):
    """Guarda dins l'arxiu NOM_ARXIU les matrius contingutes en ITERABLE.

    El format de l'arxiu és:

      dimensio
      matriu1
      matriu2
      ...

    on cada matriu es guarda per files, en una única línia, amb els
    elements separats per un espai en blanc.

    """
    with open(nom_arxiu, "w") as f:
        f.write("%i\n" % dimensio)
        for base in iterable:
            # converteix la matriu [[1, 0], [1, 1]] en la cadena "1 0 1 1"
            # utilitza "list comprehension"
            linia = " ".join([" ".join(map(str, fila)) for fila in base])
            f.write(linia)
            f.write("\n")


def carregar_matrius(nom_arxiu, tipus=int):
    """Llegeix les bases guardades dins l'arxiu NOM_ARXIU per la funció
    guardar_matrius.

    El primer element retornat és la dimensió de les matrius, després
    retorna una matriu cada vegada.

    Converteix els elements de la base al tipus TIPUS, enter per
    defecte.

    """
    with open(nom_arxiu, "r") as f:
        dimensio = int(f.readline())
        yield dimensio
        for linia in f:
            base = []
            elements = map(tipus, linia.split(" "))
            while elements:
                base.append(elements[:dimensio])
                elements = elements[dimensio:]
            if len(base) != dimensio:
                raise ValueError("Error en l'arxiu.")
            yield base



##############################################################################
#
# pretty printing de vectors i bases
#

def vector_a_jugador(vector):
    """
    (1, 0, 0) -> "1"
    (0, 1, 1) -> "23"
    """
    r = []
    for i, v in enumerate(vector):
        if v:
            r.append(str(i + 1))
    res = "".join(r)
    if len(res) == len(vector):
        return "N"
    else:
        return res

def codificar_linia_base_arxiu(arxiu, base, classe, cont):
    jugadors = [vector_a_jugador(i) for i in base]
    jugadors.append(classe)
    print >>arxiu, cont, ",".join(jugadors)

def base_a_bonic(base):
    """
    [(1, 0, 0), (1, 1, 0), (0, 1, 1)] -> "{1, 12, 23}"
    """
    return "{%s}" % ", ".join([vector_a_jugador(vector) for vector in base])



##############################################################################
#
# Calcular la taula de coalicions
#

def descodificar_coalicio(c, n):
    """
    "1",  3 -> (1, 0, 0)
    "12", 4 -> (1, 1, 0, 0)
    """
    v = [0] * n
    for i in c:
        ii = int(i)
        if 1 <= ii <= n:
            v[ii - 1] = 1
        else:
            raise ValueError("Valor no vàlid: %s" % i)
    return tuple(v)


def precalcular_hash(l, n):
    """
    precalcular_hash([("1", 2), ("12", -5) ...], 3) retorna

    {
      (1, 0, 0) : 2, # "1" codifica (1, 0, 0)
      (1, 1, 0) : -5 # "12" codifica (1, 1, 0)
      ...
    }
    """
    h = {}
    for coalicio, valor in l:
        vector = descodificar_coalicio(coalicio, n)
        if h.has_key(vector):
            raise ValueError("%s duplicat" % coalicio)
        h[vector] = valor
    # estaria be validar que no en falta cap
    return h



##############################################################################
#
# Plantejar i resoldre el sistema
#

# Aquest és l'anterior fer_algo. El canvi més destacable és que ja no
# genera les bases. Enlloc d'això rep un iterable (una llista o un
# generador) amb les matrius i planteja les corresponents equacions.
#
#

def resoldre_sistema_de_xxx(l, n, iterable):
    """Per cada matriu de ITERABLE planteja i resol el sistema de N
    equacions a partir dels valors per les coalicions que hi ha en L.

    L és una llista de parelles (coalicio, valor)

    Retorna ternes (coeficients, termes_independes, solucions)

    """
    h = precalcular_hash(l, n)
    for base in iterable:
        ti = []
        for vector in base:
            ti.append(h[vector])
        x = numpy.linalg.solve(base, ti)
        yield (base, ti, x)
