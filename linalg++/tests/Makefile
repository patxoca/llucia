# $Id:$

SRCS = $(wildcard *.cc)
HEADERS = $(wildcard *.h)
OBJECTS = $(patsubst %.cc,%.o,$(SRCS))

LINK.o = $(LINK.cc)
CXX = g++
CXXFLAGS = -Wall -pthread
LDLIBS = -lgtest -lpthread

TESTS_OBJS = $(OBJECTS) ../combinator.o


.PHONY: runtests
runtests: tests
	./tests --gtest_filter=*

tests: depend $(TESTS_OBJS)
	$(CXX) -o tests $(TESTS_OBJS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f *~ *.o core

.PHONY: allclean
allclean: clean
	rm -f tests

.PHONY: depend
depend: .depend

.depend: $(SRCS) $(HEADERS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $(SRCS) > ./.depend

include .depend
