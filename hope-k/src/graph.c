/****************************************************************************************

   Hope-K
   Copyright (C)  2008  Michele Caini, Johnny Giuntini


   This file is part of Hope-K.

   Hope-K is free software; you can redistribute it and/or modify
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


   To contact me:   skypjack@gmail.com, giuntinijohnny@interfree.it

****************************************************************************************/

#include "common.h"
#include "graph.h"
#include "list.h"

void
init_graph (graph_t* gp)
{
  if(gp) {
    gp->nnum = 0;
    gp->ednum = 0;
    gp->head = NULL;
  } else warning("Null pointer!");
}

void
free_graph (graph_t* gp)
{
  hnode_t* htmp;
  edge_t* etmp;
  if(gp) {
    while(gp->head) {
      htmp = gp->head;
      gp->head = list_next_entry(hnode_t, gp->head);
      while(htmp->head) {
	etmp = htmp->head;
	htmp->head = list_next_entry(edge_t, htmp->head);
	XFREE(etmp->element);
	XFREE(etmp->bond);
	XFREE(etmp);
      }
      XFREE(htmp->element);
      XFREE(htmp);
    }
    XFREE(gp);
  } else warning("Null pointer!");
}

hnode_t*
add_node (graph_t* gp, node_t node, char* const id)
{
  gp->head = list_rev_conv(hnode_t, list_add(list_conv(XMALLOC(hnode_t, 1)), list_conv((gp->head))));
  gp->head->head = NULL;
  gp->head->nodeId = node;
  gp->head->element = id;
  gp->nnum++;
  return gp->head;
}

void
add_edge (graph_t* gp, node_t from, node_t to, char* const id, char* const et)
{
  hnode_t** hiter;
  hiter = &(gp->head);
  while(*hiter && (*hiter)->nodeId != from)
    hiter = &((*hiter)->next);
  if(*hiter) {
    (*hiter)->head = list_rev_conv(edge_t, list_add(list_conv(XMALLOC(edge_t, 1)), list_conv((*hiter)->head)));
    (*hiter)->head->head = *hiter;
    (*hiter)->head->nodeId = to;
    (*hiter)->head->element = id;
    (*hiter)->head->bond = et;
    gp->ednum++;
  } else warning("Node not found");
}

void
print_graph (graph_t* gp)
{
  hnode_t* hiter;
  edge_t* eiter;
  printf("\nnnum: %d, ednum: %d", gp->nnum, gp->ednum);
  hiter = gp->head;
  while(hiter) {
    printf("\n%d (%s)", hiter->nodeId, hiter->element);
    eiter = hiter->head;
    while(eiter) {
      printf(" => %d (%s:%s)", eiter->nodeId, eiter->bond, eiter->element);
      eiter = list_next_entry(edge_t, eiter);
    }
    printf("\n");
    hiter = list_next_entry(hnode_t, hiter);
  }
}
