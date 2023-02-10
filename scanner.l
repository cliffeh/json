%{
#include "json.h"
#include "y.tab.h"
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
  exit(1);
}

[ \t\r\n] /* swallow whitespace */

[\[\]{},:] { return(*yytext); }

. {
  // TODO better error handling...
  fprintf(stderr, "error: unexpected character %c\n", *yytext);
  exit(1);
}

%%
