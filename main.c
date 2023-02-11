#include "json.h"

int yyparse (json_t **j);

int
main (int argc, char *argv[])
{
  json_t *j = 0;
  int rc = 0;

  rc = yyparse (&j);
  if (rc == 0)
    {
      json_t_pprint (stdout, j);
      free_json_t(j);
    }
  return (rc);
}
