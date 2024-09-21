//******************************************************************************//
//      fourier.c                                                               //
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, July 16, 2001                              //
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: March 1, 2003                                           	//
//******************************************************************************//
#include "fourier.h"
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

/********************************************************************************/
int getpower2(int num) {
	int k = 2;
	while (num > k) k *= 2;
        return k;
}

/********************************************************************************/
void four1(float data[], unsigned long nn, int isign)
// Replaces data[1..2*nn] by its discrete Fourier transform, if isign is input as 1; 
// or replaces data[1..2*nn] by nn times its inverse discrete Fourier transform, 
// if isign is input as -1.
// data is a complex array of length nn or, equivalently, a real array of length 2*nn. 
// nn MUST be an integer power of 2 (this is not checked for!).
{

unsigned long	n,mmax,m,j,istep,i;
double		wtemp,wr,wpr,wpi,wi,theta; 
		// Double precision for the trigonomet-ric recurrences. 
float		tempr,tempi;

n = nn << 1;
j = 1;

for (i = 1; i < n; i += 2) { // This is the bit-reversal section of the routine. 
	if(j > i) {
		SWAP(data[j],data[i]); // Exchange the two complex numbers.
		SWAP(data[j+1],data[i+1]);
	}
	m = n >> 1;
	while (m >= 2 && j > m) {
		j -= m;
		m >>= 1;
	}
	j += m;
}

// Here begins the Danielson-Lanczos section of the routine.
mmax = 2;
while (n > mmax) { // Outer loop executed log2(nn) times.
	istep = mmax << 1;
	theta = isign*(6.28318530717959/mmax); // Initialize the trigonometric recurrence.
	wtemp = sin(0.5*theta);
	wpr   = -2.0*wtemp*wtemp;
	wpi   = sin(theta);
	wr    = 1.0;
	wi    = 0.0;

	for (m = 1; m< mmax; m += 2) { // Here are the two nested inner loops.
		for (i = m; i <= n; i += istep) {
			j = i + mmax; // This is the Danielson-Lanczos formula: 
			tempr = wr*data[j] - wi*data[j+1];
			tempi = wr*data[j+1] + wi*data[j];
			data[j] = data[i] - tempr;
			data[j+1] = data[i+1] - tempi;
			data[i] += tempr;
			data[i+1] += tempi;
		}
		wr = (wtemp=wr)*wpr - wi*wpi + wr; // Trigonometric recurrence.
		wi = wi*wpr + wtemp*wpi + wi;
	}
	mmax = istep;
}

}

/********************************************************************************/
void dfour1(double data[], unsigned long nn, int isign)
// Replaces data[1..2*nn] by its discrete Fourier transform, if isign is input as 1; 
// or replaces data[1..2*nn] by nn times its inverse discrete Fourier transform, 
// if isign is input as -1.
// data is a complex array of length nn or, equivalently, a real array of length 2*nn. 
// nn MUST be an integer power of 2 (this is not checked for!).
{

unsigned long   n,mmax,m,j,istep,i;
double          wtemp,wr,wpr,wpi,wi,theta; 
                // Double precision for the trigonomet-ric recurrences. 
double          tempr,tempi;

n = nn << 1;
j = 1;

for (i = 1; i < n; i += 2) { // This is the bit-reversal section of the routine. 
        if(j > i) {
                SWAP(data[j],data[i]); // Exchange the two complex numbers.
                SWAP(data[j+1],data[i+1]);
        }
        m = n >> 1;
        while (m >= 2 && j > m) {
                j -= m;
                m >>= 1;
        }
        j += m;
}

// Here begins the Danielson-Lanczos section of the routine.
mmax = 2;
while (n > mmax) { // Outer loop executed log2(nn) times.
        istep = mmax << 1;
        theta = isign*(6.28318530717959/mmax); // Initialize the trigonometric recurrence.
        wtemp = sin(0.5*theta);
        wpr   = -2.0*wtemp*wtemp;
        wpi   = sin(theta);
        wr    = 1.0;
        wi    = 0.0;

        for (m = 1; m< mmax; m += 2) { // Here are the two nested inner loops.
                for (i = m; i <= n; i += istep) {
                        j = i + mmax; // This is the Danielson-Lanczos formula: 
                        tempr = wr*data[j] - wi*data[j+1];
                        tempi = wr*data[j+1] + wi*data[j];
                        data[j] = data[i] - tempr;
                        data[j+1] = data[i+1] - tempi;
                        data[i] += tempr;
                        data[i+1] += tempi;
                }
                wr = (wtemp=wr)*wpr - wi*wpi + wr; // Trigonometric recurrence.
                wi = wi*wpr + wtemp*wpi + wi;
        }
        mmax = istep;
}

}

