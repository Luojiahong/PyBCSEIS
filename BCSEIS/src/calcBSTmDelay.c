//******************************************************************************//
//      calcBSTmDelay.c                                                        	//
//                                                                              //
//      Author: Wen-xuan(Wayne) Du, February 27, 2003                         	//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 11, 2003                                      	//
//******************************************************************************//
#include "calcBSTmDelay.h"
#include <string.h>
#include <cstdlib>

// Calculate the bispectra (Bxxx, Bxyx) in one single run
void cum32(int npts, float x[], float y[], float **cumxxx, float **cumxyx) {
	// just in case
	for (int i = 0; i < 2*npts; i++) {
		for (int j = 0; j < 2*npts; j++) {
			cumxxx[i][j] = 0.0;
			cumxyx[i][j] = 0.0;
		}
	}
	for (int i = -(npts-1); i <= (npts-1); i++) {
		for (int j = -(npts-1); j <= (npts-1); j++) {
			float sumxxx = 0.0;
			float sumxyx = 0.0;

			int n1 = 0;
			int n2 = -i;
			int n3 = -j;
			int k1 = n1;
			if (n2 > k1) k1 = n2;
			if (n3 > k1) k1 = n3;

			n1 = npts - 1;
			n2 = npts - 1 - i;
			n3 = npts - 1 - j;
			int k2 = n1;
			if (n2 < k2) k2 = n2;
			if (n3 < k2) k2 = n3;

			for (int k = k1; k <= k2; k++) {
				float tmpx = x[k]*x[k+j];
				sumxxx += tmpx*x[k+i];
				sumxyx += tmpx*y[k+i];
			}
			cumxxx[i+npts-1][j+npts-1] = sumxxx/npts;
			cumxyx[i+npts-1][j+npts-1] = sumxyx/npts;
		}
	}
}

