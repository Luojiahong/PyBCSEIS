//******************************************************************************//
//	constants.h								//
//										//
//	Author: Wen-xuan (Wayne) Du, July 16, 2001				//
//	        dxw@geology.wisc.edu						//
//										//
//	Last modified: July 16, 2001						//
//******************************************************************************//
#ifndef	INCLUDED_CONSTANTS_H
#define INCLUDED_CONSTANTS_H

//******************************************************************************//
//	CONSTANTS								//
//******************************************************************************//

const double PI		= 3.141592653589793;
const int FILENAMESIZE	= 128;
const int MODESIZE	= 4;		// for opening files
const int HDRKEYSIZE	= 16;		// header key size
const float TAPERVAL	= 0.50;

const int VELCODESIZE	= 4;
const int MAXLAYER	= 12;

const float SUBCCCOEF	= 0.85;		// minimum ccCoef above which to perform sub-sample estimation
const float MAXSIGMA	= 0.50; 	// maximum sigma value above which sub-sample time delay is not used
const float BSMAXCC	= 0.70;		// minimum maxCCCoef above which to perform BS estimation (for an event pair)
const float BSFREECC	= 0.90;		// minimum ccCoef above which BS estimations is exempted (for a station)
const float BSMINCCCOEF	= 0.50;		// minimum ccCoef above which to perform BS estimation (for a station)
const float MINOUTCC	= 0.70;		// minimum maxCCCoef above which to output results
const int LAGDIFFLIM	= 2;		// maximum difference between ccLag and bsLag1 allowed before bsLag2 calculation
#endif
