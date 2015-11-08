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

#ifndef DQUEUE_H
#define DQUEUE_H 1

/*
 * Struttura base per la memorizzazione di
 * una singola immagine del puzzle.
 */
struct dqnode
{
	unsigned short int lvl;
	unsigned int item[9];
	struct dqnode* pre;
	struct dqnode* post;
};

/*
 * Struttura "amministrativa" per la gestione
 * della coda doppia.
 */
struct dqueue
{
	struct dqnode* head;
	struct dqnode* tail;
	struct dqnode* unused;
};

typedef struct dqnode dqnode_t;
typedef struct dqueue dqueue_t;

/*
 * Funzione di creazione per
 * una singola coda doppia.
 */
dqueue_t*
createdqueue();

/*
 * Funzione di inserimento in coda.
 */
unsigned short int
pushrear(dqueue_t*, unsigned int*, unsigned short int);

/*
 * Funzione di prelievo in coda.
 */
unsigned short int
poprear(dqueue_t*, unsigned int*);

/*
 * Funzione di inserimento in testa.
 */
unsigned short int
push(dqueue_t*, unsigned int*, unsigned short int);

/*
 * Funzione di prelievo in testa.
 */
unsigned short int
pop(dqueue_t*, unsigned int*);

/*
 * Funzione per la cancellazione della
 * singola coda doppia.
 */
void
freedqueue(dqueue_t*);

/*
 * Svuota la struttura coda doppia.
 */
void
emptydqueue(dqueue_t*);

#endif /* DQUEUE_H */
