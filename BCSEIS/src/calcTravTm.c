//******************************************************************************//
//      calcTravTm.c                                                           	//
//                                                                              //
//      Author: Wen-xuan(Wayne) Du, April 15, 2003				//
//	        dxw@geology.wisc.edu						//
//	        								//
//      Last modified: September 18, 2003					//
//******************************************************************************//

#include "calcTravTm.h"
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <math.h>

float calcTravTm(const char *velCode, int phaCode, float depth, float dist) {

	int vLayer = MAXLAYER;
	float *topDepth = new float[MAXLAYER];
	float *pModel = new float[MAXLAYER];
	float *sModel = new float[MAXLAYER];
	
	if (strcmp(velCode, "nz1") == 0) {
		vLayer = 7;

		topDepth[0] = 0.00;
		pModel[0] = 4.40;
		sModel[0] = 2.54;

		topDepth[1] = 0.40;
		pModel[1] = 5.63;
		sModel[1] = 3.15;

		topDepth[2] = 5.0;
		pModel[2] = 5.77;
		sModel[2] = 3.49;

		topDepth[3] = 15.0;
		pModel[3] = 6.39;
		sModel[3] = 3.50;

		topDepth[4] = 25.0;
		pModel[4] = 6.79;
		sModel[4] = 3.92;

		topDepth[5] = 35.0;
		pModel[5] = 8.07;
		sModel[5] = 4.80;

		topDepth[6] = 45.0;
		pModel[6] = 8.77;
		sModel[6] = 4.86;

	} else if (strcmp(velCode, "nz2") == 0) {
		vLayer = 8;

		topDepth[0] = 0.00;
		pModel[0] = 5.00;
		sModel[0] = 2.88;

		topDepth[1] = 3.0;
		pModel[1] = 5.50;
		sModel[1] = 3.17;

		topDepth[2] = 9.0;
		pModel[2] = 6.00;
		sModel[2] = 3.46;

		topDepth[3] = 22.0;
		pModel[3] = 6.14;
		sModel[3] = 3.54;

		topDepth[4] = 30.0;
		pModel[4] = 7.15;
		sModel[4] = 4.12;

		topDepth[5] = 43.0;
		pModel[5] = 8.06;
		sModel[5] = 4.65;

		topDepth[6] = 57.0;
		pModel[6] = 8.43;
		sModel[6] = 4.86;

		topDepth[7] = 100.0;
		pModel[7] = 8.70;
		sModel[7] = 5.02;
		
	} else {
		cerr << "Incorrect 'veloCode' as input to function calcTravTm()!\n";
	}

	// fix the source depth if it sits on the layer boundary
	for (int i = 0; i < vLayer; i++) {
		if (fabs(depth - topDepth[i]) < 1e-4) {
			depth = topDepth[i] - 1e-3;
		}
	}

	float travTm = 0.0;
	float sinAngle = 0.0;

	if (phaCode == 0) {
		ttime_(&dist, &depth, &vLayer, pModel, topDepth, &travTm, &sinAngle);
	} else {
		ttime_(&dist, &depth, &vLayer, sModel, topDepth, &travTm, &sinAngle);
	}

	delete []topDepth;
	delete []pModel;
	delete []sModel;

	return travTm;
}
