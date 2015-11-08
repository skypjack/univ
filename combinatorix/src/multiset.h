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

#ifndef MULTISET_H
#define MULTISET_H 1

#include "common.h"
#include "list.h"
#include "set.h"

struct multiset
{
  set_t set;
  size_t dorder;
  size_t step;
};

typedef
struct multiset
multiset_t;

typedef
multiset_t
mset_t;

struct item
{
  void* item;
  size_t size;
};

typedef
struct item
item_t;

#define init_multiset(set) \
  { init_set(((set_t*)set));			\
    set->dorder = 0; \
    set->step = 0; \
  }

#define multiset_cmp_function(set, func) \
  set_cmp_function(((set_t*)set), func)

#define multiset_cp_function(set, func) \
  set_cp_function(((set_t*)set), func)

#define multiset_order(set) \
  set_order(((set_t*)set))

#define multiset_sub_order(mset) \
  mset->dorder

extern void*
multiset_add_item (multiset_t*, void*);

extern void*
multiset_del_item (multiset_t*, void*);

extern void*
multiset_next_item (multiset_t*);

extern void*
multiset_next_next_item (multiset_t*);

extern void
multiset_for_each (multiset_t*, for_each_t);

extern multiset_t*
multiset_copy (multiset_t*);

extern size_t
multiset_search (multiset_t*, void*);

extern void
free_multiset (multiset_t*);

extern multiset_t*
multiset_merge (multiset_t*, multiset_t*);

extern multiset_t*
multiset_union (multiset_t*, multiset_t*);

extern multiset_t*
multiset_intersection (multiset_t*, multiset_t*);

extern multiset_t*
multiset_difference (multiset_t*, multiset_t*);

extern void
multiset_rearrange (multiset_t*);

#endif /* MULTISET_H */
