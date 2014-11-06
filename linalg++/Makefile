# $Id:$

EXECUTABLES = prova
SRCS = $(wildcard *.cc)
HEADERS = $(wildcard *.h)
OBJECTS = $(patsubst %.cc,%.o,$(SRCS))

LINK.o = $(LINK.cc)
CXX = g++
CXXFLAGS = -O2 -Wall



.PHONY: all
all: depend $(EXECUTABLES)

prova: $(OBJECTS)

.PHONY: tests
tests: $(OBJECTES)
	(cd tests && make runtests)

.PHONY: clean
clean:
	rm -f *~ *.o
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
