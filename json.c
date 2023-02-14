#include "json.h"
#include <stdlib.h>

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

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

static unsigned char
eat_whitespace (FILE *in)
{
  unsigned char c;
  do
    {
      switch (c = fgetc (in))
        {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
          break;
        default:
          return c;
        }
    }
  while (1);
}

static json_t *parseElement (FILE *in, unsigned char la);
static json_t *parseObject (FILE *in);
static json_t *parseArray (FILE *in);
static char *parseString (FILE *in);
static char *parseNumber (FILE *in, unsigned char la);

json_t *
json_t_parse (FILE *in)
{
  // TODO BOM leading bytes?
  unsigned char la = eat_whitespace (in);
  return parseElement (in, la);
  // TODO what about garbage afterward?
}

static json_t *
parseElement (FILE *in, unsigned char la)
{
  switch (la)
    {

    case 'f': // false
      {
        if (fgetc (in) == 'a' && fgetc (in) == 'l' && fgetc (in) == 's'
            && fgetc (in) == 'e')
          {
            json_t *r = new_json_t (JSON_T_BOOL);
            r->ival = 0;
            return r;
          }
      }

    case 'n': // null
      {
        if (fgetc (in) == 'u' && fgetc (in) == 'l' && fgetc (in) == 'l')
          {
            return new_json_t (JSON_T_NULL);
          }
      }

    case 't': // true
      {
        if (fgetc (in) == 'r' && fgetc (in) == 'u' && fgetc (in) == 'e')
          {
            json_t *r = new_json_t (JSON_T_BOOL);
            r->ival = 1;
            return r;
          }
      }

    case '"':
      {
        char *strval = parseString (in);
        if (!strval)
          return 0;

        json_t *r = new_json_t (JSON_T_STRING);
        r->strval = strval;
        return r;
      }

    // clang-format off
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
    case '-':
      // clang-format on
      {
        char *strval = parseNumber (in, la);
        if (!strval)
          return 0;

        json_t *r = new_json_t (JSON_T_NUMBER);
        r->strval = strval;
        return r;
      }

    case '{':
      return parseObject (in);

    case '[':
      return parseArray (in);

    default:
      {
        fprintf (stderr, "unknown character: %c\n", la);
      }
    }

  return 0;
}

static json_t *
parseArray (FILE *in)
{
  json_t *r = new_json_t (JSON_T_ARRAY);
  unsigned char la = eat_whitespace (in);

  if (la == ']')
    return r;

  r->lval = calloc (1, sizeof (json_t_list));
  r->len = 1;

  if (!(r->lval->value = parseElement (in, la)))
    {
      fprintf (stderr, "error while parsing array elements\n");
      free (r->lval);
      free (r);
      return 0;
    }

  json_t_list *curr = r->lval;

  while ((la = eat_whitespace (in)) == ',')
    {
      curr->tail = calloc (1, sizeof (json_t_list));
      curr = curr->tail;
      if (!(curr->value = parseElement (in, eat_whitespace (in))))
        {
          fprintf (stderr, "error while parsing array elements\n");
          // TODO cleanup
          return 0;
        }
    }

  if (la != ']')
    {
      fprintf (stderr, "error while parsing array\n");
      // TODO cleanup
      return 0;
    }

  return r;
}

static json_t *
parseObject (FILE *in)
{
  return 0;
}

static char *
parseString (FILE *in)
{
  int size = BUFSIZ, pos = 0;
  char *buf = malloc (size), la;

  while (1)
    {
      if (pos + 6 >= size)
        {
          buf = realloc (buf, size * 2);
          size *= 2;
        }
      switch (la = fgetc (in))
        {
        case '"':
          {
            // buf = realloc (buf, pos + 1);
            buf[pos] = 0;
            return buf;
          }
        case '\\':
          {
            buf[pos++] = la;
            switch (la = fgetc (in))
              { // \"\\/bfnrt
              case '"':
              case '\\':
              case '/':
              case 'b':
              case 'f':
              case 'n':
              case 'r':
              case 't':
                {
                  buf[pos++] = la;
                }
                break;
              case 'u':
                {
                  if (pos + 5 >= size)
                    {
                      buf = realloc (buf, size * 2);
                      size *= 2;
                    }
                  buf[pos++] = la;
                  for (int i = 0; i < 4; i++)
                    {
                      switch (la = fgetc (in))
                        {
                        // clang-format off
                        case '0': case '1': case '2': case '3': case '4':
                        case '5': case '6': case '7': case '8': case '9':
                        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                          // clang-format on
                          {
                            buf[pos++] = la;
                          }
                          break;
                        default:
                          {
                            fprintf (
                                stderr,
                                "illegal character in hex escape sequence\n");
                            free (buf);
                            return 0;
                          }
                        }
                    }
                }
                break;
              default:
                {

                  fprintf (stderr, "illegal character in escape sequence\n");
                  free (buf);
                  return 0;
                }
              }
            break;
          default:
            {
              if (la >= '\x20') // TODO is this actually correct?
                {
                  buf[pos++] = la;
                }
              else
                {
                  fprintf (stderr, "illegal character in string literal\n");
                  free (buf);
                  return 0;
                }
            }
          }
        }
    }
}

// -?(0|[1-9][0-9]*)(\.[0-9]+)?([eE][-+]?[0-9]+)?
static char *
parseNumber (FILE *in, unsigned char la)
{
  int size = BUFSIZ, pos = 0;
  char *buf = malloc (size);

  if (la == '-')
    {
      buf[pos++] = la;
      la = fgetc (in);
    }

  if (la == '0')
    {
      buf[pos++] = la;
    }
  else
    {
      switch (la)
        {
        // clang-format off
        case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
          // clang-format on
          {
            buf[pos++] = la;
          }
          break;
        default:
          {
            fprintf (stderr, "illegal character in number literal\n");
            free (buf);
            return 0;
          }
        }
    }
  // TODO use labels/gotos?

  // TODO implement
  return 0;
}