/********************************************************************************/
void twofft(float data1[], float data2[], float fft1[], float fft2[], unsigned long n)
// Given two real input arrays data1[1..n] and data2[1..n], this routine calls four1 and
// returns two complex output arrays, fft1[1..2n] and fft2[1..2n], each of complex length
// n (i.e., real length 2*n), which contain the discrete Fourier transforms of the respective data
// arrays. n MUST be an integer power of 2.
{

// void four1(float data[], unsigned long nn, int isign);
unsigned long	nn3,nn2,jj,j;
float		rep,rem,aip,aim;

nn3 = 1 + (nn2 = 2+n+n);
for (j=1,jj=2; j<=n; j++,jj+=2) { // Pack the two real arrays into one complex array. 
	fft1[jj-1] = data1[j];
	fft1[jj]   = data2[j];
}
four1(fft1,n,1); // Transform the complex array.
fft2[1] = fft1[2];
fft1[2] = fft2[2] = 0.0;
for (j=3; j<=n+1; j+=2) {
	rep = 0.5*(fft1[j]+fft1[nn2-j]); // Use symmetries to separate the two transforms. 
	rem = 0.5*(fft1[j]-fft1[nn2-j]);
	aip = 0.5*(fft1[j+1]+fft1[nn3-j]);
	aim = 0.5*(fft1[j+1]-fft1[nn3-j]);
	fft1[j] = rep; // Ship them out in two complex arrays.
	fft1[j+1] = aim;
	fft1[nn2-j] = rep;
	fft1[nn3-j] = -aim;
	fft2[j] = aip;
	fft2[j+1] = -rem;
	fft2[nn2-j] = aip;
	fft2[nn3-j] = rem;
}

}

/********************************************************************************/
void realft(float data[], unsigned long n, int isign)
// Calculates the Fourier transform of a set of n real-valued data points. 
// Replaces this data (which is stored in array data[1..n]) by the positive 
// frequency half of its complex Fourier transform.
// The real-valued first and last components of the complex transform are 
// returned as elements data[1] and data[2], respectively. n must be a power of 2. 
// This routine also calculates the inverse transform of a complex data array if 
// it is the transform of real data. (Result in this case must be multiplied by 2/n.)
{
// void four1(float data[], unsigned long nn, int isign);
unsigned long	i,i1,i2,i3,i4,np3;
float		c1=0.5,c2,h1r,h1i,h2r,h2i;
double		wr,wi,wpr,wpi,wtemp,theta; 
		// Double precision for the trigonometric recurrences.

theta = 3.141592653589793/(double) (n>>1); // Initialize the recurrence.
if (isign == 1) {
	c2 = -0.5;
	four1(data,n>>1,1); // The forward transform is here.
} else {
	c2 = 0.5; // Otherwise set up for an inverse transform. 
	theta = -theta;
}
wtemp = sin(0.5*theta);
wpr   = -2.0*wtemp*wtemp;
wpi   = sin(theta);
wr    = 1.0+wpr;
wi    = wpi;
np3   = n+3;
for (i=2; i<=(n>>2); i++) { // Case i=1 done separately below.
	i4 = 1 +(i3=np3-(i2=1+(i1=i+i-1)));
	h1r = c1*(data[i1]+data[i3]); // The two separate transforms are separated out of data. 
	h1i = c1*(data[i2]-data[i4]);
	h2r = -c2*(data[i2]+data[i4]);
	h2i = c2*(data[i1]-data[i3]);
	data[i1] = h1r+wr*h2r-wi*h2i; 
	// Here they are recombined to form the true transform of the original real data.
	data[i2] = h1i+wr*h2i+wi*h2r;
	data[i3] = h1r-wr*h2r+wi*h2i;
	data[i4] = -h1i+wr*h2i+wi*h2r;
	wr = (wtemp=wr)*wpr-wi*wpi+wr; // The recurrence.
	wi = wi*wpr+wtemp*wpi+wi;
}
if (isign == 1) {
	data[1] = (h1r=data[1])+data[2]; 
	// Squeeze the first and last data together to get them all within the original array.
	data[2] = h1r-data[2];
} else {
	data[1] = c1*((h1r=data[1])+data[2]);
	data[2] = c1*(h1r-data[2]);
	four1(data,n>>1,-1); // This is the inverse transform for the case isign=-1. 
}

}


