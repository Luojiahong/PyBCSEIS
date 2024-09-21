//******************************************************************************//
//      tmDelay.c								//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 11, 2003                        	//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 11, 2003					//
//******************************************************************************//
#include "tmDelay.h"

//******************************************************************************//
//      CLASS DEFINITION: TMDELAY						//
//******************************************************************************//
TMDELAY::TMDELAY(int traceNum1, int traceNum2, int staId, float delta, float ctDiffTm) {
	d_traceNum1 = traceNum1;
	d_traceNum2 = traceNum2;
	d_staId = staId;
	d_delta = delta;
	d_ctDiffTm = ctDiffTm;
	
	d_ccDiffTm = ctDiffTm;
	d_ccCoef = 0.0;
	d_ccTmDelay = -999;
	d_bsTmDelay1 = -999;
	d_bsTmDelay2 = -999;
	d_subTmDelay = 0.0;
	d_subStd = 0.0;
}

TMDELAY::~TMDELAY() {
}
