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

#include "common.h"
#include "parser.h"
#include "list.h"

graph_t**
parse (FILE* in, size_t* size)
{
  stat_t status;
  char buf[BUF];
  char *tok[4];
  graph_t** gp;
  graph_t* actgp;
  hnode_t* iter;
  size_t dim, nfree;
  node_t ntmp;
  status = -1;
  actgp = NULL;
  gp = XMALLOC(graph_t*, BASE);
  dim = nfree = BASE;
  while(!feof(in)) {
    fgets(buf, BUF, in);
    if(buf[0] == '\n') {
      
    }else if(strstr(buf, "MOLECULE")) {
      printf("\n%d", dim-nfree);
      if(!nfree) {
	gp = XREALLOC(graph_t*, gp, dim * 2);
	nfree = dim;
	dim *= 2;
      }
      gp[dim - nfree] = XMALLOC(graph_t, 1);
      actgp = gp[dim - nfree--];
		  init_graph(actgp);
    }else if(strstr(buf, "ATOM"))
      status = ATOM;
    else if(strstr(buf, "BOND"))
      status = BOND;
    else switch(status) {
      case ATOM:
	tok[0] = strtok(buf, " ");
	tok[1] = strtok(NULL, " ");
	if(!tok[0] || !tok[1])
	  fatal("lost token!");
	add_node(actgp, strtol(tok[0], NULL, 10), xstrdup(tok[1]));
	break;
      case BOND:
	tok[0] = strtok(buf, " ");
	tok[1] = strtok(NULL, " ");
	tok[2] = strtok(NULL, " ");
	tok[3] = strtok(NULL, " ");
	if(!tok[0] || !tok[1] || !tok[2] || !tok[3])
	  fatal("lost token!");

	// FAIL: oriented edge ... :-D
	ntmp = strtol(tok[2], NULL, 10);
	iter = actgp->head;
	while(iter && iter->nodeId != ntmp)
	  iter = list_next_entry(hnode_t, iter);
	if(!iter)
	  fatal("silly node");
	add_edge(actgp, strtol(tok[1], NULL, 10), ntmp, xstrdup(iter->element), xstrdup(tok[3]));
	// Need a duplicate to be not-oriented!
	ntmp = strtol(tok[1], NULL, 10);
	iter = actgp->head;
	while(iter && iter->nodeId != ntmp)
	  iter = list_next_entry(hnode_t, iter);
	if(!iter)
	  fatal("silly node");
	add_edge(actgp, strtol(tok[2], NULL, 10), ntmp, xstrdup(iter->element), xstrdup(tok[3]));
	// po po po ... PATCHED!
	
	break;
      default:
	fatal("silly status");
	break;
      }
  }
  *size = dim - nfree;
  return gp;
}

int*
parseTarget (FILE* file, size_t size)
{
	 char* buffer=XMALLOC(char, 15); int bufsize=15;
	 int* targetArray=XMALLOC(int, size);
	 int count=-1;
	 
	 //fgets(buffer, bufsize, file);
	 while(fgets(buffer, bufsize, file)){
		 count++;
		
		 if(strstr(buffer,"-1"))
			 targetArray[count]=-1;
		 else
			 targetArray[count]=1;
	 }
	
	 XFREE(buffer);
	 
	 /*
	 printf("\ncount: %d", count);
	 printf("\nsize: %d", size);
	 
	 
	 int i;
	 for(i=0; i<size; i++)
		 printf("\n label: %d", targetArray[i]);
	 */
	 
	 if(count+1==size)
		 return targetArray;
	 else
		 return 0;
}
