//******************************************************************************//
//	complex.h								//
//										//
//	Author: Wen-xuan(Wayne) Du, July 16, 2001				//
//	        dxw@geology.wisc.edu						//
//										//
//	Last modified: February 24, 2003					//
//******************************************************************************//
#ifndef	INCLUDED_COMPLEX_H
#define	INCLUDED_COMPLEX_H

#include <math.h>
#include <iomanip>
#include <iostream>
using namespace std;
//******************************************************************************//
//	CLASS DECLARATION: Complex						//
//******************************************************************************//
class Complex {		// complex number

	float	d_real;	// real part
	float	d_imag;	// imag part

public:
	Complex();
	Complex(float, float);
	Complex(const Complex&);
//	Complex& operator=(const Complex&);
	~Complex();	

	// ACCESSORS
	float getReal() const;	// return real part
	float getImag() const;	// return imag part

	// MANIPULATORS
	void setReal(float);	// set real part
	void setImag(float);	// set imag part

	// Other functions
	float getPower() const;	// return power
	float getAmp() const;	// return amplitude
	float getPhase() const;	// return the phase
	const Complex getConj() const;	// return the complex conjugate

	// Operators
        const Complex operator+(const Complex&);
	const Complex operator-(const Complex&);
	const Complex operator*(const Complex&);
	const Complex operator/(const Complex&);
	const Complex operator*(float);
	const Complex operator/(int);

};

ostream& operator<<(ostream&, const Complex&);

#endif	// INCLUDED_COMPLEX_H
