#include "json.h"

int yyparse (json_t **j);

int
main (int argc, const char *argv[])
{
  json_t *j = 0;
  int rc = 0;

  int indent = (JSON_T_PRINT_DEFAULT_INDENT >> 26);

  printf("using indent: %i\n", indent);

  rc = yyparse (&j);
  if (rc == 0)
    {
      json_t_print (stdout, j, 0, 0);
      free_json_t(j);
    }
  return (rc);
}
