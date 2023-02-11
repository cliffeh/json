%{
#include "json.h"
#include "y.tab.h"

/*
  from https://gist.github.com/justjkk/436828/:

  [ -!#-\[\]-~]

  ^ this pattern is perfect for ASCII characters, but doesn't handle
    multi-byte unicode characters
*/

/*
  from https://www.w3.org/2005/03/23-lex-U:

  ['\u0000'-'\u007F']
  | (['\u00C2'-'\u00DF'] ['\u0080'-'\u00BF'])
  | ( '\u00E0'           ['\u00A0'-'\u00BF'] ['\u0080'-'\u00BF'])
  | (['\u00E1'-'\u00EC'] ['\u0080'-'\u00BF'] ['\u0080'-'\u00BF'])
  | ( '\u00ED'           ['\u0080'-'\u009F'] ['\u0080'-'\u00BF'])
  | (['\u00EE'-'\u00EF'] ['\u0080'-'\u00BF'] ['\u0080'-'\u00BF'])
  | ( '\u00F0'           ['\u0090'-'\u00BF'] ['\u0080'-'\u00BF'] ['\u0080'-'\u00BF'])
  | (['\u00F1'-'\u00F3'] ['\u0080'-'\u00BF'] ['\u0080'-'\u00BF'] ['\u0080'-'\u00BF'])
  | ( '\u00F4'           ['\u0080'-'\u008F'] ['\u0080'-'\u00BF'] ['\u0080'-'\u00BF'])

  ^ it "looks cool", but this pattern doesn't (quite) match everything
*/

%}
%x STRING
%%

"true"  { return(TRU);  }
"false" { return(FALS); }
"null"  { return(NUL);  }

-?(0|[1-9][0-9]*)(\.[0-9]+)?([eE][-+]?[0-9]+)? {
  return(NUMBER);
}

\" {
  BEGIN STRING;
  return('"');
}

<STRING>\" {
  BEGIN INITIAL;
  return('"');
}
<STRING>([^\\\"\x0-\x19]|\\([\"\\/bfnrt]|u[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]))* {
  return(CHARACTERS);
}

<STRING>. {
  // TODO better error handling...
  fprintf(stderr, "error: illegal character sequence inside string\n");
  return(1);
}

[ \t\r\n] /* swallow whitespace */

[\[\]{},:] { return(*yytext); }

. {
  // TODO better error handling...
  fprintf(stderr, "error: unexpected character %c\n", *yytext);
  return(1);
}

%%
