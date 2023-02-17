/* parse.y  (requires Bison) */

/* much of the boilerplate/bison directives taken from:
   https://begriffs.com/posts/2021-11-28-practical-parsing.html#using-a-parser-as-a-library */

/* a "pure" api means communication variables like yylval
   won't be global variables, and yylex is assumed to
   have a different signature */

%define api.pure true

/* change prefix of symbols from yy to "json" to avoid
   clashes with any other parsers we may want to link */

%define api.prefix {json}

/* generate much more meaningful errors rather than the
   uninformative string "syntax error" */

%define parse.error verbose

/* Bison offers different %code insertion locations in
   addition to yacc's %{ %} construct.

   The "top" location is good for headers and feature
   flags like the _XOPEN_SOURCE we use here */

%code top {
	/* XOPEN for strdup */
	#define _XOPEN_SOURCE 600
  #include "json.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	/* Bison versions 3.7.5 and above provide the YYNOMEM
	   macro to allow our actions to signal the unlikely
	   event that they couldn't allocate memory. Thanks
	   to the Bison team for adding this feature at my
	   request. :) YYNOMEM causes yyparse() to return 2.

	   The following conditional define allows us to use
	   the functionality in earlier versions too. */

	#ifndef YYNOMEM
	#define YYNOMEM goto yyexhaustedlab
	#endif
}

/* These are the semantic types available for tokens.

   The %union construction is classic yacc as well. It
   generates a C union and sets its as the YYSTYPE, which
   will be the type of yylval */

%union {
  char *sval;
  json_t *jval;
  json_t_list *lval;
}

/* Add another argument in yyparse() so that we
   can communicate the parsed result to the caller.
   We can't return the result directly, since the
   return value is already reserved as an int, with
   0=success, 1=error, 2=nomem

   NOTE
   In our case, the param is a data pointer. However,
   if it were a function pointer (such as a callback),
   then its type would have to be put behind a typedef,
   or else parse-param will mangle the declaration. */

%parse-param {json_t **result}

/* param adds an extra param to yyparse (like parse-param)
   but also causes yyparse to send the value to yylex.
   In our case the caller will initialize their own scanner
   instance and pass it through */

%param {void *scanner}

/* unqualified %code is for internal use, things that
   our actions can see. These declarations prevent
   warnings.  Notice the final param in each that came
   from the %param directive above */

%code {
	int jsonerror(void *foo, char const *msg, const void *s);
  int jsonlex(void *lval, const void *s);
}

%token <sval> CHARACTERS NUMBER
%token FALS NUL TRU

/* whereas tokens come from the lexer, these
   non-terminals are defined in the parser, and
   we set their types with %type */

%type <jval> element array object
%type <lval> elements members member
%type <sval> string

/* if there's an error partway through parsing, the
   caller wouldn't get a chance to free memory for
   the work in progress. Bison will clean up the memory
   if we provide destructors, though. */

// %destructor { free($$); } <sval>
// %destructor { json_t_free($$); } <jval>
// TODO free json_t_lists!

%%

json: element { if(result) { *result = $1; } };

element: object { $$ = $1; }
       | array { $$ = $1; }
       | string { $$ = new_json_t(JSON_T_STRING); $$->strval = $1; }
       | NUMBER { $$ = new_json_t(JSON_T_NUMBER); $$->strval = $1; }
       | TRU { $$ = new_json_t(JSON_T_BOOL); $$->ival = 1; }
       | FALS { $$ = new_json_t(JSON_T_BOOL); $$->ival = 0; }
       | NUL { $$ = new_json_t(JSON_T_NULL); }
       ;

object: '{' '}' { $$ = new_json_t(JSON_T_OBJECT); $$->len = 0; }
      | '{' members '}' { $$ = new_json_t(JSON_T_OBJECT); $$->lval = $2; $$->len = $2->len; }
      ;

members: member { $$ = $1; }
       | member ',' members { $$ = $1; $$->tail = $3; $$->len = $3->len + 1; }
       ;

member: string ':' element { $$ = calloc(1, sizeof(json_t_list)); $$->key = $1; $$->value = $3; $$->len = 1; };

array: '[' ']' { $$ = new_json_t(JSON_T_ARRAY); $$->len = 0; }
     | '[' elements ']' { $$ = new_json_t(JSON_T_ARRAY); $$->lval = $2; $$->len = $2->len; }
     ;

elements: element { $$ = calloc(1, sizeof(json_t_list)); $$->len = 1; $$->value = $1; }
        | element ',' elements { $$ = calloc(1, sizeof(json_t_list)); $$->value = $1; $$->tail = $3; $$->len = $3->len+1; }
        ;

string: '"' '"' { $$ = calloc(1, sizeof(char)); }
      | '"' CHARACTERS '"' { $$ = $2; }
      ;
%%
/* notice the extra parameters required
   by %param and %parse-param */

int jsonerror(void *yylval, char const *msg, const void *s)
{
	(void)yylval;
	(void)s;
	return fprintf(stderr, "%s\n", msg);
}