//******************************************************************************//
//      calcBSTmDelay.h	                                                       	//
//                                                                              //
//      Author: Wen-xuan(Wayne) Du, February 27, 2003                         	//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 11, 2003                                      	//
//******************************************************************************//
#ifndef INCLUDED_CALCBSTMDELAY_H
#define INCLUDED_CALCBSTMDELAY_H

#include "constants.h"
#include "complex.h"
#include "fourier.h"
#include "mathFuncs.h"
#include "bsTaper.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

// calculate 3rd-order cumulants between time series x and y (version 2)
void cum32(int num, float x[], float y[], float **cumxxx, float **cumxyx);

void calcBSTmDelay(float data1[], float data2[], int npts, int segLen, int maxLag, int pFlag, int *lagNum, float *maxVal);

#endif	// INCLUDED_CALCBSTMDELAY_H
