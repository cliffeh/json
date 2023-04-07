BINARY=json
OBJECTS=json.o main.o parser.o scanner.o
LIBRARIES=popt/libpopt.a
CSOURCES=json.c json.h main.c
GENERATED_SOURCES=parser.c scanner.c
GENERATED_HEADERS=parser.h scanner.h
ACCEPT_TEST_DATA=$(wildcard test/data/y_*.json)
REJECT_TEST_DATA=$(wildcard test/data/n_*.json)
MEMCHECK_TEST_DATA=test/data/y_code.google.com_archive_p_json-test-suite.json
LFLAGS=--header-file=scanner.h
YACC=bison
# default yacc rule expects to `mv y.tab.c parser.c`
YFLAGS=-o y.tab.c --header=parser.h

json: $(OBJECTS) $(LIBRARIES) ## generate the json binary (default)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

allcheck: check memcheck ## run all tests
.PHONY: allcheck

check: $(BINARY) ## run suite of unit tests
	@test/accept.test $(ACCEPT_TEST_DATA)
	@test/reject.test $(REJECT_TEST_DATA)
.PHONY: check

memcheck: $(BINARY) ## test for memory leaks (requires valgrind)
	@test/memcheck.test $(MEMCHECK_TEST_DATA)
.PHONY: check

clean: ## clean up intermediate object files
	rm -f $(OBJECTS)
	make -C popt clean
.PHONY: clean

realclean: clean ## clean up generated binaries and sources, log files, etc.
	rm -f $(BINARY) $(GENERATED_SOURCES) $(GENERATED_HEADERS) test/*.log test/*.err
	make -C popt distclean
.PHONY:realclean

help: ## show this help
	@echo
	@echo "Specify a command. The choices are:"
	@echo
	@grep -E '^[0-9a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[0;36m%-12s\033[m %s\n", $$1, $$2}'
	@echo ""
.PHONY: help

main.o: main.c parser.h scanner.h

parser.h: parser.c

scanner.h: scanner.c

popt/libpopt.a: popt/Makefile
	make -C popt libpopt.a

popt/Makefile: popt/configure
	cd popt && ./configure
