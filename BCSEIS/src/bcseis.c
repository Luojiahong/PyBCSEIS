//******************************************************************************//
//      bcseis.c								//
//      									//
//      Author: Wen-xuan (Wayne) Du, September 10, 2003				//
//              dxw@geology.wisc.edu                                            //
//      									//
//      Version: 1.02								//
//      Major Changes: Allow user to specifiy different segment lengths 	//
//      	       for P and S data windows					//
//                                                                              //
//      Last modified: September 19, 2003					//
//******************************************************************************//
#include "stas.h"
#include "seisData.h"
#include "calcTmDelays.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
	const char *progname = argv[0];
	const char* evIdFile0 = "bcseis.id";
	const char* evIdFile1 = "bcseis.id1";
	const char* evIdFile2 = "bcseis.id2";

	// load in the command line arguments
	if (argc != 14) {
		cerr << "Usage:\t\t " << progname << " <option> <baseDir> <distCutOff> <preNum1> <posNum1>" << endl;
		cerr <<"\t\t\t<preNum2> <posNum2> <segLen1> <segLen2> <lf> <hf> <velCode> <startNum>" << endl;
		cerr << "Purpose:\t This program calculates waveform-based differential times (or time delays)" << endl;
	        cerr << "        \t using both cross-correlation and bispectrum methods." << endl;
		cerr << "<option>:\t 0, 1, 2\n";
		cerr << "         \t 0    - require input file '" << evIdFile0 << "';" << endl;
		cerr << "         \t      - perform calculations for all events in a single run;" << endl;
		cerr << "         \t 1, 2 - require input files '" << evIdFile1 << "' and '" << evIdFile2 << "';" << endl;
		cerr << "         \t      - mainly used to take advantage of the dual CPU power of denali;" << endl;
		cerr << "         \t    1 - perform calculations for 2 groups of events consecutively;" << endl;
		cerr << "         \t    2 - perform calculations between 2 groups of events;" << endl;
		cerr << "<baseDir>:\t directory path for the dataset to be processed;" << endl;
		cerr << "<distCutOff>:\t cutoff event-event distance beyond which time delay calculations are not performed;" << endl;
		cerr << "<preNum1>:\t number of sample points before the P arrival (for data window selection);" << endl;
		cerr << "<posNum1>:\t number of sample points after the P arrival;" << endl;
		cerr << "<preNum2>:\t number of sample points before the S arrival (for data window selection);" << endl;
		cerr << "<posNum2>:\t number of sample points after the S arrival;" << endl;
		cerr << "<segLen1>:\t number of sample points in each segment when dividing the P data window;" << endl;
		cerr << "<segLen2>:\t number of sample points in each segment when dividing the S data window;" << endl;
		cerr << "          \t <segLen1> and <segLen2> must be power of 2;" << endl;
		cerr << "<lf>:\t\t lowest frequency to perform phase spectrum fitting (for sub-sample estimation);" << endl;
		cerr << "<hf>:\t\t highest frequency to perform phase spectrum fitting (for sub-sample estimation);" << endl;
		cerr << "<velCode>:\t code of velocity model to compute theoretical S-wave travel time, 'NAN' otherwise;" << endl;
		cerr << "<startNum>:\t starting event number to continue a previously unfinished run;" << endl;
		cerr << "           \t default value is 1; meaningless if option is set to 1;" << endl << endl;
		cerr << "Version:\t 1.02" << endl;
		cerr << "Author:\t\t Wen-xuan(Wayne) Du -- dxw@geology.wisc.edu" << endl;
		exit (-1);
	}
	char baseDir[FILENAMESIZE], velCode[VELCODESIZE];
	int option, preNum1, posNum1, preNum2, posNum2, segLen1, segLen2, startNum;
	float distCutOff, lf, hf;

	option = atoi(argv[1]);
	strncpy(baseDir, argv[2], FILENAMESIZE);
	distCutOff = atof(argv[3]);
	preNum1 = atoi(argv[4]);
	posNum1 = atoi(argv[5]);
	preNum2 = atoi(argv[6]);
	posNum2 = atoi(argv[7]);
	segLen1 = atoi(argv[8]);
	segLen2 = atoi(argv[9]);
	lf = atof(argv[10]);
	hf = atof(argv[11]);
	strncpy(velCode, argv[12], VELCODESIZE);
	startNum = atoi(argv[13]);

	int winLen1 = preNum1 + posNum1 + 1;
	int winLen2 = preNum2 + posNum2 + 1;

	// load in the station information
	cout << ">>> loading the list of stations." << endl;
	char stationFile[FILENAMESIZE];
	strncpy(stationFile, baseDir, FILENAMESIZE);
	strcat(stationFile, "/Stations/station.dat");
	STAS stas(stationFile);
	//cout << ">>> Number of Station:" << stas << endl;

	// load in the lists of the event ids to be processed
	if (option == 0) {
		cout << ">>> Loading the list of event ids to be processed from '" << evIdFile0 << "'." << endl;
	} else {
		cout << ">>> Loading the two lists of event ids to be processed from '" << evIdFile1 << "' and '" << evIdFile2 << "'." << endl;
	}
	vector<int> evIdList1, evIdList2;
	for (int i = 0; i < 2; i++) {
		const char* evIdFile;
		if (option == 0) {
			evIdFile = evIdFile0;
		} else {
			if (i == 0) {
				evIdFile = evIdFile1;
			} else {
				evIdFile = evIdFile2;
			}
		}
		FILE* fp;
		if ( (fp = fopen(evIdFile, "r")) == NULL) {
			cerr << ">>> Error: Can not open file " << evIdFile << " for reading!" << endl;
			exit (-1);
		}
		char buff[128];
		while (fgets(buff, sizeof(buff), fp) != NULL) {
			int id;
			sscanf(buff, "%d\n", &id);
			if (i == 0) {
				evIdList1.push_back(id);
				//cout << ">>> Number of eventID:" << id << endl;
			} else {
				evIdList2.push_back(id);
			}
		}
		fclose(fp);
		if (option == 0) 
			break;
	}
	SEISDATA* seisData1 = new SEISDATA(baseDir, evIdList1, preNum1, posNum1, preNum2, posNum2, stas, velCode);
	int evListSize1 = seisData1->getEvListSize();
	if (option == 0) {
		cout << ">>> Performing time delay calculations for a group of " << evListSize1 << " events ..." << endl;
		if (startNum != 1) {
			cout << ">>> Calculations start from event #" << startNum << endl;
		}
		calcTmDelays(1, distCutOff, winLen1, winLen2, segLen1, segLen2, lf, hf, stas, seisData1, seisData1, (startNum-1));
		delete seisData1;
	} else {
		if (option == 1) {
			cout << ">>> Performing time delay calculations for the 1st group of " << evListSize1 << " events ..." << endl;
			calcTmDelays(1, distCutOff, winLen1, winLen2, segLen1, segLen2, lf, hf, stas, seisData1, seisData1, 0);
			delete seisData1;
			SEISDATA* seisData2 = new SEISDATA(baseDir, evIdList2, preNum1, posNum1, preNum2, posNum2, stas, velCode);
			int evListSize2 = seisData2->getEvListSize();
			cout << ">>> Performing time delay calculations for the 2nd group of " << evListSize2 << " events ..." << endl;
			calcTmDelays(1, distCutOff, winLen1, winLen2, segLen1, segLen2, lf, hf, stas, seisData2, seisData2, 0);
			delete seisData2;
		} else if (option == 2) {
			SEISDATA* seisData2 = new SEISDATA(baseDir, evIdList2, preNum1, posNum1, preNum2, posNum2, stas, velCode);
			int evListSize2 = seisData2->getEvListSize();
			cout << ">>> Performing time delay calculations between " << evListSize1 << " and " << evListSize2 << " events." << endl;
			if (startNum != 1) {
				cout << ">>> Calculations start from event #" << startNum << endl;
			}
			calcTmDelays(2, distCutOff, winLen1, winLen2, segLen1, segLen2, lf, hf, stas, seisData1, seisData2, (startNum-1));
			delete seisData1;
			delete seisData2;
		}
	}

	return 0;
}
