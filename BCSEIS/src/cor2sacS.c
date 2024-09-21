//******************************************************************************//
//      cor2sacS.c                                                         	//
//										//
//      Author: Wen-xuan(Wayne) Du, January 31, 2003	                        //
//              dxw@geology.wisc.edu						//
//              								//
//      Notice: This program is very similar to 'cor2sac' except that 		//
//      	it can deal with SAC files without catalog S picks		//
//                                                                              //
//      Last modified: September 18, 2003					//
//******************************************************************************//
#include "calcTravTm.h"
#include "readSacData.h"
#include "getMaxShiftLag.h"
#include "calcCCTmDelay.h"
#include "calcSubTmDelay.h"
#include "calcBSTmDelay.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char **argv) {
	const char *progname = argv[0];
	if (argc != 15) {
		cout << "Usage:\t\t " << progname << " <option> <sacFile1> <sacFile2> <phaCode> <preNum> <posNum> <segLen> <lf> <hf> <lagNum> <pFlag> <evdp1> <evdp2> <velCode>" << endl;
		cout << "<option>:\t 0 & 1 - time-domain & FFT cross-correlation;" << endl;
		cout << "         \t 2 - sub-sample time delay calculation;" << endl;
		cout << "         \t 3 - bispectrum time delay calculation;" << endl;
		cout << "<sacFile1>:\t filename of SAC file 1;" << endl;
		cout << "<sacFile2>:\t filename of SAC file 2;" << endl;
		cout << "<phaCode>:\t 0 - P wave; 1 - S wave;" << endl;
		cout << "<preNum>:\t number of points before the phase for data windowing;" << endl;
		cout << "<posNum>:\t number of points after the phase for data windowing;" << endl;
		cerr << "<segLen>:\t number of sample points in each segment when dividing the data window;" << endl;
		cout << "<lf>:\t\t lowest frequency for cross-spectrum phase fitting;" << endl;
		cout << "<hf>:\t\t highest frequency for cross-spectrum phase fitting;" << endl;
		cout << "<lagNum>:\t number of sample points to realign the two traces;" << endl;
		cout << "<pFlag>:\t flag for plotting CC or bispectrum results;" << endl;
		cout << "<evdp1>:\t depth of event 1;" << endl;
		cout << "<evdp2>:\t depth of event 2;" << endl;
		cout << "<velCode>:\t code of the velocity model to calculate the theoretical S-wave travel time;" << endl;
		cout << endl;
		cout << "Version:\t 1.00" << endl;
		cout << "Author:\t\t Wen-xuan(Wayne) Du -- dxw@geology.wisc.edu" << endl;
		exit (-1);
	}

	// deal with the command line inputs
	char sacFile1[FILENAMESIZE], sacFile2[FILENAMESIZE];
	int option = atoi(argv[1]);
	if (option != 0 && option != 1 && option != 2 && option != 3) {
		cerr << ">>> Incorrect 'option' in command line input: should be 0, 1, 2 or 3!\n";
		exit (-1);
	}
	strncpy(sacFile1, argv[2], FILENAMESIZE);
	strncpy(sacFile2, argv[3], FILENAMESIZE);
	int phaCode = atoi(argv[4]);
	if (phaCode != 0 && phaCode != 1) {
		cerr << ">>> Incorrect 'phaCode' in command line input: should be either 0(P) or 1(S)!\n";
		exit (-1);
	}
	int preNum = atoi(argv[5]);
	int posNum = atoi(argv[6]);
	int segLen = atoi(argv[7]);
	float lf = atof(argv[8]);
	float hf = atof(argv[9]);
	int lagNum = atoi(argv[10]);
	int pFlag = atoi(argv[11]);
	float evdp1 = atof(argv[12]);
	float evdp2 = atof(argv[13]);
	char velCode[VELCODESIZE];
	strncpy(velCode, argv[14], VELCODESIZE);

	// deal with the SAC files
	int winLen = preNum + posNum + 1;
	float *winData1 = new float[winLen];
	float *winData2 = new float[winLen];
	float delta1, delta2, tTm1, tTm2, phaWgt1, phaWgt2;
	int flag1 = readSacData(sacFile1, phaCode, preNum, posNum, &delta1, &tTm1, &phaWgt1, winData1, evdp1, velCode);
	int flag2 = readSacData(sacFile2, phaCode, (preNum+lagNum), (posNum-lagNum), &delta2, &tTm2, &phaWgt2, winData2, evdp2, velCode);
	if (flag1 == -1 || flag2 == -1) {
		cerr << ">>> Errors occur when loading the data from the SAC files!" << endl;
		exit (-1);
	}
	if (delta1 != delta2) {
		cerr << "delta1 = " << delta1 << "\tdelta2 = " << delta2 << endl;
		cerr << "Error: different delta for the two SAC files!" << endl;
		exit (-1);
	}
	float delta = delta1;
	int maxLag1 = getMaxShiftLag(phaCode, delta1, phaWgt1);
	int maxLag2 = getMaxShiftLag(phaCode, delta2, phaWgt2);
	int maxNum = maxLag1 + maxLag2;

	// Main section for time delay calculations
        if (option == 0 || option == 1) {	// cross-correlation time-delay calculation
		float ccCoef;
		int ccTmDelay;
		calcCCTmDelay(winData1, winData2, winLen, maxNum, option, pFlag, &ccTmDelay, &ccCoef);
		printf("%-6.4f %d\n", ccCoef, ccTmDelay);
	} else {	
		if (pFlag == 1) {
			ofstream DAT1("ev1.dat");
			ofstream DAT2("ev2.dat");
			DAT1.setf(ios::scientific, ios::floatfield);
			DAT1.precision(4);
			DAT2.setf(ios::scientific, ios::floatfield);
			DAT2.precision(4);
			for (int i = 0; i < winLen; i++) {
				DAT1 << i << "\t" << winData1[i] << endl;
				DAT2 << i << "\t" << winData2[i] << endl;
			}
		}
		if (option == 2) {	// sub-sample time delay calculation
			float subTmDelay = 0.0;
			float sigma = 0.0;
			calcSubTmDelay(winData1, winData2, winLen, 0, segLen, delta, lf, hf, &subTmDelay, &sigma);
			printf("%-6.3f %6.4f\n", subTmDelay, sigma);
		} else if (option == 3) {	// bispectrum time delay calculation
			float bsCoef = 0.0;
			int bsTmDelay = 0;
			calcBSTmDelay(winData1, winData2, winLen, segLen, maxNum, pFlag, &bsTmDelay, &bsCoef);
			printf("%-6.4f %d\n", bsCoef, bsTmDelay);
		}
	}

	delete	[]winData1;
	delete	[]winData2;

	return 0;
}
