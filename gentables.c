#include <stdio.h>

#define STRING_REJECT -1
#define STRING_INITIAL 0
#define STRING_ESCAPE_HEX1 1
#define STRING_ESCAPE_HEX2 2
#define STRING_ESCAPE_HEX3 3
#define STRING_ESCAPE_HEX4 4
#define STRING_ESCAPE 5
#define STRING_ACCEPT 6

int JSON_STRING_PARSE_TABLE[STRING_ACCEPT][256];

int
main ()
{
  int i, j;

  for (i = 0; i < STRING_ACCEPT; i++)
    {
      for (j = 0; j < 256; j++)
        {
          // reject by default
          JSON_STRING_PARSE_TABLE[i][j] = STRING_REJECT;
        }
    }

  for (j = '\x20'; j < 256; j++)
    {
      // accept every byte above \x20
      JSON_STRING_PARSE_TABLE[STRING_INITIAL][j] = STRING_INITIAL;
    }
  // special cases
  JSON_STRING_PARSE_TABLE[STRING_INITIAL]['"'] = STRING_ACCEPT;
  JSON_STRING_PARSE_TABLE[STRING_INITIAL]['\\'] = STRING_ESCAPE;

  // allowable escape sequences
  JSON_STRING_PARSE_TABLE[STRING_ESCAPE]['"'] = STRING_INITIAL;
  JSON_STRING_PARSE_TABLE[STRING_ESCAPE]['/'] = STRING_INITIAL;
  JSON_STRING_PARSE_TABLE[STRING_ESCAPE]['b'] = STRING_INITIAL;
  JSON_STRING_PARSE_TABLE[STRING_ESCAPE]['f'] = STRING_INITIAL;
  JSON_STRING_PARSE_TABLE[STRING_ESCAPE]['n'] = STRING_INITIAL;
  JSON_STRING_PARSE_TABLE[STRING_ESCAPE]['r'] = STRING_INITIAL;
  JSON_STRING_PARSE_TABLE[STRING_ESCAPE]['t'] = STRING_INITIAL;
  JSON_STRING_PARSE_TABLE[STRING_ESCAPE]['\\'] = STRING_INITIAL;

  JSON_STRING_PARSE_TABLE[STRING_ESCAPE]['u'] = STRING_ESCAPE_HEX1;

  // hex char #1
  for (j = '0'; j <= '9'; j++)
    {
      JSON_STRING_PARSE_TABLE[STRING_ESCAPE_HEX1][j] = STRING_ESCAPE_HEX2;
    }
  for (j = 'a'; j <= 'f'; j++)
    {
      JSON_STRING_PARSE_TABLE[STRING_ESCAPE_HEX1][j] = STRING_ESCAPE_HEX2;
    }
  for (j = 'A'; j <= 'F'; j++)
    {
      JSON_STRING_PARSE_TABLE[STRING_ESCAPE_HEX1][j] = STRING_ESCAPE_HEX2;
    }

  // hex char #2
  for (j = '0'; j <= '9'; j++)
    {
      JSON_STRING_PARSE_TABLE[STRING_ESCAPE_HEX2][j] = STRING_ESCAPE_HEX3;
    }
  for (j = 'a'; j <= 'f'; j++)
    {
      JSON_STRING_PARSE_TABLE[STRING_ESCAPE_HEX2][j] = STRING_ESCAPE_HEX3;
    }
  for (j = 'A'; j <= 'F'; j++)
    {
      JSON_STRING_PARSE_TABLE[STRING_ESCAPE_HEX2][j] = STRING_ESCAPE_HEX3;
    }

  // hex char #3
  for (j = '0'; j <= '9'; j++)
    {
      JSON_STRING_PARSE_TABLE[STRING_ESCAPE_HEX3][j] = STRING_ESCAPE_HEX4;
    }
  for (j = 'a'; j <= 'f'; j++)
    {
      JSON_STRING_PARSE_TABLE[STRING_ESCAPE_HEX3][j] = STRING_ESCAPE_HEX4;
    }
  for (j = 'A'; j <= 'F'; j++)
    {
      JSON_STRING_PARSE_TABLE[STRING_ESCAPE_HEX3][j] = STRING_ESCAPE_HEX4;
    }

  // hex char #4
  for (j = '0'; j <= '9'; j++)
    {
      JSON_STRING_PARSE_TABLE[STRING_ESCAPE_HEX4][j] = STRING_INITIAL;
    }
  for (j = 'a'; j <= 'f'; j++)
    {
      JSON_STRING_PARSE_TABLE[STRING_ESCAPE_HEX4][j] = STRING_INITIAL;
    }
  for (j = 'A'; j <= 'F'; j++)
    {
      JSON_STRING_PARSE_TABLE[STRING_ESCAPE_HEX4][j] = STRING_INITIAL;
    }

  printf ("int JSON_STRING_PARSE_TABLE[%i][256] = \n{\n", STRING_ACCEPT);

  for (i = STRING_INITIAL; i < STRING_ACCEPT - 1; i++)
    {

      printf ("  {");
      for (j = 0; j < 255; j++)
        {
          if (j % 16 == 0)
            printf ("\n    ");
          printf ("%i, ", JSON_STRING_PARSE_TABLE[i][j]);
        }

      printf ("%i\n  },\n", JSON_STRING_PARSE_TABLE[i][j]);

      // for (int i = 1; i < STRING_ACCEPT; i++)
      //   {
      //     printf (",\n{") for (j = 0; j < 256; j++) {}
      //     printf ("}\n");
      //   }
    }

  printf ("  {");
  for (j = 0; j < 255; j++)
    {
      if (j % 16 == 0)
        printf ("\n    ");
      printf ("%i, ", JSON_STRING_PARSE_TABLE[STRING_ACCEPT-1][j]);
    }
  printf ("%i\n  }", JSON_STRING_PARSE_TABLE[STRING_ACCEPT-1][j]);
  printf ("\n};");
}