bin_PROGRAMS = json
json_SOURCES = json.c json.h main.c parser.y scanner.l

BUILT_SOURCES = parser.c scanner.c

AM_YFLAGS = -d

# make tests moar fasterer; concurrency of 10 is (probably) not too aggressive
AM_MAKEFLAGS = -j10

# TODO add test/run-memcheck.sh
# TODO protect run-memcheck.sh against valgrind not being installed
# TODO make valgrind tests take less time?
TESTS = $(check_SCRIPTS)

EXTRA_DIST = LICENSE README.md TODO.md VERSION $(check_SCRIPTS)

CLEANFILES = $(BUILT_SOURCES) parser.h

include Makefile.test.am
