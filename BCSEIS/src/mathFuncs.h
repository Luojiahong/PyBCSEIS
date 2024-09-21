//******************************************************************************//
//      mathFuncs.h                                                          	//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, March 14, 2002                             //
//                                                                              //
//      Last modified: June 2, 2003						//
//******************************************************************************//
//      Copyright (c) by Wen-xuan Du -- dxw@ldeo.columbia.edu                   //
//                                                                              //
//      The author reserves all rights of the package.  You are not             //
//      allowed to printout, copy, use, distribute, or modify any part of       //
//      the files in this package without permission.  The author is not        //
//      responsible for any damage of any computer hardware and software by     //
//      any appropriate or inappropriate use of the package.                    //
//******************************************************************************//
#ifndef INCLUDED_MATHFUNCS_H
#define INCLUDED_MATHFUNCS_H

#include "constants.h"
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

void gaussj(double **a, int n, double **b, int m);
// Linear equation solution by Gauss-Jordan elimination method. 
// (equation 2.1.1 in Numerical Recipe in C).
// a[0..n-1][0..n-1] is the input matrix. 
// b[0..n-1][0..m-1] is input containing the m right-hand side vectors. 
// On output, a[][] is replaced by its matrix inverse, and b[][] is replaced by 
// the corresponding set of solution vectors.

void matrixMultiplication(double **a, double **b, double **c, int l, int m, int n);
// a[0..l-1][0..m-1], b[0..m-1][0..n-1] and c[0..l-1][0..n-1]

void movingAverage(double data[], int nBeg, int nEnd, int halfWidth);
// make moving average on 'data[]' starting at position 'nBeg' and ending at 'nEnd' using
// box car with halfwidth of 'halfWidth'

void linearRegression(double x[], double y[], double sig[], int ndata, double *intercept, 
	double *interceptDev, double *slope, double *slopeDev, double *chisq);
// make linear regression on (x, y) with standard deviation sig[]

void demean(float data[], int ndata);

void demean2(float data[], int ndata, int preNum, int posNum);

void detrend(float data[], int ndata);

double getStandardDeviation(double *data_p, double dataMean, int num);

void getMax(float *data, int npts, int *pos, float *max);
// get the maximum value 'max' and its position in the first 'npts' points of array 'data'

void getMaxMin(float *data, int npts, int *pos1, float *max, int *pos2, float *min);

void tdcorr(float data1[], float data2[], int npts, float corr[]);
// time-domain cross-correlation

void hannwindow(float percent, float window[], int npts);
// hanning window

void hanntaper(float percent, float data[], int npts);
// hanning taper on a time series

void hanntaper2(float percent, float **data1, float **data2, int npts);
// hanning taper on two matrices
//
#endif	// INCLUDED_MATHFUNCS_H
