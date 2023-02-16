bin_PROGRAMS = json
json_SOURCES = json.c json.h main.c parser.y scanner.l

BUILT_SOURCES = parser.c scanner.c

AM_YFLAGS = -d

# TODO test scripts that run individual tests (instead of everything all at once)
# TODO include check_SCRIPTS = test/run-tests.sh
# TODO add test/run-memcheck.sh
# TODO protect run-memcheck.sh against valgrind not being installed
# TODO make valgrind tests take less time?
TESTS = test/run-tests.sh

EXTRA_DIST = LICENSE README.md TODO.md VERSION

CLEANFILES = $(BUILT_SOURCES) parser.h
