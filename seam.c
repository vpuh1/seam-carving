#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXN 1000 
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct color {
	int r;
	int g;
	int b;
} color; 

int bw[MAXN][MAXN];
int res[MAXN][MAXN];
color in[MAXN][MAXN];

void readimg(FILE *img, int w, int h) { int i, j;
	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++) {
			fscanf(img, "%d", &in[i][j].r);
			fscanf(img, "%d", &in[i][j].g);
			fscanf(img, "%d", &in[i][j].b);
		}
	}
}

void diff(FILE *img, int w, int h) {
	int i, j;
	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++) {
			bw[i][j] = 0;
		}
	}
	int cnt = 0;
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
	int i, j;
	int comp[h][*w];
	for(i = 0; i < *w; i++) {
		comp[0][i] = bw[0][i];
	}
	for(i = 1; i < h; i++) {
		for(j = 0; j < *w; j++) {
			comp[i][j] = comp[i-1][j];
			if(j > 0)
				comp[i][j] = MIN(comp[i][j], comp[i-1][j-1]);
			if(j < *w-1)
				comp[i][j] = MIN(comp[i][j], comp[i-1][j+1]);
			comp[i][j] += bw[i][j];
		}
	}
	for(i = 0; i < h; i++) {
		for(j = 0; j < *w; j++) {
		if(comp[i][j] < 0)
			printf("%d\n", comp[i][j]);
		}
	}
	int mindst = 255001;
	int index[h];
	for(i = 0; i < *w; i++) {
		if(comp[h-1][i] < mindst) {
			mindst = comp[h-1][i];
			index[h-1] = i;
		}
	}
	int l;
	int once = 0;
	for(i = h-1; i >= 1; i--) { 
		once = 0;
		if(index[i] > 0) {
			if(mindst - bw[i][index[i]] == comp[i-1][index[i]-1]) {
				index[i-1] = index[i]-1;
				mindst = comp[i-1][index[i]-1];
				once = 1;
			}
		}
		if(index[i] < *w-1 && once == 0) {
			if(mindst - bw[i][index[i]] == comp[i-1][index[i]+1]) {
				index[i-1] = index[i]+1;
				mindst = comp[i-1][index[i]+1];
				once = 1;
			}
		}
		if(once == 0 && mindst - bw[i][index[i]] == comp[i-1][index[i]]) {
				index[i-1] = index[i];
				mindst = comp[i-1][index[i]];
				once = 1;
		}
	}
	for(i = h-1; i >= 0; i--) {
		color tmpc;
		tmpc.r = in[i][index[i]].r;
		tmpc.g = in[i][index[i]].g;
		tmpc.b = in[i][index[i]].b;
		for(l = index[i]; l < *w-1; l++) {
			in[i][l].r = in[i][l+1].r;
			in[i][l].g = in[i][l+1].g;
			in[i][l].b = in[i][l+1].b;
		}
		in[i][*w-1].r = tmpc.r;
		in[i][*w-1].g = tmpc.g;
		in[i][*w-1].b = tmpc.b;
	}
	*w-=1;
}

int main(int argc, char *argv[]) {
	FILE *img, *out;
	int nseams, w, h, ncolor, i, j;
	char format[2] = {};
	if(argc != 4) {
		fprintf(stderr, "usage: seam: <number of seams> <input image name> <output image name>\n");
		return 0;
	}
	nseams = atoi(argv[1]);
	img = fopen(argv[2], "r");
	out = fopen(argv[3], "w");
	if(!img) {
		fprintf(stderr, "seam: cannot open %s\ncheck if the file exists and you have permission to open it\n", argv[2]);
		return 0;
	}
	if(!out) {
		fprintf(stderr, "seam: cannot open %s\ncheck if the file exists and you have permission to open it\n", argv[3]);
		return 0;
	}
	fscanf(img, "%s", format);
	if(strcmp(format, "P3") != 0) {
		fprintf(stderr, "seam: wrong image format.\n please provide .ppm (P3) image\n");
		return 0;
	}
	fscanf(img, "%d", &w); 
	fscanf(img, "%d", &h);
	if(w > 1000 || h > 1000) {
		fprintf(stderr, "seam: image size is too big (max size: 1000x1000)\n");
		return 0;
	}
	if(nseams > w) {
		fprintf(stderr, "seam: number of seams is too big (max %d)\n", w-1);
		return 0;
	}
	fscanf(img, "%d", &ncolor);
	readimg(img, w, h);
	for(i = 0; i < nseams; i++) {
		diff(img, w, h);
		seam(h, &w);
	}
	fprintf(out, "P3\n");
	fprintf(out, "%d %d\n", w, h);
	fprintf(out, "%d\n", ncolor);
	for(i = 0; i < h; i++) {
		for(j = 0; j < w; j++) {
			fprintf(out, "%d %d %d ", in[i][j].r, in[i][j].g, in[i][j].b);
		}
	}
	fclose(img);
	fclose(out);
	return 0;
}
