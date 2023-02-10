%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "json.h"

// quietens compiler warnings
int yylex(void);
int yyerror(char* s);

extern char *yytext;

%}

%token TRU FALS NUL CHARACTERS NUMBER

%union {
  char *sval;
  json_t *jval;
  json_t_list *lval;
}

%start json

%type <jval> element array object
%type <lval> elements members member
%type <sval> string characters number

%%

json: element { json_t_pprint(stdout, $1); free_json_t($1); };

element: object { $$ = $1; }
       | array { $$ = $1; }
       | string { $$ = new_json_t(JSON_T_STRING); $$->strval = $1; }
       | number { $$ = new_json_t(JSON_T_NUMBER); $$->strval = $1; }
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
      | '"' characters '"' { $$ = $2; }
      ;

characters: CHARACTERS { $$ = strdup(yytext); };

/* TODO keep tabs on whether or not there's a fraction and/or exponent? */
number: NUMBER { $$ = strdup(yytext); };

%%
// TODO better error messages
int yyerror(s)
char *s;
{
  return fprintf(stderr, "%s\n",s);
}

int yywrap()
{
  return(1);
}
