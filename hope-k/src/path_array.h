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

#ifndef PATH_ARRAY_H_
#define PATH_ARRAY_H_

#include "common.h"
#include "string.h"

#define WITH_MULT 1
#define NO_MULT 0

/*
 * Percorso in grafo molecola, con molteplicità
 */
struct Path
{
  char* path;
  int mult;
};

typedef
struct Path
Path_t;

/*
 * Array percorsi del grafo
 */
struct pathArray
{
  Path_t* array;
  size_t dim;
  size_t free;
};

typedef
struct pathArray
pathArray_t;

/*
 * Inizializzazione array
 */
extern void
initPathArray(pathArray_t* pathArray, int init_size);

/*
 * Aggiunta percorso all'array
 */
extern int
addPathToArray(pathArray_t* pathArray, char* path, unsigned int with_mult);

/*
 * Stampa array
 */
extern void
displayPathArray(pathArray_t* pathArray);

/*
 * Calcolo percorsi comuni fra due array
 */
extern int
commonPaths(pathArray_t* pA1, pathArray_t* pA2);

/*
 * Calcolo rapporto min/max numero percorsi in comune con molteplicità  
 */
extern float
commonPathsWithMult(pathArray_t* pA1, pathArray_t* pA2);

/*
 * Deallocazione array
 */
extern void
freePathArray(pathArray_t* pathArray);

#endif /*PATH_ARRAY_H_*/
