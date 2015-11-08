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

#ifndef HEURISTIC_H
#define HEURISTIC_H 1

/*
 * Tipo puntatore a funzione euristica per assegnamento
 * dinamico a run-time.
 */
typedef unsigned short int (*hfp_t)(unsigned int*);

/*
 * Eurisitca Standard, conta il numero di caselle fuori posto.
 * La configurazione goal è intesa essere: 123804765.
 */
unsigned short int
heustd(unsigned int*);

/*
 * Euristica Manhattan, valuta gli offset orizzontali e verticali
 * del singolo elemento rispetto alla sua posizione.
 * La configurazione goal è intesa essere: 123804765.
 */
unsigned short int
heuman(unsigned int*);

#endif /* HEURISTIC_H */
