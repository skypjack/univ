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
#include "multiset.h"

int
intcmp(void* i1, void* i2)
{
  int v1, v2;
  v1 = *((int*)i1);
  v2 = *((int*)i2);
  if(v1 < v2) return -1;
  else if(v1 > v2) return 1;
  else return 0;
  return 0;
}

void*
intcp(void* val)
{
  int* res;
  res = XMALLOC(int, 1);
  *res = *((int*)val);
  return ((void*)res);
}

void
power(void* val)
{
  *(int*)val = *(int*)val * *(int*)val;
}

void
same(void* val)
{
  *(int*)val = 1;
}

void
part_test()
{
  size_t iter, size, cnt;
  set_t *sa, *sb;
  set_part_manager_t* spm;
  int* iptr;
  printf("\n------------ PART [ TESTS ] ------------\n");
  printf("\n--> create/fill set");
  sa = XMALLOC(set_t, 1);
  init_set(sa);
  set_cmp_function(sa, intcmp);
  set_cp_function(sa, intcp);
  iptr = XMALLOC(int, 1);
  *iptr = 1;
  set_add_item(sa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 3;
  set_add_item(sa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 4;
  set_add_item(sa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 6;
  set_add_item(sa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 2;
  set_add_item(sa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 5;
  set_add_item(sa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 6;
  if(!set_add_item(sa, iptr))
    XFREE(iptr);
  printf("\nset = { ");
  for(iter = 0; iter < set_order(sa); iter++)
    printf("%d ", (*(int*)set_next_item(sa)));
  printf("}");
  printf("\n--> init part manager");
  spm = XMALLOC(set_part_manager_t, 1);
  init_part_manager(spm, sa);
  printf("\n--> part enum");
  size = part_count(spm);
  for(iter = 0; iter < size; iter++) {
    printf("\npart [%d]: { ", iter+1);
    next_part(spm);
    for(cnt = 0; cnt < part_order(spm); cnt++)
      printf("%d ", *(int*)(part_next_item(spm)));
    printf("}");
  }
  printf("\n--> for_each (power)");
  part_start(spm, 3);
  printf("\npart [%d]: { ", iter+1);
  for(cnt = 0; cnt < part_order(spm); cnt++)
    printf("%d ", *(int*)(part_next_item(spm)));
  printf("}");
  sb = part_for_each(spm, power);
  printf("\nset = { ");
  for(iter = 0; iter < set_order(sa); iter++)
    printf("%d ", (*(int*)set_next_item(sa)));
  printf("}");
  printf("\nset = { ");
  for(iter = 0; iter < set_order(sb); iter++)
    printf("%d ", (*(int*)set_next_item(sb)));
  printf("}");
  set_for_each(sb, free);
  free_set(sb);
  printf("\n--> free all");
  set_for_each(sa, free);
  free_set(sa);
  free_part_manager(spm);
  printf("\n------------ END ------------\n");
}

void
multiset_test()
{
  size_t iter;
  multiset_t *msa, *msb, *msc;
  int* iptr;
  void* ptr;
  printf("\n------------ MULTISET [ TESTS ] ------------\n");
  printf("\n--> create/fill set");
  msa = XMALLOC(multiset_t, 1);
  init_multiset(msa);
  multiset_cmp_function(msa, intcmp);
  multiset_cp_function(msa, intcp);
  iptr = XMALLOC(int, 1);
  *iptr = 1;
  multiset_add_item(msa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 1;
  if(multiset_add_item(msa, iptr) != iptr)
    XFREE(iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 3;
  if(multiset_add_item(msa, iptr) != iptr)
    XFREE(iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 3;
  if(multiset_add_item(msa, iptr) != iptr)
    XFREE(iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 2;
  if(multiset_add_item(msa, iptr) != iptr)
    XFREE(iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 3;
  if(multiset_add_item(msa, iptr) != iptr)
    XFREE(iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 2;
  if(multiset_add_item(msa, iptr) != iptr)
    XFREE(iptr);
  printf("\nmultiset = { ");
  for(iter = 0; iter < multiset_order(msa); iter++)
    printf("%d ", (*(int*)multiset_next_item(msa)));
  printf("}");
  printf("\nmultiset [group] = { ");
  for(iter = 0; iter < multiset_sub_order(msa); iter++)
    printf("%d ", (*(int*)multiset_next_next_item(msa)));
  printf("}");
  printf("\n--> delete item");
  iptr = XMALLOC(int, 1);
  *iptr = 3;
  ptr = multiset_del_item(msa, iptr);
  if(!multiset_search(msa, ptr))
    XFREE(ptr);
  XFREE(iptr);
  printf("\nmultiset = { ");
  for(iter = 0; iter < multiset_order(msa); iter++)
    printf("%d ", (*(int*)multiset_next_item(msa)));
  printf("}");
  printf("\n--> image (power)");
  msb = multiset_copy(msa);
  multiset_for_each(msb, power);
  multiset_rearrange(msb);
  printf("\nmultiset = { ");
  for(iter = 0; iter < multiset_order(msa); iter++)
    printf("%d ", (*(int*)multiset_next_item(msa)));
  printf("}");
  printf("\nmultiset = { ");
  for(iter = 0; iter < multiset_order(msb); iter++)
    printf("%d ", (*(int*)multiset_next_item(msb)));
  printf("}");
  printf("\n--> merge/union/intersection/difference");
  printf("\nmerge: multiset = { ");
  msc = multiset_merge(msa, msb);
  for(iter = 0; iter < multiset_order(msc); iter++)
    printf("%d ", (*(int*)multiset_next_item(msc)));
  printf("}");
  multiset_for_each(msc, free);
  free_multiset(msc);
  printf("\nunion: multiset = { ");
  msc = multiset_union(msa, msb);
  for(iter = 0; iter < multiset_order(msc); iter++)
    printf("%d ", (*(int*)multiset_next_item(msc)));
  printf("}");
  multiset_for_each(msc, free);
  free_multiset(msc);
  printf("\nintersection: multiset = { ");
  msc = multiset_intersection(msa, msb);
  for(iter = 0; iter < multiset_order(msc); iter++)
    printf("%d ", (*(int*)multiset_next_item(msc)));
  printf("}");
  multiset_for_each(msc, free);
  free_multiset(msc);
  printf("\ndifference: multiset = { ");
  msc = multiset_difference(msa, msb);
  for(iter = 0; iter < multiset_order(msc); iter++)
    printf("%d ", (*(int*)multiset_next_item(msc)));
  printf("}");
  multiset_for_each(msc, free);
  free_multiset(msc);
  printf("\n--> free all");
  multiset_for_each(msa, free);
  multiset_for_each(msb, free);
  free_multiset(msa);
  free_multiset(msb);
  printf("\n------------ END ------------\n");
}

void
set_test()
{
  size_t iter;
  set_t *sa, *sb, *sc;
  int* iptr;
  void* ptr;
  printf("\n------------ SET [ TESTS ] ------------\n");
  printf("\n--> part size");
  printf("\n(10, 2): %Ld", set_part_size(10, 2));
  printf("\n(1000, 5): %Ld", set_part_size(1000, 5));
  printf("\n(100000, 3): %Ld", set_part_size(100000, 3));
  printf("\n--> create/fill set");
  sa = XMALLOC(set_t, 1);
  init_set(sa);
  set_cmp_function(sa, intcmp);
  set_cp_function(sa, intcp);
  iptr = XMALLOC(int, 1);
  *iptr = 1;
  set_add_item(sa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 3;
  set_add_item(sa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 4;
  set_add_item(sa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 6;
  set_add_item(sa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 2;
  set_add_item(sa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 5;
  set_add_item(sa, iptr);
  iptr = XMALLOC(int, 1);
  *iptr = 6;
  if(!set_add_item(sa, iptr))
    XFREE(iptr);
  printf("\nset = { ");
  for(iter = 0; iter < set_order(sa); iter++)
    printf("%d ", (*(int*)set_next_item(sa)));
  printf("}");
  printf("\n--> delete item");
  iptr = XMALLOC(int, 1);
  *iptr = 3;
  ptr = set_del_item(sa, iptr);
  XFREE(iptr);
  XFREE(ptr);
  printf("\nset = { ");
  for(iter = 0; iter < set_order(sa); iter++)
    printf("%d ", (*(int*)set_next_item(sa)));
  printf("}");
  printf("\n--> image (power)");
  sb = set_copy(sa);
  set_for_each(sb, power);
  printf("\nset = { ");
  for(iter = 0; iter < set_order(sa); iter++)
    printf("%d ", (*(int*)set_next_item(sa)));
  printf("}");
  printf("\nset = { ");
  for(iter = 0; iter < set_order(sa); iter++)
    printf("%d ", (*(int*)set_next_item(sb)));
  printf("}");
  printf("\n--> union/intersection/difference");
  sc = set_union(sa, sb);
  printf("\nunion: set = { ");
  for(iter = 0; iter < set_order(sc); iter++)
    printf("%d ", (*(int*)set_next_item(sc)));
  printf("}");
  set_for_each(sc, free);
  free_set(sc);
  sc = set_intersection(sa, sb);
  printf("\nintersection: set = { ");
  for(iter = 0; iter < set_order(sc); iter++)
    printf("%d ", (*(int*)set_next_item(sc)));
  printf("}");
  set_for_each(sc, free);
  free_set(sc);
  sc = set_difference(sa, sb);
  printf("\ndifference: set = { ");
  for(iter = 0; iter < set_order(sc); iter++)
    printf("%d ", (*(int*)set_next_item(sc)));
  printf("}");
  set_for_each(sc, free);
  free_set(sc);
  printf("--> i eq j");
  set_for_each(sa, same);
  printf("\nset = { ");
  for(iter = 0; iter < set_order(sa); iter++)
    printf("%d ", (*(int*)set_next_item(sa)));
  printf("}");
  printf("\n--> check");
  while((iptr = set_check(sa))) {
    iptr = set_del_item(sa, iptr);
    XFREE(iptr);
  }
  set_rearrange(sa);
  printf("\nset = { ");
  for(iter = 0; iter < set_order(sa); iter++)
    printf("%d ", (*(int*)set_next_item(sa)));
  printf("}");
  printf("\n--> free all");
  set_for_each(sa, free);
  set_for_each(sb, free);
  free_set(sa);
  free_set(sb);
  printf("\n------------ END ------------\n");
}

int
main()
{
  set_test();
  multiset_test();
  part_test();
  return EXIT_SUCCESS;
}
