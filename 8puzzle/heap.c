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
#include "heap.h"

/*
 * Funzione di creazione per
 * un singolo heap.
 */
heap_t*
createheap()
{
	heap_t* hp;
	if((hp = (heap_t*)malloc(sizeof(heap_t))) != 0) {
		hp->root = 0;
		hp->unused = 0;
	}
	return hp;
}

/*
 * Funzione di generazione / recupero di un
 * elemento per l'inserimento.
 */
hpnode_t*
getnode(heap_t* hp)
{
	hpnode_t* hpn;
	if(hp->unused != 0) {
		hpn = hp->unused;
		hp->unused = hpn->sup;
	} else hpn = (hpnode_t*)malloc(sizeof(hpnode_t));
	return hpn;
}

/*
 * Funzione per l'inserimento
 * di elementi nella coda.
 */
unsigned short int
put(heap_t* hp, unsigned int* conf, unsigned short int lvl, double weight)
{
	unsigned short int ret;
	unsigned int iter;
	hpnode_t* hpn;
	hpnode_t** hpi;
	if((hp != 0) && (conf != 0)) {
		if((hpn = getnode(hp)) != 0) {
			ret = 1;
			for(iter = 0; iter < 9; iter++)
				hpn->item[iter] = conf[iter];
			hpn->lvl = lvl;
			hpn->weight = weight;
			hpn->sup = 0;
			hpn->inf = 0;
			hpn->par = 0;
			hpi = &(hp->root);
			while(*hpi != 0) {
				hpn->par = *hpi;
				if((*hpi)->weight < weight)
					hpi = &((*hpi)->inf);
				else hpi = &((*hpi)->sup);
			}
			*hpi = hpn;
		} else ret = 0;
	} else ret = 0;
	return ret;
}

/*
 * Funzione di prelievo
 * di elementi dalla coda.
 */
unsigned short int
get(heap_t* hp, unsigned int* conf)
{
	unsigned short int ret;
	unsigned int iter;
	hpnode_t* hpn;
	hpnode_t** hpi;
	if((hp != 0) && (conf != 0)) {
		if(hp->root != 0) {
			hpi = &(hp->root);
			while((*hpi)->sup != 0)
				hpi = &((*hpi)->sup);
			hpn = *hpi;
			if(hpn->inf != 0)
				*hpi = hpn->inf;
			else *hpi = 0;
			for(iter = 0; iter < 9; iter++)
				conf[iter] = hpn->item[iter];
			ret = hpn->lvl;
			hpn->sup = hp->unused;
			hp->unused = hpn;
		} else ret = 0;
	} else ret = 0;
	return ret;
}

/*
 * Funzione di supporto per la ricerca.
 */
unsigned short int
findif(hpnode_t* hpn, hpnode_t** hph, unsigned int* conf)
{
	unsigned int iter;
	unsigned short int fnd;
	hpnode_t* hpi;
	if(hpn != 0) {
		fnd = 1;
		for(iter = 0; (iter < 9) && (fnd); iter++)
			if(hpn->item[iter] != conf[iter]) fnd = 0;
		if(!fnd) {
			fnd = findif(hpn->sup, hph, conf);
			if(!fnd) fnd = findif(hpn->inf, hph, conf);
		} else {
			*hph = hpn;
			if(hpn->par->sup == hpn) {
				if(hpn->sup == 0) {
					hpn->par->sup = hpn->inf;
					if(hpn->inf != 0)
						hpn->inf->par = hpn->par;
				} else {
					hpn->par->sup = hpn->sup;
					hpn->sup->par = hpn->par;
					if(hpn->inf != 0) {
						hpi = hpn->sup;
						while(hpi->inf != 0)
							hpi = hpi->inf;
						hpi->inf = hpn->inf;
						hpn->inf->par = hpi;
					}
				}
			} else {
				if(hpn->sup == 0) {
					hpn->par->inf = hpn->inf;
					if(hpn->inf != 0)
						hpn->inf->par = hpn->par;
				} else {
					hpn->par->inf = hpn->sup;
					hpn->sup->par = hpn->par;
					if(hpn->inf != 0) {
						hpi = hpn->sup;
						while(hpi->inf != 0)
							hpi = hpi->inf;
						hpi->inf = hpn->inf;
						hpn->inf->par = hpi;
					}
				}
			}
		}
	} else fnd = 0;
	return fnd;
}

/*
 * Funzione di cambiamento condizionale
 * del peso di un elemento in coda, solo
 * se il nuovo peso è minore.
 */
unsigned short int
setif(heap_t* hp, unsigned int* conf, unsigned short int lvl, double weight)
{
	unsigned short int ret;
	hpnode_t* hph;
	if(hp != 0) {
		ret = findif(hp->root, &hph, conf);
		if(ret) {
			hph->sup = hp->unused;
			hp->unused = hph;
			weight = (hph->weight < weight) ? hph->weight : weight;
			put(hp, conf, lvl, weight);
		}
	} else ret = 0;
	return ret;
}

/*
 * Funzione di supporto per la cancellazione.
 */
void
freenode(hpnode_t* hpn)
{
	if(hpn != 0) {
	  freenode(hpn->sup);
	  freenode(hpn->inf);
	  free(hpn);
	}
}

/*
 * Funzione per la cancellazione del
 * singolo heap.
 */
void
freeheap(heap_t* hp)
{
	hpnode_t* hpn;
	if(hp != 0) {
		freenode(hp->root);
		while(hp->unused != 0) {
			hpn = hp->unused;
			hp->unused = hpn->sup;
			free(hpn);
		}
		free(hp);
	}
}

/*
 * Funzione di supporto per lo svuotamento.
 */
void
emptynode(hpnode_t** hpb, hpnode_t* hpn)
{
	if(hpn != 0) {
		emptynode(hpb, hpn->sup);
		emptynode(hpb, hpn->inf);
		hpn->sup = *hpb;
		*hpb = hpn;
	}
}

/*
 * Svuota la struttura heap.
 */
void
emptyheap(heap_t* hp)
{
	emptynode(&(hp->unused),hp->root);
	hp->root = 0;
}
