//******************************************************************************//
//      seisData.c								//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 10, 2003                        	//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 10, 2003					//
//******************************************************************************//
#include "seisData.h"

//******************************************************************************//
//      CLASS DEFINITION: SEISDATA						//
//******************************************************************************//

SEISDATA::SEISDATA(char *baseDir, const vector<int>& evIdList, int preNum1, int posNum1, int preNum2, int posNum2, const STAS& stas, const char *velCode) {

	d_evSize = 0;
	char ccInfoFile[FILENAMESIZE];
	strncpy(ccInfoFile, baseDir, FILENAMESIZE);
	strcat(ccInfoFile, "/Events/CCEventInfo/ccEvInfo.dat");
	FILE *CCINFO;
	if ( (CCINFO = fopen(ccInfoFile, "r")) == NULL) {
		cerr << ">>> Error: Can not open file " << ccInfoFile << " for reading!" << endl;
		exit (-1);
	}
	char buff[256];
	char pound;
	int evId, pSize, sSize;		// event id; # of stations with P pick; # of stations with S pick
	float evla, evlo, evdp;
	char evDir[FILENAMESIZE];
	char shortDir[FILENAMESIZE];
	int evIdListSize = evIdList.size();
	int *evIdFlags = new int[evIdListSize];
	for (int i = 0; i < evIdListSize; i++) {
		evIdFlags[i] = -1;
	}
	cout << ">>> Loading the waveform data for time delay calculations ... \n";
	cout << ">>> evIdListSize:" << evIdListSize << endl;
	while (fgets(buff, sizeof(buff), CCINFO) != NULL) {
		if (buff[0] == '#') {
			sscanf(buff, "%c %d %f %f %f %s %d %d\n", &pound, &evId, &evla, &evlo, &evdp, shortDir, &pSize, &sSize);
			// check whether the ccEvInfo belongs to an event to be processed
			//cout << ">>> evIdListSize:3" << evIdListSize << endl;
			int flag = -1;
			for (int i = 0; i < evIdListSize; i++) {
				if (evId == evIdList[i]) {
					evIdFlags[i] = 0;
					flag = 0;
					break;
				}
			}
			if (flag == -1) {	// ccEvInfo belongs to an event not to be processed
				int size = pSize + sSize;
				if (size > 0) {
					for (int n = 0; n < size; n++)
						fgets(buff, sizeof(buff), CCINFO);
				}
				continue;
			}
			//cout << ">>> evIdListSize:2" << evIdListSize << endl;
			strncpy(evDir, baseDir, FILENAMESIZE);
			strcat(evDir, "/Events/Data/");
			strcat(evDir, shortDir);
			int calcSTravTmFlag = 0;
			if (strcmp(velCode, "NAN") != 0) {
				calcSTravTmFlag = 1;
				d_evList.push_back(new EVDATA(evId, evla, evlo, evdp, evDir, pSize, pSize));
			} else {
				d_evList.push_back(new EVDATA(evId, evla, evlo, evdp, evDir, pSize, sSize));
			}
			//cout << ">>> evIdListSize:4" << evIdListSize << endl;
			if (pSize != 0) {
				for (int i = 0; i < pSize; i++) {
					//cout << ">>> buff" << buff <<endl;
					fgets(buff, sizeof(buff), CCINFO);
					char staName[8];
					//cout << ">>> buff 2 " << buff <<endl;
					sscanf(buff, "%s\n", staName);
					//cout << ">>> staName:" << staName <<endl;
					//cout << ">>> staName:" << i <<endl;
					//cout << ">>> staName:" << preNum1 <<endl;
					//cout << ">>> staName:" << posNum1 <<endl;
					//cout << ">>> staName:" << stas <<endl;
					//cout << ">>> staName:" << d_evSize <<endl;
					d_evList[d_evSize]->addStaData(0, i, staName, preNum1, posNum1, stas, velCode);
					//cout << ">>> staNameWz:" << d_evSize <<endl;
					if (calcSTravTmFlag == 1) {
						cout << "break!" << endl;
						d_evList[d_evSize]->addStaData(1, i, staName, preNum2, posNum2, stas, velCode);
					}
					cout << ">>> staName 2:" << staName <<endl;
				}
			}
			cout << ">>> evIdListSize:1" << evIdListSize << endl;
			if (sSize != 0) {
				for (int i = 0; i < sSize; i++) {
					fgets(buff, sizeof(buff), CCINFO);
					char staName[8];
					sscanf(buff, "%s\n", staName);
					if (calcSTravTmFlag == 0) {
						d_evList[d_evSize]->addStaData(1, i, staName, preNum2, posNum2, stas, velCode);
					}
				}
			}
			d_evSize++;
		}
	//cout << ">>> evIdListSize:" << evIdListSize << endl;
	}
	fclose(CCINFO);

	int goodEvSize = d_evSize;
	if (goodEvSize == evIdListSize) {
		cout << ">>> Waveform data for " << goodEvSize << " events are loaded.\n";
	} else {
		cout << ">>> Waveform data for " << goodEvSize << "/" << evIdListSize << " events are loaded.\n";
		int badEvSize = evIdListSize - goodEvSize;
		cout << ">>> " << badEvSize << " requested events have no ccEvInfo in file '" << ccInfoFile << "':\n";
		for (int i = 0; i < evIdListSize; i++) {
			if (evIdFlags[i] == -1)
				cout << evIdList[i] << endl;
		}
	}
	delete []evIdFlags;
}

SEISDATA::~SEISDATA() {
	for (int i = 0; i < d_evSize; i++)
		delete d_evList[i];
}
