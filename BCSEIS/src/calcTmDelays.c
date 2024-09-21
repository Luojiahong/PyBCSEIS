//******************************************************************************//
//      calcTmDelays.c								//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 10, 2003                        	//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 12, 2003					//
//******************************************************************************//
#include "calcTmDelays.h"

void calcTmDelays(int option, float distCutOff, int winLen1, int winLen2, int segLen1, int segLen2, float lf, float hf, const STAS& stas, SEISDATA* seisData1, SEISDATA* seisData2, int startNum) {
	int begNum1, endNum1;
	begNum1 = startNum;
	endNum1 = seisData1->getEvListSize() - 1;
	if (option == 2) {
		endNum1++;
	}
	if (startNum < 0 || startNum > (endNum1-1)) {
		cerr << ">>> Error: Incorrect <startNum> on the command line. Exit!" << endl;
		return;
	}
	for (int i = begNum1; i < endNum1; i++) {
		EVDATA *ev1 = seisData1->getEvData(i);
		int evId1 = ev1->getEvId();
		cout << ">>> Processing event #" << (i+1) << ": " << evId1 << " ... ";
		float evla1 = ev1->getEvla();
		float evlo1 = ev1->getEvlo();
		int pSize1 = ev1->getPSize();	// number of stations with P data
		int sSize1 = ev1->getSSize();	// number of stations with S data
		int *pStaIds1 = ev1->getPStaIds();
		int *sStaIds1 = ev1->getSStaIds();
		float *pDeltas1 = ev1->getPDeltas();
		float *sDeltas1 = ev1->getSDeltas();
		float *pTTms1 = ev1->getPTTms();
		float *sTTms1 = ev1->getSTTms();
		int *pMaxLags1 = ev1->getPMaxLags();
		int *sMaxLags1 = ev1->getSMaxLags();
		char outFile[FILENAMESIZE];
		sprintf(outFile, "%d.CC", evId1);
		ofstream CCOUT(outFile);
		if (!CCOUT) {
			cerr << "Can not open data file '" << outFile << "' for reading!" << endl;
		}
		CCOUT.setf(ios::fixed, ios::floatfield);
		CCOUT.precision(4);
		int begNum2, endNum2;
		if (option == 1) {
			begNum2 = i+1;
		} else {
			begNum2 = 0;
		}
		endNum2 = seisData2->getEvListSize();
		for (int j = begNum2; j < endNum2; j++) {
			EVDATA *ev2 = seisData2->getEvData(j);
			int evId2 = ev2->getEvId();
			float evla2 = ev2->getEvla();
			float evlo2 = ev2->getEvlo();
			int pSize2 = ev2->getPSize();
			int sSize2 = ev2->getSSize();
			int *pStaIds2 = ev2->getPStaIds();
			int *sStaIds2 = ev2->getSStaIds();
			float *pDeltas2 = ev2->getPDeltas();
			float *sDeltas2 = ev2->getSDeltas();
			float *pTTms2 = ev2->getPTTms();
			float *sTTms2 = ev2->getSTTms();
			int *pMaxLags2 = ev2->getPMaxLags();
			int *sMaxLags2 = ev2->getSMaxLags();

			// calculate the distance for the event pair
			float temp = cos((evla1+evla2)/2.0*PI/180);
			float dist = sqrt((evla1-evla2)*(evla1-evla2)+(evlo1-evlo2)*(evlo1-evlo2)*temp*temp)*111.195;
			if (dist > distCutOff) 
				continue;
			vector<int> commPStaIdPos[2];
			vector<int> commSStaIdPos[2];
			// get the position list of common station ids for P window
			for (int k = 0; k < pSize1; k++) {
				if (pStaIds1[k] == -1) {
					continue;
				}
				for (int m = 0; m < pSize2; m++) {
					if (pStaIds2[m] == -1) {
						continue;
					}
					if (pStaIds1[k] == pStaIds2[m]) {
						commPStaIdPos[0].push_back(k);
						commPStaIdPos[1].push_back(m);
					}
				}
			}
			// get the position list of common station ids for S window
			for (int k = 0; k < sSize1; k++) {
				if (sStaIds1[k] == -1) {
					continue;
				}
				for (int m = 0; m < sSize2; m++) {
					if (sStaIds2[m] == -1) {
						continue;
					}
					if (sStaIds1[k] == sStaIds2[m]) {
						commSStaIdPos[0].push_back(k);
						commSStaIdPos[1].push_back(m);
					}
				}
			}

			vector<TMDELAY*> ccResults[2];
			int ccResultNum[2];
			ccResultNum[0] = 0;
			ccResultNum[1] = 0;

			float maxCCCoef = 0.0;		// maximum CC coefficient
			for (int n = 0; n < 2; n++) {	// 0 for P windows; 1 for S windows
				int commSize;
				int *staIds1, *staIds2;
			       	float *deltas1, *deltas2, *tTms1, *tTms2;
				int *maxLags1, *maxLags2;
				if (n == 0) {
					commSize = commPStaIdPos[0].size();
					staIds1 = pStaIds1;
					staIds2 = pStaIds2;
					deltas1 = pDeltas1;
					deltas2 = pDeltas2;
					tTms1 = pTTms1;
					tTms2 = pTTms2;
					maxLags1 = pMaxLags1;
					maxLags2 = pMaxLags2;
				} else {
					commSize = commSStaIdPos[0].size();
					staIds1 = sStaIds1;
					staIds2 = sStaIds2;
					deltas1 = sDeltas1;
					deltas2 = sDeltas2;
					tTms1 = sTTms1;
					tTms2 = sTTms2;
					maxLags1 = sMaxLags1;
					maxLags2 = sMaxLags2;
				}
				if (commSize <= 0)
					continue;
				// perform the CC calculations for all common stations
				for (int m = 0; m < commSize; m++) {
					int k1, k2;
					if (n == 0) {
						k1 = commPStaIdPos[0][m];
						k2 = commPStaIdPos[1][m];
					} else {
						k1 = commSStaIdPos[0][m];
						k2 = commSStaIdPos[1][m];
					}
					if (k1 == -1 || k2 == -1)	// check the availability of the data arrays
						continue;
					int staId1 = staIds1[k1];
					int staId2 = staIds2[k2];
					float delta1 = deltas1[k1];
					float delta2 = deltas2[k2];
					if (staId1 != staId2) {
						cerr << ">>> Internal error: station ids do not match!\n";
					}
					if (delta1 != delta2) {
						cerr << ">>> Internal error: deltas do not match for station '" << stas.getStaNamebyId(staId1) << "'!\n";
					}
					float tTm1 = tTms1[k1];
					float tTm2 = tTms2[k2];
					float ctDiffTm = tTm1 - tTm2;
					int maxLag = maxLags1[k1] + maxLags2[k2];
					ccResults[n].push_back(new TMDELAY(k1, k2, staId1, delta1, ctDiffTm));

					// perform the time-domain CC
					float ccCoef = 0.0;
					int ccTmDelay = 0;
					int pos = ccResultNum[n];
					float *filtData1, *filtData2;
					int winLen, segLen;
					if (n == 0) {
						winLen = winLen1;
						segLen = segLen1;
						filtData1 = ev1->getPData1(k1,winLen);
						filtData2 = ev2->getPData1(k2,winLen);
					} else {
						winLen = winLen2;
						segLen = segLen2;
						filtData1 = ev1->getSData1(k1,winLen);
						filtData2 = ev2->getSData1(k2,winLen);
					}
					calcCCTmDelay(filtData1, filtData2, winLen, maxLag, 0, 0, &ccTmDelay, &ccCoef);
					ccResults[n][pos]->setCCCoef(ccCoef);
					ccResults[n][pos]->setCCTmDelay(ccTmDelay);
					if (ccCoef > maxCCCoef) {
						maxCCCoef = ccCoef;
					}
					float ccDiffTm = ctDiffTm + ccTmDelay*delta1;
					if (ccCoef >= SUBCCCOEF) {
						// perform the sub-sample estimation
						float subTmDelay = 0.0;
						float subStd = 0.0;
						calcSubTmDelay(filtData1, filtData2, winLen, ccTmDelay, segLen, delta1, lf, hf, &subTmDelay, &subStd);
						ccResults[n][pos]->setSubTmDelay(subTmDelay);
						ccResults[n][pos]->setSubStd(subStd);
						if (subStd < MAXSIGMA) {
							ccDiffTm += subTmDelay*delta1;
						}
					}
					ccResults[n][pos]->setCCDiffTm(ccDiffTm);	
					ccResultNum[n]++;
				}
			}

			if (maxCCCoef >= BSMAXCC) {	// perform the BS estimation
				for (int n = 0; n < 2; n++) {
					for (int m = 0; m < ccResultNum[n]; m++) {
						int k1 = ccResults[n][m]->getTraceNum1();
						int k2 = ccResults[n][m]->getTraceNum2();
						// check the ccCoef
						float ccCoef = ccResults[n][m]->getCCCoef();
						int ccTmDelay = ccResults[n][m]->getCCTmDelay();
						if (ccCoef >= BSFREECC) {	// BS calculations free
							ccResults[n][m]->setBSTmDelay1(ccTmDelay);
							ccResults[n][m]->setBSTmDelay2(ccTmDelay);
							continue;
						} else if (ccCoef < BSMINCCCOEF) {	// BS calculations not justified
							continue;
						}

						if (ccCoef < 0.75) {
							ccTmDelay = 0;
						}

						int maxLag = 0;
						if (n == 0) {
							maxLag = pMaxLags1[k1] + pMaxLags2[k2];
						} else if (n == 1) {
							maxLag = sMaxLags1[k1] + sMaxLags2[k2];
						}
						for (int p = 0; p < 2; p++) {
							float *data1, *data2;
							int winLen, segLen;
							int bsTmDelay = 0;
							float bsCoef = 0.0;
							if (n == 0) {
								winLen = winLen1;
								segLen = segLen1;
								if (p == 0) {	// filtered data
									data1 = ev1->getPData1(k1, (winLen+ccTmDelay));
									data2 = ev2->getPData1(k2, winLen);
								} else if (p == 1) {	// raw data
									data1 = ev1->getPData2(k1, (winLen+ccTmDelay));
									data2 = ev2->getPData2(k2, winLen);
								}
							} else if (n == 1) {
								winLen = winLen2;
								segLen = segLen2;
								if (p == 0) {
									data1 = ev1->getSData1(k1, (winLen+ccTmDelay));
									data2 = ev2->getSData1(k2, winLen);
								} else if (p == 1) {
									data1 = ev1->getSData2(k1, (winLen+ccTmDelay));
									data2 = ev2->getSData2(k2, winLen);
								}
							}
							calcBSTmDelay(data1, data2, winLen, segLen, maxLag, 0, &bsTmDelay, &bsCoef);
							bsTmDelay += ccTmDelay;
							if (p == 0) {
								ccResults[n][m]->setBSTmDelay1(bsTmDelay);
								if (abs(bsTmDelay - ccResults[n][m]->getCCTmDelay()) > LAGDIFFLIM) {
									break;
								}
							} else if (p == 1) {
								ccResults[n][m]->setBSTmDelay2(bsTmDelay);
							}
						}
					}
				}
			}

			// print the results if necessary
			if (maxCCCoef >= MINOUTCC) {
				CCOUT << "# " << evId1 << " " << evId2 << endl;
				for (int n = 0; n < 2; n++) {
					char phaName = 'P';
					if (n == 1) 
						phaName = 'S';
					for (int m = 0; m < ccResultNum[n]; m++) {
						if (ccResults[n][m]->getCCCoef() < BSMINCCCOEF)
							continue;
						int staId = ccResults[n][m]->getStaId();
						CCOUT << stas.getStaNamebyId(staId) << "\t";
						CCOUT << ccResults[n][m]->getCCDiffTm() << "\t";
						CCOUT << ccResults[n][m]->getCCCoef() << "\t" << phaName << "\t";
						CCOUT << ccResults[n][m]->getCCTmDelay() << "\t";
						CCOUT << ccResults[n][m]->getBSTmDelay1() << "\t";
						CCOUT << ccResults[n][m]->getBSTmDelay2() << "\t";
						CCOUT << ccResults[n][m]->getSubTmDelay() << "\t";
						CCOUT << ccResults[n][m]->getSubStd() << endl;
					}
				}
			}

			// release the memories
			for (int n = 0; n < 2; n++) {
				for (int m = 0; m < ccResultNum[n]; m++) {
					delete ccResults[n][m];
				}
			}
		}
		cout << " Done\n";
	}
}
