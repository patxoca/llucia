# -*- coding: utf-8 -*-

# $Id$

# Aquest mòdul defineix opcions configurables centralitzadament,
# facilitant canviar-les sense haver de tocar el codi del programa.

import logging

# nombre de jugadors

DIMENSIO = 5


# ------------------------------------------------------------------------
# Si True mostra més informació al executar el productor/treballador.
# Actualment mostra el progrés.

VERBOSE = False

# ------------------------------------------------------------------------
# Adreça del productor

PRODUCTOR = "tcp://127.0.0.1:5555"

# ------------------------------------------------------------------------
# Configuració del logging

SERVIDOR_LOG = "localhost"
PORT_LOG = 8001
NIVELL_LOG_COORDINADOR = logging.INFO
NIVELL_LOG_TREBALLADOR = logging.INFO

# ------------------------------------------------------------------------
# Configuració interna del coordinador

# nombre d'elements en cada paquet enviat als treballadors
MAX_COMBINACIONS_PAQUET = 25000

# nombre màxim de treballadors que accepta el coodinador
MAX_TREBALLADORS = 10

# nivell de compressio dels paquets enviats als treballadors: 0 no es
# comprimeix, 9 compressió màxima. Cal trobar un equilibri entre el
# temps requerit per comprimir les dades i l'estalvi que suposa al
# enviar-lo per xarxa.
NIVELL_COMPRESSIO = 0

# ------------------------------------------------------------------------
# Configuració interna del treballador

# Temps màxim d'espera, en segons, entre reintents quan es produeixen
# errors de connexió entre el treballador i el coordinador.
MAX_ESPERA = 64
