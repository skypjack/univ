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

#ifndef HASH_H
#define HASH_H 1

/*
 * Struttura singolo nodo per hashing.
 */
struct hash_node
{
	unsigned long int key;
	unsigned int data[9];
	struct hash_node* next;
};

/*
 * Struttura hash per l'indicizzazione
 * e la ricerca degli elementi.
 */
struct hash
{
	struct hash_node* bucket[1000];
	struct hash_node* unused;
	unsigned int entries;
};

typedef struct hash_node hnode_t;
typedef struct hash hash_t;

/*
 * Funzione per la creazione di una
 * struttura hash vuota.
 */
hash_t*
createhash();

/*
 * Funzione per l'eliminazione di
 * una struttura hash.
 */
void
freehash(hash_t*);

/*
 * Svuota la struttura hash.
 */
void
emptyhash(hash_t*);

/*
 * Funzione per l'inserimento di un
 * elemento nella struttura hash.
 */
unsigned long int
insert(hash_t*, unsigned int*);

/*
 * Funzione di ricerca di un elemento
 * nella struttura hash.
 */
unsigned long int
search(hash_t*, unsigned int*);

/*
 * Funzione per il recupero dei dati.
 */
unsigned short int
retrieve(hash_t*, unsigned long int, unsigned int*);

#endif /* HASH_H */
