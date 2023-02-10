BINARIES=json
OBJECTS=json.o parser.o scanner.o
SOURCES=json.c json.h parser.y scanner.l
GENERATED_SOURCES=parser.c scanner.c y.tab.h
YFLAGS=-d -y
# LFLAGS=--debug
# CFLAGS=-g

json: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

parser.o: parser.c json.h

scanner.o: scanner.c y.tab.h json.h

json.o: json.c json.h

scanner.c: scanner.l

parser.c y.tab.h: parser.y

check: json
	make -C test
.PHONY: check

valgrind:
	make -C test valgrind
.PHONY: valgrind

clean:
	rm -f $(OBJECTS)
	make -C test clean
.PHONY: clean

realclean: clean
	rm -f $(GENERATED_SOURCES) $(BINARIES)
.PHONE: realclean
