# -*- coding: utf-8 -*-

# $Id$

# Aquest mòdul és importat per bases_coordinador y bases_treballador.
# S'utilitza per definir opcions configurables centralitzadament,
# facilitant canviar-les sense haver de tocar el codi del programa.

import logging

# nombre de jugadors

DIMENSIO = 5

# ------------------------------------------------------------------------
# Adreça del coordinador

SERVIDOR_RPC = "localhost"
PORT_RPC = 8000

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

# ------------------------------------------------------------------------
# Configuració interna del treballador

# Temps màxim d'espera, en segons, entre reintents quan es produeixen
# errors de connexió entre el treballador i el coordinador.
MAX_ESPERA = 64