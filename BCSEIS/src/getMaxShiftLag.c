//******************************************************************************//
//      getMaxShiftLag.c                                                       	//
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, September 10, 2003				//
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: September 10, 2003                			//
//******************************************************************************//
#include "getMaxShiftLag.h"

int getMaxShiftLag(int phaCode, float delta, float phaWgt) {
	float maxShiftTm = 0.0;
	if (phaWgt > 0.99) {
		maxShiftTm = 0.10;
	} else if (phaWgt > 0.74) {
		maxShiftTm = 0.15;
	} else if (phaWgt > 0.49) {
		maxShiftTm = 0.20;
	} else if (phaWgt > 0.19) {
		maxShiftTm = 0.25;
	} else {
		maxShiftTm = 0.30;
	}
	if (phaCode == 1)
		maxShiftTm *= 2.0;
	int maxShiftLag = (int)(maxShiftTm/delta);
	return maxShiftLag;
}
