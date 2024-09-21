//******************************************************************************//
//      calcCCTmDelay.c                                                        	//
//										//
//      Author: Wen-xuan(Wayne) Du, September 11, 2003	                        //
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 11, 2003     					//
//******************************************************************************//
#include "calcCCTmDelay.h"

void calcCCTmDelay(float data1[], float data2[], int winLen, int maxNum, int option, int pFlag, int *ccLag, float *ccCoef) {
	int padWinLen = getpower2(winLen);	// length of the data arrays after zero padding
	int padNum = padWinLen-winLen;
	int prePadNum = (int)floor(padNum/2.0);	// # of padded zeros before windowed data
	float *winData1 = new float[padWinLen];
	float *winData2 = new float[padWinLen];
	for (int i = 0; i < prePadNum; i++) {
		*(winData1+i) = 0.0;
		*(winData2+i) = 0.0;
	}
	for (int i = 0; i < winLen; i++) {
		*(winData1+prePadNum+i) = *(data1+i);
		*(winData2+prePadNum+i) = *(data2+i);
	}
	for (int i = (winLen+prePadNum); i < padWinLen; i++) {
		*(winData1+i) = 0.0;
		*(winData2+i) = 0.0;
	}
	float *corrSeq = new float[2*padWinLen];
	int ccLen = padWinLen/2;
	if (maxNum != 0 && maxNum < ccLen)
		ccLen = maxNum;
	float *ccSeq = new float[2*ccLen-1];
	// lagnum in range [-(ccLen-1), (ccLen-1)]
	if (option == 1) {
		correl(winData1-1,winData2-1,padWinLen,corrSeq-1);
		for (int nn = 0; nn < (ccLen-1); nn++)
			ccSeq[nn] = corrSeq[padWinLen-ccLen+1+nn];
		for (int nn = 0; nn < ccLen; nn++)
			ccSeq[ccLen-1+nn] = corrSeq[nn];
	} else if (option == 0) {
		tdcorr(winData1,winData2,padWinLen,corrSeq);
		for (int nn = 0; nn < (2*ccLen-1); nn++)
			ccSeq[nn] = corrSeq[padWinLen-1-(ccLen-1)+nn];
	}
	delete  []corrSeq;
	getMax(ccSeq,(2*ccLen-1),ccLag,ccCoef);
	float powData1 = 0.0;
	float powData2 = 0.0;
	for (int i = 0; i < padWinLen; i++) {
		powData1 += (*(winData1+i))*(*(winData1+i));
		powData2 += (*(winData2+i))*(*(winData2+i));
	}
	float powData = sqrt(powData1*powData2);
	*ccCoef /= powData;
	*ccLag -= (ccLen-1);

	if (pFlag == 1) {
		ofstream CC("cc.dat");
		if (!CC) {
			cerr << "Can not open data file 'cc.dat' for writing. Exit!";
			exit (-1);
		}
		CC.setf(ios::scientific, ios::floatfield);
		CC.precision(4);
		for (int i = 0; i < (2*ccLen-1); i++)
			CC << (i-(ccLen-1)) << "\t" << ccSeq[i]/powData << endl;
	}

	// release the memories
	delete  []ccSeq;
	delete  []winData1;
	delete  []winData2;

}
