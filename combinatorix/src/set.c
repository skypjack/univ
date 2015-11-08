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
#include "set.h"
#include "list.h"

int
stdcmp (void* fi, void* si)
{
  return (fi == si ? 0 : 1);
}

void*
set_add_item (set_t* set, void* item)
{
  void* ret;
  int cmp;
  list_t** liter;
  ret = NULL;
  if(set && set->cmp && item) {
    liter = &(set->items);
    while(*liter) {
      cmp = set->cmp(list_get_data(void, (*liter)), item);
      if(cmp >= 0)
	break;
      else
	liter = &((*liter)->next);
    }
    if(!*liter || cmp) {
      *liter = list_add_data(XMALLOC(list_t, 1), *liter, item);
      set->imark = NULL;
      set->order++;
      ret = item;
    }
  } else warning("null pointer");
  return ret;
}

void*
set_del_item (set_t* set, void* item)
{
  void* ret;
  list_t** liter;
  list_t* ltmp;
  ret = NULL;
  if(set && set->cmp && item) {
    liter = &(set->items);
    while(*liter) {
      if(!set->cmp(list_get_data(void, (*liter)), item))
	break;
      else
	liter = &((*liter)->next);
    }
    if(*liter) {
      ret = list_get_data(void, (*liter));
      ltmp = *liter;
      *liter = list_next(*liter);
      XFREE(ltmp);
      set->imark = NULL;
      set->order--;
    }
  } else warning("null pointer");
  return ret;
}

void*
set_next_item (set_t* set)
{
  void* ret;
  ret = NULL;
  if(set) {
    if(!set->imark)
      set->imark = set->items;
    if(set->imark) {
      ret = list_get_data(void, set->imark);
      set->imark = list_next(set->imark);
    }
  } else warning("null pointer");
  return ret;
}

void
set_for_each (set_t* set, for_each_t fef)
{
  list_t* liter;
  if(set && fef) {
    liter = set->items;
    while(liter) {
      fef(list_get_data(void, liter));
      liter = list_next(liter);
    }
  } else warning("null pointer");
}

set_t*
set_copy (set_t* set)
{
  set_t* scp;
  list_t* liter;
  scp = NULL;
  if(set && set->cmp && set->cp) {
    scp = XMALLOC(set_t, 1);
    init_set(scp);
    set_cmp_function(scp, set->cmp);
    set_cp_function(scp, set->cp);
    liter = set->items;
    while(liter) {
      set_add_item(scp, set->cp(list_get_data(void, liter)));
      liter = list_next(liter);
    }
  } else warning ("null pointer");
  return scp;
}

void*
set_search (set_t* set, void* item)
{
  void* ret;
  list_t* liter;
  ret = NULL;
  if(set && set->cmp && item) {
    liter = set->items;
    while(liter && set->cmp(list_get_data(void, liter), item))
      liter = list_next(liter);
    if(liter)
      ret = list_get_data(void, liter);
  } else warning ("null pointer");
  return ret;
}

void
free_set (set_t* set)
{
  list_t* liter;
  if(set) {
    while(set->items) {
      liter = set->items;
      set->items = list_next(set->items);
      XFREE(liter);
    }
    XFREE(set);
  } else warning("null pointer");
}

set_t*
set_union (set_t* sa, set_t* sb)
{
  void* item;
  size_t iter;
  set_t* set;
  set = NULL;
  if(sb && sa && (sa->cmp || sb->cmp) && (sa->cp || sb->cp)) {
    set = XMALLOC(set_t, 1);
    init_set(set);
    set_cmp_function(set, (sa->cmp ? sa->cmp : sb->cmp));
    set_cp_function(set, (sa->cp ? sa->cp : sb->cp));
    for(iter = 0; iter < set_order(sa); iter++) {
      item = set_next_item(sa);
      if(!set_search(set, item))
	set_add_item(set, set->cp(item));
    }
    for(iter = 0; iter < set_order(sb); iter++) {
      item = set_next_item(sb);
      if(!set_search(set, item))
	set_add_item(set, set->cp(item));
    }
  } else warning ("null pointer");
  return set;
}

set_t*
set_intersection (set_t* sa, set_t* sb)
{
  size_t iter, cnt;
  set_t* set;
  void* sae;
  void* sbe;
  set = sae = sbe = NULL;
  if(sb && sa && (sa->cmp || sb->cmp) && (sa->cp || sb->cp)) {
    set = XMALLOC(set_t, 1);
    init_set(set);
    set_cmp_function(set, sa->cmp ? sa->cmp : sb->cmp);
    set_cp_function(set, sa->cp ? sa->cp : sb->cp);
    for(iter = 0; iter < sa->order; iter++) {
      sae = set_next_item(sa);
      for(cnt = 0; cnt < sb->order; cnt++) {
	sbe = set_next_item(sb);
	if(!set->cmp(sae, sbe) && !set_search(set, sae))
	  set_add_item(set, set->cp(sae));
      }
    }
  } else warning ("null pointer");
  return set;
}

set_t*
set_difference (set_t* sa, set_t* sb)
{
  size_t iter;
  void* sae;
  set_t* set;
  set = NULL;
  if(sb && sa && (sa->cmp || sb->cmp)) {
    set = XMALLOC(set_t, 1);
    init_set(set);
    set_cmp_function(set, sa->cmp ? sa->cmp : sb->cmp;);
    set_cp_function(set, sa->cp ? sa->cp : sb->cp);
    for(iter = 0; iter < sa->order; iter++) {
      sae = set_next_item(sa);
      if(!set_search(sb, sae) && !set_search(set, sae))
	set_add_item(set, set->cp(sae));
    }
  } else warning ("null pointer");
  return set;
}

