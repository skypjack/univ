/****************************************************************************************

   Combinatorix
   Copyright (C)  2008  Michele Caini


   This file is part of Combinatorix.

   Combinatorix is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA


   To contact me:   skypjack@gmail.com

****************************************************************************************/

#ifndef COMMON_H
#define COMMON_H 1

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <sys/types.h>

#ifdef STDC_HEADERS
#  include <stdlib.h>
#  include <string.h>
#elif HAVE_STRINGS_H
#  include <strings.h>
#endif /* STDC_HEADERS */

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif /* HAVE_UNISTD_H */

#ifdef HAVE_STDARG_H
#  include <stdarg.h>
#endif /* HAVE_STDARG_H */

#ifdef HAVE_ERRNO_H
#  include <errno.h>
#endif /* HAVE_ERRNO_H */
#ifndef errno
/* Some systems #define this! */
extern int errno;
#endif


#ifndef EXIT_SUCCESS
/* Exit with success */
#  define EXIT_SUCCESS  0
/* Exit with failure */
#  define EXIT_FAILURE  1
#endif

#ifndef NULL
/* NULL */
#  define NULL 0
#endif

#if !HAVE_BZERO && HAVE_MEMSET
# define bzero(buf, bytes) ((void) memset (buf, 0, bytes))
#endif


// Flags (Environment Management)

/* Simply, some flags (only 8 bits) */
unsigned short int flags;

/* used to clear the flags (reset environment) */
#define CLEAR_FLAGS() \
  ( flags &= 0x00 )

/* sets runnable flag */
#define SET_RUNNABLE() \
  ( flags |= 0x01 )

/* gets runnable flag */
#define RUNNABLE() \
  ( flags & 0x01 )

/* sets verbose flag */
#define SET_VERBOSE() \
  ( flags |= 0x02 )

/* gets verbose flag */
#define VERBOSE(str) \
  if( flags & 0x02 ) fprintf(stderr,str)

/* sets info flag */
#define SET_INFO() \
  ( flags |= 0x04 )

/* gets info flag */
#define INFO() \
  ( flags & 0x04 )

/* sets help flag */
#define SET_HELP() \
  ( flags |= 0x08 )

/* gets help flag */
#define HELP() \
  ( flags & 0x08 )


// Memory management related functions

extern void*
xcalloc (size_t num, size_t size);

extern void*
xmalloc (size_t num);

extern void*
xrealloc (void *p, size_t num);

extern char*
xstrdup (const char* str);

/* xcalloc with automatic parameters management */
#define XALLOC(type, num) \
  ((type *) xcalloc ((num), sizeof(type)))
/* xmalloc with automatic parameters management */
#define XMALLOC(type, num) \
  ((type *) xmalloc ((num) * sizeof(type)))
/* xrealloc with automatic parameters management */
#define XREALLOC(type, p, num) \
  ((type *) xrealloc ((p), (num) * sizeof(type)))
/* xfree managed invocation */
#define XFREE(ref) \
  do { if (ref) { free (ref);  ref = 0; } } while (0)


// Error management related function

extern void
warning (const char*);

extern void
error (const char*);

extern void
fatal (const char*);

#endif /* COMMON_H */
