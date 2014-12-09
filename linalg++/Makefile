# $Id:$

EXECUTABLES = lcp lcw

LCP_SRC = acumulador.cc combinator.cc missatge.cc cmdline.cc gameloader.cc productor.cc
LCW_SRC = calcul.cc combinator.cc missatge.cc treballador.cc cmdline.cc
SRCS = $(LCP_SRC) $(LCW_SRC) prova.cc

HEADERS = $(wildcard *.h)

LCP_OBJ = $(patsubst %.cc,%.o,$(LCP_SRC))
LCW_OBJ = $(patsubst %.cc,%.o,$(LCW_SRC))
OBJECTS = $(LCP_OBJ) $(LCW_OBJ)

LINK.o = $(LINK.cc)
CXX = g++
CPPFLAGS = -I $(HOME)/.local/include
CXXFLAGS = -O2 -Wall
LDLIBS = -lzmq -lboost_program_options -lboost_system -lboost_thread
LDFLAGS = -L $(HOME)/.local/lib


.PHONY: all
all: depend $(EXECUTABLES)

prova: cmdline.o combinator.o gameloader.o prova.o
lcp:   $(LCP_OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
lcw:   $(LCW_OBJ)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

.PHONY: tests
tests: $(OBJECTS)
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
