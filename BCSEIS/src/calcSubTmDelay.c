//******************************************************************************//
//      calcSubTmDelay.c                                                       	//
//										//
//      Author: Wen-xuan(Wayne) Du, January 31, 2003	                        //
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 12, 2003					//
//******************************************************************************//
#include "calcSubTmDelay.h"

void calcSubTmDelay(float odata1[], float odata2[], int npts, int shiftLag, int segLen, float delta, float lf, float hf, float *delay, float *sigma) {

	// construct the new data arrays by shifting the second trace
	float *data1 = odata1; 
	float *data2 = new float[npts];
	if (shiftLag > 0) {
		for (int i = 0; i < shiftLag; i++)
			data2[i] = 0.0;
		for (int i = shiftLag; i < npts; i++)
			data2[i] = odata2[i-shiftLag];
	} else if (shiftLag < 0) {
		for (int i = 0; i < (npts-1+shiftLag); i++)
			data2[i] = odata2[i-shiftLag];
		for (int i = (npts-1+shiftLag); i < npts; i++)
			data2[i] = 0.0;
	} else {
		for (int i = 0; i < npts; i++)
			data2[i] = odata2[i];
	}
	
	int nfft = segLen;	// number of points to do FFT
	int halfNfft = nfft/2;
	// number of segments to divide the time series for spectrum calculation
	int segNum = (int)ceil((float)npts/halfNfft) - 1;

	vector<Complex> compFFT1;	// complex fft vector for each segment of SAC data 1
	vector<Complex> compFFT2;	// complex fft vector for each segment of SAC data 2
	vector<Complex> csp;		// cross-spectrum vector
	compFFT1.reserve(halfNfft);
	compFFT2.reserve(halfNfft);
	csp.reserve(halfNfft);
	vector<Complex>::iterator it1, it2, it3;
	float *psd1 = new float[halfNfft];	// psd array of SAC data 1
	float *psd2 = new float[halfNfft];	// psd array of SAC data 2
	float *phase = new float[halfNfft];	// phase array
	float *coh = new float[halfNfft];	// coherence array
	float *wgt = new float[halfNfft];	// weighting array
	for (int n = 0; n < halfNfft; n++) {
		compFFT1.push_back(Complex(0.0, 0.0));
		compFFT2.push_back(Complex(0.0, 0.0));
		csp.push_back(Complex(0.0, 0.0));
		psd1[n] = 0.0;
		psd2[n] = 0.0;
		phase[n] = 0.0;
	}

	float *segData1 = new float[nfft];
	float *segData2 = new float[nfft];
	float *fftData1 = new float[2*nfft];
	float *fftData2 = new float[2*nfft];
	for (int i = 1; i <= segNum; i++) {

		// prepare the data in each segment
		if (i != segNum) {
			for (int n = 0; n < nfft; n++) {
				segData1[n] = *(data1+(i-1)*halfNfft+n);
				segData2[n] = *(data2+(i-1)*halfNfft+n);
			}
		} else {
			for (int n = 0; n < nfft; n++) {
				segData1[n] = *(data1+npts-nfft+n);
				segData2[n] = *(data2+npts-nfft+n);
			}
		}

		// detrend the data
		detrend(segData1, nfft);
		detrend(segData2, nfft);

		// apply the hanning taper
		hanntaper(TAPERVAL, segData1, nfft);
		hanntaper(TAPERVAL, segData2, nfft);

                // FFT operation
		twofft(segData1-1,segData2-1,fftData1-1,fftData2-1,nfft);
		
//		realft(segData1-1,nfft,1);
//		realft(segData2-1,nfft,1);

		it1 = compFFT1.begin();
		it2 = compFFT2.begin();
		it3 = csp.begin();
		Complex fd1, fd2, tmp;
		for (int n = 0; (it1 != compFFT1.end() && it2 != compFFT2.end() && it3 != csp.end());) { 
			fd1 = Complex(fftData1[2*n],fftData1[2*n+1]);
			fd2 = Complex(fftData2[2*n],fftData2[2*n+1]);
			tmp = fd2*fd1.getConj();
			*it1 = fd1;
			*it2 = fd2;
			*it3 = tmp + *it3;
			psd1[n] = psd1[n] + fd1.getPower();
			psd2[n] = psd2[n] + fd2.getPower();
			phase[n] = phase[n] + tmp.getPhase();
			it1++; it2++; it3++; n++;
		}
	}
	delete	[]segData1;
	delete	[]segData2;
	delete	[]fftData1;
	delete	[]fftData2;

	// calculate the coherence and weighting values
	it1 = csp.begin();
	for (int n = 0; (n < halfNfft && it1 != csp.end());) {
		phase[n] /= segNum;
		coh[n] = (*it1).getPower()/(psd1[n]*psd2[n]);
		if (coh[n] >= 0.90) {
			float coh2 = coh[n]*coh[n];
			if (coh2 > 0.99)
				coh2 = 0.99;
			wgt[n] = sqrt(coh2/(1-coh2));
		} else {
			wgt[n] = 0.0;
		}
		n++; it1++;
	}
	delete	[]psd1;
	delete	[]psd2;

	// set the low and high end numbers for frequency cutoff in weighted least-square fitting
	float deltaFreq = 1.0/(nfft*delta);
	int tmp = (int)floor(0.75*(halfNfft-1));
	int lowend = (int)floor(lf/deltaFreq) - 1;
	int highend = (int)floor(hf/deltaFreq) - 1;
	if (lowend < 0) lowend = 0;
	if (highend > tmp) highend = tmp;

	int fittingNum = 0;
	float tmp1 = 0.0;
	float tmp2 = 0.0;
	for (int n = lowend; n <= highend; n++) {
		float weight = wgt[n];
		float freq = n * deltaFreq;
		if (weight > 0) {
			tmp1 += weight*weight*freq*phase[n];
			tmp2 += weight*weight*freq*freq;
			fittingNum++;
		}
	}
	if (fittingNum < 2) {
		*delay = 0.0;
		*sigma = -1.0;
		delete	[]phase;
		delete	[]wgt;
		delete	[]coh;
		return;
	}
	float slope = tmp1/tmp2;
	*delay = -1.0*slope/(2.0*PI*delta);
	if (fabs(*delay) >= 1.0) {	// delay out of range (-1, 1)
		*delay = 0.0;
		*sigma = -1.0;
		delete	[]phase;
		delete	[]wgt;
		delete	[]coh;
		return;
	}
	
	// calculate the standard deviation of the slope
	tmp1 = 0.0;
	float tmp3 = 0.0;
	for (int n = lowend; n <= highend; n++) {
		float weight = wgt[n];
		float freq = n * deltaFreq;
		if (weight > 0) {
			tmp1 += weight*freq;
			float diff = phase[n] - slope * freq;
			tmp3 += weight*weight*diff*diff;
		}
	}
	float variance = (tmp3/(fittingNum-1))*(tmp1*tmp1)/(tmp2*tmp2);
	*sigma = sqrt(variance)/(2.0*PI*delta);
		
	// release the memories
	delete	[]phase;
	delete	[]wgt;
	delete	[]coh;
	delete	[]data2;
}	
