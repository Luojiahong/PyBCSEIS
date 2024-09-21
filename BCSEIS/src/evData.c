//******************************************************************************//
//      evData.c								//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 10, 2003                        	//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 10, 2003					//
//******************************************************************************//
#include "evData.h"

//******************************************************************************//
//      CLASS DEFINITION: EVDATA						//
//******************************************************************************//
EVDATA::EVDATA(int evId, float evla, float evlo, float evdp, char *evDir, int pSize, int sSize) {
	d_evId	= evId;
	d_evla	= evla;
	d_evlo	= evlo;
	d_evdp	= evdp;
	strncpy(d_evDir, evDir, FILENAMESIZE);
	d_pSize	= pSize;
	d_sSize	= sSize;
	if (d_pSize != 0) {
		d_pStaIds	= new int[d_pSize];
		d_pData1	= new float*[d_pSize];
		d_pData2	= new float*[d_pSize];
		d_pDeltas	= new float[d_pSize];
		d_pTTms		= new float[d_pSize];
		d_pMaxLags	= new int[d_pSize];
	}
	if (d_sSize != 0) {
		d_sStaIds	= new int[d_sSize];
		d_sData1	= new float*[d_sSize];
		d_sData2	= new float*[d_sSize];
		d_sDeltas	= new float[d_sSize];
		d_sTTms		= new float[d_sSize];
		d_sMaxLags	= new int[d_sSize];
	}
}

EVDATA::~EVDATA() {
	if (d_pSize != 0) {
		delete	[]d_pStaIds;
		for (int i = 0; i < d_pSize; i++) {
			delete	[]d_pData1[i];
			delete	[]d_pData2[i];
		}
		delete	[]d_pData1;
		delete	[]d_pData2;
		delete	[]d_pDeltas;
		delete	[]d_pTTms;
		delete	[]d_pMaxLags;
	}
	if (d_sSize != 0) {
		delete	[]d_sStaIds;
		for (int i = 0; i < d_sSize; i++) {
			delete	[]d_sData1[i];
			delete	[]d_sData2[i];
		}
		delete	[]d_sData1;
		delete	[]d_sData2;
		delete	[]d_sDeltas;
		delete	[]d_sTTms;
		delete	[]d_sMaxLags;
	}
}

void EVDATA::addStaData(int phaCode, int num, const char *staName, int preNum, int posNum, const STAS& stas, const char *velCode) {
	char dataFile1[FILENAMESIZE];
	char dataFile2[FILENAMESIZE];
	int winNum = preNum + posNum + 1;
	int staId = stas.getStaIdbyName(staName);
	strncpy(dataFile1, d_evDir, FILENAMESIZE);
	strncpy(dataFile2, d_evDir, FILENAMESIZE);
	strcat(dataFile1, "/filt.");
	strcat(dataFile1, staName);
	strcat(dataFile1, ".Z.SAC");
	strcat(dataFile2, "/");
	strcat(dataFile2, staName);
	strcat(dataFile2, ".Z.SAC");
	float evdp = this->getEvdp();

	if (phaCode == 0) {		// P window
		d_pStaIds[num] = staId;
		//cout  << ">>> staId:" << staId <<endl;
		d_pData1[num] = new float[winNum*3];	//new
		d_pData2[num] = new float[winNum*3];	//new
		float delta = 0.0;
		float tTm = 0.0;
		float phaWgt = 0.0;
		int flag1 = readSacData(dataFile1, phaCode, (preNum+winNum), (posNum+winNum), &delta, &tTm, &phaWgt, d_pData1[num], evdp, velCode);
		int flag2 = readSacData(dataFile2, phaCode, (preNum+winNum), (posNum+winNum), &delta, &tTm, &phaWgt, d_pData2[num], evdp, velCode);
		if (flag1 == -1 || flag2 == -1) {
			// if error occurs during SAC file reading, set the station id to -1
			// cerr << "P Error" << endl;
			d_pStaIds[num] = -1;
		}
		int maxLag = 0;
		maxLag = getMaxShiftLag(phaCode, delta, phaWgt);
		d_pDeltas[num] = delta;
		d_pTTms[num] = tTm;
		d_pMaxLags[num] = maxLag;
	} else if (phaCode == 1) {	// S window
		d_sStaIds[num] = staId;
		d_sData1[num] = new float[winNum*3];
		d_sData2[num] = new float[winNum*3];
		float delta = 0.0;
		float tTm = 0.0;
		float phaWgt = 0.0;
		int flag1 = readSacData(dataFile1, phaCode, (preNum+winNum), (posNum+winNum), &delta, &tTm, &phaWgt, d_sData1[num], evdp, velCode);
		int flag2 = readSacData(dataFile2, phaCode, (preNum+winNum), (posNum+winNum), &delta, &tTm, &phaWgt, d_sData2[num], evdp, velCode);
		if (flag1 == -1 || flag2 == -1) {
			// cerr << "S Error" << endl;
			d_sStaIds[num] = -1;
		}
		int maxLag = 0;
		maxLag = getMaxShiftLag(phaCode, delta, phaWgt);
		d_sDeltas[num] = delta;
		d_sTTms[num] = tTm;
		d_sMaxLags[num] = maxLag;
	}
}

float* EVDATA::getPData1(int n, int begNum) {
	float *data = d_pData1[n] + begNum;
	return data;
}

float* EVDATA::getPData2(int n, int begNum) {
	float *data = d_pData2[n] + begNum;
	return data;
}

float* EVDATA::getSData1(int n, int begNum) {
	float *data = d_sData1[n] + begNum;
	return data;
}

float* EVDATA::getSData2(int n, int begNum) {
	float *data = d_sData2[n] + begNum;
	return data;
}

