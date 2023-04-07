#include "json.h"
#include "popt/popt.h"

#define YYSTYPE JSONSTYPE

#include "parser.h"
#include "scanner.h"

int
main (int argc, const char *argv[])
{
  json_t *j = 0;
  json_t_print_options print_options
      = { .out = DEFAULT_JSON_T_PRINT_OUTFILE,
          .indent = DEFAULT_JSON_T_PRINT_INDENT };
  yyscan_t scanner;
  int rc = 0;

  poptContext optCon;

  struct poptOption options[]
      = { /* longName, shortName, argInfo, arg, val, descrip, argDescript */
          { "indent", 'n',
            POPT_ARG_INT | POPT_ARGFLAG_OPTIONAL | POPT_ARGFLAG_SHOW_DEFAULT,
            &print_options.indent, 0,
            "indent N spaces/tabs; 0 implies compact formatting", "N" },
          { "use-tabs", 't', POPT_ARG_NONE, &print_options.use_tabs, 0,
            "use tabs for indentation", 0 },
          POPT_AUTOHELP POPT_TABLEEND
        };

  optCon = poptGetContext (0, argc, argv, options, 0);
  while ((rc = poptGetNextOpt (optCon)) > 0)
    ;

  if (print_options.indent < 0)
    {
      fprintf (stderr, "warning: negative indent (%i); setting to 0\n",
               print_options.indent);
      print_options.indent = 0;
    }

  if ((rc = jsonlex_init (&scanner)) != 0)
    exit (rc);

  rc = jsonparse (&j, scanner);
  if (rc == 0)
    {
      json_t_print (j, &print_options);
      free_json_t (j);
    }

  jsonlex_destroy (scanner);
  poptFreeContext (optCon);
  return (rc);
}
