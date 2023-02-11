#include "json.h"
#include <stdlib.h>

#define PRINT_SPACES                                                          \
  do                                                                          \
    {                                                                         \
      if (*indentstr)                                                         \
        {                                                                     \
          for (int i = 0; i < depth; i++)                                     \
            {                                                                 \
              r += printf ("%s", indentstr);                                  \
            }                                                                 \
        }                                                                     \
    }                                                                         \
  while (0)

static int
__json_t_print (FILE *out, const json_t *j, int depth, const char *indentstr)
{
  int r = 0;
  switch (j->type)
    {
    case JSON_T_NULL:
      {
        PRINT_SPACES;
        r += fprintf (out, "null");
        if (!depth)
          r += fprintf (out, "\n");
        return r;
      }
    case JSON_T_BOOL:
      {
        PRINT_SPACES;
        r + fprintf (out, "%s", j->ival == 0 ? "false" : "true");
        if (!depth)
          r += fprintf (out, "\n");
        return r;
      }
    case JSON_T_STRING:
      {
        PRINT_SPACES;
        r + fprintf (out, "\"%s\"", j->strval);
        if (!depth)
          r += fprintf (out, "\n");
        return r;
      }
    case JSON_T_NUMBER:
      {
        PRINT_SPACES;
        r + fprintf (out, "%s", j->strval);
        if (!depth)
          r += fprintf (out, "\n");
        return r;
      }
    case JSON_T_ARRAY:
      {
        PRINT_SPACES;
        r += fprintf (out, "[");
        if (j->len > 0)
          {
            if (*indentstr)
              r += fprintf (stdout, "\n");
            json_t_list *l = j->lval;
            r += __json_t_print (out, l->value, depth + 1, indentstr);
            l = l->tail;
            while (l)
              {
                r += fprintf (out, ",");
                if (*indentstr)
                  r += fprintf (stdout, "\n");
                r += __json_t_print (out, l->value, depth + 1, indentstr);
                l = l->tail;
              }
            if (*indentstr)
              r += fprintf (stdout, "\n");
            PRINT_SPACES;
          }

        r += fprintf (out, "]");
        if (!depth)
          r += fprintf (out, "\n");
        return r;
      }
    case JSON_T_OBJECT:
      {
        r = fprintf (out, "{");
        if (j->len > 0)
          {
            json_t_list *l = j->lval;
            r += fprintf (out, "\"%s\":", l->key);
            r += __json_t_print (out, l->value, depth + 1, indentstr);
            l = l->tail;
            while (l)
              {
                r += fprintf (out, ",");
                r += fprintf (out, "\"%s\":", l->key);
                r += __json_t_print (out, l->value, depth + 1, indentstr);
                l = l->tail;
              }
          }
        return r + fprintf (out, "}");
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

  char indentstr[indent + 1];
  indentstr[0] = 0;

  if (indent)
    {
      char c = use_tabs ? '\t' : ' ';
      for (int i = 0; i < indent; i++)
        {
          indentstr[i] = c;
        }
    }

  rc = __json_t_print (out, j, 0, indentstr);

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
