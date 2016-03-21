#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned char map[256][256];
int queX[70000], queY[70000]; int queL = 0, queR = 1;

int dirX[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
int dirY[8] = {-1, 0, 1, 1, 1, 0, -1, -1};

// ***
// * Here is the Advance Clock Algorithm 
unsigned int ps[16]; 
int pp = 0;
int pt[256];

int hit = 0, miss = 0;

unsigned char map_visit(int x, int y) {
	static int pre_id = -1;
	int id = ((y>>4)<<4)|(x>>4);
	if (id != pre_id)
		printf("visit id=%d ", id);
	hit += (pt[id] != -1);
	miss += (pt[id] == -1);
	if (pt[id] == -1) {
		printf("miss! ");
		while (ps[pp]&3) {
			ps[pp] = ((ps[pp]>>2)<<2) | ((ps[pp]&3)==3 ? 1 : 0);
			pp = (pp==15 ? 0 : pp+1);
		}
		pt[ps[pp]>>8] = -1;
		pt[id] = pp;
		ps[pp] = (id<<8)|2;
		pp = (pp==15 ? 0 : pp+1);
	}
	ps[pt[id]] |= 2;
	int i = 0;
	if (id != pre_id) {
		for ( ; i < 16 ; i++)
			if (i == pp) 
				printf(">%d|%d%d< ", ps[i]>>8, (ps[i]>>1)&1, ps[i]&1);	
			else
				printf("%d|%d%d ", ps[i]>>8, (ps[i]>>1)&1, ps[i]&1);	
		printf("\n");
	}
	pre_id = id;
	return map[x][y];
}

void map_modify(int x, int y, unsigned char val) {
	static int pre_id = -1;
	int id = ((y>>4)<<4)|(x>>4);
	if (id != pre_id)
		printf("modify id=%d ", id);
	hit += (pt[id] != -1);
	miss += (pt[id] == -1);
	if (pt[id] == -1) {
		printf("miss! ");
		while (ps[pp]&3) {
			ps[pp] = ((ps[pp]>>2)<<2) | ((ps[pp]&3)==3 ? 1 : 0);
			pp = (pp==15 ? 0 : pp+1);
		}
		pt[ps[pp]>>8] = -1;
		pt[id] = pp;
		ps[pp] = (id<<8)|3;
		pp = (pp==15 ? 0 : pp+1);
	}
	ps[pt[id]] |= 3;
	int i = 0;
	if (id != pre_id) {
		for ( ; i < 16 ; i++) 
			if (i == pp) 
				printf(">%d|%d%d< ", ps[i]>>8, (ps[i]>>1)&1, ps[i]&1);	
			else
				printf("%d|%d%d ", ps[i]>>8, (ps[i]>>1)&1, ps[i]&1);	
		printf("\n");
	}
	pre_id = id;
	map[x][y] = val;
}
// * The rest of code is used to test the performance.
// *** 

unsigned char map_next(int dir) {
	int nx = queX[queL]+dirX[dir];
	int ny = queY[queL]+dirY[dir];
	//printf("map_next queL=%d x=%d y=%d nx=%d ny=%d\n", queL, queX[queL], queY[queL], nx, ny);
	return (nx >= 0 && nx < 256 && ny >= 0 && ny < 256 && !map_visit(nx, ny));
}

void set_quene_next(int dir) {
	int nx = queX[queL]+dirX[dir];
	int ny = queY[queL]+dirY[dir];
	//printf("set_next queL=%d x=%d y=%d nx=%d ny=%d\n", queL, queX[queL], queY[queL], nx, ny);
	//assert(nx >= 0 && nx < 256 && ny >= 0 && ny < 256);
	map_modify(nx, ny, 1);
	queX[queR] = nx;
	queY[queR++] = ny;
}

int main() {
	srand(time(0));
	int i = 0, j = 0, k = 0, c = 0;
	queX[0] = 128; queY[0] = 128;
	map[128][128] = 1;
	for (i = 0 ; i < 256 ; i++)
		pt[i] = -1;
	for (i = 0 ; i < 16 ; i++) 
		ps[i] = (i<<8), pt[i] = i;
	for (i = 0 ; i < 256 ; i++)
		for (j = 0 ; j < 256 ; j++)
			map_modify(i, j, 0);
	for (; queL < 65536 && queL != queR ; queL++) {
		//printf("queL=%d queX=%d queY=%d\n", queL, queX[queL], queY[queL]);
		for (i = 0 ; i < 2 ; i++) {
			for (c = rand()%4 ; c ; c--) {
				for (j = 0 ; j < 8 ; j++) {
					k = (k==7 ? 0 : k+1);
					if (map_next(k)) break;
				} 
				if (j >= 8) break;
			}
			if (c) break;
			set_quene_next(k);
		}
	}
	printf("hit = %d miss = %d miss_rate=%.4lf\n", hit, miss, (double)(miss)/(hit+miss));
}
