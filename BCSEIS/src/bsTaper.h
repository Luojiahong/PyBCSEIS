//******************************************************************************//
//      bsTaper.h                                                           	//
//                                                                              //
//      Author: Wen-xuan(Wayne) Du, September 24, 2003				//
//	        dxw@geology.wisc.edu						//
//	        								//
//      Last modified: September 24, 2003					//
//******************************************************************************//
#ifndef INCLUDED_BSTAPER_H
#define INCLUDED_BSTAPER_H

#include "constants.h"
#include <math.h>

void bsWindow(int winMode, int winLen, float window[]);		// winLen is an even number
void bsTaper(int winMode, int winLen, float **data1, float **data2);

#endif	// INCLUDED_BSTAPER_H
