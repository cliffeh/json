BINARIES=json
OBJECTS=json.o main.o parser.o scanner.o
CSOURCES=json.c json.h main.c
YYSOURCES=parser.y scanner.l
SOURCES=$(CSOURCES) $(YYSOURCES)
GENERATED_SOURCES=parser.c scanner.c y.tab.h
YFLAGS=-d -y
LDFLAGS=-lpopt
# LFLAGS=--debug
# CFLAGS=-g

json: $(OBJECTS) ## build the json binary (default)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

check: json ## run the suite of unit tests
	test/run-tests.sh
.PHONY: check

memcheck: json ## run a set of memory leak checks (requires valgrind)
	test/run-memcheck.sh
.PHONY: memcheck

allcheck:  ## run the suite of unit tests including extra test cases
	test/run-tests.sh --all
.PHONY:allcheck

clean: ## clean up generated object files
	rm -f $(OBJECTS) test/data/*.log test/data/*.out test/data/*.err test/data/*.diff
.PHONY: clean

realclean: clean ## clean up all generated objects, source, binaries, logs, etc.
	rm -f $(GENERATED_SOURCES) $(BINARIES)
.PHONY: realclean

format: .clang-format ## format C sources and headers (requires clang-format)
	clang-format -i $(CSOURCES)
.PHONY: format

help: ## show this help
	@echo "\nSpecify a command. The choices are:\n"
	@grep -E '^[0-9a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[0;36m%-12s\033[m %s\n", $$1, $$2}'
	@echo ""
.PHONY: help

json.o: json.c json.h

main.o: main.c json.h

parser.c y.tab.h: parser.y

parser.o: parser.c json.h

scanner.c: scanner.l

scanner.o: scanner.c json.h y.tab.h
