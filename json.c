#include "json.h"
#include <stdlib.h>

void
json_t_pprint (FILE *out, json_t *j)
{
  switch (j->type)
    {
    case JSON_T_NULL:
      {
        fprintf (out, "null");
        return;
      }
    case JSON_T_BOOL:
      {
        fprintf (out, "%s", j->ival == 0 ? "false" : "true");
        return;
      }
    case JSON_T_STRING:
      {
        fprintf (out, "\"%s\"", j->strval);
        return;
      }
    case JSON_T_NUMBER:
      {
        fprintf (out, "%s", j->strval);
        return;
      }
    case JSON_T_ARRAY:
      {
        fprintf (out, "[");
        if (j->len > 0)
          {
            json_t_list *l = j->lval;
            json_t_pprint (out, l->value);
            l = l->tail;
            while (l)
              {
                fprintf (out, ",");
                json_t_pprint (out, l->value);
                l = l->tail;
              }
          }
        fprintf (out, "]");
        return;
      }
    case JSON_T_OBJECT:
      {
        fprintf (out, "{");
        if (j->len > 0)
          {
            json_t_list *l = j->lval;
            fprintf (out, "\"%s\":", l->key);
            json_t_pprint (out, l->value);
            l = l->tail;
            while (l)
              {
                fprintf (out, ",");
                fprintf (out, "\"%s\":", l->key);
                json_t_pprint (out, l->value);
                l = l->tail;
              }
          }
        printf ("}");
        return;
      }
      // TODO add a default?
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
