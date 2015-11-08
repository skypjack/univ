#include <stdlib.h>
#include <stdio.h>
#include "ngame.h"

void
usage(const char* filename)
{
	printf("Usage: %s #token #step\n", filename);
}

int
main(int argc, char** argv)
{
	unsigned int token;
	unsigned int step;
	int val;
	player pl;
	pl = A;
	if(argc != 3) usage(argv[0]);
	else {
		token = strtol(argv[1], NULL, 10);
		step = strtol(argv[2], NULL, 10);
		printf("Gioco a %d gettoni, con passo massimo pari a %d.\n", token, step);
		printf("> Inizio io... Ti rompe?\n");
		while(token > 0) {
			printf("> Sono rimasti %d gettoni: ", token);
			val = checkfor(A, token, step);
			if(val == -1) val = 1;
			printf("Prendo %d gettoni...\n", val);
			if((token -= val) == 0) pl = B;
			else {
				printf("> Sono rimasti %d gettoni: ", token);
				printf("Quanti gettoni prendi?\n");
				do {
					printf("> ");
					scanf("%d", &val);
				} while((val <= 0) || (val > token) || (val > step));
				if((token -= val) == 0) pl = A;
			}
		}
		if(pl == A) printf("> Ho vinto io... Dubitavi?\n");
		else printf("> Hai vinto... Complimenti!\n");
	}
	return 0;
}
