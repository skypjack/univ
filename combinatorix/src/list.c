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
#include "list.h"

list_t*
list_add_data (list_t* head, list_t* tail, void* data)
{
  list_t* list;
  list = list_add(head, tail);
  if(list)
    list->data = data;
  return list;
}

list_t*
list_add (list_t* head, list_t* tail)
{
  if(head)
    head->next = tail;
  return head ? head : tail;
}

list_t*
list_next (list_t* head)
{
  if(head)
    return head->next;
  else return NULL;
}

size_t
list_length (list_t* head)
{
  int cnt;
  for(cnt = 0; head; cnt++)
    head = head->next;
  return cnt;
}
