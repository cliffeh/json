%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "json.h"

// quietens compiler warnings
int yylex(void);

int yyerror(json_t **j, char *msg)
{
  return fprintf(stderr, "%s\n", msg);
}

extern char *yytext;
%}

%parse-param { json_t **j }

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

json: element { if(j) { *j = $1; } };

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

characters: CHARACTERS { $$ = calloc(yylen+1, sizeof(char)); memcpy($$, yytext, yylen); };

/* TODO keep tabs on whether or not there's a fraction and/or exponent? */
number: NUMBER { $$ = calloc(yylen+1, sizeof(char)); memcpy($$, yytext, yylen); };

%%
int yywrap()
{
  return(1);
}
