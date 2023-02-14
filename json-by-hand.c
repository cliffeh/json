#include "json.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

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

// TODO expose non-static functions in json.h
json_t *parseJson (FILE *in);
static json_t *parseElement (FILE *in, unsigned char la);
static json_t *parseObject (FILE *in);
static json_t *parseArray (FILE *in);
static char *parseString (FILE *in);
static char *parseNumber (FILE *in, unsigned char la);

json_t *
parseJson (FILE *in)
{
  // TODO BOM leading bytes?
  unsigned char la = eat_whitespace (in);
  return parseElement (in, la);
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
            buf = realloc (buf, pos + 1);
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
            }
          }
        }
    }
}

// -?(0|[1-9][0-9]*)(\.[0-9]+)?([eE][-+]?[0-9]+)?
static char *
parseNumber (FILE *in, unsigned char la)
{

}