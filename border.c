#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 1000
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct color {
	short r;
	short g;
	short b;
} color; 

short bw[MAXN][MAXN];
short res[MAXN][MAXN];

void diff(FILE *img, short w, short h) {
	color in[h][w];
	int i, j;
	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++) {
			fscanf(img, "%d", &in[i][j].r);
			fscanf(img, "%d", &in[i][j].g);
			fscanf(img, "%d", &in[i][j].b);
		}
	}
	short cnt = 0;
	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++) {
			cnt = 0;
			if(i > 0) {
				bw[i][j] += (abs(in[i][j].r - in[i-1][j].r) + abs(in[i][j].g - in[i-1][j].g) + abs(in[i][j].b - in[i-1][j].b))/3;
				cnt++;
			}
			if(i < h-1) {
				bw[i][j] += (abs(in[i][j].r - in[i+1][j].r) + abs(in[i][j].g - in[i+1][j].g) + abs(in[i][j].b - in[i+1][j].b))/3;
				cnt++;
			}
			if(j > 0) {
				bw[i][j] += (abs(in[i][j].r - in[i][j-1].r) + abs(in[i][j].g - in[i][j-1].g) + abs(in[i][j].b - in[i][j-1].b))/3;
				cnt++;
			}
			if(j < w-1) {
				bw[i][j] += (abs(in[i][j].r - in[i][j+1].r) + abs(in[i][j].g - in[i][j+1].g) + abs(in[i][j].b - in[i][j+1].b))/3;
				cnt++;
			}
			bw[i][j] /= cnt;
		}
	}
}

void seam(int h, int *w) {
	//int i, j;
	//int comp[h][*w];
	int tmpw = *w;
	for(int q = 0; q <= 100; q++) {
		int i, j;
		int comp[h][*w];
		int index[h];
		for(i = 0; i < *w; i++) {
			comp[0][i] = (int) bw[0][i];
		}
		for(i = 1; i < h; i++) {
			for(j = 0; j < *w; j++) {
				comp[i][j] = comp[i-1][j];
				if(j > 0)
					comp[i][j] = MAX(comp[i][j], comp[i-1][j-1]);
				if(j < *w-1)
					comp[i][j] = MAX(comp[i][j], comp[i-1][j+1]);
				comp[i][j] += (int) bw[i][j];
			}
		}
		int mindst = 10000000;
		//short ans = 0;
		//ans = 0;
		for(i = 0; i < *w; i++) {
			if(comp[h-1][i] < mindst) {
				mindst = comp[h-1][i];
				index[h-1] = i;
			}
		}
		//j = index[h-1];
		int l;
		int once;
		for(i = h-1; i >= 1; i--) { 
			once = 0;
			if(index[i] > 0) {
				if(mindst - (int) bw[i][index[i]] == comp[i-1][index[i]-1]) {
					index[i-1] = index[i]-1;
					mindst = comp[i-1][index[i]-1];
					once = 1;
				}
			}
			if(index[i] < *w-1 && once == 0) {
				if(mindst - (int) bw[i][index[i]] == comp[i-1][index[i]+1]) {
					index[i-1] = index[i]+1;
					mindst = comp[i-1][index[i]+1];
					once = 1;
				}
			}
			if(once == 0 && mindst - (int) bw[i][index[i]] == comp[i-1][index[i]]) {
					index[i-1] = index[i];
					mindst = comp[i-1][index[i]];
			}
		}
		for(i = h-1; i >= 0; i--) {
			//int tmp = comp[i][index[i]];
			//for(l = index[i]; l < *w-1; l++) {
				//comp[i][l] = comp[i][l+1];
			//}
			//comp[i][*w-1] = tmp;
			int tmp = bw[i][index[i]];
			for(l = index[i]; l < *w-1; l++) {
				bw[i][l] = bw[i][l+1];
			}
			bw[i][*w-1] = tmp;
		}
		*w-=1;
	}
}

int main(int argc, char *argv[]) {
	if(argc != 3) {
		fprintf(stderr, "usage: csg <input image name> <output image name>");
	}
	argv[0] = "./img/2(3).ppm"; /* test ver */
	FILE *img = fopen(argv[0], "rw");
	int w, h, ncolor;
	char format[2] = {};
	int i, j;
	fscanf(img, "%s", format);
	fscanf(img, "%d", &w);
	fscanf(img, "%d", &h);
	fscanf(img, "%d", &ncolor);
	printf("%s %d %d\n", format, w, h);
	diff(img, w, h);
	FILE *out;
	out = fopen("./res/border.ppm", "w");
	seam(h, &w);
	fprintf(out, "P3\n");
	fprintf(out, "%d %d\n", w, h);
	fprintf(out, "%d\n", ncolor);
	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++) {
			int color;
			color = 2*bw[i][j];
			fprintf(out, "%d %d %d ", color, color, color);
		}
	}
	fclose(img);
	fclose(out);
	return 0;
}
