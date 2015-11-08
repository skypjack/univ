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

#ifndef GRAPH_H
#define GRAPH_H 1

#include "common.h"

typedef
short int
node_t;

typedef
short int
color_t;

typedef
enum elem
elem_t;

struct edge;

typedef
struct edge
edge_t;

/*
 * Nodo di testa lista di adiacenza
 */
struct head_node
{
  struct head_node* next;
  edge_t* head;
  node_t nodeId;
  char* element;
  
  color_t color;
};

typedef
struct head_node
hnode_t;

/*
 * Arco del grafo, nodo di arrivo lista di adiacenza 
 */

struct edge
{
  struct edge* next;
  hnode_t* head;
  node_t nodeId;
  char* element;
  char* bond;
  
  color_t color;
};

/*
 * Grafo molecola
 */
struct graph
{
  int nnum;
  int ednum;
  hnode_t* head;
};

typedef
struct graph
graph_t;

/*
 * Inizializza grafo
 */
extern void
init_graph (graph_t*);

/*
 * Dealloca grafo
 */
extern void
free_graph (graph_t*);

/*
 * Aggiungi nodo di testa
 */
extern hnode_t*
add_node (graph_t*, node_t, char* const);

/*
 * Aggiungi nodo di coda / arco
 */
extern void
add_edge (graph_t*, node_t, node_t, char* const, char* const);

/*
 * Stampa grafo
 */
extern void
print_graph (graph_t* gp);

#endif /* GRAPH_H */
