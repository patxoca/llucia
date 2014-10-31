# Makefile per compilar el mòdul 'combinator.so' de forma que faciliti
# testejar-lo.
#
# No utilitzar per generar el mòdul final. Distutils/setuptools el
# generen automàticament al instal·lar-lo.

# $Id:$

CC = gcc
PYTHON_CONFIG = /opt/python/2.7.8/bin/python-config
CFLAGS = -g -std=c99 $(shell $(PYTHON_CONFIG) --includes) -fPIC -Wall -Wstrict-prototypes
OBJECTS = combinatormodule.o

all: combinator.so

combinator.so: $(OBJECTS)
	$(CC) -shared -o combinator.so $(OBJECTS)

.PHONY: clean
clean:
	rm -f $(OBJECTS) *~
