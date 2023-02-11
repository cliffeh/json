#ifndef __JSON_JSON_H
#define __JSON_JSON_H 1

#include <stdio.h>

// types
#define JSON_T_NULL 0
#define JSON_T_BOOL 1
#define JSON_T_STRING 2
#define JSON_T_NUMBER 3
#define JSON_T_ARRAY 4
#define JSON_T_OBJECT 5

// print flags
// most significant 5 bits reserved for indent level (max 31 spaces/tabs)
#define JSON_T_PRINT_INDENT (1 << 1)   // default: no indent
#define JSON_T_PRINT_USE_TABS (1 << 2) // default: spaces
// TODO colorize?

// default indent: 2 spaces
#define JSON_T_PRINT_DEFAULT_INDENT (2 << 26)
#define JSON_T_PRINT_MAX_INDENT 31

typedef struct json_t
{
  int type, len;
  union
  {
    int ival;
    char *strval;
    struct json_t_list *lval;
  };
} json_t;

typedef struct json_t_list
{
  int len;
  char *key;     // only used for object
  json_t *value; // used for object and array types
  struct json_t_list *tail;
} json_t_list;

int json_t_print (FILE *out, const json_t *j, int depth, int flags);
json_t *new_json_t (int type);
void free_json_t (json_t *j);

#endif
