//******************************************************************************//
//      evData.h								//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 10, 2003                        	//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 10, 2003					//
//******************************************************************************//
#ifndef INCLUDED_EVDATA_H
#define INCLUDED_EVDATA_H

#include "constants.h"
#include "stas.h"
#include "readSacData.h"
#include "getMaxShiftLag.h"
#include <iostream>
using namespace std;

//******************************************************************************//
//      CLASS DECLARATION: EVDATA						//
//******************************************************************************//
class EVDATA {

private:
	int d_evId;			// event id number
	float d_evla, d_evlo, d_evdp;	// event latitude, longitude, depth
	char d_evDir[FILENAMESIZE];	// event directory
	int d_pSize, d_sSize;		// number of P and S data windows
	int *d_pStaIds, *d_sStaIds;	// station ids (P and S windows)
	float **d_pData1, **d_pData2;	// P window data arrays
	float **d_sData1, **d_sData2;	// S window data arrays
	float *d_pDeltas, *d_sDeltas;	// delta arrays for the stations
	float *d_pTTms, *d_sTTms;	// P and S travel time arrays
	int *d_pMaxLags, *d_sMaxLags;	// maximum lags allowed to shift
public:
	EVDATA(int, float, float, float, char*, int, int);
	~EVDATA();
	int getEvId() const { return d_evId; }
	float getEvla() const { return d_evla; }
	float getEvlo() const { return d_evlo; }
	float getEvdp() const { return d_evdp; }
	int getPSize() const { return d_pSize; }
	int getSSize() const { return d_sSize; }
	int* getPStaIds() const { return d_pStaIds; }
	int* getSStaIds() const { return d_sStaIds; }
	float* getPData1(int n) { return d_pData1[n]; }
	float* getPData2(int n) { return d_pData2[n]; }
	float* getSData1(int n) { return d_sData1[n]; }
	float* getSData2(int n) { return d_sData2[n]; }
	float* getPDeltas() const { return d_pDeltas; }
	float* getSDeltas() const { return d_sDeltas; }
	float* getPTTms() const { return d_pTTms; }
	float* getSTTms() const { return d_sTTms; }
	int* getPMaxLags() const { return d_pMaxLags; }
	int* getSMaxLags() const { return d_sMaxLags; }
	void addStaData(int, int, const char*, int, int, const STAS&, const char*);
	float* getPData1(int n, int begNum);
	float* getPData2(int n, int begNum);
	float* getSData1(int n, int begNum);
	float* getSData2(int n, int begNum);
};

#endif	// INCLUDED_EVDATA_H
