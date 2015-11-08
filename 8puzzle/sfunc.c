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
#include "sfunc.h"
#include "dqueue.h"
#include "heap.h"
#include "hash.h"
#include "map.h"
#include "iofunc.h"

/*
 * Funzione di controllo di appartenenza alla
 * stessa orbita e quindi di esistenza della
 * soluzione.
 */
unsigned short int
chkblng(unsigned int* iconf)
{
	unsigned int iter;
	unsigned int cnt;
	unsigned int isum;
	unsigned int fsum;
	unsigned int fconf[9] =
	{0, 1, 2, 3, 4, 5, 6, 7, 8};
	isum = 0;
	fsum = 0;
	for(iter = 0; iter < 9; iter++) {
		for(cnt = iter + 1; cnt < 9; cnt++) {
			if((iconf[iter] != 0) && (iconf[cnt] != 0) && (iconf[iter] > iconf[cnt]))
				isum++;
			if((fconf[iter] != 0) && (fconf[cnt] != 0) && (fconf[iter] > fconf[cnt]))
				fsum++;
		}
	}
	return (((isum % 2) == (fsum % 2)) ? 1 : 0);
}

/*
 * Funzione di controllo raggiungimento goal.
 */
unsigned short int
chkgoal(unsigned int* iconf)
{
	unsigned int iter;
	unsigned short int flag;
	unsigned int fconf[9] =
	{0, 1, 2, 3, 4, 5, 6, 7, 8};
	flag = 0;
	for(iter = 0; (iter <  9) && (!flag); iter++)
		if(iconf[iter] != fconf[iter]) flag = !flag;
	return !flag;
}

/*
 * Funzione per lo swap fra elementi su un vettore.
 */
void
swap(unsigned int* vect, unsigned int apos, unsigned int bpos)
{
	unsigned int tmp;
	tmp = vect[apos];
	vect[apos] = vect[bpos];
	vect[bpos] = tmp;
}

/*
 * Funzione controllo possibilita' spostamento su direzione.
 */
unsigned short int
swpdir(unsigned int* conf, dir_t dir)
{
	unsigned int pos;
	unsigned short int flag;
	for(pos = 0; (pos < 9) && (conf[pos] != 0); pos++);
	flag = conf[pos];
	if(!flag) switch(dir) {
		case UP:
			if(((pos - (pos % 3)) / 3) != 0) {
				flag = 1;
				swap(conf, pos, pos - 3);
			} else flag = 0;
			break;
		case DOWN:
			if((pos + ((9 - pos - 1) % 3) + 1) != 9) {
				flag = 1;
				swap(conf, pos, pos + 3);
			} else flag = 0;
			break;
		case LEFT:
			if((pos % 3) != 0) {
				flag = 1;
				swap(conf, pos, pos - 1);
			} else flag = 0;
			break;
		case RIGHT:
			if(((pos + 1) % 3) != 0) {
				flag = 1;
				swap(conf, pos, pos + 1);
			} else flag = 0;
			break;
		default:
			flag = 0;
	} else flag = 0;
	return flag;
}

/*
 * Funzione per il conteggio dei passi necessari
 * alla risoluzione di un dato problema (se risolvibile).
 */
unsigned int
stepcnt(map_t* mp, unsigned long int key)
{
	unsigned long int pk;
	unsigned int step;
	step = 0;
	pk = key;
	if(key != 0) {
		pk = key;
		do {
			step++;
			pk = find(mp, pk);
		} while(pk != 0);
		step--;
	}
	return step;
}

/*
 * Funzione di analisi su set di dati, con
 * possibilità di definire la funzione di ricerca,
 * di peso e l'euristica volute.
 */
