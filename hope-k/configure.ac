#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([Hope-K],[alpha],[skypjack@gmail.com, giuntinijohnny@interfree.it])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([src/hope-k.c])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h strings.h unistd.h sys/wait.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([bzero])

# AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([Makefile
		 src/Makefile])
AC_OUTPUT
