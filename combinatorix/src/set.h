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

#ifndef SET_H
#define SET_H 1

#include "common.h"
#include "list.h"

// <0 iff first < second (or !=)
// =0 iff first == second
// >0 iff first > second
typedef
int (*cmp_t) (void*, void*);

extern int
stdcmp (void*, void*);

typedef
void (*for_each_t) (void*);

typedef
void* (*cp_t) (void*);

struct set
{
  size_t order;
  list_t* items;
  list_t* imark;
  cmp_t cmp;
  cp_t cp;
};

typedef
struct set
set_t;

#define init_set(set) \
  { set->order = 0; \
    set->cmp = stdcmp; \
    set->items = NULL; \
    set->imark = NULL; \
    set->cp = NULL; \
  }

#define set_cmp_function(set, func) \
  set->cmp = func

#define set_cp_function(set, func) \
  set->cp = func

#define set_order(set) \
  set->order

extern void*
set_add_item (set_t*, void*);

extern void*
set_del_item (set_t*, void*);

extern void*
set_next_item (set_t*);

extern void
set_for_each (set_t*, for_each_t);

extern set_t*
set_copy (set_t*);

extern void*
set_search (set_t*, void*);

extern void
free_set (set_t*);

extern set_t*
set_union (set_t*, set_t*);

extern set_t*
set_intersection (set_t*, set_t*);

extern set_t*
set_difference (set_t*, set_t*);

extern void
set_rearrange (set_t*);

extern void*
set_check (set_t*);

extern long long
set_part_size (int, int);

struct set_part_manager
{
  set_t* set;
  size_t porder;
  list_t** part;
  size_t pmark;
};

typedef
struct set_part_manager
set_part_manager_t;

#define init_part_manager(spm,spt) \
  { spm->set = spt; \
    spm->porder = 0; \
    spm->part = NULL; \
    spm->pmark = 0; \
  }

#define part_order(spm) \
  spm->porder

extern set_t*
part_for_each (set_part_manager_t*, for_each_t);

extern void
next_part (set_part_manager_t*);

extern long long
part_count (set_part_manager_t*);

extern void
part_start (set_part_manager_t*, size_t);

extern void*
part_next_item (set_part_manager_t*);

extern void
free_part_manager (set_part_manager_t*);

#endif /* SET_H */
