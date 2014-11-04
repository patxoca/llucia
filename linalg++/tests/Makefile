TESTS = test_rational

CXX = g++
CXXFLAGS = -Wall -pthread
LDLIBS = -lgtest -lpthread

TESTS_OBJS = main.o $(addsuffix .o, $(TESTS))


all: tests
	./tests

test_rational.o: test_rational.cc ../rational.h

tests: $(TESTS_OBJS)
	$(CXX) -o tests $(TESTS_OBJS) $(LDLIBS)

.PHONY: clean
clean:
	rm -f *~ *.o

.PHONY: allclean
allclean: clean
	rm -f tests
