#ifndef __JSON_JSON_H
#define __JSON_JSON_H 1

#include <stdio.h>

#define JSON_T_NULL 0
#define JSON_T_BOOL 1
#define JSON_T_STRING 2
#define JSON_T_NUMBER 3
#define JSON_T_ARRAY 4
#define JSON_T_OBJECT 5

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

void json_t_pprint (FILE *out, json_t *j, int depth, int flags);
json_t *new_json_t (int type);
void free_json_t (json_t *j);

#endif
