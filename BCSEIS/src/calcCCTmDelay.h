//******************************************************************************//
//      calcCCTmDelay.h                                                        	//
//										//
//      Author: Wen-xuan(Wayne) Du, September 11, 2003	                        //
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 11, 2003     					//
//******************************************************************************//
#ifndef INCLUDED_CALCCCTMDELAY_H
#define INCLUDED_CALCCCTMDELAY_H

#include "constants.h"
#include "fourier.h"
#include "mathFuncs.h"
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

void calcCCTmDelay(float data1[], float data2[], int winLen, int maxNum, int option, int pFlag, int *ccLag, float *ccCoef);

#endif	// INCLUDED_CALCCCTMDELAY_H
