//******************************************************************************//
//      stas.h									//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 10, 2003                        	//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 10, 2003					//
//******************************************************************************//
#ifndef INCLUDED_STAS_H
#define INCLUDED_STAS_H

#include <vector>
#include <iostream>
#include <cstdio>
using namespace std;

//******************************************************************************//
//      CLASS DECLARATION: STAS							//
//******************************************************************************//
class STATION {

private:
	float	d_stla;
	float	d_stlo;
	float	d_stel;
	char	d_kstnm[8];
public:
	STATION(float,float,float,char*);
	~STATION();
	const char* getStaName() const { return d_kstnm; }
};

class STAS {
private:
	int d_staNum;			// total number of stations
	vector<STATION> d_staList;	// vector of station names

public:
	STAS(const char *staFile);	// constructor
	~STAS();			// deconstructor
	const char* getStaNamebyId(int evId) const { return d_staList[evId].getStaName(); }
	int getStaIdbyName(const char*) const;
	int getStaNum() const { return d_staNum; }
};

#endif	// INCLUDED_STAS_H
