//******************************************************************************//
//      fourier.h 								//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, July 16, 2001                              //
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: July 16, 2001                                            //
//******************************************************************************//
#ifndef INCLUDED_FOURIER_H
#define INCLUDED_FOURIER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int getpower2(int num);
// function prototypes for FFT calculations
void four1(float data[], unsigned long nn, int isign);	// deal with complex data
void dfour1(double data[], unsigned long nn, int isign);
void twofft(float data1[], float data2[], float fft1[], float fft2[], unsigned long n);
void realft(float data[], unsigned long n, int isign);	// deal with real data
void drealft(double data[], unsigned long n, int isign); 

// 2-D FFT functions
void fourn(float data[], unsigned long nn[], int ndim, int isign);

// function prototypes for cross-correlation
void correl(float data1[], float data2[], unsigned long n, float ans[]);

// general purpose function prototyes
float *float_vector(int nl);

#endif	// INCLUDED_FOURIER_H
