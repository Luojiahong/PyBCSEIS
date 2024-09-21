//******************************************************************************//
//      getsachdr.c                                                         	//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, December 23, 2002                          //
//                                                                              //
//      Last modified: December 23, 2002                                        //
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
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv) {
	const char *progname = argv[0];
	if (argc < 3) {
		cout << "Purpose:\t" << "Get the value of a header variable in a SAC file." << endl;
		cout << "Usage:\t\t" << progname << " <sacFile> <hdrKey1> [hdrKey2] ..." << endl;
		cout << "<sacFile>:\tSAC file name" << endl;
		cout << "<hdrKey>:\tHeader key for value checking" << endl;
		cout << "\nList of hdrKey currently supported are:\n";
		cout << "nzyear\tnzjday\tnzhour\tnzmin\tnzsec\tnzmsec\tnzdttm\tkzdate\tkztime\tnpts\n";
		cout << "delta\todelta\tb\te\to\ta\tf\tko\tka\tkf\n";
		cout << "t0\tt1\tt2\tt3\tt4\tt5\tt6\tt7\tt8\tt9\n";
		cout << "kt0\tkt1\tkt2\tkt3\tkt4\tkt5\tkt6\tkt7\tkt8\tkt9\n";
		cout << "knetwk\tkstnm\tstla\tstlo\tstel\tstdp\tsloc\tcmpaz\tcmpinc\tkcmpnm\n";
		cout << "kevnm\tevla\tevlo\tevel\tevdp\teloc\tdist\taz\tbaz\tgcarc\n";
		cout << "user0\tuser1\n\n";
		cout << "This program will output space separated header values for all requested header keys in sequence.\n";
		exit (-1);
	}

	char sacFile[FILENAMESIZE];
	char hdrKey[HDRKEYSIZE];
	strncpy(sacFile, argv[1], FILENAMESIZE);
	SacSeisgram sg(sacFile, "r");
	sg.loadHeader();
	SacHeader sacHdr = sg.getHeader();

	// header value reporting
	int keyNum = argc - 2;		// number of header variable need to be reported
	cout.setf(ios::fixed, ios::floatfield);
	cout.precision(4);
	for (int i = 1; i <= keyNum; i++) {
		strncpy(hdrKey, argv[i+1], HDRKEYSIZE);
		if (strcmp(hdrKey, "nzyear") == 0) {
			cout << sacHdr.getNzyear();
		} else if (strcmp(hdrKey, "nzjday") == 0) {
			cout << sacHdr.getNzjday();
		} else if (strcmp(hdrKey, "nzhour") == 0) {
			cout << sacHdr.getNzhour();
		} else if (strcmp(hdrKey, "nzmin") == 0) {
			cout << sacHdr.getNzmin();
		} else if (strcmp(hdrKey, "nzsec") == 0) {
			cout << sacHdr.getNzsec();
		} else if (strcmp(hdrKey, "nzmsec") == 0) {
			cout << sacHdr.getNzmsec();
		} else if (strcmp(hdrKey, "nzdttm") == 0) {
			cout << sacHdr.getNzyear() << " " << sacHdr.getNzjday() << " ";
			cout << sacHdr.getNzhour() << " " << sacHdr.getNzmin() << " ";
			cout << sacHdr.getNzsec() << " " << sacHdr.getNzmsec();
		} else if (strcmp(hdrKey, "kzdate") == 0) {
			cout << sacHdr.getNzyear() << " " << sacHdr.getNzjday();
		} else if (strcmp(hdrKey, "kztime") == 0) {
			cout << sacHdr.getNzhour() << " " << sacHdr.getNzmin() << " ";
			cout << sacHdr.getNzsec() << " " << sacHdr.getNzmsec();
		} else if (strcmp(hdrKey, "npts") == 0) {
			cout << sacHdr.getNpts();
		} else if (strcmp(hdrKey, "delta") == 0) {
			cout << sacHdr.getDelta();
		} else if (strcmp(hdrKey, "odelta") == 0) {
			cout << sacHdr.getOdelta();
		} else if (strcmp(hdrKey, "b") == 0) {
			cout << sacHdr.getB();
		} else if (strcmp(hdrKey, "e") == 0) {
			cout << sacHdr.getE();
		} else if (strcmp(hdrKey, "o") == 0) {
			cout << sacHdr.getO();
		} else if (strcmp(hdrKey, "a") == 0) {
			cout << sacHdr.getA();
		} else if (strcmp(hdrKey, "f") == 0) {
			cout << sacHdr.getF();
		} else if (strcmp(hdrKey, "t0") == 0) {
			cout << sacHdr.getT0();
		} else if (strcmp(hdrKey, "t1") == 0) {
			cout << sacHdr.getT1();
		} else if (strcmp(hdrKey, "t2") == 0) {
			cout << sacHdr.getT2();
		} else if (strcmp(hdrKey, "t3") == 0) {
			cout << sacHdr.getT3();
		} else if (strcmp(hdrKey, "t4") == 0) {
			cout << sacHdr.getT4();
		} else if (strcmp(hdrKey, "t5") == 0) {
			cout << sacHdr.getT5();
		} else if (strcmp(hdrKey, "t6") == 0) {
			cout << sacHdr.getT6();
		} else if (strcmp(hdrKey, "t7") == 0) {
			cout << sacHdr.getT7();
		} else if (strcmp(hdrKey, "t8") == 0) {
			cout << sacHdr.getT8();
		} else if (strcmp(hdrKey, "t9") == 0) {
			cout << sacHdr.getT9();
		} else if (strcmp(hdrKey, "ko") == 0) {
			cout << sacHdr.getKo();
		} else if (strcmp(hdrKey, "ka") == 0) {
			cout << sacHdr.getKa();
		} else if (strcmp(hdrKey, "kf") == 0) {
			cout << sacHdr.getKf();
		} else if (strcmp(hdrKey, "kt0") == 0) {
			cout << sacHdr.getKt0();
		} else if (strcmp(hdrKey, "kt1") == 0) {
	                cout << sacHdr.getKt1();
		} else if (strcmp(hdrKey, "kt2") == 0) {
	                cout << sacHdr.getKt2();
		} else if (strcmp(hdrKey, "kt3") == 0) {
	                cout << sacHdr.getKt3();
		} else if (strcmp(hdrKey, "kt4") == 0) {
			cout << sacHdr.getKt4();
		} else if (strcmp(hdrKey, "kt5") == 0) {
        	        cout << sacHdr.getKt5();
		} else if (strcmp(hdrKey, "kt6") == 0) {
        	        cout << sacHdr.getKt6();
		} else if (strcmp(hdrKey, "kt7") == 0) {
        	        cout << sacHdr.getKt7();
		} else if (strcmp(hdrKey, "kt8") == 0) {
	                cout << sacHdr.getKt8();
		} else if (strcmp(hdrKey, "kt9") == 0) {
        	        cout << sacHdr.getKt9();
		} else if (strcmp(hdrKey, "knetwk") == 0) {
			cout << sacHdr.getKnetwk();
		} else if (strcmp(hdrKey, "kstnm") == 0) {
			cout << sacHdr.getKstnm();
		} else if (strcmp(hdrKey, "stla") == 0) {
			cout << sacHdr.getStla();
		} else if (strcmp(hdrKey, "stlo") == 0) {
			cout << sacHdr.getStlo();
		} else if (strcmp(hdrKey, "stel") == 0) {
			cout << sacHdr.getStel();
		} else if (strcmp(hdrKey, "stdp") == 0) {
			cout << sacHdr.getStdp();
		} else if (strcmp(hdrKey, "sloc") == 0) {
			cout << sacHdr.getStla() << " " << sacHdr.getStlo() << " ";
			cout << sacHdr.getStdp() << " " << sacHdr.getStel();
		} else if (strcmp(hdrKey, "cmpaz") == 0) {
			cout << sacHdr.getCmpaz();
		} else if (strcmp(hdrKey, "cmpinc") == 0) {
			cout << sacHdr.getCmpinc();
		} else if (strcmp(hdrKey, "kcmpnm") == 0) {
			cout << sacHdr.getKcmpnm();
		} else if (strcmp(hdrKey, "kevnm") == 0) {
			cout << sacHdr.getKevnm();
		} else if (strcmp(hdrKey, "evla") == 0) {
			cout << sacHdr.getEvla();
		} else if (strcmp(hdrKey, "evlo") == 0) {
			cout << sacHdr.getEvlo();
		} else if (strcmp(hdrKey, "evel") == 0) {
			cout << sacHdr.getEvel();
		} else if (strcmp(hdrKey, "evdp") == 0) {
			cout << sacHdr.getEvdp();
		} else if (strcmp(hdrKey, "eloc") == 0) {
			cout << sacHdr.getEvla() << " " << sacHdr.getEvlo() << " ";
			cout << sacHdr.getEvdp() << " " << sacHdr.getEvel();
		} else if (strcmp(hdrKey, "dist") == 0) {
			cout << sacHdr.getDist();
		} else if (strcmp(hdrKey, "az") == 0) {
			cout << sacHdr.getAz();
		} else if (strcmp(hdrKey, "baz") == 0) {
			cout << sacHdr.getBaz();
		} else if (strcmp(hdrKey, "gcarc") == 0) {
			cout << sacHdr.getGcarc();
		} else if (strcmp(hdrKey, "user0") == 0) {
			cout << sacHdr.getUser0();
		} else if (strcmp(hdrKey, "user1") == 0) {
			cout << sacHdr.getUser1();
		} else {
			cerr << "Sorry: hdrKey '" << hdrKey << "' is currently not supported." << endl;
			exit (-1);
		}

		if (i < keyNum) {	// add in the space if needed
			cout << " ";
		}
	}
	cout << endl;
	cout.unsetf(ios::floatfield);

	return 0;
}
