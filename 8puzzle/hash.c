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
#include "hash.h"

/*
 * Funzione per la creazione di una
 * struttura hash vuota.
 */
hash_t*
createhash() {
	unsigned int iter;
	hash_t* hp;
	if((hp = (hash_t*)malloc(sizeof(hash_t))) != 0) {
		for(iter = 0; iter < 1000; iter++)
			hp->bucket[iter] = 0;
		hp->entries = 0;
		hp->unused = 0;
	}
	return hp;
}

/*
 * Funzione per l'eliminazione di
 * una struttura hash.
 */
void
freehash(hash_t* hp)
{
	unsigned int iter;
	hnode_t* hnp;
	if(hp != 0) {
		for(iter = 0; iter < 1000; iter++) {
			while(hp->bucket[iter] != 0) {
				hnp = hp->bucket[iter];
				hp->bucket[iter] = hnp->next;
				free(hnp);
			}
		}
		while(hp->unused != 0) {
			hnp = hp->unused;
			hp->unused = hnp->next;
			free(hnp);
		}
		free(hp);
	}
}

/*
 * Svuota la struttura hash.
 */
void emptyhash(hash_t* hp)
{
	unsigned int iter;
	hnode_t** hni;
	if(hp != 0) {
		hni = &(hp->unused);
		for(iter = 0; iter < 1000; iter++) {
			while(*hni != 0)
				hni = &((*hni)->next);
			*hni = hp->bucket[iter];
			hp->bucket[iter] = 0;
		}
		hp->entries = 0;
	}
}

/*
 * Recupera un elemento per inserirlo
 * nella struttura hash.
 */
hnode_t*
gethnode(hash_t* hp)
{
	hnode_t* hnp;
	if(hp->unused != 0) {
		hnp = hp->unused;
		hp->unused = hnp->next;
	} else hnp = (hnode_t*)malloc(sizeof(hnode_t));
	return hnp;
}

/*
 * Funzione per l'inserimento di un
 * elemento nella struttura hash.
 */
unsigned long int
insert(hash_t* hp, unsigned int* conf)
{
	unsigned long int ret;
	unsigned int iter;
	hnode_t* hnp;
	if((hp != 0) && (conf != 0)) {
		if((hnp = gethnode(hp)) != 0) {
			hp->entries++;
			hnp->key = 0;
			for(iter = 0; iter < 9; iter++)
				hnp->data[iter] = conf[iter];
			for(iter = 0; iter < 8; iter++) {
				hnp->key *= 10;
				hnp->key += conf[iter];
			}
			hnp->next = hp->bucket[ hnp->key % 1000 ];
			hp->bucket[ hnp->key % 1000] = hnp;
			ret = hnp->key;
		} else ret = 0;
	} else ret = 0;
	return ret;
}

/*
 * Funzione di ricerca di un elemento
 * nella struttura hash.
 */
unsigned long int
search(hash_t* hp, unsigned int* conf)
{
	unsigned long int ret;
	unsigned int iter;
	unsigned long int key;
	hnode_t* hnp;
	if((hp != 0) && (conf != 0)) {
		key = 0;
		for(iter = 0; iter < 8; iter++) {
			key *= 10;
			key += conf[iter];
		}
		hnp = hp->bucket[key % 1000];
		ret = 0;
		while((hnp != 0) && (!ret)) {
			if(hnp->key == key) ret = key;
			else hnp = hnp->next;
		}
	} else ret = 0;
	return ret;
}

/*
 * Funzione per il recupero dei dati.
 */
unsigned short int
retrieve(hash_t* hp, unsigned long int key, unsigned int* conf)
{
	unsigned short int ret;
	unsigned int iter;
	hnode_t* hnp;
	if((hp != 0) && (conf != 0)) {
		hnp = hp->bucket[key % 1000];
		ret = 0;
		while((hnp != 0) && (!ret)) {
			if(hnp->key == key) {
				ret = !ret;
				for(iter = 0; iter < 9; iter++)
					conf[iter] = hnp->data[iter];
			} else hnp = hnp->next;
		}
	} else ret = 0;
	return ret;
}
