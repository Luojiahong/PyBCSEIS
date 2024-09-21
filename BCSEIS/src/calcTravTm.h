//******************************************************************************//
//      calcTravTm.h                                                           	//
//                                                                              //
//      Author: Wen-xuan(Wayne) Du, April 15, 2003				//
//	        dxw@geology.wisc.edu						//
//	        								//
//      Last modified: September 18, 2003					//
//******************************************************************************//
#ifndef INCLUDED_CALCTRAVTM_H
#define INCLUDED_CALCTRAVTM_H

#include "constants.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

extern "C" void ttime_(float*, float*, int*, float[], float[], float*, float*);
float calcTravTm(const char *velCode, int phaCode, float depth, float dist);

#endif	// INCLUDED_CALCTRAVTM_H
