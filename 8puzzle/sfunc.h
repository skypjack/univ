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

#ifndef SFUNC_H
#define SFUNC_H 1

#include "heuristic.h"
#include "wfunc.h"

/*
 * Tipo puntatore a funzione di ricerca per
 * assegnamento dinamico a run-time.
 */
typedef unsigned short int (*sfp_t)(unsigned int*, wfp_t, hfp_t, unsigned int*, unsigned short int);

/*
 * Possibili direzioni di spostamento per la
 * casella vuota.
 */
typedef enum direction { UP, DOWN, LEFT, RIGHT } dir_t;

/*
 * Funzione di analisi su set di dati, con
 * possibilità di definire la funzione di ricerca,
 * di peso e l'euristica volute.
 */
void
dtest(const char*, FILE*, sfp_t, wfp_t, hfp_t, unsigned short int);

/*
 * Funzione di ricerca Breadth-First.
 */
unsigned short int
sfbf(unsigned int*, wfp_t, hfp_t, unsigned int*, unsigned short int);

/*
 * Funzione di ricerca Iterative Depth-First.
 */
unsigned short int
sfdf(unsigned int*, wfp_t, hfp_t, unsigned int*, unsigned short int);

/*
 * Funzione di ricerca A*.
 */
unsigned short int
sfas(unsigned int*, wfp_t, hfp_t, unsigned int*, unsigned short int);

/*
 * Funzione di ricerca IDA*.
 */
unsigned short int
sfidas(unsigned int*, wfp_t, hfp_t, unsigned int*, unsigned short int);

#endif /* SFUNC_H */
