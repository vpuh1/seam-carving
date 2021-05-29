#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXN 640000
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main() {
	int h, w;
	int ncolor;
	char format[2] = {};
	FILE *border;
	//FILE *res;
	int i, j;
	border = fopen("./res/border.ppm", "r");
	//res = fopen("./res/res.ppm", "w");
	fscanf(border, "%s", format);
	fscanf(border, "%d", &w);
	fscanf(border, "%d", &h);
	fscanf(border, "%d", &ncolor);
	int diff[h][w];
	int comp[h][w];
	//printf("%d %d %d\n", w, h, ncolor);
	for(i = 0; i < h; i++) { 
		for(j = 0; j < w; j++) {
			fscanf(border, "%d", &diff[i][j]);
			fscanf(border, "%d", &diff[i][j]);
			fscanf(border, "%d", &diff[i][j]);
			//printf("%d\n", diff[i][j]);
		}
	}
	for(i = 0; i < w; i++) {
		comp[0][i] = bw[0][i];
	}
	for(i = 1; i < h; i++) {
		for(j = 0; j < w; j++) {
			comp[i][j] = comp[i-1][j];
			if(j > 0)
				comp[i][j] = MAX(comp[i][j], comp[i-1][j-1]);
			if(j < w-1)
				comp[i][j] = MAX(comp[i][j], comp[i-1][j+1]);
			comp[i][j] += (int) diff[i][j];
		}
	}
	int mindst = 1000000;
	short ans = 0;
	for(i = 0; i < 3; i++) {
		if(comp[h-1][i] < mindst) {
			mindst = comp[h-1][i];
			ans = i;
		}
	}
	printf("%d\n", ans);
	fclose(border);
	return 0;
}
