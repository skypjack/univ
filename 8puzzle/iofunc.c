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
#include "iofunc.h"

/*
 * Funzione di lettura da file delle
 * configurazione del puzzle.
 */
unsigned short int
parse(FILE* fref, unsigned int* conf)
{

	unsigned short int ret;
	unsigned int mask[9];
	unsigned int iter;
	char buf[2];
	if((fref != 0) && (conf != 0)) {
		ret = 1;
		buf[1] = '\0';
		for(iter = 0; (iter < 9) && (ret); iter++) {
			while((buf[0] = fgetc(fref)) == '\n');
			if((buf[0] != EOF) && (isdigit(buf[0])))
				conf[iter] = strtol(buf, 0, 10);
			else ret = 0;
		}
		if(ret) {
			for(iter = 0; iter < 9; iter++)
				mask[iter] = 0;
			for(iter = 0; (iter < 9) && (ret); iter++)
				mask[conf[iter]] = 1;
			for(iter = 0; iter < 9; iter++)
				ret *= mask[iter];
		}
	} else ret = 0;
	return ret;
}

/*
 * Funzione di stampa dello stack
 * esecutivo dalla configurazione
 * iniziale a quella finale.
 */
void
splash(hash_t* hp, map_t* mp, unsigned long int key)
{
	unsigned int buf[9];
	unsigned int iter;
	unsigned long int pk;
	if((hp != 0) && (mp != 0) && (key != 0)) {
		pk = find(mp, key);
		splash(hp, mp, pk);
		if(retrieve(hp, key, buf)) {
			for(iter = 0; iter < 9; iter++) {
				printf("[%d] ", buf[iter]);
				if(((iter + 1) % 3) == 0) printf("\n");
			}
		}
	}
	printf("\n");
}
