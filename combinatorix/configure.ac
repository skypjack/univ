AC_PREREQ(2.61)
AC_INIT([combinatorix],[0.1],[skypjack@gmail.com])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/combinatorix.c])
AM_INIT_AUTOMAKE

AC_PROG_CC

AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h strings.h unistd.h stdarg.h])

AC_C_CONST
AC_TYPE_SIZE_T

AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([bzero])

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
