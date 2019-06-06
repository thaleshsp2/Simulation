/*
Copyright (c) 2017, Rafat Hussain
*/
#ifndef WAUXLIB_H_
#define WAUXLIB_H_

#include "wavelib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct denoise_set* denoise_object;

denoise_object denoise_init(int length, int J,char* wname);

struct denoise_set{
	int N; //signal length
	int J; // Levels of Wavelet decomposition
	char wname[10]; //Wavelet name
	char wmethod[10]; //Wavelet decomposition method - dwt or swt
	char ext[10]; // Signal Extension - sym or per
	char thresh[10]; // thresholding - soft or hard
	char level[10]; // Noise Estimation level - first or all
	char dmethod[20]; //Denoising Method -sureshrink or visushrink
	//double params[0];
};

void visushrink(double *signal,int N,int J,char *wname,char *method,char *ext,char *thresh,char *level,double *denoised);

void sureshrink(double *signal,int N,int J,char *wname,char *method,char *ext,char *thresh,char *level,double *denoised);

void denoise(denoise_object obj, double *signal,double *denoised);

void setDenoiseMethod(denoise_object obj, char *dmethod);

void setDenoiseWTMethod(denoise_object obj, char *wmethod);

void setDenoiseWTExtension(denoise_object obj, char *extension);

void setDenoiseParameters(denoise_object obj, char *thresh,char *level);

void denoise_free(denoise_object object);

void getDWTRecCoeff(double *coeff,int *length,char *ctype,char *ext, int level, int J,double *lpr,
		double *hpr,int lf,int siglength,double *reccoeff);

double mad(double *x, int N);


#ifdef __cplusplus
}
#endif


#endif /* WAUXLIB_H_ */
