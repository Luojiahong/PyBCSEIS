//******************************************************************************//
//      calcTmDelays.h								//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 10, 2003                        	//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 12, 2003					//
//******************************************************************************//
#ifndef INCLUDED_CALCTMDELAYS_H
#define INCLUDED_CALCTMDELAYS_H

#include "constants.h"
#include "seisData.h"
#include "tmDelay.h"
#include "calcCCTmDelay.h"
#include "calcSubTmDelay.h"
#include "calcBSTmDelay.h"
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

void calcTmDelays(int option, float distCutOff, int winLen1, int winLen2, int segLen1, int segLen2, float lf, float hf, const STAS& stas, SEISDATA* seisData1, SEISDATA* seisData2, int startNum);

#endif	// INCLUDED_CALCTMDELAYS_H
