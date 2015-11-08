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

#include "kernel.h"
#include "stdlib.h"

void
mix (graph_t** gp, int* target, size_t size, unsigned int seed)
{
  size_t iter, cnt;
  int val; int targetTmp;
  graph_t* gtmp;
  srand(seed);
  for(cnt = 0; cnt < size; cnt++) {
    for(iter = 0; iter < size; iter++) {
      val = (int) (size * (rand() / (RAND_MAX + 1.0)));
      gtmp = gp[iter];
      gp[iter] = gp[val];
      gp[val] = gtmp;
      
      targetTmp = target[iter];
      target[iter] = target[val];
      target[val]=targetTmp; 
    }
    srand(cnt);
  }
}


void
FoldCrossValidation (core_t core, graph_t** gtrain, int* target, size_t ltrain, size_t depth)
{
	float* mat; int i;
	FILE* trainFile,*trainTargetFile,*testFile,*testTargetFile;	

	mix(gtrain, target, ltrain, ltrain);
	
	mat=KernelMatrix(core, gtrain, ltrain, depth);
	
	for(i=0; i<10; i++){
		if(i==0){
			trainFile = fopen("./fold0/trainFile0.res", "w"); trainTargetFile = fopen("./fold0/trainTargetFile0.res", "w"); testFile = fopen("./fold0/testFile0.res", "w"); testTargetFile = fopen("./fold0/testTargetFile0.res", "w");			
		}else if(i==1){
			trainFile = fopen("./fold1/trainFile1.res", "w"); trainTargetFile = fopen("./fold1/trainTargetFile1.res", "w"); testFile = fopen("./fold1/testFile1.res", "w"); testTargetFile = fopen("./fold1/testTargetFile1.res", "w");			
		}else if(i==2){
			trainFile = fopen("./fold2/trainFile2.res", "w"); trainTargetFile = fopen("./fold2/trainTargetFile2.res", "w"); testFile = fopen("./fold2/testFile2.res", "w"); testTargetFile = fopen("./fold2/testTargetFile2.res", "w");			
		}else if(i==3){
			trainFile = fopen("./fold3/trainFile3.res", "w"); trainTargetFile = fopen("./fold3/trainTargetFile3.res", "w"); testFile = fopen("./fold3/testFile3.res", "w"); testTargetFile = fopen("./fold3/testTargetFile3.res", "w");			
		}else if(i==4){
			trainFile = fopen("./fold4/trainFile4.res", "w"); trainTargetFile = fopen("./fold4/trainTargetFile4.res", "w"); testFile = fopen("./fold4/testFile4.res", "w"); testTargetFile = fopen("./fold4/testTargetFile4.res", "w");			
		}else if(i==5){
			trainFile = fopen("./fold5/trainFile5.res", "w"); trainTargetFile = fopen("./fold5/trainTargetFile5.res", "w"); testFile = fopen("./fold5/testFile5.res", "w"); testTargetFile = fopen("./fold5/testTargetFile5.res", "w");			
		}else if(i==6){
			trainFile = fopen("./fold6/trainFile6.res", "w"); trainTargetFile = fopen("./fold6/trainTargetFile6.res", "w"); testFile = fopen("./fold6/testFile6.res", "w"); testTargetFile = fopen("./fold6/testTargetFile6.res", "w");			
		}else if(i==7){
			trainFile = fopen("./fold7/trainFile7.res", "w"); trainTargetFile = fopen("./fold7/trainTargetFile7.res", "w"); testFile = fopen("./fold7/testFile7.res", "w"); testTargetFile = fopen("./fold7/testTargetFile7.res", "w");			
		}else if(i==8){
			trainFile = fopen("./fold8/trainFile8.res", "w"); trainTargetFile = fopen("./fold8/trainTargetFile8.res", "w"); testFile = fopen("./fold8/testFile8.res", "w"); testTargetFile = fopen("./fold8/testTargetFile8.res", "w");			
		}else if(i==9){
			trainFile = fopen("./fold9/trainFile9.res", "w"); trainTargetFile = fopen("./fold9/trainTargetFile9.res", "w"); testFile = fopen("./fold9/testFile9.res", "w"); testTargetFile = fopen("./fold9/testTargetFile9.res", "w");			
		}
		
		printf("\nScrittura fold %d", i);
		createTrainTestFile(mat, target, ltrain, 10, i, trainFile, trainTargetFile, testFile, testTargetFile);
		
		fclose(trainFile);
		fclose(trainTargetFile);
		fclose(testFile);
		fclose(testTargetFile);
	}
	
	XFREE(mat);
}

float*
KernelMatrix (core_t core, graph_t** gtrain, size_t length, size_t depth)
{
	size_t i, j;
	float* mat;
	
	mat = XMALLOC(float, length * length);
	for(i = 0; i < length; i++)
		mat[i*length + i] = 1.;
  
	for(i=0; i<length; i++)
		for(j=i+1; j<length; j++){  		  
			printf("\n%d : %d", i, j);
		  	//if(j>i)
		  		mat[i * length + j] = (*core)(gtrain[i],gtrain[j], depth);
		  	//else if(j<i)
		  		//mat[i * length + j] = mat[j * length + i];
		}
  
	return mat;
}