void
dtest(const char* desc, FILE* fref, sfp_t sfp, wfp_t wfp, hfp_t hfp, unsigned short int verbose)
{
	double sum;
	double slsol;
	unsigned int lsol;
	unsigned int step;
	unsigned int cnt;
	unsigned int skip;
	unsigned int iconf[9];
	time_t stime = 0;
	time_t etime = 0;
	if((fref != 0) && (sfp != 0)) {
		sum = 0;
		step = 0;
		cnt = 0;
		skip = 0;
		slsol = 0;
		printf("%s", desc);
		if(!verbose) stime = time(0);
		while(parse(fref, iconf)) {
			if((step = (*sfp)(iconf, wfp, hfp, &lsol, verbose)) != 0) {
				sum += step;
				slsol += lsol;
				cnt++;
			} else {
				skip++;
				if(verbose) printf("Skip #%d\n", cnt + skip);
			}
		}
		if(!verbose) etime = time(0);
		printf("Average (node): %f\nAverage (solution): %f\nPuzzles: %d/%d\n", sum / cnt, slsol / cnt, cnt, cnt + skip);
		if(!verbose) printf("Elapsed: %f seconds\n", difftime(etime, stime));
	}
}

/*
 * Funzione di ricerca Breadth-First.
 */
unsigned short int
sfbf(unsigned int* iconf, wfp_t wfp, hfp_t hfp, unsigned int* lsol, unsigned short int verbose)
{
	unsigned short int ret;
	unsigned short int goal;
	unsigned short int lvl;
	unsigned long int ptk;
	unsigned long int tk;
	unsigned int buf[9];
	unsigned int iter;
	unsigned int step;
	map_t*  mp;
	hash_t* hp;
	dqueue_t* dqp;
	if((iconf != 0) && (lsol != 0)) {
		hp = createhash();
		mp = createmap();
		dqp = createdqueue();
		if((chkblng(iconf)) && (hp != 0) && (mp != 0) && (dqp != 0)) {
			ret = 1;
			step = 1;
			lvl = 0;
			for(iter = 0; iter < 9; iter++)
				buf[iter] = iconf[iter];
			ptk = 0;
			if((tk = insert(hp, buf)) != 0) ret = add(mp, tk, ptk);
			else ret = !ret;
			goal = chkgoal(buf);
			while((ret) && (!goal)) {
				ptk = tk;
				if((ret) && (!goal) && (swpdir(buf, UP))) {
					if(!search(hp, buf)) {
						tk = insert(hp, buf);
						if(tk) ret = add(mp, tk, ptk);
						else ret = 0;
						goal = chkgoal(buf);
						if(!goal) ret = pushrear(dqp, buf, lvl + 1);
					}
					swpdir(buf, DOWN);
				}
				if((ret) && (!goal) && (swpdir(buf, DOWN))) {
					if(!search(hp, buf)) {
						tk = insert(hp, buf);
						if(tk) ret = add(mp, tk, ptk);
						else ret = 0;
						goal = chkgoal(buf);
						if(!goal) ret = pushrear(dqp, buf, lvl + 1);
					}
					swpdir(buf, UP);
				}
				if((ret) && (!goal) && (swpdir(buf, LEFT))) {
					if(!search(hp, buf)) {
						tk = insert(hp, buf);
						if(tk) ret = add(mp, tk, ptk);
						else ret = 0;
						goal = chkgoal(buf);
						if(!goal) ret = pushrear(dqp, buf, lvl + 1);
					}
					swpdir(buf, RIGHT);
				}
				if((ret) && (!goal) && (swpdir(buf, RIGHT))) {
					if(!search(hp, buf)) {
						tk = insert(hp, buf);
						if(tk) ret = add(mp, tk, ptk);
						else ret = 0;
						goal = chkgoal(buf);
						if(!goal) ret = pushrear(dqp, buf, lvl + 1);
					}
					swpdir(buf, LEFT);
				}
				if(!goal) {
					if(ret) ret = (((lvl = pop(dqp, buf)) != 0) ? 1 : 0);
					if((tk = search(hp, buf)) == 0) ret = !ret;
					step++;
				}
			}
			if(ret) {
				if(verbose) {
					splash(hp, mp, tk);
					printf("Press any key to continue...\n");
					getchar();
				}
				*lsol = stepcnt(mp, tk);
			}
		} else ret = 0;
		freedqueue(dqp);
		freemap(mp);
		freehash(hp);
	} else ret = 0;
	return (ret ? step : 0);
}

