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
#include "graph.h"
#include "list.h"
#include "graph_function.h"
#include "path_array.h"
#include "kernel.h"
#include "parser.h"
#include "getopt.h"

void
usage ()
{
  printf("\n");
  printf("Hope-K: The Hope Kernel\n");
  printf("Usage: hope-k [OPTION] core\n");
  printf("Options:\n\t-k : training_file\n\t-t : target_training_file\n\t-c : resultSVMGist_file\n\t-a : target_file\n\t-i : info\n\t-h : help\n\t-v : verbose\n\t-d : value of depth\n");
  printf("available core: tanimoto, minmax\n");
  printf("default core: tanimoto\n");
  printf("\n");
}

void
info ()
{
  printf("\n");
  printf("Hope-K: The Hope Kernel\n");
  //printf("Version: %s\n", VERSION);
  printf("Copyright (C)  2008\n");
  printf("Michele Caini <skypjack@gmail.com>, Johnny Giuntini <giuntinijohnny@interfree.it>\n");
  printf("\n");
}

int
main (int argc, char** argv)
{
  char *kfile, *tfile, *cfile, *tafile, *lfile;
  size_t ksize, tsize, iter, depth;
 // FILE *kout, *tout;
  char opt;
  graph_t **gtrain; int* target;
  FILE* fileTrain, *fileResult, *fileTarget;
  core_t core;
  gtrain = NULL;
  lfile = tafile = cfile = kfile = NULL;
  depth = 3;
  CLEAR_FLAGS();
  SET_RUNNABLE();
  ksize = tsize = 0;
  core = TanimotoCore;
  while((opt = getopt(argc, argv, "mvihd:k:t:c:a:l:")) != -1) {
    switch(opt){
    case 'v':
      SET_VERBOSE();
      break;
    case 'i':
      SET_INFO();
      break;
    case 'd':
      depth = strtol((const char*) optarg, 0, 10);
      break;
    case 'k':
      SET_K_FILE();
      kfile = optarg;
      break;
    case 't':
      SET_T_FILE();
      tfile = optarg;
      break;      
    case 'c':
      SET_C_FILE();
      cfile = optarg;
      break;
    case 'a':
      SET_TA_FILE();
      tafile = optarg;
      break;
    case 'l':
      SET_L_FILE();
      lfile = optarg;
      break;
    case 'h':
      SET_HELP();
      break;
      
    case 'm':
      SET_PROVA_MINMAX();
      break;
      
    default:
      SET_HELP();
    }
  }
  if(HELP()) {
    usage();
    CLEAR_FLAGS();
  }
  if(INFO()) {
    info();
    CLEAR_FLAGS();
  }
  if(optind < argc) {
    if(!strcmp(argv[optind], "tanimoto")) {
      printf("tanimoto kernel\n");    	
    } else if(!strcmp(argv[optind], "minmax")) {
      printf("minmax kernel\n");
      core = MinMaxCore;
    } else
      printf("unavailable core ...\ndefault core: tanimoto\n");
  } //else
    //printf("default core: tanimoto\n");
  if(RUNNABLE()) {
    if(K_FILE() && T_FILE()) {
      if((fileTrain = fopen(kfile, "r")) && (fileTarget = fopen(tfile, "r"))) {
    	  gtrain = parse(fileTrain, &ksize);
    	  fclose(fileTrain);
    	  
    	  target=parseTarget(fileTarget, ksize);
    	  fclose(fileTarget);
    	  
    	  if(target==0)
    		  printf("\ncheck training file and target file");
    	  else
    		  FoldCrossValidation(core, gtrain, target, ksize, depth);

    	  for(iter = 0; iter < ksize; iter++)
    		  free_graph(gtrain[iter]);

    	  XFREE(gtrain);
    	  XFREE(target);  
      	}else fatal("unable to open training file");
    }
    
    if(TA_FILE() && C_FILE()) {
      fileResult = fopen(cfile, "r");
      fileTarget = fopen(tafile, "r");
      printf("\nAccuratezza: %f\n", getAccuracy(fileTarget, fileResult));
      fclose(fileResult);
      fclose(fileTarget);
    }
    /*
    if(L_FILE()) {
      file = fopen(lfile, "r");
      printf("\nLeaveOneOut: %f\n", getLeaveOneOut(file));
      fclose(file);
    }
    */

    if(K_FILE() && !T_FILE()) {
      if((fileTrain = fopen(kfile, "r"))) {
    	  gtrain = parse(fileTrain, &ksize);
    	  fclose(fileTrain);

    	print_graph(gtrain[0]);
    	print_graph(gtrain[1]);
    	  
    	  if(gtrain==0)
    		  printf("\ncheck training file");
    	  else
    		  WriteKernelMatrix(core, gtrain, ksize, depth);
    		  
    	  for(iter = 0; iter < ksize; iter++)
    		  free_graph(gtrain[iter]);

    	  XFREE(gtrain);  
      	}else fatal("unable to open training file");
    }
    
    if(PROVA_MINMAX()) {
    	printf("\nPROVA MIN MAX\n");
    	
    	fileTrain = fopen("./minmax.mol2", "r");
    	gtrain = parse(fileTrain, &ksize);
    	//print_graph(gtrain[0]);
    	//print_graph(gtrain[1]);
    	
    	print_graph(gtrain[0]);
    	print_graph(gtrain[1]);
    	
    	printf("\nMinMax: %f", MinMaxCore(gtrain[0], gtrain[1], 3));
    	//printf("\nTanimoto: %f", TanimotoCore(gtrain[0], gtrain[1], 3));
    	
    	fclose(fileTrain);
     }

    
  } //else usage();
  return EXIT_SUCCESS;
}
