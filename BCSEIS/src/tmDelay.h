//******************************************************************************//
//      tmDelay.h								//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 11, 2003                        	//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 11, 2003					//
//******************************************************************************//
#ifndef INCLUDED_TMDELAY_H
#define INCLUDED_TMDELAY_H

#include <iostream>
using namespace std;

//******************************************************************************//
//      CLASS DECLARATION: TMDELAY						//
//******************************************************************************//
class TMDELAY {

private:
	int	d_traceNum1;		// position in the data traces for event 1
	int	d_traceNum2;		// position in the data traces for event 2
	int	d_staId;		// station id
	float	d_delta;		// delta
	float	d_ctDiffTm;		// catalog differential time
	float	d_ccDiffTm;		// CC differential time
	float	d_ccCoef;		// CC coefficient
	int	d_ccTmDelay;		// CC time delay (using filtered data; sample points)
	int	d_bsTmDelay1;		// BS time delay 1 (using filtered data; sample points)
	int	d_bsTmDelay2;		// BS time delay 2 (using raw data; sample points)
	float	d_subTmDelay;		// sub-sample time delay
	float	d_subStd;		// std for sub-sample time delay estimate
public:
	TMDELAY(int traceNum1, int traceNum2, int staId, float delta, float ctDiffTm);
	~TMDELAY();

	// accessors
	int getTraceNum1() const { return d_traceNum1; }
	int getTraceNum2() const { return d_traceNum2; }
	int getStaId() const { return d_staId; }
	float getCCDiffTm() const { return d_ccDiffTm; }
	float getCCCoef() const { return d_ccCoef; }
	int getCCTmDelay() const { return d_ccTmDelay; }
	int getBSTmDelay1() const { return d_bsTmDelay1; }
	int getBSTmDelay2() const { return d_bsTmDelay2; }
	float getSubTmDelay() const { return d_subTmDelay; }
	float getSubStd() const { return d_subStd; }

	// modifiers
	void setCCDiffTm(float ccDiffTm) { d_ccDiffTm = ccDiffTm; }
	void setCCCoef(float ccCoef) { d_ccCoef = ccCoef; }
	void setCCTmDelay(int ccTmDelay) { d_ccTmDelay = ccTmDelay; }
	void setBSTmDelay1(int bsTmDelay1) { d_bsTmDelay1 = bsTmDelay1; }
	void setBSTmDelay2(int bsTmDelay2) { d_bsTmDelay2 = bsTmDelay2; }
	void setSubTmDelay(float subTmDelay) { d_subTmDelay = subTmDelay; }
	void setSubStd(float subStd) { d_subStd = subStd; }
};

#endif	// INCLUDED_TMDELAY_H
