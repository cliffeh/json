# cliffeh/json
an RFC 8259 compliant JSON parser
...sort of

## What is this?
I stumbled across [JSONTestSuite](https://github.com/nst/JSONTestSuite) and figured "Hell, I think I can do that." ...and I (mostly) have. So, if you're interested in a JSON parser using good ol' lex/yacc/C with good JSONTestSuite test coverage, then read on.

## Building

`make`

It's on my [TODO](TODO.md) list to set up autotools for this - and I probably will once I have to start pulling in dependencies for things - but for now it's a straightforward `make`. You should also be able to do `make check` to run the test suite, and if you have valgrind installed `make valgrind` will run checks for memory leaks.

## Running
After you've built it, try this:

`echo '["test", { "foo": "bar"}, false]' | ./json`

Feel free to pipe in your json of choice.

## Caveats
Some of these are covered in the TODO, but IMHO I think the two that need called out:

* String processing in a UTF-n world is a goddam nightmare, and I am almost *certain* there are bugs in the approach I've taken (test coverage notwithstanding).
* At time of writing this code doesn't do any interpretation of numeric values.

## Links

Some links 

* https://www.json.org/
* https://github.com/nst/JSONTestSuite
* https://gist.github.com/justjkk/436828/
