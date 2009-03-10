# makefile for the analyzers

CXX = g++
RM = rm -f
CXXFLAGS = -I.. -O0 -Wall -Wextra -g3 -gdwarf-2 -DUNITTEST
TESTS_SRC = $(wildcard ./*Test.cpp)
TESTS = $(TESTS_SRC:%.cpp=%)

test: $(TESTS)
	@echo "Running tests."
	@for t in $(TESTS); do \
		$$t > $$t.output; \
		if [ $$? -ne 0 ]; then \
			echo $$t FAILED; \
		else \
			rm -f $$t.output; \
		fi; \
	done
	@echo "Tests completed."

PasstuneTest: PasstuneTest.o ../Analyzer.o ../Analyzers/Passtune.o ../Point.o ../Debug.o
	$(CXX) $(LDFLAGS) -o $@ $^

.PHONY: clean
clean:
	$(RM) *.o *.gch $(TESTS) $(TESTS:%=%.log)
