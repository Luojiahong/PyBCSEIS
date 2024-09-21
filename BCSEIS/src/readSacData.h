//******************************************************************************//
//      readSacData.h                                                         	//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 10, 2003				//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 10, 2003                			//
//******************************************************************************//
#ifndef INCLUDED_READSACDATA_H
#define INCLUDED_READSACDATA_H

#include "mathFuncs.h"
#include "calcTravTm.h"
#include "sacSeisgram.h"

int readSacData(const char *sacFile, int phaCode, int preNum, int posNum, float *delta, float *tTm, float *phaWgt, float winData[], float evdp, const char *velCode);

#endif	// INCLUDED_READSACDATA_H
