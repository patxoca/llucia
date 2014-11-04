EXECUTABLES = prova

CXX = g++
CXXFLAGS = -O2 -Wall


.PHONY: all
all: $(EXECUTABLES)

prova: prova.cc rational.h matrix.h

.PHONY: tests
tests:
	(cd tests && make)

.PHONY: clean
clean:
	rm -f *~ *.o
	make -C tests clean

.PHONY: allclean
allclean: clean
	rm -f $(EXECUTABLES)
	make -C tests allclean