void
WriteKernelMatrix (core_t core, graph_t** gtrain, size_t length, size_t depth){
	float* mat;
	int i, j;
	FILE* matrixFile;	
	
	mat=KernelMatrix(core, gtrain, length, depth);

	matrixFile = fopen("./matrix.res", "w");			
	
	printf("\nScrittura matrice del kernel");
	fprintf(matrixFile, "K<x,y>");
	for(i=0; i<length; i++)  
		fprintf(matrixFile, "\ttr%d", i+1);
	fprintf(matrixFile, "\n");
	for(i=0; i<length; i++){
		fprintf(matrixFile, "tr%d", i+1);	  
		for(j=0; j<length; j++){
			if(i<=j)
				fprintf(matrixFile, "\t%.4f", mat[i * length + j]);
			else
				fprintf(matrixFile, "\t%.4f", mat[j * length + i]);
		}
		fprintf(matrixFile, "\n");
	}  	

	fclose(matrixFile);
	XFREE(mat);    		  
}

void
createTrainTestFile (float* mat, int* target, size_t length, size_t numFold, size_t indexTestFold, FILE* trainFile, FILE* trainTargetFile,  FILE* testFile, FILE* testTargetFile)
{
	size_t i, j, dimFold;
	dimFold=(int)length/numFold;
	
	if(indexTestFold==numFold-1){
		fprintf(trainFile, "K<x,y>");
		for(i=0; i<(numFold-1)*dimFold; i++)  
			fprintf(trainFile, "\ttr%d", i+1);
		fprintf(trainFile, "\n");
		for(i=0; i<(numFold-1)*dimFold; i++){
			fprintf(trainFile, "tr%d", i+1);	  
			for(j=0; j<(numFold-1)*dimFold; j++){
				if(i<=j)
					fprintf(trainFile, "\t%.4f", mat[i * length + j]);
				else
					fprintf(trainFile, "\t%.4f", mat[j * length + i]);
			}
			fprintf(trainFile, "\n");
		}  
	
		fprintf(trainTargetFile, "label\tclass\n");
		for(i=0; i<(numFold-1)*dimFold; i++)
			fprintf(trainTargetFile, "tr%d\t%d\n", i+1, target[i]);
		
		fprintf(testFile, "K<x,y>");  
		for(i=0; i<(numFold-1)*dimFold; i++)  
			fprintf(testFile, "\ttr%d", i+1);
		fprintf(testFile, "\n");
		for(i=(numFold-1)*dimFold; i<length; i++){
			fprintf(testFile, "te%d", i+1);	  
			for(j=0; j<(numFold-1)*dimFold; j++){
				if(i<=j)
					fprintf(testFile, "\t%.4f", mat[i * length + j]);
				else
					fprintf(testFile, "\t%.4f", mat[j * length + i]);
			}
			fprintf(testFile, "\n");
		}	
		
		fprintf(testTargetFile, "label\tclass\n");
		for(i=(numFold-1)*dimFold; i<length; i++)
			fprintf(testTargetFile, "te%d\t%d\n", i+1, target[i]);	
	}else
	{	
		fprintf(trainFile, "K<x,y>");
		fprintf(testFile, "K<x,y>");
		for(i=0; i<length; i++)
			if(i<indexTestFold*dimFold || i>=(indexTestFold+1)*dimFold){
				fprintf(trainFile, "\ttr%d", i+1);
				fprintf(testFile, "\ttr%d", i+1);
			}
		fprintf(trainFile, "\n");
		fprintf(testFile, "\n");
		for(i=0; i<length; i++){
			if(i<indexTestFold*dimFold || i>=(indexTestFold+1)*dimFold){
				fprintf(trainFile, "tr%d", i+1);	  
				for(j=0; j<length; j++)
					if(j<indexTestFold*dimFold || j>=(indexTestFold+1)*dimFold){
						if(i<=j)
							fprintf(trainFile, "\t%.4f", mat[i * length + j]);
						else
							fprintf(trainFile, "\t%.4f", mat[j * length + i]);
					}
				fprintf(trainFile, "\n");
			}else{
				fprintf(testFile, "te%d", i+1);	  
				for(j=0; j<length; j++)
					if(j<indexTestFold*dimFold || j>=(indexTestFold+1)*dimFold){
						if(i<=j)
							fprintf(testFile, "\t%.4f", mat[i * length + j]);
						else
							fprintf(testFile, "\t%.4f", mat[j * length + i]);
					}
				fprintf(testFile, "\n");				
				
			}
		}  
	
		fprintf(trainTargetFile, "label\tclass\n");
		fprintf(testTargetFile, "label\tclass\n");
		for(i=0; i<length; i++){
			if(i<indexTestFold*dimFold || i>=(indexTestFold+1)*dimFold)
				fprintf(trainTargetFile, "tr%d\t%d\n", i+1, target[i]);
			else
				fprintf(testTargetFile, "te%d\t%d\n", i+1, target[i]);
		}	
	}
}
	
