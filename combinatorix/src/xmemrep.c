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

void*
xmalloc (size_t num)
{
  void* new = malloc(num);
  if (!new) fatal("memory exhausted");
  return new;
}

void*
xrealloc (void *ref, size_t num)
{
  void* new;
  if (!ref) return xmalloc(num);
  new = realloc (ref, num);
  if (!new) fatal("memory exhausted");
  return new;
}

void*
xcalloc (size_t num, size_t size)
{
  void* new = xmalloc(num * size);
  bzero(new, num * size);
  return new;
}

char*
xstrdup (const char* str)
{
  char* new = (char*) xmalloc(strlen(str) + 1);
  strcpy(new, str);
  return new;
}
