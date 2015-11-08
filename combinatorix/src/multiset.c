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
#include "multiset.h"
#include "list.h"

void*
multiset_add_item (multiset_t* mset, void* item)
{
  void* ret;
  int cmp;
  list_t** liter;
  set_t* set;
  ret = NULL;
  set = (set_t*)mset;
  if(set && set->cmp) {
    cmp = 1;
    liter = &(set->items);
    while(*liter) {
      cmp = set->cmp((list_get_data(item_t, (*liter)))->item, item);
      if(cmp >= 0)
	break;
      else
	liter = &((*liter)->next);
    }
    if(!cmp) {
      (list_get_data(item_t, (*liter)))->size++;
      ret = (list_get_data(item_t, (*liter)))->item;
    } else {
      *liter = list_add_data(XMALLOC(list_t, 1), *liter, XMALLOC(item_t, 1));
      (list_get_data(item_t, (*liter)))->item = item;
      (list_get_data(item_t, (*liter)))->size = 1;
      mset->dorder++;
      ret = item;
    }
    set->imark = NULL;
    set->order++;
    mset->step = 0;
  } else warning("null pointer");
  return ret;
}

void*
multiset_del_item (multiset_t* mset, void* item)
{
  void* ret;
  list_t** liter;
  list_t* ltmp;
  set_t* set;
  ret = NULL;
  set = (set_t*)mset;
  if(set && set->cmp) {
    liter = &(set->items);
    while(*liter) {
      if(!set->cmp((list_get_data(item_t, (*liter)))->item, item))
	break;
      else
	liter = &((*liter)->next);
    }
    if(*liter) {
      ret = (list_get_data(item_t, (*liter)))->item;
      if((list_get_data(item_t, (*liter)))->size > 1) {
	(list_get_data(item_t, (*liter)))->size--;
      } else {
	ltmp = *liter;
	*liter = list_next(*liter);
	XFREE(ltmp);
	mset->dorder--;
      }
      set->imark = NULL;
      set->order--;
      mset->step = 0;
    }
  } else warning("null pointer");
  return ret;
}

void*
multiset_next_item (multiset_t* mset)
{
  void* ret;
  item_t* item;
  set_t* set;
  ret = NULL;
  set = (set_t*)mset;
  if(set) {
    if(!set->imark) {
      set->imark = set->items;
      mset->step = 0;
    }
    if(set->imark) {
      item = list_get_data(item_t, set->imark);
      ret = item->item;
      if(++(mset->step) == item->size) {
	mset->step = 0;
	set->imark = list_next(set->imark);
      }
    }
  } else warning ("null pointer");
  return ret;
}

void*
multiset_next_next_item (multiset_t* mset)
{
  void* ret;
  set_t* set;
  item_t* item;
  ret = NULL;
  set = (set_t*)mset;
  if(set) {
    if(!set->imark) {
      set->imark = set->items;
      mset->step = 0;
    }
    if(set->imark) {
      item = list_get_data(item_t, set->imark);
      ret = item->item;
      mset->step = 0;
      set->imark = list_next(set->imark);
    }
  } else warning ("null pointer");
  return ret;
}

void
multiset_for_each(multiset_t* mset, for_each_t fef)
{
  list_t* liter;
  set_t* set;
  set = (set_t*)mset;
  if(set && fef) {
    liter = set->items;
    while(liter) {
      fef((list_get_data(item_t, liter))->item);
      liter = list_next(liter);
    }
  } else warning("null pointer");
}

multiset_t*
multiset_copy (multiset_t* mset)
{
  size_t iter;
  multiset_t* mscp;
  item_t* item;
  list_t* liter;
  set_t* set;
  void* obj;
  mscp = NULL;
  set = (set_t*)mset;
  if(set && set->cmp && set->cp) {
    mscp = XMALLOC(multiset_t, 1);
    init_multiset(mscp);
    multiset_cmp_function(mscp, set->cmp);
    multiset_cp_function(mscp, set->cp);
    liter = set->items;
    while(liter) {
      item = list_get_data(item_t, liter);
      obj = set->cp(item->item);
      for(iter = 0; iter < item->size; iter++)
	multiset_add_item(mscp, obj);
      liter = list_next(liter);
    }
  } else warning("null pointer");
  return mscp;
}

size_t
multiset_search (multiset_t* mset, void* item)
{
  size_t ret;
  list_t* liter;
  set_t* set;
  ret = 0;
  set = (set_t*)mset;
  if(set && set->cmp && item) {
    liter = set->items;
    while(liter && set->cmp((list_get_data(item_t, liter))->item, item))
      liter = list_next(liter);
    if(liter)
      ret = (list_get_data(item_t, liter))->size;
  } else warning ("null pointer");
  return ret;
}

void
free_multiset (multiset_t* mset)
{
  item_t* item;
  set_t* set;
  list_t* liter;
  set = (set_t*)mset;
  if(set) {
    while(set->items) {
      liter = set->items;
      set->items = list_next(set->items);
      item = list_get_data(item_t, liter);
      XFREE(item);
      XFREE(liter);
    }
    XFREE(set);
  } else warning("null pointer");
}