// March 11, 2003: Impose limit maxLag 
void calcBSTmDelay(float data1[], float data2[], int npts, int segLen, int maxLag, int pFlag, int *lagNum, float *maxVal) {
	
	int segNum = (int)ceil((float)npts*2/segLen) - 1;
	int halfSegLen = segLen/2;
	int cumLen = 2*segLen;	// length of the cumulant matrix
	float **cum1 = new float*[cumLen];	// bispectrum for time series 1
	float **cum2 = new float*[cumLen];	// cross-bispectrum between time series 1 and 2
	float **segCum1 = new float*[cumLen];	// segment bispectrum for time series 1
	float **segCum2 = new float*[cumLen];	// segment cross-bispectrum between time series 1 and 2
	for (int i = 0; i < cumLen; i++) {	// initialization
		cum1[i] = new float[cumLen];
		cum2[i] = new float[cumLen];
		segCum1[i] = new float[cumLen];
		segCum2[i] = new float[cumLen];
		for (int j = 0; j < cumLen; j++) {
			cum1[i][j] = 0.0;
			cum2[i][j] = 0.0;
			segCum1[i][j] = 0.0;
			segCum2[i][j] = 0.0;
		}
	}

	// data array for each segment
	float *segData1 = new float[segLen];
	float *segData2 = new float[segLen];
	for (int i = 1; i <= segNum; i++) {

		// prepare the data in the segment
		if (i != segNum) {
			for (int n = 0; n < segLen; n++) {
				segData1[n] = *(data1+(i-1)*halfSegLen+n);
				segData2[n] = *(data2+(i-1)*halfSegLen+n);
			}
		} else {
	 		for (int n = 0; n < segLen; n++) {
		 		segData1[n] = *(data1+npts-segLen+n);
			 	segData2[n] = *(data2+npts-segLen+n);
			}
		}
	                
		// demean the data
		demean(segData1, segLen);
		demean(segData2, segLen);

		// taper the data with hanning window
		hanntaper(TAPERVAL, segData1, segLen);
		hanntaper(TAPERVAL, segData2, segLen);

		// calculate the 3rd-order cumulants
		cum32(segLen, segData1, segData2, segCum1, segCum2);
		for (int m = 0; m < cumLen; m++) {
			for (int n = 0; n < cumLen; n++) {
				/*
				if (i == 2 && m == 0)
					cout << segCum1[m][n] << endl;
				*/
				cum1[m][n] += segCum1[m][n];
				cum2[m][n] += segCum2[m][n];
			}
		}
	}
	for (int m = 0; m < cumLen; m++) {
		for (int n = 0; n < cumLen; n++) {
			cum1[m][n] /= segNum;
			cum2[m][n] /= segNum;
		}
	}
	delete []segData1;
	delete []segData2;
	for (int i = 0; i < cumLen; i++) {
		delete [](segCum1[i]);
		delete [](segCum2[i]);
	}
	delete []segCum1;
	delete []segCum2;

	// tapering the cumulant matrices
//	hanntaper2(TAPERVAL, cum1, cum2, cumLen);
	bsTaper(2, cumLen, cum1, cum2);

	// prepare the 1-D complex data arrays
	int cumLen2 = 2*cumLen*cumLen;
	float *cumData1 = new float[cumLen2];
	float *cumData2 = new float[cumLen2];
	for (int m = 0; m < cumLen; m++) {
		for (int n = 0; n < cumLen; n++) {
			int i = m*cumLen + n;
			cumData1[2*i] = cum1[m][n];
			/*
			if (m == 0) 
				cout << cumData1[2*i] << endl;
			*/
			cumData1[2*i+1] = 0.0;
			cumData2[2*i] = cum2[m][n];
			cumData2[2*i+1] = 0.0;
		}
	}

	for (int i = 0; i < cumLen; i++) {
		delete [](cum1[i]);
		delete [](cum2[i]);
	}
	delete []cum1;
	delete []cum2;

	// 2-D FFT on the cumulants to get bispectra
	long unsigned int *nn = new (long unsigned int);
	nn[0] = cumLen;
	nn[1] = cumLen;
	fourn(cumData1-1, nn-1, 2, 1);
	fourn(cumData2-1, nn-1, 2, 1);
       	delete []nn;
	
	// March 3, 2003: Equivalent ways to sum over omega_2:
	//                Using inverse FFT and take the first two slots (t = 0) is the same as 
	//                adding up each row and divide the result by 'cumLen'
	Complex *bispecRatio = new Complex[cumLen];
	Complex tmp1, tmp2, tmp3;
	for (int m = 0; m < cumLen; m++) {
		bispecRatio[m] = Complex(0.0, 0.0);
		for (int n = 0; n < cumLen; n++) {
			int i = m*cumLen + n;
			float real1 = cumData1[2*i];
			float imag1 = cumData1[2*i+1];
			tmp1 = Complex(real1, imag1);
			tmp2 = Complex(cumData2[2*i], cumData2[2*i+1]);
/*
			if (real1 == 0.0 && imag1 == 0.0) {
				tmp3 = Complex(0.0, 0.0);
			} else {
				tmp3 = tmp2 / tmp1;
			}
*/
			float phaseDiff = tmp2.getPhase() - tmp1.getPhase();
			tmp3 = Complex(cos(phaseDiff), sin(phaseDiff));
			tmp1 = bispecRatio[m];
			bispecRatio[m] = tmp1 + tmp3;
		}
		tmp1 = bispecRatio[m];
		bispecRatio[m] = tmp1 / cumLen;
	}
	delete []cumData1;
	delete []cumData2;

	// do inverse FFT to get the time series
	float *bispecRatio2 = (float*)bispecRatio;
	four1(bispecRatio2-1, cumLen, -1);
	
	float *lagArr = new float[cumLen];
	for (int i = 0; i < cumLen; i++) {
		lagArr[i] = bispecRatio2[2*i] / cumLen;
//		cout << lagArr[i] << endl;
	}

	int maxPos = 0;
	int bsLen = segLen - 1;
	if (maxLag != 0 && maxLag < bsLen)
		bsLen = maxLag;
	int arrNum2 = 2*bsLen+1;
	float *lagArr2 = new float[arrNum2];
	for (int i = 0; i < bsLen; i++)
		lagArr2[i] = lagArr[cumLen-bsLen+i];
	for (int i = bsLen; i < arrNum2; i++)
		lagArr2[i] = lagArr[i-bsLen];

	getMax(lagArr2, arrNum2, &maxPos, maxVal);
	maxPos = maxPos - bsLen;
	*lagNum = -1*maxPos;

	if (pFlag == 1) {	// print out the BS-correlation series if necessary
		ofstream BS("bs.dat");
		if (!BS) {
			cerr << "Can not open data file 'bs.dat' for writing. Exit!";
			exit (-1);
		}
		BS.setf(ios::scientific, ios::floatfield);
		BS.precision(4);
		for (int i = 0; i < arrNum2; i++) {
			int pos = i - bsLen;
			BS << -pos << "\t" << lagArr2[i] << endl;
		}
	}

	delete []lagArr2;
	delete []lagArr;
	delete []bispecRatio;
}
