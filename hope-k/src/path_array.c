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

#include "path_array.h"

static void
check_path(pathArray_t* pa) 
{
  if(!pa->free) {
    pa->free = pa->dim;
    pa->array = XREALLOC(Path_t, pa->array, (pa->dim*2));
    pa->dim *= 2;
  }
}

void
initPathArray(pathArray_t* pathArray, int init_size){
	int i;
	pathArray->array= XMALLOC(Path_t, init_size);
	
	for(i=0; i<init_size;i++){
		(pathArray->array[i]).path=NULL;
		(pathArray->array[i]).mult=0;
	}
	pathArray->free=init_size;
	pathArray->dim=init_size;
}


int
addPathToArray(pathArray_t* pa, char* path, unsigned int with_mult){
  size_t iter;
  int ret;
  ret = 0;
  if(pa) {
    ret = 1;
    check_path(pa);
    for(iter = 0; iter < pa->dim - pa->free; iter++)
      if(!strcmp(pa->array[iter].path, path)) {
    	ret = 0;
    	break;
      }
    
    if(!ret){
    	if(with_mult==WITH_MULT)
    		(pa->array[iter].mult)++;
    }else{
    	pa->array[pa->dim-(pa->free)].path = path;
     	pa->array[pa->dim-(pa->free)].mult = 1;
     	pa->free--;
    }
  } else warning ("null pointer");
  return ret;
}

void
displayPathArray(pathArray_t* pa){
	size_t i;
	for(i=0; i < pa->dim - pa->free; i++)
		printf("\nPath n. %d: %s (mult: %d)", i+1, pa->array[i].path, pa->array[i].mult);
}

int
commonPaths(pathArray_t* pA1, pathArray_t* pA2){
	size_t i, j, count=0, found;
	for(i=0; i < pA1->dim - pA1->free; i++) {
		found=0;
		for(j=0; j < pA2->dim - pA2->free && found==0; j++)
			if(strcmp(pA1->array[i].path,pA2->array[j].path)==0){
				count++;
				found=1;
			}
	}
	return count;
}

float
commonPathsWithMult(pathArray_t* pA1, pathArray_t* pA2){
	size_t i, j, min=0, max=0, found;
	for(i=0; i < pA1->dim - pA1->free; i++) {
		found=0;
		for(j=0; j < pA2->dim - pA2->free && found==0; j++){
			if(strcmp(pA1->array[i].path,pA2->array[j].path)==0){
				
			  printf("\nCommonPath: %s", pA1->array[i].path);
				
				if(pA1->array[i].mult<pA2->array[j].mult){
					min+=pA1->array[i].mult;
					max+=pA2->array[j].mult;
				}else{
					max+=pA1->array[i].mult;
					min+=pA2->array[j].mult;
				}
				
				pA2->array[j].mult=0;
				
				found=1;
			}
		}
		
		if(found==0)
			max+=pA1->array[i].mult;
	}
	
	
	for(j=0; j < pA2->dim - pA2->free; j++){
		if(pA2->array[j].mult>0)
			max+=pA2->array[j].mult;
	}
	
	
	printf("\n min: %d", min);
	printf("\n max: %d", max);
	
	return (float) min/max;
}

void
freePathArray(pathArray_t* pa){
	size_t i;
	for(i=0; i < pa->dim - pa->free; i++)
	  XFREE(pa->array[i].path);
	XFREE(pa->array);
	XFREE(pa);
}
