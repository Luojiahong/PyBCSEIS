//******************************************************************************//
//	sacSeisgram.c								//
//										//
//	Author: Wen-xuan (Wayne) Du, July 16, 2001				//
//										//
//	Last modified: November 19, 2002					//
//******************************************************************************//
//	Copyright (c) by Wen-xuan Du -- dxw@geology.wisc.edu			//
//										//
//      The author reserves all rights of the package.  You are not  		//
//      allowed to printout, copy, use, distribute, or modify any part of     	//
//      the files in this package without permission.  The author is not      	//
//      responsible for any damage of any computer hardware and software by   	//
//      any appropriate or inappropriate use of the package.                  	//
//******************************************************************************//
#include "sacSeisgram.h"

//******************************************************************************//
//	CLASS DEFINITION: SacSeisgram						//
//******************************************************************************//
SacSeisgram::SacSeisgram(const char *filename, const char *mode) : d_sacHeader() { // constructor
	strncpy(d_mode, mode, MODESIZE);
	if ( (d_fp = fopen(filename, mode)) == NULL) {
		cerr << "Error: Can not open file " << filename << " for reading!" << endl;
		exit (-1);
	}
	d_size = 0;
}

SacSeisgram::~SacSeisgram() {	// destructor
	if (d_size != 0) delete []d_data_p;
}

void SacSeisgram::loadHeader() {
	if (strcmp(d_mode, "r") != 0) {
		cerr << "Error: trying to read from a SAC file opened not with 'r' mode!" << endl;
		exit (-1);
	}
	d_sacHeader.ioSacHeader(d_fp, d_mode);
	fclose(d_fp);
}

void SacSeisgram::loadData(int npts, float data[]) {
	if (d_sacHeader.getNpts() != npts) {
		cerr << "Error in SacSeisgram::loadData" << endl;
		exit (-1);
	}
	if (d_size == 0) {
		d_size = npts;
		d_data_p = new float[d_size];
	}
	for (int i = 0; i < d_size; i++)
		*(d_data_p+i) = data[i];
}

void SacSeisgram::loadFromFile() {
	if (strcmp(d_mode, "r") != 0) {
		cerr << "Error: trying to read from a SAC file opened not with 'r' mode!" << endl;
		exit (-1);
	}
	d_sacHeader.ioSacHeader(d_fp, d_mode);
	ioSacData();
	fclose(d_fp);
}

void SacSeisgram::saveToFile(const char *filename) {
	strncpy(d_mode, "w", MODESIZE);
	if ( (d_fp = fopen(filename, d_mode)) == NULL) {
                cerr << "Error: Can not open file " << filename << " for writing!" << endl;
                exit (-1);
        }
	d_sacHeader.ioSacHeader(d_fp, d_mode);
	ioSacData();
	fclose(d_fp);
}

void SacSeisgram::showHeader(ostream& o) const {
	o << d_sacHeader;
}
	
void SacSeisgram::showData(ostream& o) const {
        if (d_size == 0) {
		cerr << "Error in SacSeisgram::showData(): size of data array is zero!" << endl;
		exit (-1);
	}
	o << "data" << endl;
	o.setf(ios::scientific, ios::floatfield);
	o.precision(6);
	for (int i = 0; i < d_size; i++)
		o << *(d_data_p+i) << endl;
	o.unsetf(ios::floatfield);
}

// private functions
void SacSeisgram::ioSacData() {
	if (d_size == 0) {
		d_size		= d_sacHeader.getNpts();
		d_data_p	= new float[d_size];
	}
	if (strcmp(d_mode, "r") == 0) {	// read mode
		fread(d_data_p, d_size*sizeof(float), 1, d_fp);
	} else if (strcmp(d_mode, "w") == 0) { // write mode
		fwrite(d_data_p, d_size*sizeof(float), 1, d_fp);
	}
}
