#include "json.h"
#include <stdlib.h>

#define PRINT_SPACES                                                          \
  do                                                                          \
    {                                                                         \
      for (int i = 0; i < (indent * depth); i++)                              \
        {                                                                     \
          r += fprintf (out, "%s", ic);                                       \
        }                                                                     \
    }                                                                         \
  while (0)

static int __json_t_print (FILE *out, const json_t *j, int depth, int indent,
                           const char *ic);
static int __json_t_list_print (FILE *out, const json_t_list *l, int depth,
                                int indent, const char *ic);

static int
__json_t_list_print (FILE *out, const json_t_list *l, int depth, int indent,
                     const char *ic)
{
  int r = 0;
  const json_t_list *curr = l;

  while (curr)
    {
      if (curr->key)
        { // assume we're printing an object's list
          PRINT_SPACES;
          r += fprintf (out, "\"%s\":", curr->key);
          if (indent)
            r += fprintf (out, "%s", ic);
          r += __json_t_print (out, curr->value, depth, indent, ic);
          if (curr->tail)
            {
              r += fprintf (out, ",");
              if (indent)
                r += fprintf (out, "\n");
            }
        }
      else
        { // we're printing an array
          PRINT_SPACES;
          r += __json_t_print (out, curr->value, depth, indent, ic);
          if (curr->tail)
            {
              r += fprintf (out, ",");
              if (indent)
                r += fprintf (out, "\n");
            }
        }
      curr = curr->tail;
    }

    return r;
}

static int
__json_t_print (FILE *out, const json_t *j, int depth, int indent,
                const char *ic)
{
  int r = 0;
  switch (j->type)
    {
    case JSON_T_NULL:
      {
        r += fprintf (out, "null");
        return r;
      }
    case JSON_T_BOOL:
      {
        r += fprintf (out, "%s", j->ival == 0 ? "false" : "true");
        return r;
      }
    case JSON_T_STRING:
      {
        r += fprintf (out, "\"%s\"", j->strval);
        return r;
      }
    case JSON_T_NUMBER:
      {
        r += fprintf (out, "%s", j->strval);
        return r;
      }
    case JSON_T_ARRAY:
      {
        r += fprintf (out, "[");
        if (j->len > 0)
          {
            if (indent)
              r += fprintf (out, "\n");
            r += __json_t_list_print (out, j->lval, depth + 1, indent, ic);
            if (indent)
              r += fprintf (out, "\n");
            PRINT_SPACES;
          }
        r += fprintf (out, "]");
        return r;
      }
    case JSON_T_OBJECT:
      {
        r += fprintf (out, "{");
        if (j->len > 0)
          {
            if (indent)
              r += fprintf (out, "\n");
            r += __json_t_list_print (out, j->lval, depth + 1, indent, ic);
            if (indent)
              r += fprintf (out, "\n");
            PRINT_SPACES;
          }
        r += fprintf (out, "}");
        return r;
      }
    default:
      {
        fprintf (stderr, "I don't know how to print this!\n");
        return 0;
      }
    }
}

int
json_t_print (const json_t *j, const json_t_print_options *opt)
{
  FILE *out = DEFAULT_JSON_T_PRINT_OUTFILE;
  int indent = DEFAULT_JSON_T_PRINT_INDENT, use_tabs = 0, rc;

  if (opt)
    {
      out = opt->out;
      indent = opt->indent;
      use_tabs = opt->use_tabs;
    }

  const char *ic = (indent) ? (use_tabs ? "\t" : " ") : "";

  rc = __json_t_print (out, j, 0, indent, ic);

  if (indent)
    rc += fprintf (out, "\n");

  return rc;
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
