ACCEPT=$(wildcard y_*.json)
REJECT=$(wildcard n_*.json)
EITHER=$(wildcard i_*.json)
TESTFILES=$(ACCEPT) $(REJECT) $(EITHER)


run-tests:
	./run-tests.sh
.PHONY: run-tests

# TODO output pass/fail!
valgrind:
	for file in $(ACCEPT); do echo "valgrinding $$file"; valgrind --leak-check=full --log-file=$$file.valgrind.log ../json > /dev/null 2>&1 < $$file; done
.PHONY: valgrind

clean:
	rm -f *.out *.err *.log
.PHONY: clean