/*
 * Funzione di ricerca Iterative Depth-First.
 */
unsigned short int
sfdf(unsigned int* iconf, wfp_t wfp, hfp_t hfp, unsigned int* lsol, unsigned short int verbose)
{
	unsigned short int ret;
	unsigned short int empty;
	unsigned short int goal;
	unsigned short int lvl;
	unsigned long int ptk;
	unsigned long int tk;
	unsigned int buf[9];
	unsigned int iter;
	unsigned int cnt;
	unsigned int step;
	map_t*  mp;
	hash_t* hp;
	dqueue_t* dqp;
	if((iconf != 0) && (lsol != 0)) {
		hp = createhash();
		mp = createmap();
		dqp = createdqueue();
		if((chkblng(iconf)) && (hp != 0) && (mp != 0) && (dqp != 0)) {
			cnt = 0;
			ret = 1;
			goal = 0;
			while((ret) && (!goal)) {
				emptyhash(hp);
				emptydqueue(dqp);
				emptymap(mp);
				cnt++;
				empty = 1;
				step = 1;
				lvl = 0;
				for(iter = 0; iter < 9; iter++)
					buf[iter] = iconf[iter];
				ptk = 0;
				if((tk = insert(hp, buf)) != 0) ret = add(mp, tk, ptk);
				else ret = !ret;
				goal = chkgoal(buf);
				while((ret) && (!goal) && (empty)) {
					ptk = tk;
					if((ret) && (!goal) && (swpdir(buf, UP))) {
						if(!search(hp, buf)) {
							tk = insert(hp, buf);
							if(tk) ret = add(mp, tk, ptk);
							else ret = 0;
							goal = chkgoal(buf);
							if((!goal) && (lvl + 1 < cnt)) ret = push(dqp, buf, lvl + 1);
						}
						swpdir(buf, DOWN);
					}
					if((ret) && (!goal) && (swpdir(buf, DOWN))) {
						if(!search(hp, buf)) {
							tk = insert(hp, buf);
							if(tk) ret = add(mp, tk, ptk);
							else ret = 0;
							goal = chkgoal(buf);
							if((!goal) && (lvl + 1 < cnt)) ret = push(dqp, buf, lvl + 1);
						}
						swpdir(buf, UP);
					}
					if((ret) && (!goal) && (swpdir(buf, LEFT))) {
						if(!search(hp, buf)) {
							tk = insert(hp, buf);
							if(tk) ret = add(mp, tk, ptk);
							else ret = 0;
							goal = chkgoal(buf);
							if((!goal) && (lvl + 1 < cnt)) ret = push(dqp, buf, lvl + 1);
						}
						swpdir(buf, RIGHT);
					}
					if((ret) && (!goal) && (swpdir(buf, RIGHT))) {
						if(!search(hp, buf)) {
							tk = insert(hp, buf);
							if(tk) ret = add(mp, tk, ptk);
							else ret = 0;
							goal = chkgoal(buf);
							if((!goal) && (lvl + 1 < cnt)) ret = push(dqp, buf, lvl + 1);
						}
						swpdir(buf, LEFT);
					}
					if(!goal) {
						if(ret) empty = (((lvl = pop(dqp, buf)) != 0) ? 1 : 0);
						if((tk = search(hp, buf)) == 0) ret = !ret;
						step++;
					}
				}
			}
			if(ret) {
				if(verbose) {
					splash(hp, mp, tk);
					printf("Press any key to continue...\n");
					getchar();
				}
				*lsol = stepcnt(mp, tk);
			}
		} else ret = 0;
		freedqueue(dqp);
		freemap(mp);
		freehash(hp);
	} else ret = 0;
	return (ret ? step : 0);
}