float
TanimotoCore (graph_t* gp1, graph_t* gp2, int depth){
	int paths, paths_pA1, paths_pA2;
	pathArray_t* pA1=XMALLOC(pathArray_t,1);
	pathArray_t* pA2=XMALLOC(pathArray_t,1);
	
	initPathArray(pA1, 32);
    initPathArray(pA2, 32);
        
    /* MOLECOLA 1*/
    hnode_t* hiter = gp1->head;
    while(hiter) {
    	DFS_Search(gp1, hiter, depth, pA1, NO_MULT);	    	
    	hiter = list_next_entry(hnode_t, hiter);
    }
    
    
    /* MOLECOLA 2*/
    hiter = gp2->head;
    while(hiter) {
    	DFS_Search(gp2, hiter, depth, pA2, NO_MULT);	    	
    	hiter = list_next_entry(hnode_t, hiter);
    }
    
    /* RISULTATI */
    displayPathArray(pA1);
    displayPathArray(pA2);
    
    paths=commonPaths(pA1, pA2);
    paths_pA1=pA1->dim - pA1->free;
    paths_pA2=pA2->dim - pA2->free;
    printf("\nNumero path pA1; %d", paths_pA1);
    printf("\nNumero path pA2: %d", paths_pA2);
    printf("\nNumero path comuni: %d\n", paths);
	
    freePathArray(pA1);
    freePathArray(pA2);
    
    return (float) paths / (paths_pA1 + paths_pA2 - paths);
}

float
MinMaxCore (graph_t* gp1, graph_t* gp2, int depth){
	float min_max;
	pathArray_t* pA1=XMALLOC(pathArray_t,1);
    pathArray_t* pA2=XMALLOC(pathArray_t,1);
    
    initPathArray(pA1, 32);
    initPathArray(pA2, 32);
        
    /* MOLECOLA 1*/
    hnode_t* hiter = gp1->head;
    while(hiter) {
    	DFS_Search(gp1, hiter, depth, pA1, WITH_MULT);	    	
    	hiter = list_next_entry(hnode_t, hiter);
    }
    
    /* MOLECOLA 2*/
    hiter = gp2->head;
    while(hiter) {
    	DFS_Search(gp2, hiter, depth, pA2, WITH_MULT);	    	
    	hiter = list_next_entry(hnode_t, hiter);
    }
    
    /* RISULTATI */
    displayPathArray(pA1);
    printf("\n");
    displayPathArray(pA2);
    
    min_max=commonPathsWithMult(pA1, pA2);

    freePathArray(pA1);
    freePathArray(pA2);
    
    return min_max;
}

float
getAccuracy(FILE* fileTarget, FILE* fileResult){
	 char* buffer1=XMALLOC(char, 150); int buf1size=150;
	 char* buffer2=XMALLOC(char, 150); int buf2size=150;
	 char* p;
	 int target1; int target2;
	 int corretti=0; int totali=0;
	 
	 fgets(buffer1, buf1size, fileTarget);
	 
	 fgets(buffer2, buf2size, fileResult);
	 while(strstr(buffer2,"#") || strstr(buffer2,"K<x,y>"))
		 fgets(buffer2, buf2size, fileResult);
	 
	 fgets(buffer1, buf1size, fileTarget);
	 do{
		 totali++;
		 
		 if(strstr(buffer1,"-1"))
			 target1=-1;
		 else
			 target1=1;

		 p = strtok(buffer2, "\t");
		 p = strtok(NULL, "\t"); 
		 target2=strtol(p, 0, 10);
		 
		 if(target2==target1)
			 corretti++;		
		 
		 fgets(buffer1, buf1size, fileTarget);
	 }while(fgets(buffer2, buf2size, fileResult));
	
	 printf("\nCorretti: %d", corretti);
	 printf("\nTotali: %d", totali);
	 
	 XFREE(buffer1);
	 XFREE(buffer2);	 
	 return (float)corretti/totali;
}

float
getLeaveOneOut(FILE* file){
	 int bufsize=110; char* buffer=XMALLOC(char, bufsize);
	 int i; char* p; int target1; int target2;
	 int corretti=0; int totali=0;
	 
	 while(fgets(buffer, bufsize, file)){
		 if(!strstr(buffer,"#") && !strstr(buffer,"K<x,y>")){
			  //totali++;
			  //printf("\n%s", buffer);
			  p = strtok(buffer, "\t");
			  i=1;
			  while (p != NULL) {
				  if(i==2)
					  target1=strtol(p, 0, 10);
					  
				  if(i==6)
					  target2=strtol(p, 0, 10);
				  
				  i++;
				  p = strtok(NULL, "\t");
			  }
			  
			  if(target2==1 || target2==-1){
				  totali++;
				  if(target1==target2)
					  corretti++;  
			  }
		 }	 		 
	 }
	 
	 XFREE(buffer);
	 
	 printf("\nCorretti: %d", corretti);
	 printf("\nTotali: %d", totali);
	 
	 return (float) corretti/totali;
}
