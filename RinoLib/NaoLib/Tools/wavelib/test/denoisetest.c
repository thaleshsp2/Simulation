#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../header/wauxlib.h"

static double rmse(int N,double *x,double *y) {
	double rms;
	int i;

	rms = 0.0;

	for(i = 0; i < N;++i) {
		rms += (x[i] - y[i]) * (x[i] - y[i]);
	}

	rms = sqrt(rms/(double)N);

	return rms;
}

static double corrcoef(int N,double *x,double *y) {
	double cc,xm,ym,tx,ty,num,den1,den2;
	int i;
	xm = ym = 0.0;
	for(i = 0; i < N;++i) {
		xm += x[i];
		ym += y[i];
	}

	xm = xm/N;
	ym = ym / N;
	num = den1 = den2 = 0.0;

	for(i = 0; i < N;++i) {
		tx = x[i] - xm;
		ty = y[i] - ym;
		num += (tx*ty);
		den1 += (tx*tx);
		den2 += (ty*ty);
	}

	cc = num / sqrt(den1*den2);

	return cc;
}

int main() {
	// gcc -Wall -I../header -L../Bin denoisetest.c -o denoise -lwauxlib -lwavelib -lm
	double *sig,*inp,*oup;
	int i,N,J;
	FILE *ifp;

	denoise_object obj;
	double temp[2400];

	char *wname = "db5";
	char *method = "dwt";
	char *ext = "sym";
	char *thresh = "soft";
	char *level = "all";

	ifp = fopen("pieceregular1024.txt", "r");
	i = 0;
	if (!ifp) {
	    printf("Cannot Open File");
	    exit(100);
	}

	while (!feof(ifp)) {
	    fscanf(ifp, "%lf \n", &temp[i]);
	    i++;
	}

	fclose(ifp);

	N = i;
	J = 4;

	sig = (double*)malloc(sizeof(double)* N);
	inp = (double*)malloc(sizeof(double)* N);
	oup = (double*)malloc(sizeof(double)* N);

	for(i = 0; i < N;++i) {
		sig[i] = temp[i];
	}

	ifp = fopen("PieceRegular10.txt", "r");
	i = 0;
	if (!ifp) {
		printf("Cannot Open File");
		exit(100);
	}

	while (!feof(ifp)) {
		fscanf(ifp, "%lf \n", &temp[i]);
		i++;
	}

	fclose(ifp);

	for(i = 0; i < N;++i) {
		inp[i] = temp[i];
	}
	obj = denoise_init(N,J,wname);
	setDenoiseMethod(obj,"visushrink");// sureshrink is also the default. The other option is visushrink
	setDenoiseWTMethod(obj,method);// Default is dwt. the other option is swt
	setDenoiseWTExtension(obj,ext);// Default is sym. the other option is per
	setDenoiseParameters(obj,thresh,level);// Default for thresh is soft. Other option is hard
	// Default for level is first. The other option is all

	denoise(obj,inp,oup);

	// Alternative to denoise_object
	// Just use visushrink and sureshrink functions
	//visushrink(inp,N,J,wname,method,ext,thresh,level,oup);
	//sureshrink(inp,N,J,wname,method,ext,thresh,level,oup);

	//ofp = fopen("denoiseds.txt", "w");


	printf("Signal - Noisy Signal Stats \n");
	printf("RMSE %g\n",rmse(N,sig,inp));
	printf("Corr Coeff %g\n",corrcoef(N,sig,inp));

	printf("Signal - DeNoised Signal Stats \n");
	printf("RMSE %g\n",rmse(N,sig,oup));
		printf("Corr Coeff %g\n",corrcoef(N,sig,oup));

	free(sig);
	free(inp);
	denoise_free(obj);
	free(oup);
	return 0;
}