/********************************************************************************/
void drealft(double data[], unsigned long n, int isign)
// Calculates the Fourier transform of a set of n real-valued data points. 
// Replaces this data (which is stored in array data[1..n]) by the positive 
// frequency half of its complex Fourier transform.
// The real-valued first and last components of the complex transform are 
// returned as elements data[1] and data[2], respectively. n must be a power of 2. 
// This routine also calculates the inverse transform of a complex data array if 
// it is the transform of real data. (Result in this case must be multiplied by 2/n.)
{
// void dfour1(double data[], unsigned long nn, int isign);
unsigned long   i,i1,i2,i3,i4,np3;
double           c1=0.5,c2,h1r,h1i,h2r,h2i;
double          wr,wi,wpr,wpi,wtemp,theta; 
                // Double precision for the trigonometric recurrences.

theta = 3.141592653589793/(double) (n>>1); // Initialize the recurrence.
if (isign == 1) {
        c2 = -0.5;
        dfour1(data,n>>1,1); // The forward transform is here.
} else {
        c2 = 0.5; // Otherwise set up for an inverse transform. 
        theta = -theta;
}
wtemp = sin(0.5*theta);
wpr   = -2.0*wtemp*wtemp;
wpi   = sin(theta);
wr    = 1.0+wpr;
wi    = wpi;
np3   = n+3;
for (i=2; i<=(n>>2); i++) { // Case i=1 done separately below.
        i4 = 1 +(i3=np3-(i2=1+(i1=i+i-1)));
        h1r = c1*(data[i1]+data[i3]); // The two separate transforms are separated out of data. 
        h1i = c1*(data[i2]-data[i4]);
        h2r = -c2*(data[i2]+data[i4]);
        h2i = c2*(data[i1]-data[i3]);
        data[i1] = h1r+wr*h2r-wi*h2i; 
        // Here they are recombined to form the true transform of the original real data.
        data[i2] = h1i+wr*h2i+wi*h2r;
        data[i3] = h1r-wr*h2r+wi*h2i;
        data[i4] = -h1i+wr*h2i+wi*h2r;
        wr = (wtemp=wr)*wpr-wi*wpi+wr; // The recurrence.
        wi = wi*wpr+wtemp*wpi+wi;
}
if (isign == 1) {
        data[1] = (h1r=data[1])+data[2]; 
        // Squeeze the first and last data together to get them all within the original array.
        data[2] = h1r-data[2];
} else {
        data[1] = c1*((h1r=data[1])+data[2]);
        data[2] = c1*(h1r-data[2]);
        dfour1(data,n>>1,-1); // This is the inverse transform for the case isign=-1. 
}

}


