//******************************************************************************//
//      distaz.c                                                             	//
//                                                                              //
//      Author: Wen-xuan(Wayne) Du, July 16, 2001                               //
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: February 13, 2003                                        //
//******************************************************************************//
#include "distaz.h"

// This function is written in a way that both C and C++ programs can use it
void distaz(float olat, float olon, float stlat, float stlon,
	    float *zdelta, float *zazep, float *zazst, float *zdkm) 
//	olat	-- event latitude
// 	olon	-- event longitude
//	stlat	-- station latitude
//	stlon	-- station longitude
//	zdelta	-- distance in degrees
//	zazep	-- azimuth
//	zazst	-- back azimuth
//	zdkm	-- distance in km
{
	double convkm,geoco,hpi,twopi,rad,reprad;
	double el,stl,as,bs,cs,ds,a,b,c,d,cdel,delt,ddelta,sdel;
	double caze,aze,dif,azep,azst,cazs,azs;
	double eplon, sstlon;

/*
	if ( (stlat == 0.0) || (stlon == 0.0) || (olat == 0.0) || (olon == 0.0) ) {
		fprintf(stderr,	"Station lat= %f lon= %f\n",stlat,stlon);
		fprintf(stderr,	"Event   lat= %f lon= %f\n",olat,olon);
	}
*/

	convkm	= 111.194927;	// conversion from degree to km
	geoco	= 0.993277;
	hpi	= 1.5707963;	// half pi
	twopi	= 6.2831853072;	// two pi
	rad	= 0.01745329;	// pi/180
	reprad	= 57.295779512; // 180/pi

	el	= atan(geoco*tan((double)olat*rad));
	el	= hpi - el;
	stl	= atan(geoco*tan((double)stlat*rad));
	stl	= hpi - stl;
	eplon	= (double)olon*rad;
	sstlon	= (double)stlon*rad;

	// a -- latitude cosine
	// b -- latitude sine
	// c -- longitude cosine
	// d -- longitude sine

	// station locations
	as	= cos(stl);
	bs	= sin(stl);
	cs	= cos(sstlon);
	ds	= sin(sstlon);

	// event locations
	a	= cos(el);
	b	= sin(el);
	c	= cos(eplon);
	d	= sin(eplon);
	
	// calculate distance in degrees
	cdel	= a*as+b*bs*(c*cs+d*ds);
	if (cdel > 1.0) cdel = 1.0;
	if (cdel < -1.0) cdel = -1.0;
	delt	= acos(cdel);
	ddelta	= delt*reprad;

	// calculate azimuth and back azimuth
	sdel	= sin(delt);
	caze	= (as-a*cdel)/(sdel*b);
	if (caze > 1.0) caze = 1.0;
	if (caze < -1.0) caze = -1.0;

	aze	= acos(caze);
	if (bs > 0.0) cazs = (a-as*cdel)/(bs*sdel);
	if (bs == 0.0) {
		if (cazs >= 0) {
			cazs = 1.0;
		} else {
			cazs = -1.0;
		}
	}
	if (cazs > 1.0) cazs = 1.0;
	if (cazs < -1.0) cazs = -1.0;

	azs	= acos(cazs);
	dif	= ds*c - cs*d;
	if (dif < 0.0) aze = twopi - aze;
	azep	= reprad*aze;
	if (dif > 0.0) azs = twopi - azs;
	azst	= reprad*azs;

	*zazst	= (float)(azst);
	*zazep	= (float)(azep);
	*zdelta = (float)(ddelta);
	*zdkm	= (*zdelta)*convkm;

	if (*zdkm < 0.0) 
		fprintf(stderr, "Error in function distaz()! DistKm = %f\n", *zdkm);
	if (*zdelta < 0.0) 
		fprintf(stderr, "Error in function distaz()! DistDegree = %f\n", *zdelta);
	if (*zazep < 0.0 || *zazep > 360.0) 
		fprintf(stderr, "Error in function distaz()! Azimuth = %f\n", *zazep);
	if (*zazst < 0.0 || *zazst > 360.0)
		fprintf(stderr, "Error in function distaz()! BackAzimuth = %f\n", *zazst);

	return;
}

float calcdist(float olat, float olon, float stlat, float stlon) {
        float	convkm,geoco,hpi,twopi,rad,reprad;
        float	el,stl,as,bs,cs,ds,a,b,c,d,cdel,delt,ddelta;
        float	eplon, sstlon;
	float	dkm;	// distance in km

	convkm  = 111.194927;   // conversion from degree to km
        geoco   = 0.993277;
        hpi     = 1.5707963;    // half pi
        twopi   = 6.2831853072; // two pi
        rad     = 0.01745329;   // pi/180
        reprad  = 57.295779512; // 180/pi

	el      = atan(geoco*tan(olat*rad));
        el      = hpi - el;
        stl     = atan(geoco*tan(stlat*rad));
        stl     = hpi - stl;
        eplon   = olon*rad;
        sstlon  = stlon*rad;

        // station locations
        as      = cos(stl);
        bs      = sin(stl);
        cs      = cos(sstlon);
        ds      = sin(sstlon);

        // event locations
        a       = cos(el);
        b       = sin(el);
        c       = cos(eplon);
        d       = sin(eplon);

        // calculate distance in degrees
        cdel    = a*as+b*bs*(c*cs+d*ds);
        if (cdel > 1.0) cdel = 1.0;
        if (cdel < -1.0) cdel = -1.0;
        delt    = acos(cdel);
        ddelta  = delt*reprad;
	dkm = ddelta*convkm;

	return dkm;
}

