//******************************************************************************//
//      mathFuncs.c                                                             //
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, March 14, 2002                             //
//                                                                              //
//      Last modified: March 3, 2003                                           	//
//******************************************************************************//
//      Copyright (c) by Wen-xuan Du -- dxw@ldeo.columbia.edu                   //
//                                                                              //
//      The author reserves all rights of the package.  You are not             //
//      allowed to printout, copy, use, distribute, or modify any part of       //
//      the files in this package without permission.  The author is not        //
//      responsible for any damage of any computer hardware and software by     //
//      any appropriate or inappropriate use of the package.                    //
//******************************************************************************//
#include "mathFuncs.h"
#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}

void gaussj(double **a, int n, double **b, int m) {
// Linear equation solution by Gauss-Jordan elimination method. 
// (equation 2.1.1 in Numerical Recipe in C).
// a[0..n-1][0..n-1] is the input matrix. 
// b[0..n-1][0..m-1] is input containing the m right-hand side vectors. 
// On output, a[][] is replaced by its matrix inverse, and b[][] is replaced by 
// the corresponding set of solution vectors.

	int i,icol,irow,j,k,l,ll;
	double big,dum,pivinv,temp;

	// The integer arrays are used to bookeep on pivoting
	int	*indxc	= new int[n];
	int	*indxr	= new int[n];
	int	*ipiv	= new int[n];
	for (i = 0; i < n; i++) ipiv[i]=0;

	for (i = 0; i < n; i++) {
		big = 0.0;
		for (j = 0; j < n; j++) { // loop over each row
			if (ipiv[j] != 1) {
				for (k = 0; k < n; k++) { // loop over each column
					if (ipiv[k] == 0) {
						if (fabs(a[j][k]) >= big) {
							big	= fabs(a[j][k]);
							irow	= j;
							icol	= k;
						}
					} else if (ipiv[k] > 1) {
						cerr << "Internal error in gaussj(): Singular Matrix-1" << endl;
						exit (-1);
					}
				}
			}
		}
		++(ipiv[icol]);

		// We now have the pivot element, so we interchange rows, if needed, 
		// to put the pivot element on the diagonal. 
		// The columns are not physically interchanged, only relabeled:
		// indxc[i], the column of the i-th pivot element, is the ith column that
		// is reduced;
		// indxr[i] is the row in which that pivot element was originally located.
		// if indxc[i] != indxr[i] there is an implied column exchange.

		if (irow != icol) {
			for (l = 0; l < n; l++) SWAP(a[irow][l],a[icol][l])
			for (l = 0; l < m; l++) SWAP(b[irow][l],b[icol][l])
		}
		indxr[i] = irow;
		indxc[i] = icol;
		if (a[icol][icol] == 0.0) {
			cerr << "Internal error in gaussj(): Singular Matrix-2" << endl;
			exit (-1);
		}

		// We now are ready to divide the pivot row by the pivot element, 
		// which is located at irow and icol.
		pivinv	= 1.0 / a[icol][icol];
		a[icol][icol] = 1.0;
		for (l = 0; l < n; l++) a[icol][l] *= pivinv;
		for (l = 0; l < m; l++) b[icol][l] *= pivinv;

		// We now reduce the rows, except for the pivot one
		for (ll = 0; ll < n; ll++) {
			if (ll != icol) {
				dum = a[ll][icol];
				a[ll][icol] = 0.0;
				for (l = 0; l < n; l++) a[ll][l] -= a[icol][l]*dum;
				for (l = 0; l < m; l++) b[ll][l] -= b[icol][l]*dum;
			}
		}
	}

	for (l = (n-1); l >= 0; l--) {
		if (indxr[l] != indxc[l])
			for (k = 0; k < n; k++)
				SWAP(a[k][indxr[l]],a[k][indxc[l]]);
	}

	delete[]	ipiv;
	delete[]	indxr;
	delete[]	indxc;
}

void matrixMultiplication(double **a, double **b, double **c, int l, int m, int n) {
	// a[0..l-1][0..m-1], b[0..m-1][0..n-1] and c[0..l-1][0..n-1]
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < n; j++) {
			c[i][j]	= 0.0;
			for (int k = 0; k < m; k++) {
				c[i][j]	+= a[i][k]*b[k][j];
			}
		}
	}
}

void movingAverage(double data[], int nBeg, int nEnd, int halfWidth) {
	double	dataSum;
	double	*newData_p = new double[nEnd-nBeg+1];

	// smoothing part I
	for (int i = nBeg; i < (nBeg+halfWidth); i++) {
		dataSum	= 0.0;
		for (int j = nBeg; j <= (i+halfWidth); j++)
			dataSum	+= *(data+j);
		dataSum	/= (i+1+halfWidth-nBeg);
		*(newData_p+i-nBeg)	= dataSum;
	}

	// smoothing part II
	for (int i = (nBeg+halfWidth); i <= (nEnd-halfWidth); i++) {
		dataSum	= 0.0;
		for (int j = -halfWidth; j <= halfWidth; j++)
			dataSum	+= *(data+i+j);
		dataSum	/= (2*halfWidth+1);
		*(newData_p+i-nBeg)	= dataSum;
	}

	// smoothing part III
	for (int i = (nEnd-halfWidth+1); i <= nEnd; i++) {
		dataSum	= 0.0;
		for (int j = (i-halfWidth); j <= nEnd; j++)
			dataSum	+= *(data+j);
		dataSum	/= (nEnd-i+halfWidth+1);
		*(newData_p+i-nBeg)	= dataSum;
	}

	// assign the smoothed version back to the orginal data array
	for (int i = nBeg; i <= nEnd; i++)
		*(data+i) = *(newData_p+i-nBeg);

	// release the memory
	delete	[]newData_p;
}

