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

#ifndef KERNEL_H
#define KERNEL_H 1

#include "common.h"
#include "graph.h"
#include "list.h"
#include "path_array.h"
#include "graph_function.h"

/*
 * Definizione puntatore a funzione kernel
 */
typedef
float (*core_t) (graph_t*, graph_t*, int);

/*
 * Kernel di Tanimoto
 */
extern float
TanimotoCore (graph_t* gp1, graph_t* gp2, int depth);

/*
 * Kernel MinMax
 */
extern float
MinMaxCore (graph_t* gp1, graph_t* gp2, int depth);

/*
 * Kernel generico fra due grafi, con profondità e tipologia kernel
 */
extern float*
KernelMatrix (core_t core, graph_t** gtrain, size_t length, size_t depth);

extern void
WriteKernelMatrix (core_t core, graph_t** gtrain, size_t length, size_t depth);

extern void
createTrainTestFile (float* mat, int* target, size_t length, size_t numFold, size_t indexTestFold, FILE* trainFile, FILE* trainTargetFile,  FILE* testFile, FILE* testTargetFile);


/*
 * 3-fold cross validation con kernel passato in input
 */
extern void
FoldCrossValidation (core_t, graph_t**, int*, size_t, size_t);

/*
 * Mescola esempi
 */
extern void
mix (graph_t**, int*, size_t, unsigned int);

/*
 * Calcolo accuratezza da file SVM Gist
 */
extern float
getAccuracy (FILE*, FILE*);

/*
 * Calcolo leave one out da file SVM Gist
 */
extern float
getLeaveOneOut (FILE*);

#endif /* KERNEL_H */
