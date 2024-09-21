//******************************************************************************//
//      stas.c									//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 10, 2003                        	//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 10, 2003					//
//******************************************************************************//
#include "stas.h"
#include <string.h>
#include <cstdlib>

//******************************************************************************//
//      CLASS DEFINITION: STAS							//
//******************************************************************************//
STATION::STATION(float stla, float stlo, float stel, char *kstnm) {
	d_stla = stla;
	d_stlo = stlo;
	d_stel = stel;
	strncpy(d_kstnm, kstnm, 8);
}

STATION::~STATION() {
}

STAS::STAS(const char *staFile) {
	FILE* fp;
	if ( (fp = fopen(staFile, "r")) == NULL) {
		cerr << ">>> Error: Can not open file " << staFile << " for reading!" << endl;
		exit (-1);
	}
	char buff[128];
	float stla, stlo, stel;
	char kstnm[8];
	stel = 0.0;
	int staNum = 0;
	while (fgets(buff, sizeof(buff), fp) != NULL) {
		sscanf(buff, "%s %f %f %f\n", kstnm, &stla, &stlo, &stel);
		//cout << ">>> kstnm:" << kstnm << endl;
		//cout << ">>> stla:" << stla << endl;
		//cout << ">>> stlo:" << stlo << endl;
		//cout << ">>> stel:" << stel << endl;
		STATION sta(stla, stlo, stel, kstnm);
		d_staList.push_back(sta);
		staNum++;
	}
	fclose(fp);
	d_staNum = staNum;
}

STAS::~STAS() {
}

int STAS::getStaIdbyName(const char* kstnm) const {
	for (int i = 0; i < d_staNum; i++) {
		if ( strcmp(kstnm, d_staList[i].getStaName()) == 0) {
			return i;
			break;
		}
	}
	return -1;
}