/*
 * Funzione di ricerca A*.
 */
unsigned short int
sfas(unsigned int* iconf, wfp_t wfp, hfp_t hfp, unsigned int* lsol, unsigned short int verbose)
{
	unsigned short int ret;
	unsigned short int goal;
	unsigned short int lvl;
	unsigned long int ptk;
	unsigned long int tk;
	unsigned int buf[9];
	unsigned int iter;
	unsigned int step;
	map_t*  mp;
	hash_t* hp;
	heap_t* heap;
	if((iconf != 0) && (wfp != 0) && (hfp != 0) && (lsol != 0)) {
		hp = createhash();
		mp = createmap();
		heap = createheap();
		if((chkblng(iconf)) && (hp != 0) && (mp != 0) && (heap != 0)) {
			ret = 1;
			step = 1;
			lvl = 0;
			for(iter = 0; iter < 9; iter++)
				buf[iter] = iconf[iter];
			ptk = 0;
			if((tk = insert(hp, buf)) != 0) ret = add(mp, tk, ptk);
			else ret = !ret;
			goal = chkgoal(buf);
			while((ret) && (!goal)) {
				ptk = tk;
				if((ret) && (!goal) && (swpdir(buf, UP))) {
					if(!search(hp, buf)) {
						tk = insert(hp, buf);
						if(tk) ret = add(mp, tk, ptk);
						else ret = 0;
						goal = chkgoal(buf);
						if(!goal) ret = put(heap, buf, lvl + 1, (*wfp)(buf, lvl + 1, hfp));
					} else setif(heap, buf, lvl + 1, (*wfp)(buf, lvl, hfp));
					swpdir(buf, DOWN);
				}
				if((ret) && (!goal) && (swpdir(buf, DOWN))) {
					if(!search(hp, buf)) {
						tk = insert(hp, buf);
						if(tk) ret = add(mp, tk, ptk);
						else ret = 0;
						goal = chkgoal(buf);
						if(!goal) ret = put(heap, buf, lvl + 1, (*wfp)(buf, lvl + 1, hfp));
					} else setif(heap, buf, lvl + 1, (*wfp)(buf, lvl, hfp));
					swpdir(buf, UP);
				}
				if((ret) && (!goal) && (swpdir(buf, LEFT))) {
					if(!search(hp, buf)) {
						tk = insert(hp, buf);
						if(tk) ret = add(mp, tk, ptk);
						else ret = 0;
						goal = chkgoal(buf);
						if(!goal) ret = put(heap, buf, lvl + 1, (*wfp)(buf, lvl + 1, hfp));
					} else setif(heap, buf, lvl + 1, (*wfp)(buf, lvl, hfp));
					swpdir(buf, RIGHT);
				}
				if((ret) && (!goal) && (swpdir(buf, RIGHT))) {
					if(!search(hp, buf)) {
						tk = insert(hp, buf);
						if(tk) ret = add(mp, tk, ptk);
						else ret = 0;
						goal = chkgoal(buf);
						if(!goal) ret = put(heap, buf, lvl + 1, (*wfp)(buf, lvl + 1, hfp));
					} else setif(heap, buf, lvl + 1, (*wfp)(buf, lvl, hfp));
					swpdir(buf, LEFT);
				}
				if(!goal) {
					if(ret) ret = (((lvl = get(heap, buf)) != 0) ? 1 : 0);
					if((tk = search(hp, buf)) == 0) ret = !ret;
					step++;
				}
			}
			if(ret) {
				if(verbose) {
					splash(hp, mp, tk);
					printf("Press any key to continue...\n");
					getchar();
				}
				*lsol = stepcnt(mp, tk);
			}
		} else ret = 0;
		freeheap(heap);
		freemap(mp);
		freehash(hp);
	} else ret = 0;
	return (ret ? step : 0);
}

