AC_INIT([json], m4_esyscmd_s([cat VERSION]), [cliff.snyder@gmail.com])
# TODO make this optional?
AC_CHECK_HEADERS(popt.h, [],
  [
    AC_MSG_ERROR([one or more required headers not found])
  ])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
LT_INIT
AC_PROG_CC
AC_PROG_YACC
AC_PROG_LEX(noyywrap)

# TODO make this optional?
AC_SEARCH_LIBS(poptGetContext, popt, [], [
  AC_MSG_ERROR([unable to find libpopt])
])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_OUTPUT
