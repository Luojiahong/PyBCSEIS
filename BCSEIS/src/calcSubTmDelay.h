//******************************************************************************//
//      calcSubTmDelay.h                                                       	//
//										//
//      Author: Wen-xuan(Wayne) Du, January 31, 2003	                        //
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 12, 2003					//
//******************************************************************************//
#ifndef INCLUDED_CALCSUBTMDELAY_H
#define INCLUDED_CALCSUBTMDELAY_H

#include "constants.h"
#include "complex.h"
#include "fourier.h"
#include "mathFuncs.h"
#include <math.h>
#include <iostream>
#include <vector>
using namespace std;

// subsample time-delay subroutine
void calcSubTmDelay(float odata1[], float odata2[], int npts, int shiftLag, int segLen, float delta, float lf, float hf, float *delay, float *sigma); 

#endif	// INCLUDED_CALCSUBTMDELAY_H
