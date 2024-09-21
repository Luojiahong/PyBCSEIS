//******************************************************************************//
//	sacSeisgram.h								//
//										//
//	Author: Wen-xuan(Wayne) Du, July 16, 2001				//
//	        dxw@geology.wisc.edu						//
//										//
//	Last modified: February 26, 2003					//
//******************************************************************************//
#ifndef	INCLUDED_SACSEISGRAM_H
#define INCLUDED_SACSEISGRAM_H

#include "sacHeader.h"

//******************************************************************************//
//	CLASS DECLARATION: SacSeisgram						//
//******************************************************************************//
class SacSeisgram {

private:
	SacHeader	d_sacHeader;
	FILE		*d_fp;
	char		d_mode[MODESIZE];	// file mode 'r', 'w' et al.
	float		*d_data_p;
	int		d_size;

public:
	SacSeisgram(const char*, const char*);
	~SacSeisgram();

	// Accessors
	SacHeader& getHeader() { return d_sacHeader; }
	float* getData() { return d_data_p; }

	void loadHeader();
	void loadData(int, float []);
	void loadFromFile();
	void saveToFile(const char*);
	void showHeader(ostream&) const;
	void showData(ostream&) const;
	
private:
	void ioSacData();

};

#endif 	// INCLUDED_SACSEISGRAM_H
