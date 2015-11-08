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

#ifndef GRAPH_FUNCTION_H
#define GRAPH_FUNCTION_H 1

#include "common.h"
#include "path_array.h"
#include "graph.h"
#include "list.h"

#define WHITE 0
#define GRAY 1
#define BLACK 2

/*
 * Ricerca in profondità su grafo
 */
extern void
DFS_Search (graph_t* gp, hnode_t* node, int maxDepth, pathArray_t* pathArray, unsigned int with_mult);

/*
 * Routine ricorsiva per DFS su grafo
 */
extern void
DFS_Visit (graph_t* gp, hnode_t* node, int depth, int maxDepth, char* path, pathArray_t* pathArray, unsigned int with_mult);

/*
 * Recupera nodo dato l'id
 */ 
extern hnode_t*
getHeadOfNode (graph_t* gp, node_t id);

/*
 * Marca il nodo con 'colore'
 */
extern void
putColorOnNode (graph_t* gp, node_t id, color_t newColor);

/*
 * Inizializzazione marcatura nodi con 'colore'
 */
extern void
putColorOnAllNodes (graph_t* gp, color_t newColor);


#endif /*GRAPH_FUNCTION_H*/
