//******************************************************************************//
//      seisData.h								//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 10, 2003                        	//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 10, 2003					//
//******************************************************************************//
#ifndef INCLUDED_SEISDATA_H
#define INCLUDED_SEISDATA_H

#include "evData.h"
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

//******************************************************************************//
//      CLASS DECLARATION: SEISDATA						//
//******************************************************************************//

class SEISDATA {

private:
	int d_evSize;			// total number of events
	vector<EVDATA*> d_evList;

public:
	SEISDATA(char *baseDir, const vector<int>& evIdList, int preNum1, int posNum1, int preNum2, int posNum2, const STAS& stas, const char *velCode);
	~SEISDATA();
	EVDATA* getEvData(int num) { return d_evList[num]; }
	int getEvListSize() const { return d_evSize; }

};

#endif	// INCLUDED_SEISDATA_H
