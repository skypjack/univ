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

#include "common.h"

static void
error_handler (int exit_status, const char *mode, const char *message)
{
  fprintf (stderr, "combinatorix: %s: %s\n", mode, message);
  if (exit_status >= 0)
    exit(exit_status);
}

void
warning (const char *message)
{
  error_handler(-1, "Warning", message);
}

void
error (const char *message)
{
  error_handler(-1, "ERROR", message);
}

void
fatal (const char *message)
{
  error_handler(EXIT_FAILURE, "FATAL", message);
}
