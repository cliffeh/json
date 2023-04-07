# cliffeh/json
an [RFC 8259](https://www.rfc-editor.org/rfc/rfc8259) compliant JSON parser

## What is this?
I stumbled across [JSONTestSuite](https://github.com/nst/JSONTestSuite) and figured "Hell, I think I can do that." ...and I (mostly) have. So, if you're interested in a JSON parser using good ol' lex/yacc/C with good JSONTestSuite test coverage, then read on.

## Dependencies
This project should have no external dependencies. (It uses popt for arg parsing, which has been packaged alongside it.)

## Building
`make`

(You might also want to take a look at `make help` for other build targets.)

## Running
After you've built it, try this:

    echo '["test", { "foo": "bar"}, false]' | ./json

Feel free to pipe in your json of choice.

## Testing
A full suite of tests can be run by doing:

    make check

## Caveats/Errata
* String processing in a UTF-n world is a goddam nightmare, and I am almost *certain* there are bugs in the approach I've taken (test coverage notwithstanding). I tried the approach listed [here](https://www.w3.org/2005/03/23-lex-U). It's close...but the regexes as-provided don't quite meet all needs. I've also tested unicode processing using [every Unicode code point](https://github.com/bits/UTF-8-Unicode-Test-Documents). The test passed...but the file is huge so I'm not including it here. Feel free to give it a whirl if you're so inclined.
* At time of writing this code doesn't do any interpretation of numeric values. Shoring this up is also on my [TODO](TODO.md) list.

## References
* [json.org](https://www.json.org/)
* [RFC 8259](https://www.rfc-editor.org/rfc/rfc8259)
* [JSONTestSuite](https://github.com/nst/JSONTestSuite)
* [Parsing JSON with lex and yacc](https://gist.github.com/justjkk/436828/)
* [Adding utf-8 Encoding to Lex](https://www.w3.org/2005/03/23-lex-U)
* [json-test-suite](https://code.google.com/archive/p/json-test-suite/)
* [Every Unicode code point](https://github.com/bits/UTF-8-Unicode-Test-Documents)
