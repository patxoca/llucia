# $Id:$

EXECUTABLES = prova productor treballador
SRCS = $(wildcard *.cc)
HEADERS = $(wildcard *.h)
OBJECTS = $(patsubst %.cc,%.o,$(SRCS))

LINK.o = $(LINK.cc)
CXX = g++
CPPFLAGS = -I $(HOME)/.local/include
CXXFLAGS = -O2 -Wall
LDLIBS = -lzmq -lboost_program_options
LDFLAGS = -L $(HOME)/.local/lib


.PHONY: all
all: depend $(EXECUTABLES)

prova:       cmdline.o combinator.o  prova.o
democalcul:  democalcul.o calcul.o
productor:   cmdline.o combinator.o  missatge.o productor.o
treballador: cmdline.o combinator.o  missatge.o treballador.o


.PHONY: tests
tests: $(OBJECTES)
	(cd tests && make runtests)

.PHONY: clean
clean:
	rm -f *~ *.o core
	make -C tests clean

.PHONY: allclean
allclean: clean
	rm -f $(EXECUTABLES) .depend
	make -C tests allclean

.PHONY: depend
depend: .depend

.depend: $(SRCS) $(HEADERS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $(SRCS) > ./.depend

include .depend
