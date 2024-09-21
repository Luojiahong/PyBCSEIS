//******************************************************************************//
//	complex.c								//
//										//
//	Author: Wen-xuan (Wayne) Du, July 16, 2001				//
//	        dxw@geology.wisc.edu						//
//										//
//	Last modified: July 16, 2001						//
//******************************************************************************//
#include "complex.h"

//******************************************************************************//
//	CLASS DEFINITION: Complex						//
//******************************************************************************//
Complex::Complex() {
	d_real	= 0;
	d_imag	= 0;
}

Complex::Complex(float real, float imag) {
	d_real	= real;
	d_imag	= imag;
}

Complex::Complex(const Complex& number) {
	d_real	= number.getReal();
	d_imag	= number.getImag();
}

//Complex& Complex::operator=(const Complex& number) {
//	d_real	= number.getReal();
//	d_imag	= number.getImag();
//	return *this;
//}

Complex::~Complex() {
}

// ACCESSORS
float Complex::getReal() const {
	return d_real;
}

float Complex::getImag() const {
	return d_imag;
}

// MANIPULATORS
void Complex::setReal(float real) {
	d_real = real;
}

void Complex::setImag(float imag) {
	d_imag = imag;
}

// Other functions
float Complex::getPower() const {
        float power;
        power = d_real*d_real + d_imag*d_imag;
        return power;
}

float Complex::getAmp() const {
	return sqrt(this->getPower());
}

float Complex::getPhase() const {
	float phase;
	phase = atan2(d_imag, d_real);
	return phase;
}

const Complex Complex::getConj() const {
	return Complex(d_real, -1.0*d_imag);
}

// Operators
const Complex Complex::operator+(const Complex& number) {
	return Complex((d_real + number.getReal()), (d_imag + number.getImag()));
}

const Complex Complex::operator-(const Complex& number) {
        return Complex((d_real - number.getReal()), (d_imag - number.getImag()));
}
	
const Complex Complex::operator*(const Complex& number) {
	float temp_r = d_real*number.getReal() - d_imag*number.getImag();
	float temp_i = d_real*number.getImag() + d_imag*number.getReal();
	return Complex(temp_r, temp_i);
}

const Complex Complex::operator/(const Complex& number) {
	float temp_r = (d_real*number.getReal() + d_imag*number.getImag()) / number.getPower();
	float temp_i = (d_imag*number.getReal() - d_real*number.getImag()) / number.getPower();
	return Complex(temp_r, temp_i);
}

const Complex Complex::operator*(float f) {
	return Complex(d_real*f, d_imag*f);
}

const Complex Complex::operator/(int i) {
	return Complex(d_real/i, d_imag/i);
}

ostream& operator<<(ostream& o, const Complex& complex) {
	o.setf(ios::scientific, ios::floatfield);
	o << setprecision(6) << complex.getReal() << "\t" << complex.getImag();
	o.unsetf(ios::floatfield);
	return o;
} 
