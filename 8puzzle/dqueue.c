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

#include "common.h"
#include "dqueue.h"

/*
 * Funzione di creazione per
 * una singola coda doppia.
 */
dqueue_t*
createdqueue()
{
	dqueue_t* dqp;
	if((dqp = (dqueue_t*)malloc(sizeof(dqueue_t))) != 0) {
		dqp->head = 0;
		dqp->tail = 0;
		dqp->unused = 0;
	}
	return dqp;
}

/*
 * Funzione di generazione / recupero di un
 * elemento per l'inserimento.
 */
dqnode_t*
getitem(dqueue_t* dqp)
{
	dqnode_t* dqn;
	if(dqp->unused != 0) {
		dqn = dqp->unused;
		dqp->unused = dqn->post;
	} else dqn = (dqnode_t*)malloc(sizeof(dqnode_t));
	return dqn;
}

/*
 * Funzione di inserimento in coda.
 */
unsigned short int
pushrear(dqueue_t* dqp, unsigned int* conf, unsigned short int lvl)
{
	unsigned short int ret;
	unsigned int iter;
	dqnode_t* dqn;
	if((dqp != 0) && (conf != 0)) {
		if((dqn = getitem(dqp)) != 0) {
			ret = 1;
			dqn->post = 0;
			if((dqp->head == 0) && (dqp->tail == 0)) {
				dqp->head = dqn;
				dqp->tail = dqn;
				dqn->pre = 0;
			} else {
				dqp->tail->post = dqn;
				dqn->pre = dqp->tail;
				dqp->tail = dqn;
			}
			for(iter = 0; iter < 9; iter++)
				dqn->item[iter] = conf[iter];
			dqn->lvl = lvl;
		} else ret = 0;
	} else ret = 0;
	return ret;
}

/*
 * Funzione di prelievo in coda.
 */
unsigned short int
poprear(dqueue_t* dqp, unsigned int* conf)
{
	unsigned short int ret;
	unsigned int iter;
	dqnode_t* dqn;
	if((dqp != 0) && (conf != 0)) {
		if(dqp->tail != 0) {
			dqn = dqp->tail;
			dqp->tail = dqn->pre;
			if(dqp->tail != 0) dqp->tail->post = 0;
			else dqp->head = 0;
			for(iter = 0; iter < 9; iter++)
				conf[iter] = dqn->item[iter];
			ret = dqn->lvl;
			dqn->post = dqp->unused;
			dqp->unused = dqn;
		} else ret = 0;
	} else ret = 0;
	return ret;
}

/*
 * Funzione di inserimento in testa.
 */
unsigned short int
push(dqueue_t* dqp, unsigned int* conf, unsigned short int lvl)
{
	unsigned short int ret;
	unsigned int iter;
	dqnode_t* dqn;
	if((dqp != 0) && (conf != 0)) {
		if((dqn = getitem(dqp)) != 0) {
			ret = 1;
			dqn->pre = 0;
			if((dqp->head == 0) && (dqp->tail == 0)) {
				dqp->head = dqn;
				dqp->tail = dqn;
				dqn->post = 0;
			} else {
				dqn->post = dqp->head;
				dqp->head = dqn;
			}
			for(iter = 0; iter < 9; iter++)
				dqn->item[iter] = conf[iter];
			dqn->lvl = lvl;
		} else ret = 0;
	} else ret = 0;
	return ret;
}

/*
 * Funzione di prelievo in testa.
 */
unsigned short int
pop(dqueue_t* dqp, unsigned int* conf)
{
	unsigned short int ret;
	unsigned int iter;
	dqnode_t* dqn;
	if((dqp != 0) && (conf != 0)) {
		if(dqp->head != 0) {
			dqn = dqp->head;
			dqp->head = dqn->post;
			if(dqp->head != 0) dqp->head->pre = 0;
			else dqp->tail = 0;
			for(iter = 0; iter < 9; iter++)
				conf[iter] = dqn->item[iter];
			ret = dqn->lvl;
			dqn->post = dqp->unused;
			dqp->unused = dqn;
		} else ret = 0;
	} else ret = 0;
	return ret;
}

/*
 * Funzione per la cancellazione della
 * singola coda doppia.
 */
void
freedqueue(dqueue_t* dqp)
{
	dqnode_t* dqn;
	if(dqp != 0) {
		while(dqp->head != 0) {
			dqn = dqp->head;
			dqp->head = dqn->post;
			free(dqn);
		}
		while(dqp->unused != 0) {
			dqn = dqp->unused;
			dqp->unused = dqn->post;
			free(dqn);
		}
		free(dqp);
	}
}

/*
 * Svuota la struttura coda doppia.
 */
void
emptydqueue(dqueue_t* dqp)
{
	dqnode_t** dqi;
	if(dqp != 0) {
		dqi = &(dqp->unused);
		while(*dqi != 0)
			dqi = &((*dqi)->post);
		*dqi = dqp->head;
		dqp->head = 0;
		dqp->tail = 0;
	}
}
