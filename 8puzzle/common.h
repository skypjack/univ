/****************************************************************************************

   8puzzle
   Copyright (C)  2006  Michele Caini, Johnny Giuntini


   This file is part of 8puzzle.

   8puzzle is free software; you can redistribute it and/or modify
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

****************************************************************************************/

#ifndef COMMON_H
#define COMMON_H 1

#include <stdio.h>

#ifdef STDC_HEADERS
#  include <stdlib.h>
#  include <ctype.h>
#else
#  ifdef HAVE_STDLIB_H
#    include <stdlib.h>
#  endif
#endif
#ifdef HAVE_STRING_H
#  include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#ifdef HAVE_TIME_H
#  include <time.h>
#endif

unsigned short int flags;

#define INIT_FLAGS() \
  (flags = 0x00)

#define SET_GEN() \
  (flags |= 0x10)

#define GEN_REQ() \
  (flags & 0x10)

#define SET_VERBOSE() \
  (flags |= 0x20)

#define VERBOSE() \
  (flags & 0x20)

#define SET_BF() \
  (flags |= 0x01)

#define BF() \
  (flags & 0x01)

#define SET_DF() \
  (flags |= 0x02)

#define DF() \
  (flags & 0x02)

#define SET_A_STAR() \
  (flags |= 0x04)

#define A_STAR() \
  (flags & 0x04)

#define SET_IDA_STAR() \
  (flags |=0x08)

#define IDA_STAR() \
  (flags & 0x08)

#define SET_HELP() \
  (flags |= 0x40)

#define HELP_REQ() \
  (flags & 0x40)

#endif
