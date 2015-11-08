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

#include "common.h"
#include "pgenerator.h"
#include "heuristic.h"
#include "wfunc.h"
#include "sfunc.h"

void
usage(const char* filename)
{
	printf(" Usage: %s -<option> [<puzzle-filename>]\n\n", filename);
	printf(" Option:\n");
	printf("\t-g #\tPuzzle's file Generator\n\t\t(Number of examples = 10 ^ Number of figures)\n");
	printf("\t-v \tVerbose Mode\n");
	printf("\t-b\tBreadth First Search\n");
	printf("\t-d\tIterative Depth First Search\n");
	printf("\t-a\tA* Search\n");
	printf("\t-i\tIDA* Search\n");
	printf("\t-h\tthis help\n");
	printf(" Goal-Configuration is: [0][1][2][3][4][5][6][7][8]\n");
	printf("\n\n[8puzzle, v. %s]\n", VERSION);
}

int
main(int argc, char** argv)
{
	FILE* pfn;
	char opt;
	unsigned int seed = 0;
	INIT_FLAGS();
	while((opt = getopt(argc, argv, "g:vbdaih")) != -1) {
		switch(opt) {
			case 'g':
				SET_GEN();
				seed = strtol(optarg, 0, 10);
				break;
			case 'v':
				SET_VERBOSE();
				break;
			case 'b':
				SET_BF();
				break;
			case 'd':
				SET_DF();
				break;
			case 'a':
				SET_A_STAR();
				break;
			case 'i':
				SET_IDA_STAR();
				break;
			case 'h':
				SET_HELP();
				break;
		}
	}
	if(HELP_REQ()) {
		usage(argv[0]);
	} else {
		if(GEN_REQ()) pgen(seed);
		if(optind < argc) pfn = fopen(argv[optind], "r");
		else pfn = fopen("./puzzle", "r");
		if(pfn != 0) {
			if(BF()) {
				fseek(pfn, SEEK_SET, 0);
				dtest("\nBreadth First Search\n", pfn, &sfbf, 0, 0, VERBOSE());
				printf("\nPress any key to continue...\n");
				getchar();
			}
			if(DF()) {
				fseek(pfn, SEEK_SET, 0);
				dtest("\nIterative Depth First Search\n", pfn, &sfdf, 0, 0, VERBOSE());
				printf("\nPress any key to continue...\n");
				getchar();
			}
			if(A_STAR()) {
				fseek(pfn, SEEK_SET, 0);
				dtest("\nA*/STD WFunc/STD Heu - Search\n", pfn, &sfas, &wfstd, &heustd, VERBOSE());
				printf("\nPress any key to continue...\n");
				getchar();
				// fseek(pfn, SEEK_SET, 0);
				// dtest("\nA*/RevLvl WFunc/STD Heu - Search\n", pfn, &sfas, &wfwgt, &heustd, VERBOSE());
				// printf("\nPress any key to continue...\n");
				// getchar();
				fseek(pfn, SEEK_SET, 0);
				dtest("\nA*/STD WFunc/Man Heu - Search\n", pfn, &sfas, &wfstd, &heuman, VERBOSE());
				printf("\nPress any key to continue...\n");
				getchar();
				// fseek(pfn, SEEK_SET, 0);
				// dtest("\nA*/RevLvl WFunc/Man Heu - Search\n", pfn, &sfas, &wfwgt, &heuman, VERBOSE());
				// printf("\nPress any key to continue...\n");
				// getchar();
			}
			if(IDA_STAR()) {
				fseek(pfn, SEEK_SET, 0);
				dtest("\nIDA*/STD WFunc/STD Heu - Search\n", pfn, &sfidas, &wfstd, &heustd, VERBOSE());
				printf("\nPress any key to continue...\n");
				getchar();
				// fseek(pfn, SEEK_SET, 0);
				// dtest("\nIDA*/RevLvl WFunc/STD Heu - Search\n", pfn, &sfidas, &wfwgt, &heustd, VERBOSE());
				// printf("\nPress any key to continue...\n");
				// getchar();
				fseek(pfn, SEEK_SET, 0);
				dtest("\nIDA*/STD WFunc/Man Heu - Search\n", pfn, &sfidas, &wfstd, &heuman, VERBOSE());
				printf("\nPress any key to continue...\n");
				getchar();
				// fseek(pfn, SEEK_SET, 0);
				// dtest("\nIDA*/RevLvl WFunc/Man Heu - Search\n", pfn, &sfidas, &wfwgt, &heuman, VERBOSE());
				// printf("\nPress any key to continue...\n");
				// getchar();
			}
			fclose(pfn);
		}
	}
	return 0;
}
