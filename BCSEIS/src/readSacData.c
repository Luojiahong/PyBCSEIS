//******************************************************************************//
//      readSacData.c                                                         	//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 10, 2003				//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 10, 2003                			//
//******************************************************************************//
#include "readSacData.h"

int readSacData(const char *sacFile, int phaCode, int preNum, int posNum, float *delta, float *tTm, float *phaWgt, float winData[], float evdp, const char *velCode) {
	SacSeisgram sg(sacFile, "r");
	sg.loadFromFile();
	float refTm = 0.0;
	float oTm = sg.getHeader().getO();
	if (phaCode == 0) {
		refTm = sg.getHeader().getA();
		*phaWgt = sg.getHeader().getUser0();
	} else if (phaCode == 1) {
		refTm = sg.getHeader().getT0();
		*phaWgt = sg.getHeader().getUser1();
		if (refTm == -12345.0) {
			float stla = sg.getHeader().getStla();
			float stlo = sg.getHeader().getStlo();
			float evla = sg.getHeader().getEvla();
			float evlo = sg.getHeader().getEvlo();
			float temp = cos((stla+evla)/2.0*PI/180);
			float dist = sqrt((stla-evla)*(stla-evla)+(stlo-evlo)*(stlo-evlo)*temp*temp)*111.195;
			float travTm = calcTravTm(velCode, 1, evdp, dist);
			refTm = oTm + travTm;
			*phaWgt = 0.0;
		}
	}
	//cout << ">>> break point in readSacData.c" << endl; 
	if (oTm == -12345.0 || refTm == -12345.0)
		return -1;
	float *sacData = sg.getData();
	*delta = sg.getHeader().getDelta();
	int npts = sg.getHeader().getNpts();
	int refNum = (int)floor((refTm - sg.getHeader().getB())/(*delta));

	*tTm = refNum*(*delta) + sg.getHeader().getB() - oTm;	
	// Note that it is not "refTm - oTm" because we need to align the two traces to the sample point before 
	// we calculate their relative time delays
	// Also we need to note that the 'b' time in a SAC file is not always zero
	
	int begNum = refNum - preNum;
	int endNum = refNum + posNum;
	if (begNum < 0 || endNum >= npts) {
		return -1;
	}
	int winLen = endNum - begNum + 1;
	for (int i = 0; i < winLen; i++)
		winData[i] = *(sacData+i+begNum);

	// demean the data
	demean(winData, winLen);

	// normalize the data
	int pos1 = 0;
	int pos2 = 0;
	float maxVal = 0.0;
	float minVal = 0.0;
	getMaxMin(winData, winLen, &pos1, &maxVal, &pos2, &minVal);
	float peakVal = fabs(maxVal);
	if (fabs(minVal) > peakVal)
		peakVal = fabs(minVal);
	for (int i = 0; i < winLen; i++)
		winData[i] /= peakVal;
	
	return 0;
}
