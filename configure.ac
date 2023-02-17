AC_INIT([json], m4_esyscmd_s([cat VERSION]), [cliff.snyder@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
LT_INIT
AC_PROG_CC
AC_PROG_YACC
# TODO clean up warning: AC_PROG_LEX without either yywrap or noyywrap is obsolete
AC_PROG_LEX

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_OUTPUT
