/****************************************************************************************

   8puzzle
   Copyright (C)  2006  Michele Caini, Johnny Giuntini


   This file is part of 8puzzle.

   8puzzle is free software; you can redistribute it and/or modify
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

****************************************************************************************/

#ifndef MAP_H
#define MAP_H 1

/*
 * Struttura singolo nodo per la mappatura
 * delle relazioni padre-figlio, in forma
 * chiave-valore.
 */
struct map_node
{
	unsigned long int parent;
	unsigned long int key;
	struct map_node* next;
};

/*
 * Struttura per la mappatura e la
 * conseguente ricerca degli elementi.
 */
struct map
{
	struct map_node* bucket[1000];
	struct map_node* unused;
	unsigned int entries;
};

typedef struct map_node mnode_t;
typedef struct map map_t;

/*
 * Funzione per la creazione di una
 * struttura map vuota.
 */
map_t*
createmap();

/*
 * Funzione per l'eliminazione di
 * una struttura map.
 */
void
freemap(map_t*);

/*
 * Svuota la struttura map.
 */
void
emptymap(map_t*);

/*
 * Funzione per l'inserimento di una coppia
 * chiave-valore nella struttura map.
 */
unsigned short int
add(map_t*, unsigned long int, unsigned long int);

/*
 * Funzione per il recupero dei dati, ovvero
 * del valore associato ad una data chiave.
 */
unsigned long int
find(map_t*, unsigned long int);

#endif /* MAP_H */
