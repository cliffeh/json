#include "json.h"

int yyparse (json_t **j);

int
main (int argc, char *argv[])
{
  json_t *j;
  int r = yyparse (&j);
  if (r == 0)
    {
      json_t_pprint (stdout, j);
      free_json_t(j);
    }
  return (r);
}
