BINARIES=json
OBJECTS=json.o parser.o scanner.o
SOURCES=json.c json.h parser.y scanner.l
GENERATED_SOURCES=parser.c scanner.c y.tab.h
YFLAGS=-d -y
# LFLAGS=--debug
# CFLAGS=-g

json: $(OBJECTS) ## build the json binary (default)
	$(CC) $(CFLAGS) -o $@ $^

parser.o: parser.c json.h

scanner.o: scanner.c y.tab.h json.h

json.o: json.c json.h

scanner.c: scanner.l

parser.c y.tab.h: parser.y

check: json ## run the suite of unit tests
	make -C test
.PHONY: check

valgrind: ## run a set of memory leak checks (requires valgrind)
	make -C test valgrind
.PHONY: valgrind

clean: ## clean up generated object files
	rm -f $(OBJECTS)
	make -C test clean
.PHONY: clean

realclean: clean ## clean up all generated objects, source, binaries, logs, etc.
	rm -f $(GENERATED_SOURCES) $(BINARIES)
.PHONE: realclean

help: ## show this help
	@echo "\nSpecify a command. The choices are:\n"
	@grep -E '^[0-9a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[0;36m%-12s\033[m %s\n", $$1, $$2}'
	@echo ""
.PHONY: help
