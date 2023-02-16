bin_PROGRAMS = json
json_SOURCES = json.c json.h main.c parser.y scanner.l

BUILT_SOURCES = parser.c scanner.c

AM_YFLAGS = -d

# make tests moar fasterer; concurrency of 10 is (probably) not too aggressive
AM_MAKEFLAGS = -j10

EXTRA_DIST = LICENSE README.md TODO.md VERSION $(check_SCRIPTS)

CLEANFILES = $(BUILT_SOURCES) parser.h

# $(check_SCRIPTS) is set in Makefile.test.am
include Makefile.test.am
TESTS = $(check_SCRIPTS)

