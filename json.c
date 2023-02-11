#include "json.h"
#include <stdlib.h>

int
json_t_print (FILE *out, const json_t *j, int depth, int flags)
{
  switch (j->type)
    {
    case JSON_T_NULL:
      {
        return fprintf (out, "null");
      }
    case JSON_T_BOOL:
      {
        return fprintf (out, "%s", j->ival == 0 ? "false" : "true");
      }
    case JSON_T_STRING:
      {
        return fprintf (out, "\"%s\"", j->strval);
      }
    case JSON_T_NUMBER:
      {
        return fprintf (out, "%s", j->strval);
      }
    case JSON_T_ARRAY:
      {
        int r = 0;
        r += fprintf (out, "[");
        if (j->len > 0)
          {
            json_t_list *l = j->lval;
            r += json_t_print (out, l->value, depth + 1, flags);
            l = l->tail;
            while (l)
              {
                r += fprintf (out, ",");
                r += json_t_print (out, l->value, depth + 1, flags);
                l = l->tail;
              }
          }
        return r + fprintf (out, "]");
      }
    case JSON_T_OBJECT:
      {
        int r = 0;
        r = fprintf (out, "{");
        if (j->len > 0)
          {
            json_t_list *l = j->lval;
            r += fprintf (out, "\"%s\":", l->key);
            r += json_t_print (out, l->value, depth + 1, flags);
            l = l->tail;
            while (l)
              {
                r += fprintf (out, ",");
                r += fprintf (out, "\"%s\":", l->key);
                r += json_t_print (out, l->value, depth + 1, flags);
                l = l->tail;
              }
          }
        return r + fprintf (out, "}");
      }
      default: {
        fprintf(stderr, "I don't know how to print this!\n");
        return 0;
      }
    }
}

json_t *
new_json_t (int type)
{
  json_t *j = calloc (1, sizeof (json_t));
  j->type = type;
  return j;
}

void
free_json_t (json_t *j)
{
  switch (j->type)
    {
    case JSON_T_NULL:
    case JSON_T_BOOL:
      { /* nothing to do (j is freed at the bottom) */
      }
      break;
    case JSON_T_STRING:
    case JSON_T_NUMBER:
      {
        free (j->strval);
      }
      break;
    case JSON_T_ARRAY:
      {
        for (json_t_list *l = j->lval; l;
             /* we'll advance the tail in the loop */)
          {
            json_t_list *tmp = l;
            free_json_t (l->value);
            l = l->tail;
            free (tmp);
          }
      }
      break;
    case JSON_T_OBJECT:
      {
        for (json_t_list *l = j->lval; l;
             /* we'll advance the tail in the loop */)
          {
            json_t_list *tmp = l;
            free (l->key);
            free_json_t (l->value);
            l = l->tail;
            free (tmp);
          }
      }
      break;
    }
  free (j);
}
