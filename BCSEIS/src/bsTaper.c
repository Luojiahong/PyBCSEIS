//******************************************************************************//
//      bsTaper.c                                                           	//
//                                                                              //
//      Author: Wen-xuan(Wayne) Du, September 24, 2003				//
//	        dxw@geology.wisc.edu						//
//	        								//
//      Last modified: September 24, 2003					//
//******************************************************************************//
#include "bsTaper.h"

void bsWindow(int winMode, int winLen, float window[]) {	// winLen is an even number
	for (int i = 0; i < winLen; i++)
		window[i] = 0.0;
	int len = winLen/2 - 1;
	if (winMode == 1) {	// Optimum window
		for (int m = 0; m <= len; m++) {
			window[len+m] = 1.0/PI*fabs(sin(PI*m/len))+(1.0-1.0*m/len)*(cos(PI*m/len));
			window[len-m] = window[len+m];
		}
	} else if (winMode == 2) {	// Parzen window
		int halfLen = len/2;
		for (int m = 0; m < halfLen; m++) {
			float tmp = 1.0*m/len;
			window[len+m] = 1.0 - 6.0*tmp*tmp*(1.0-tmp);
			window[len-m] = window[len+m];
		}
		for (int m = halfLen; m <= len; m++) {
			float tmp = 1.0-1.0*m/len;
			window[len+m] = 2.0*tmp*tmp*tmp;
			window[len-m] = window[len+m];
		}
	}
}

void bsTaper(int winMode, int winLen, float **data1, float **data2) {
	int len = winLen/2 - 1;
	float *window = new float[winLen];
	bsWindow(winMode, winLen, window);
	for (int i = 0; i < winLen; i++) {
		for (int j = 0; j < winLen; j++) {
			int index = len+(i-j);
			float tmp = 0.0;
			if (index < winLen && index >= 0) 
				tmp = window[index];
			float weight = window[i]*window[j]*tmp;
			data1[i][j] *= weight;
			data2[i][j] *= weight;
		}
	}
	delete	[]window;
}

