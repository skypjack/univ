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
#include "map.h"

/*
 * Funzione per la creazione di una
 * struttura map vuota.
 */
map_t*
createmap() {
	unsigned int iter;
	map_t* mp;
	if((mp = (map_t*)malloc(sizeof(map_t))) != 0) {
		for(iter = 0; iter < 1000; iter++)
			mp->bucket[iter] = 0;
		mp->entries = 0;
		mp->unused = 0;
	}
	return mp;
}

/*
 * Funzione per l'eliminazione di
 * una struttura map.
 */
void
freemap(map_t* mp)
{
	unsigned int iter;
	mnode_t* mnp;
	if(mp != 0) {
		for(iter = 0; iter < 1000; iter++) {
			while(mp->bucket[iter] != 0) {
				mnp = mp->bucket[iter];
				mp->bucket[iter] = mnp->next;
				free(mnp);
			}
		}
		while(mp->unused != 0) {
			mnp = mp->unused;
			mp->unused = mnp->next;
			free(mnp);
		}
		free(mp);
	}
}

/*
 * Svuota la struttura map.
 */
void
emptymap(map_t* mp)
{
	unsigned int iter;
	mnode_t** mni;
	if(mp != 0) {
		mni = &(mp->unused);
		for(iter = 0; iter < 1000; iter++) {
			while(*mni != 0)
				mni = &((*mni)->next);
			*mni = mp->bucket[iter];
			mp->bucket[iter] = 0;
		}
		mp->entries = 0;
	}
}

/*
 * Recupera un elemento inutilizzato o
 * ne crea uno nuovo, se non presenti.
 */
mnode_t*
getmnode(map_t* mp)
{
	mnode_t* mnp;
	if(mp != 0) {
		if(mp->unused != 0) {
			mnp = mp->unused;
			mp->unused = mnp->next;
		} else mnp = (mnode_t*)malloc(sizeof(mnode_t));
	} else mnp = 0;
	return mnp;
}

/*
 * Funzione per l'inserimento di una coppia
 * chiave-valore nella struttura map.
 */
unsigned short int
add(map_t* mp, unsigned long int key, unsigned long int parent)
{
	unsigned short int ret;
	mnode_t* mnp;
	if(mp != 0) {
		if((mnp = getmnode(mp)) != 0) {
			ret = 1;
			mp->entries++;
			mnp->parent = parent;
			mnp->key = key;
			mnp->next = mp->bucket[key % 1000];
			mp->bucket[key % 1000] = mnp;
		} else ret = 0;
	} else ret = 0;
	return ret;
}

/*
 * Funzione per il recupero dei dati, ovvero
 * del valore associato ad una data chiave.
 */
unsigned long int
find(map_t* mp, unsigned long int key)
{
	unsigned long int ret;
	mnode_t* mnp;
	if(mp != 0) {
		mnp = mp->bucket[key % 1000];
		ret = 0;
		while((mnp != 0) && (!ret)) {
			if(mnp->key == key) ret = !ret;
			else mnp = mnp->next;
		}
	} else ret = 0;
	return (ret ? mnp->parent : ret);
}
