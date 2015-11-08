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
#include "wfunc.h"

/*
 * Funzione di valutazione standard, coinvolge il livello
 * del nodo e la funzione euristica associata.
 */
double
wfstd(unsigned int* puzzle, unsigned int lvl, hfp_t hfunc)
{
	unsigned short int cost;
	if((puzzle != 0) && (hfunc != 0)) {
		cost = 0;
		cost += lvl + (*hfunc)(puzzle);
	} else cost = 0;
	return cost;
}

/*
 * Funzione di valutazione pesata, coinvolge il livello del
 * nodo e la funzione euristica associata, con influenza
 * inversamente proporzionale al livello.
 */
double
wfwgt(unsigned int* puzzle, unsigned int lvl, hfp_t hfunc)
{
	unsigned short int cost;
	if((puzzle != 0) && (hfunc != 0)) {
		cost = 0;
		cost += lvl + ((*hfunc)(puzzle) / ((lvl != 0 )? lvl : 1));
	} else cost = 0;
	return cost;
}
