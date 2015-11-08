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

#ifndef HEAP_H
#define HEAP_H 1

/*
 * Struttura base per la memorizzazione di
 * una singola immagine del puzzle.
 */
struct hpnode
{
	double weight;
	unsigned short int lvl;
	unsigned int item[9];
	struct hpnode* sup;
	struct hpnode* inf;
	struct hpnode* par;
};

/*
 * Struttura "amministrativa" per la gestione
 * dell'heap.
 */
struct heap
{
	struct hpnode* root;
	struct hpnode* unused;
};

typedef struct hpnode hpnode_t;
typedef struct heap heap_t;

/*
 * Funzione di creazione per
 * un singolo heap.
 */
heap_t*
createheap();

/*
 * Funzione per l'inserimento
 * di elementi nell'heap.
 */
unsigned short int
put(heap_t*, unsigned int*, unsigned short int, double);

/*
 * Funzione di prelievo
 * di elementi dall'heap.
 */
unsigned short int
get(heap_t*, unsigned int*);

/*
 * Funzione di cambiamento condizionale
 * del peso di un elemento nell'heap, solo
 * se il nuovo peso è minore.
 */
unsigned short int
setif(heap_t*, unsigned int*, unsigned short int, double);

/*
 * Funzione per la cancellazione del
 * singolo heap.
 */
void
freeheap(heap_t*);

/*
 * Svuota la struttura heap.
 */
void
emptyheap(heap_t*);

#endif /* HEAP_H */