/********************************************************************************/
void correl(float data1[], float data2[], unsigned long n, float ans[])
// Computes the correlation of two real data sets data1[1..n] and data2[1..n] (including any
// user-supplied zero padding). n MUST be an integer power of two. The answer is returned as
// the first n points in ans[1..2*n] stored in wrap-around order, i.e., correlations at increasingly
// negative lags are in ans[n] on down to ans[n/2+1], while correlations at increasingly positive
// lags are in ans[1] (zerolag)on up toans[n/2]. Notethatans must be supplied in the calling
// program with length at least 2*n, since it is also used as working space. Sign convention of
// this routine: if data1 lags data2, i.e., is shifted to the right of it, then ans will show a peak
// at positive lags.
{

// void realft(float data[], unsigned long n, int isign);
// void twofft(float data1[], float data2[], float fft1[], float fft2[], unsigned long n);
unsigned long	no2,i;
float		dum,*fft;

fft = float_vector(1 + (n<<1));
twofft(data1,data2,fft,ans,n); // Transform both data vectors at once.
no2 = n>>1; // Normalization for inverse FFT.
for (i=2; i<=n+2; i+=2) {
	ans[i-1] = (fft[i-1]*(dum=ans[i-1])+fft[i]*ans[i])/no2; 
	// Multiply to find FFT of their correlation.
	ans[i] = (fft[i]*dum-fft[i-1]*ans[i])/no2;
}
ans[2] = ans[n+1]; // Pack first and last into one element.
realft(ans,n,-1); // Inverse transform gives correlation.

free(fft);

}

/********************************************************************************/
float *float_vector(int nl)
{
	float *v;
	if ( (v = (float *)malloc(nl*sizeof(float))) == NULL) {
		fprintf(stderr, "allocation failure in vector()");
		exit(1);
	}
	return(v);
}


/********************************************************************************/
void fourn(float data[], unsigned long nn[], int ndim, int isign)
{
	int idim;
	unsigned long i1,i2,i3,i2rev,i3rev,ip1,ip2,ip3,ifp1,ifp2;
	unsigned long ibit,k1,k2,n,nprev,nrem,ntot;
	float tempi,tempr;
	double theta,wi,wpi,wpr,wr,wtemp;

	for (ntot=1,idim=1;idim<=ndim;idim++)
		ntot *= nn[idim];
	nprev=1;
	for (idim=ndim;idim>=1;idim--) {
		n=nn[idim];
		nrem=ntot/(n*nprev);
		ip1=nprev << 1;
		ip2=ip1*n;
		ip3=ip2*nrem;
		i2rev=1;
		for (i2=1;i2<=ip2;i2+=ip1) {
			if (i2 < i2rev) {
				for (i1=i2;i1<=i2+ip1-2;i1+=2) {
					for (i3=i1;i3<=ip3;i3+=ip2) {
						i3rev=i2rev+i3-i2;
						SWAP(data[i3],data[i3rev]);
						SWAP(data[i3+1],data[i3rev+1]);
					}
				}
			}
			ibit=ip2 >> 1;
			while (ibit >= ip1 && i2rev > ibit) {
				i2rev -= ibit;
				ibit >>= 1;
			}
			i2rev += ibit;
		}
		ifp1=ip1;
		while (ifp1 < ip2) {
			ifp2=ifp1 << 1;
			theta=isign*6.28318530717959/(ifp2/ip1);
			wtemp=sin(0.5*theta);
			wpr = -2.0*wtemp*wtemp;
			wpi=sin(theta);
			wr=1.0;
			wi=0.0;
			for (i3=1;i3<=ifp1;i3+=ip1) {
				for (i1=i3;i1<=i3+ip1-2;i1+=2) {
					for (i2=i1;i2<=ip3;i2+=ifp2) {
						k1=i2;
						k2=k1+ifp1;
						tempr=(float)wr*data[k2]-(float)wi*data[k2+1];
						tempi=(float)wr*data[k2+1]+(float)wi*data[k2];
						data[k2]=data[k1]-tempr;
						data[k2+1]=data[k1+1]-tempi;
						data[k1] += tempr;
						data[k1+1] += tempi;
					}
				}
				wr=(wtemp=wr)*wpr-wi*wpi+wr;
				wi=wi*wpr+wtemp*wpi+wi;
			}
			ifp1=ifp2;
		}
		nprev *= n;
	}
}

#undef SWAP
