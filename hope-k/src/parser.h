/****************************************************************************************

   Hope-K
   Copyright (C)  2008  Michele Caini, Johnny Giuntini


   This file is part of Hope-K.

   Hope-K is free software; you can redistribute it and/or modify
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


   To contact me:   skypjack@gmail.com, giuntinijohnny@interfree.it

****************************************************************************************/

#ifndef PARSER_H
#define PARSER_H

#define BUF 80
#define BASE 4096

#include "common.h"
#include "graph.h"

/*
 * Tipi dato file mol2
 */
enum stat
  {
    ATOM,
    BOND
  };

typedef
enum stat
stat_t;

/*
 * Parsing file mol2: generazione grafi molecole
 */ 
graph_t**
parse (FILE*, size_t*);

int*
parseTarget (FILE*, size_t);

#endif /* PARSER_H */