multiset_t*
multiset_merge (multiset_t* msa, multiset_t* msb)
{
  set_t* sa;
  set_t* sb;
  size_t size;
  void* item;
  size_t iter, cnt;
  multiset_t* mset;
  mset = NULL;
  sa = (set_t*)msa;
  sb = (set_t*)msb;
  if(sb && sa && (sa->cmp || sb->cmp) && (sa->cp || sb->cp)) {
    mset = XMALLOC(multiset_t, 1);
    init_multiset(mset);
    multiset_cmp_function(mset, (sa->cmp ? sa->cmp : sb->cmp));
    multiset_cp_function(mset, (sa->cp ? sa->cp : sb->cp));
    for(iter = 0; iter < multiset_sub_order(msa); iter++) {
      item = multiset_next_next_item(msa);
      size = multiset_search(msa, item);
      multiset_add_item(mset, ((set_t*)mset)->cp(item));
      for(cnt = 0; cnt < (size - 1); cnt++)
	multiset_add_item(mset, item);
    }
    for(iter = 0; iter < multiset_sub_order(msb); iter++) {
      item = multiset_next_next_item(msb);
      size = multiset_search(msb, item);
      if(multiset_search(mset, item)) {
	for(cnt = 0; cnt < size; cnt++)
	  multiset_add_item(mset, item);
      } else {
	multiset_add_item(mset, ((set_t*)mset)->cp(item));
	for(cnt = 0; cnt < (size - 1); cnt++)
	  multiset_add_item(mset, item);
      }
    }
  } else warning ("null pointer");
  return mset;
}

extern multiset_t*
multiset_union (multiset_t* msa, multiset_t* msb)
{
  set_t* sa;
  set_t* sb;
  size_t st, size, iter, cnt;
  multiset_t* mset;
  void* item;
  mset = NULL;
  sa = (set_t*)msa;
  sb = (set_t*)msb;
  if(sb && sa && (sa->cmp || sb->cmp) && (sa->cp || sb->cp)) {
    mset = XMALLOC(multiset_t, 1);
    init_multiset(mset);
    multiset_cmp_function(mset, (sa->cmp ? sa->cmp : sb->cmp));
    multiset_cp_function(mset, (sa->cp ? sa->cp : sb->cp));
    for(iter = 0; iter < multiset_sub_order(msa); iter++) {
      item = multiset_next_next_item(msa);
      size = multiset_search(msa, item);
      multiset_add_item(mset, ((set_t*)mset)->cp(item));
      for(cnt = 0; cnt < (size - 1); cnt++)
	multiset_add_item(mset, item);
    }
    for(iter = 0; iter < multiset_sub_order(msb); iter++) {
      item = multiset_next_next_item(msb);
      size = multiset_search(msb, item);
      st = multiset_search(mset, item);
      if(!st) {
	multiset_add_item(mset, ((set_t*)mset)->cp(item));
	st++;
      }
      for(; st < size; st++)
	multiset_add_item(mset, item);
    }
  } else warning ("null pointer");
  return mset;
}

extern multiset_t*
multiset_intersection (multiset_t* msa, multiset_t* msb)
{
  set_t* sa;
  set_t* sb;
  size_t sac, sbc;
  size_t iter, cnt;
  multiset_t* mset;
  void* item;
  mset = NULL;
  sa = (set_t*)msa;
  sb = (set_t*)msb;
  if(sb && sa && (sa->cmp || sb->cmp) && (sa->cp || sb->cp)) {
    mset = XMALLOC(multiset_t, 1);
    init_multiset(mset);
    multiset_cmp_function(mset, (sa->cmp ? sa->cmp : sb->cmp));
    multiset_cp_function(mset, (sa->cp ? sa->cp : sb->cp));
    for(iter = 0; iter < multiset_sub_order(msa); iter++) {
      item = multiset_next_next_item(msa);
      if((sbc = multiset_search(msb, item))) {
	sac = multiset_search(msa, item);
	multiset_add_item(mset, ((set_t*)mset)->cp(item));
	sac = ((sac < sbc) ? sac : sbc);
	for(cnt = 0; cnt < (sac - 1); cnt++)
	  multiset_add_item(mset, item);
      }
    }
  } else warning ("null pointer");
  return mset;
}

multiset_t*
multiset_difference (multiset_t* msa, multiset_t* msb)
{
  set_t* sa;
  set_t* sb;
  size_t iter, size, cnt;
  multiset_t* mset;
  void* item;
  mset = NULL;
  sa = (set_t*)msa;
  sb = (set_t*)msb;
  if(sb && sa && (sa->cmp || sb->cmp) && (sa->cp || sb->cp)) {
    mset = XMALLOC(multiset_t, 1);
    init_multiset(mset);
    multiset_cmp_function(mset, (sa->cmp ? sa->cmp : sb->cmp));
    multiset_cp_function(mset, (sa->cp ? sa->cp : sb->cp));
    for(iter = 0; iter < multiset_sub_order(msa); iter++) {
      item = multiset_next_next_item(msa);
      size = multiset_search(msa, item);
      size -= multiset_search(msb, item);
      if(size-- > 0) {
	multiset_add_item(mset, ((set_t*)mset)->cp(item));
	for(cnt = 0; cnt < size; cnt++)
	  multiset_add_item(mset, item);
      }
    }
  } else warning ("null pointer");
  return mset;
}

void
multiset_rearrange (multiset_t* mset)
{
  size_t iter;
  list_t* liter;
  list_t* ltmp;
  cmp_t cmp;
  cp_t cp;
  set_t* set;
  item_t* item;
  set = (set_t*)mset;
  if(set && set->cmp) {
    liter = set->items;
    cmp = set->cmp;
    cp = set->cp;
    init_multiset(mset);
    multiset_cmp_function(mset, cmp);
    multiset_cp_function(mset, cp);
    while(liter) {
      ltmp = liter;
      liter = list_next(liter);
      item = list_get_data(item_t, ltmp);
      for(iter = 0; iter < item->size; iter++)
	multiset_add_item(mset, item->item);
      XFREE(item);
      XFREE(ltmp);
    }
  } else warning ("null pointer");
}