void linearRegression(double x[], double y[], double sig[], int ndata, double *intercept, 
		double *interceptDev, double *slope, double *slopeDev, double *chisq) {

	double	s	= 0.0;
	double	sx	= 0.0;
	double	sy	= 0.0;
	double	sxx	= 0.0;
	double	sxy	= 0.0;
	double	sig2;
	for (int i = 0; i < ndata; i++) {
		sig[i]	= 1.0;
		sig2	= sig[i]*sig[i];
		s	+= 1.0/sig2;
		sx	+= x[i]/sig2;
		sy	+= y[i]/sig2;
		sxx	+= x[i]*x[i]/sig2;
		sxy	+= x[i]*y[i]/sig2;
	}
	double	delta	= s*sxx - sx*sx;
	*intercept	= (sxx*sy - sx*sxy) / delta;
	*slope		= (s*sxy - sx*sy) / delta;

	double	realVar	= 0.0;
	double	diff;
	for (int i = 0; i < ndata; i++) {
		diff	= y[i] - (*intercept + (*slope)*x[i]);
		realVar	+= diff*diff;
	}
	*chisq	= realVar;
	realVar	/= (ndata - 2);

	s	/= realVar;
	sx	/= realVar;
	sy	/= realVar;
	sxx	/= realVar;
	sxy	/= realVar;
	delta	= s*sxx - sx*sx;
	*interceptDev	= sqrt(sxx/delta);
	*slopeDev	= sqrt(s/delta);
	
}

void demean(float data[], int ndata) {
	float sum = 0.0;
	for (int i = 0; i < ndata; i++)
		sum += data[i];

	sum /= ndata;
	for (int i = 0; i < ndata; i++)
		data[i] -= sum;
}

void demean2(float data[], int ndata, int preNum, int posNum) {
	int dmNum = ndata - preNum - posNum;
	float sum = 0.0;
	for (int i = preNum; i < (ndata - posNum); i++)
		sum += data[i];
	sum /= dmNum;
	for (int i = preNum; i < (ndata - posNum); i++)
		data[i] -= sum;
}

void detrend(float data[], int ndata) {
	double *x = new double[ndata];
	double *y = new double[ndata];
	double *sig = new double[ndata];
	for (int i = 0; i < ndata; i++) {
		x[i] = i*1.0;
		y[i] = data[i];
		sig[i] = 1.0;
	}
	double intercept = 0.0;
	double interceptDev = 0.0;
	double slope = 0.0;
	double slopeDev = 0.0;
	double chisq = 0.0;
	linearRegression(x, y, sig, ndata, &intercept, &interceptDev, &slope, &slopeDev, &chisq);
	for (int i = 0; i < ndata; i++) {
		data[i] -= (intercept+slope*x[i]);
	}
	delete	[]x;
	delete	[]y;
	delete	[]sig;
}

double getStandardDeviation(double *data_p, double dataMean, int num) {
	double  dataErr = 0.0;
	for (int i = 0; i < num; i++) {
		dataErr += (*(data_p+i) - dataMean)*(*(data_p+i) - dataMean);
	}
	dataErr = sqrt(dataErr/(num-1));
	return dataErr;
}

void getMax(float *data, int npts, int *pos, float *max) {
	*max = -1e20;
	for (int i = 0; i < npts; i++) {
		if (*(data+i) > *max) {
			*max = *(data+i);
			*pos = i;
		}
	}
}

void getMaxMin(float *data, int npts, int *pos1, float *max, int *pos2, float *min) {
	*max = -1e20;
	*min = 1e20;
	for (int i = 0; i < npts; i++) {
		if (*(data+i) > *max) {
			*max = *(data+i);
			*pos1 = i;
		}
		if (*(data+i) < *min) {
			*min = *(data+i);
			*pos2 = i;
		}
	}
}

void tdcorr(float data1[], float data2[], int npts, float corr[]) {
	for (int i = 0; i < npts; i++) {
                float sum1 = 0.0;       // data1 move left; lag positive
                float sum2 = 0.;        // data1 move right; lag negative
                for (int j = 0; j < (npts-i); j++) {
                        sum1 += (*(data1+i+j))*(*(data2+j));
                        sum2 += (*(data2+i+j))*(*(data1+j));
                }
                *(corr+(npts-1)+i) = sum1;
                if (i == 0) continue;
                *(corr+(npts-1)-i) = sum2;
        }
}

void hannwindow(float percent, float window[], int npts) {
	if (percent < 0.0) percent = 0.0;
	if (percent > 0.5) percent = 0.5;
	int taperLen = (int)(npts*percent);
	for (int i = 0; i < taperLen; i++)
		window[i] = 0.5*(1.0-cos(i*PI/taperLen));
	for (int i = taperLen; i < (npts-taperLen); i++)
		window[i] = 1.0;
	for (int i = (npts-taperLen); i < npts; i++)
		window[i] = 0.5*(1.0-cos((i-npts+1)*PI/taperLen));
}

void hanntaper(float percent, float data[], int npts) {
	float *window = new float[npts];
	hannwindow(percent, window, npts);
	for (int i = 0; i < npts; i++)
		data[i] *= window[i];
	delete []window;
}

void hanntaper2(float percent, float **data1, float **data2, int npts) {
	float *window = new float[npts];
	hannwindow(percent, window, npts);
	for (int m = 0; m < npts; m++) {
		for (int n = 0; n < npts; n++) {
			data1[m][n] *= window[m]*window[n];
			data2[m][n] *= window[m]*window[n];
		}
	}
	delete []window;
}

#undef SWAP
