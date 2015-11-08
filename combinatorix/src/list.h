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

#ifndef LIST_H
#define LIST_H 1

#include "common.h"

#define list_conv(ptr) \
  (list_t*) ptr

#define list_rev_conv(type, ptr) \
  (type*) ptr

#define list_next_entry(type, head) \
  (type*) list_next ((list_t*) head)

#define list_get_data(type, head) \
  (type*) head->data

#define list_set_data(head, ptr) \
  head->data = (void*) ptr

struct list
{
  struct list* next;
  void* data;
};

typedef
struct list
list_t;

extern list_t*
list_add_data (list_t*, list_t*, void*);

extern list_t*
list_add (list_t*, list_t*);

extern list_t*
list_next (list_t*);

extern size_t
list_length (list_t*);

#endif /* LIST_H */
