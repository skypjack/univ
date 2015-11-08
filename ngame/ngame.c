#include "ngame.h"

player
checkby(player pl, int tk, int sp)
{
	int cnt;
	if(tk == 0) return pl;
	else {
		cnt = 1;
		while((cnt <= sp) && (tk - cnt > 0)) {
			if(checkby(!pl, tk - cnt, sp) == pl)
				return pl;
			else cnt++;
		}
	}
	return !pl;
}

int
checkfor(player pl, int tk, int sp)
{
	int mm;
	int cnt;
	if((tk != 0) && (sp != 0)) {
		mm = -1;
		cnt = 1;
		while((cnt <= sp) && (tk - cnt > 0) && (mm == -1)) {
				if(checkby(!pl, tk - cnt, sp) == pl)
					mm = cnt;
				else cnt++;
		}
	} else return mm = -1;
	return mm;
}
