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
#include "pgenerator.h"

/*
 * Funzione di controllo di appartenenza
 * alla stessa orbita e quindi di esistenza
 * della soluzione.
 */
unsigned short int
chksol(unsigned int* iconf)
{
	unsigned int iter;
	unsigned int cnt;
	unsigned int isum;
	unsigned int fsum;
	unsigned int fconf[9] =
	{0, 1, 2, 3, 4, 5, 6, 7, 8};
	isum = 0;
	fsum = 0;
	for(iter = 0; iter < 9; iter++) {
		for(cnt = iter + 1; cnt < 9; cnt++) {
			if((iconf[iter] != 0) && (iconf[cnt] != 0) && (iconf[iter] > iconf[cnt]))
				isum++;
			if((fconf[iter] != 0) && (fconf[cnt] != 0) && (fconf[iter] > fconf[cnt]))
				fsum++;
		}
	}
	return (((isum % 2) == (fsum % 2)) ? 1 : 0);
}

/*
 * Funzione per la generazione di
 * un file contenente TESTSET
 * puzzle con configurazione iniziale
 * casuale e finale: 123804765
 */
void
pgen(unsigned int seed)
{
	FILE* fref;
	unsigned int tdim;
	unsigned int step;
	unsigned int cnt;
	unsigned int iter;
	unsigned int value;
	unsigned int mask[9];
	unsigned int conf[9];
	unsigned int touch;
	fref = fopen("./puzzle", "w");
	if(fref != 0) {
		if(seed < 10) tdim = 10;
		else if (seed < 100) tdim = 100;
		else if (seed < 1000) tdim = 1000;
		else tdim = 10000;
		srand(seed);
		for(iter = 0; iter < tdim; iter++) {
			touch = 0;
			while(!touch) {
				for(cnt = 0; cnt < 9; cnt++)
					mask[cnt] = 0;
				step = 0;
				while(!touch) {
					value = (int)(10.0 * (rand() / (RAND_MAX + 1.0)));
					if(mask[value] == 0) {
						mask[value]++;
						conf[step++] = value;
					}
					touch = 1;
					for(cnt = 0; cnt < 9; cnt++)
						touch *= mask[cnt];
				}
				touch = chksol(conf);
			}
			for(cnt = 0; cnt < 9; cnt++)
				fprintf(fref, "%d", conf[cnt]);
			fprintf(fref, "\n");
		}
		fclose(fref);
	}
}
