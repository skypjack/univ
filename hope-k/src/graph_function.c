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

#include "graph_function.h"

void
DFS_Search (graph_t* gp, hnode_t* node, int maxDepth, pathArray_t* pathArray, unsigned int with_mult)
{
  putColorOnAllNodes(gp, WHITE);  
  char* path=xstrdup(node->element);
  DFS_Visit(gp, node, 0, maxDepth, path, pathArray, with_mult);
  XFREE(path);
}


void
DFS_Visit (graph_t* gp, hnode_t* node, int depth, int maxDepth, char* path, pathArray_t* pathArray, unsigned int with_mult)
{
  putColorOnNode(gp, node->nodeId, GRAY);
  char* newPath;
  size_t len;
  edge_t* eiter;
  eiter = node->head;
  while(eiter) {
	  if(eiter->color==WHITE){
	    len = 1;
	    len += strlen(path);
	    if(eiter->bond) len += strlen(eiter->bond);
	    if(eiter->element) len += strlen(eiter->element);

	    newPath = XMALLOC(char, len);
	    strcpy(newPath,path);
	    if(eiter->bond!=NULL)
	      strcat(newPath,eiter->bond);
	    if(eiter->element!=NULL)
	      strcat(newPath,eiter->element);

	    if(maxDepth==-1 || (depth+1)<maxDepth)
	      DFS_Visit(gp, getHeadOfNode(gp,eiter->nodeId), depth+1, maxDepth, newPath, pathArray, with_mult);

	    if(!addPathToArray(pathArray, newPath, with_mult))
	      XFREE(newPath);

	    putColorOnNode(gp,node->nodeId, BLACK);
	  }
	  
	  eiter = list_next_entry(edge_t, eiter);
  }
}

hnode_t*
getHeadOfNode (graph_t* gp, node_t id)
{
  hnode_t* hiter;
  hiter = gp->head;
  while(hiter) {
	  if(hiter->nodeId==id)
		  return hiter;
	  hiter = list_next_entry(hnode_t, hiter);
  }
  return NULL;
}


void
putColorOnNode (graph_t* gp, node_t id, color_t newColor)
{
  hnode_t* hiter;
  edge_t* eiter;
  hiter = gp->head;
  while(hiter) {
	if(hiter->nodeId==id)
		hiter->color=newColor;
	
    eiter = hiter->head;
    while(eiter) {
      if(eiter->nodeId==id)
        eiter->color=newColor;
      
      eiter = list_next_entry(edge_t, eiter);
    }
    hiter = list_next_entry(hnode_t, hiter);
  }
}

void
putColorOnAllNodes (graph_t* gp, color_t newColor)
{
  hnode_t* hiter;
  edge_t* eiter;
  hiter = gp->head;
  while(hiter) {
	hiter->color=newColor;
	
    eiter = hiter->head;
    while(eiter) {
      eiter->color=newColor;
      
      eiter = list_next_entry(edge_t, eiter);
    }
    hiter = list_next_entry(hnode_t, hiter);
  }
}
