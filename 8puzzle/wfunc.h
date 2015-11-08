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

#ifndef WFUNC_H
#define WFUNC_H 1

#include "heuristic.h"

/*
 * Tipo puntatore a funzione di valutazione per nodo
 * per assegnamento dinamico a run-time.
 */
typedef double (*wfp_t)(unsigned int*, unsigned int, hfp_t);

/*
 * Funzione di valutazione standard, coinvolge il livello
 * del nodo e la funzione euristica associata.
 */
double
wfstd(unsigned int*, unsigned int, hfp_t);

/*
 * Funzione di valutazione pesata, coinvolge il livello del
 * nodo e la funzione euristica associata, con influenza
 * inversamente proporzionale al livello.
 */
double
wfwgt(unsigned int*, unsigned int, hfp_t);

#endif /* WFUNC_H */
