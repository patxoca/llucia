CC=G++
CPPFLAGS = -O2 -Wall

.PHONY: all
all: prova

.PHONY: tests
tests:
	(cd tests && make)

.PHONY: clean
clean:
	rm -f *~ *.o
	make -C tests clean