/*
 * Funzione di ricerca IDA*.
 */
unsigned short int
sfidas(unsigned int* iconf, wfp_t wfp, hfp_t hfp, unsigned int* lsol, unsigned short int verbose)
{
	unsigned short int ret;
	unsigned short int empty;
	unsigned short int goal;
	unsigned short int lvl;
	unsigned long int ptk;
	unsigned long int tk;
	unsigned int buf[9];
	unsigned int iter;
	unsigned int cnt;
	unsigned int step;
	double weight;
	map_t*  mp;
	hash_t* hp;
	dqueue_t* dqp;
	if((iconf != 0) && (wfp != 0) && (hfp != 0) && (lsol != 0)) {
		hp = createhash();
		mp = createmap();
		dqp = createdqueue();
		if((chkblng(iconf)) && (hp != 0) && (mp != 0) && (dqp != 0)) {
			cnt = 0;
			ret = 1;
			goal = 0;
			while((ret) && (!goal)) {
				emptyhash(hp);
				emptydqueue(dqp);
				emptymap(mp);
				cnt++;
				empty = 1;
				step = 1;
				lvl = 0;
				for(iter = 0; iter < 9; iter++)
					buf[iter] = iconf[iter];
				ptk = 0;
				if((tk = insert(hp, buf)) != 0) ret = add(mp, tk, ptk);
				else ret = !ret;
				goal = chkgoal(buf);
				while((ret) && (!goal) && (empty)) {
					ptk = tk;
					if((ret) && (!goal) && (swpdir(buf, UP))) {
						weight = (*wfp)(buf, lvl + 1, hfp);
						if(!search(hp, buf)) {
							tk = insert(hp, buf);
							if(tk) ret = add(mp, tk, ptk);
							else ret = 0;
							goal = chkgoal(buf);
							if((!goal) && (weight < cnt)) ret = push(dqp, buf, lvl + 1);
						}
						swpdir(buf, DOWN);
					}
					if((ret) && (!goal) && (swpdir(buf, DOWN))) {
						weight = (*wfp)(buf, lvl + 1, hfp);
						if(!search(hp, buf)) {
							tk = insert(hp, buf);
							if(tk) ret = add(mp, tk, ptk);
							else ret = 0;
							goal = chkgoal(buf);
							if((!goal) && (weight < cnt)) ret = push(dqp, buf, lvl + 1);
						}
						swpdir(buf, UP);
					}
					if((ret) && (!goal) && (swpdir(buf, LEFT))) {
						weight = (*wfp)(buf, lvl + 1, hfp);
						if(!search(hp, buf)) {
							tk = insert(hp, buf);
							if(tk) ret = add(mp, tk, ptk);
							else ret = 0;
							goal = chkgoal(buf);
							if((!goal) && (weight < cnt)) ret = push(dqp, buf, lvl + 1);
						}
						swpdir(buf, RIGHT);
					}
					if((ret) && (!goal) && (swpdir(buf, RIGHT))) {
						weight = (*wfp)(buf, lvl + 1, hfp);
						if(!search(hp, buf)) {
							tk = insert(hp, buf);
							if(tk) ret = add(mp, tk, ptk);
							else ret = 0;
							goal = chkgoal(buf);
							if((!goal) && (weight < cnt)) ret = push(dqp, buf, lvl + 1);
						}
						swpdir(buf, LEFT);
					}
					if(!goal) {
						if(ret) empty = (((lvl = pop(dqp, buf)) != 0) ? 1 : 0);
						if((tk = search(hp, buf)) == 0) ret = !ret;
						step++;
					}
				}
			}
			if(ret) {
				if(verbose) {
					splash(hp, mp, tk);
					printf("Press any key to continue...\n");
					getchar();
				}
				*lsol = stepcnt(mp, tk);
			}
		} else ret = 0;
		freedqueue(dqp);
		freemap(mp);
		freehash(hp);
	} else ret = 0;
	return (ret ? step : 0);
}
