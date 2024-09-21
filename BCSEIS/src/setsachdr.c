//******************************************************************************//
//      setsachdr.c                                                         	//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, December 24, 2002                          //
//                                                                              //
//      Last modified: December 24, 2002                                        //
//******************************************************************************//
//      Copyright (c) by Wen-xuan Du -- dxw@geology.wisc.edu                    //
//                                                                              //
//      The author reserves all rights of the package.  You are not             //
//      allowed to printout, copy, use, distribute, or modify any part of       //
//      the files in this package without permission.  The author is not        //
//      responsible for any damage of any computer hardware and software by     //
//      any appropriate or inappropriate use of the package.                    //
//******************************************************************************//
#include "sacSeisgram.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv) {
	const char *progname = argv[0];
	int hdrPair = (argc-2)/2;
	if ((argc < 4) || ((argc - 2 - hdrPair*2) != 0)) {
		cout << "Purpose:\t" << "Set the values of a set of header variables in a SAC file." << endl;
		cout << "Usage:\t\t" << progname << " <sacFile> <hdrKey1> <hdrVal1> [hdrKey2] [hdrVal2] ..." << endl;
		cout << "<sacFile>:\tSAC file name" << endl;
		cout << "<hdrKey>:\tHeader key for value setting" << endl;
		cout << "<hdrVal>:\tHeader value" << endl;
		cout << "\nList of hdrKey currently supported are:\n";
		cout << "nzyear\tnzjday\tnzhour\tnzmin\tnzsec\tnzmsec\n";
		cout << "o\tko\ta\tka\tt0\tkt0\n";
		cout << "stla\tstlo\tstel\tstdp\n";
		cout << "kevnm\tevla\tevlo\tevel\tevdp\n";
		cout << "user0\tuser1\tdelta\n";
		cout << "\nNote that when 'evla' and 'evlo' are both set in command line, this program will also set\n";
		cout << "station-event info parameters (dist,az,baz,gcarc) to the header if station info is in the header.\n";
		exit (-1);
	}

	char	sacFile[FILENAMESIZE];
	char	hdrKey[HDRKEYSIZE];
	strncpy(sacFile, argv[1], FILENAMESIZE);
	SacSeisgram sg(sacFile, "r");
	sg.loadFromFile();

	// header value setting
	int keyFlag = 1;	// indicating whether user 'hdrKey' is currently supported
	int distFlag = 0;	// calculate ev-sta info if (evla and evlo are both set through command line)
	for (int i = 1; i <= hdrPair; i++) {
		int n1 = 2*i;
		int n2 = n1+1;
		strncpy(hdrKey, argv[n1], HDRKEYSIZE);
		if (strcmp(hdrKey, "nzyear") == 0) {
			sg.getHeader().setNzyear(atol(argv[n2]));
		} else if (strcmp(hdrKey, "nzjday") == 0) {
			sg.getHeader().setNzjday(atol(argv[n2]));
		} else if (strcmp(hdrKey, "nzhour") == 0) {
			sg.getHeader().setNzhour(atol(argv[n2]));
		} else if (strcmp(hdrKey, "nzmin") == 0) {
			sg.getHeader().setNzmin(atol(argv[n2]));
		} else if (strcmp(hdrKey, "nzsec") == 0) {
			sg.getHeader().setNzsec(atol(argv[n2]));
		} else if (strcmp(hdrKey, "nzmsec") == 0) {
			sg.getHeader().setNzmsec(atol(argv[n2]));
		} else if (strcmp(hdrKey, "o") == 0) {
			sg.getHeader().setO(atof(argv[n2]));
		} else if (strcmp(hdrKey, "ko") == 0) {
			sg.getHeader().setKo(argv[n2]);
		} else if (strcmp(hdrKey, "a") == 0) {
			sg.getHeader().setA(atof(argv[n2]));
		} else if (strcmp(hdrKey, "ka") == 0) {
			sg.getHeader().setKa(argv[n2]);
		} else if (strcmp(hdrKey, "t0") == 0) {
			sg.getHeader().setT0(atof(argv[n2]));
		} else if (strcmp(hdrKey, "kt0") == 0) {
			sg.getHeader().setKt0(argv[n2]);
		} else if (strcmp(hdrKey, "stla") == 0) {
			sg.getHeader().setStla(atof(argv[n2]));
		} else if (strcmp(hdrKey, "stlo") == 0) {
			sg.getHeader().setStlo(atof(argv[n2]));
		} else if (strcmp(hdrKey, "stel") == 0) {
			sg.getHeader().setStel(atof(argv[n2]));
		} else if (strcmp(hdrKey, "stdp") == 0) {
			sg.getHeader().setStdp(atof(argv[n2]));
		} else if (strcmp(hdrKey, "kevnm") == 0) {
			sg.getHeader().setKevnm(argv[n2]);
		} else if (strcmp(hdrKey, "evla") == 0) {
			sg.getHeader().setEvla(atof(argv[n2]));
			distFlag++;
		} else if (strcmp(hdrKey, "evlo") == 0) {
			sg.getHeader().setEvlo(atof(argv[n2]));
			distFlag++;
		} else if (strcmp(hdrKey, "evel") == 0) {
			sg.getHeader().setEvel(atof(argv[n2]));
		} else if (strcmp(hdrKey, "evdp") == 0) {
			sg.getHeader().setEvdp(atof(argv[n2]));
		} else if (strcmp(hdrKey, "user0") == 0) {
			sg.getHeader().setUser0(atof(argv[n2]));
		} else if (strcmp(hdrKey, "user1") == 0) {
			sg.getHeader().setUser1(atof(argv[n2]));
		} else if (strcmp(hdrKey, "delta") == 0) {
			sg.getHeader().setDelta(atof(argv[n2]));
		} else {
			keyFlag	= 0;
			cout << "Sorry: hdrKey '" << hdrKey << "' is currently not supported." << endl;
			break;
		}
	}

	if (distFlag == 2) {	// set ev-sta info
		float dist, az, baz, gcarc;
		float evla = sg.getHeader().getEvla();
		float evlo = sg.getHeader().getEvlo();
		float stla = sg.getHeader().getStla();
		float stlo = sg.getHeader().getStlo();
		if (stla != -12345.0 && stlo != -12345.0) {
			distaz(evla,evlo,stla,stlo,&gcarc,&az,&baz,&dist);
			sg.getHeader().setDist(dist);
			sg.getHeader().setAz(az);
			sg.getHeader().setBaz(baz);
			sg.getHeader().setGcarc(gcarc);
		}
	}

	// write over to the SAC file
	if (keyFlag == 1) sg.saveToFile(sacFile);

	return 0;
}
