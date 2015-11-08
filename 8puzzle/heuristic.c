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
#include "heuristic.h"

/*
 * Euristica Standard, conta il numero di caselle fuori posto.
 * La configurazione goal è intesa essere: 123804765.
 */
unsigned short int
heustd(unsigned int* puzzle)
{
	unsigned int iter;
	unsigned short int cost;
	if(puzzle != 0) {
		cost = 0;
		for(iter = 0; iter < 9; iter++)
			if((puzzle[iter] != 0) && (puzzle[iter] != iter)) cost++;
	} else cost = 0;
	return cost;
}

/*
 * Euristica Manhattan, valuta gli offset orizzontali e verticali
 * del singolo elemento rispetto alla sua posizione.
 * La configurazione goal è intesa essere: 123804765.
 */
unsigned short int
heuman(unsigned int* puzzle)
{
	unsigned int iter;
	unsigned short int cost;
	unsigned short int hoff;
	unsigned short int voff;
	unsigned short int pos;
	if(puzzle != 0) {
		cost = 0;
		for(iter = 0; iter < 9; iter++) {
			if((puzzle[iter] != 0) && (puzzle[iter] != iter)) {
				hoff = iter % 3;
				voff = (iter - (iter % 3)) / 3;
				pos = puzzle[iter] % 3;
				if(hoff < pos) hoff = pos - hoff;
				else if(hoff > pos) hoff = hoff - pos;
				else hoff = 0;
				pos = (puzzle[iter] - (puzzle[iter] % 3)) / 3;
				if(voff < pos) voff = pos - voff;
				else if(voff > pos) voff = voff - pos;
				else voff = 0;
				cost += hoff + voff;
			}
		}
	} else cost = 0;
	return cost;
}
