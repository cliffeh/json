bin_PROGRAMS = json
json_SOURCES = json.c json.h main.c parser.h parser.y scanner.h scanner.l

BUILT_SOURCES = parser.c parser.h scanner.c scanner.h

scanner.h: scanner.c

AM_LFLAGS = -oscanner.c --header-file=scanner.h
AM_YFLAGS = -d -Wno-yacc

# make tests moar fasterer; concurrency of 10 is (probably) not too aggressive
AM_MAKEFLAGS = -j10

EXTRA_DIST = LICENSE README.md TODO.md VERSION $(check_SCRIPTS) $(check_DATA)

CLEANFILES = $(BUILT_SOURCES) valgrind.log

# $(check_SCRIPTS) $(check_DATA) $(TESTS) are set in Makefile.test.am
include Makefile.test.am