void
set_rearrange (set_t* set)
{
  list_t* liter;
  list_t* ltmp;
  cmp_t cmp;
  cp_t cp;
  if(set && set->cmp) {
    liter = set->items;
    cmp = set->cmp;
    cp = set->cp;
    init_set(set);
    set_cmp_function(set, cmp);
    set_cp_function(set, cp);
    while(liter) {
      ltmp = liter;
      liter = list_next(liter);
      set_add_item(set, list_get_data(void, ltmp));
      XFREE(ltmp);
    }
  } else warning ("null pointer");
}

void*
set_check (set_t* set)
{
  int cmp;
  list_t* lmark;
  list_t* liter;
  void* data;
  data = NULL;
  if(set && set->cmp) {
    cmp = 0;
    lmark = set->items;
    while(lmark && !cmp) {
      liter = list_next(lmark);
      while(liter && !cmp)
	if(!set->cmp(list_get_data(void, lmark), list_get_data(void, liter)))
	  cmp++;
	else liter = list_next(liter);
      lmark = list_next(lmark);
    }
    if(cmp)
      data = list_get_data(void, liter);
  } else warning ("null pointer");
  return data;
}

long long
set_part_size (int nv, int kv)
{
  long long value;
  int max, min, iter, cnt;
  int* part;
  max = (kv > (nv - kv) ? kv : (nv - kv));
  min = (kv < (nv - kv) ? kv : (nv - kv));
  part = XMALLOC(int, min);
  for(iter = 0; iter < min; iter++)
    part[iter] = min - iter;
  value = 1;
  for(cnt = nv; cnt > max; cnt--) {
    value *= cnt;
    for(iter = 0; iter < min; iter++)
      if(part[iter] && !(value % part[iter])) {
	value /= part[iter];
	part[iter] = 0;
      }
  }
  for(iter = 0; iter < min; iter++)
    if(part[iter])
      value /= part[iter];
  XFREE(part);
  return value;
}

set_t*
part_for_each (set_part_manager_t* spm, for_each_t fef)
{
  void* item;
  set_t* image;
  size_t iter;
  image = NULL;
  if(spm && spm->set && spm->set->cmp && spm->set->cp) {
    if(spm->porder) {
      image = XMALLOC(set_t, 1);
      init_set(image);
      set_cmp_function(image, spm->set->cmp);
      set_cp_function(image, spm->set->cp);
      for(iter = 0; iter < spm->porder; iter++) {
	item = spm->set->cp(list_get_data(void, spm->part[iter]));
	fef(item);
	if(!set_add_item(image, item))
	  XFREE(item);
      }
    }
  } else warning("null pointer");
  return image;
}

void
next_part (set_part_manager_t* spm)
{
  size_t iter;
  if(spm) {
    if(!spm->part) {
      if(set_order(spm->set))
	spm->part = XMALLOC(list_t*, set_order(spm->set));
      for(iter = 0; iter < set_order(spm->set); iter++)
	spm->part[iter] = NULL;
      spm->porder = 0;
    } else {
      if(!spm->porder)
	spm->part[spm->porder++] = spm->set->items;
      else {
	iter = spm->porder - 1;
	if(list_next(spm->part[iter]))
	  spm->part[iter] = list_next(spm->part[iter]);
	else if(spm->porder == set_order(spm->set)) {
	  for(iter = 0; iter < set_order(spm->set); iter++)
	    spm->part[iter] = NULL;
	  spm->porder = 0;
	} else {
	  for(; iter; iter--)
	    if(list_next(spm->part[iter-1]) != spm->part[iter])
	      break;
	  if(iter) {
	    spm->part[iter-1] = list_next(spm->part[iter-1]);
	    for(; spm->part[iter]; iter++)
	      spm->part[iter] = list_next(spm->part[iter-1]);
	  } else {
	    spm->porder++;
	    spm->part[(iter=0)] = spm->set->items;
	    for(++iter; iter < spm->porder; iter++)
	      spm->part[iter] = list_next(spm->part[iter-1]);
	  }
	}
      }
    }
  } else warning ("null pointer");
}

long long
part_count (set_part_manager_t* spm)
{
  size_t iter;
  long long ret;
  ret = 1;
  if(spm) {
    for(iter = 0; iter < set_order(spm->set); iter++)
      ret *=2;
  } else warning ("null pointer");
  return ret;
}

void
part_start (set_part_manager_t* spm, size_t dim)
{
  size_t iter;
  if(spm) {
    if(!spm->part) {
      if(set_order(spm->set))
	spm->part = XMALLOC(list_t*, set_order(spm->set));
      for(iter = 0; iter < set_order(spm->set); iter++)
	spm->part[iter] = NULL;
      spm->porder = 0;
    }
    spm->porder = dim;
    if(dim) {
      spm->part[(iter=0)] = spm->set->items;
      for(++iter; iter < spm->porder; iter++)
	spm->part[iter] = list_next(spm->part[iter-1]);
      for(; iter < set_order(spm->set); iter++)
	spm->part[iter] = NULL;
    } else {
      for(iter = 0; iter < set_order(spm->set); iter++)
	spm->part[iter] = NULL;
    }
  } else warning ("null pointer");
}

void*
part_next_item (set_part_manager_t* spm)
{
  void* ret;
  ret = NULL;
  if(spm) {
    if(spm->porder) {
      ret = list_get_data(void, spm->part[spm->pmark++]);
      if(spm->pmark == spm->porder)
	spm->pmark = 0;
    }
  } else warning ("null pointer");
  return ret;
}

void
free_part_manager (set_part_manager_t* spm)
{
  if(spm) {
    XFREE(spm->part);
    XFREE(spm);
  } else warning ("null pointer");
}
