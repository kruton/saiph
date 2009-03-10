# makefile for the analyzers

CXX = g++
RM = rm -f
VALGRIND = valgrind
VALGRIND_OPTS = --leak-check=full --num-callers=15
CXXFLAGS = -I.. -O0 -Wall -Wextra -g3 -gdwarf-2 -DUNITTEST
TESTS_SRC = $(wildcard ./*Test.cpp)
TESTS = $(TESTS_SRC:%.cpp=%)

test: $(TESTS)
	@echo "### Running tests."
	@for t in $(TESTS); do \
		$$t > $$t.output; \
		if [ $$? -ne 0 ]; then \
			echo $$t FAILURE; \
		else \
			echo $$t success; \
		fi; \
		rm -f $$t.output; \
	done
	@echo "### Tests completed."

valgrind: $(TESTS)
	@echo "### Running tests under valgrind."
	@for t in $(TESTS); do \
		$(VALGRIND) $(VALGRIND_OPTS) --quiet --log-fd=3 -- $$t > $$t.output 2>&1 3> $$t.valgrind; \
		if [ ! -z $$t.valgrind ]; then \
			echo "$$t FAILURE under valgrind!"; \
		else \
			echo "$$t success"; \
		fi; \
		rm -f $$t.output $$t.valgrind; \
	done
	@echo "### Tests under valgrind completed."

PasstuneTest: PasstuneTest.o ../Analyzer.o ../Analyzers/Passtune.o ../Point.o ../Debug.o
	$(CXX) $(LDFLAGS) -o $@ $^

.PHONY: clean
clean:
	$(RM) *.o *.gch $(TESTS) $(TESTS:%=%.log)
