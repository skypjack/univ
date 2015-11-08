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

#ifndef IOFUNC_H
#define IOFUNC_H 1

#include <stdio.h>
#include "hash.h"
#include "map.h"

/*
 * Funzione di lettura da file delle
 * configurazione del puzzle.
 */
unsigned short int
parse(FILE*, unsigned int*);

/*
 * Funzione di stampa dello stack
 * esecutivo dalla configurazione
 * iniziale a quella finale.
 */
void
splash(hash_t*, map_t*, unsigned long int);

#endif /* IOFUNC_H */
