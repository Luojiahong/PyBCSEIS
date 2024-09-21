//******************************************************************************//
//      sacHeader.h                                                             //
//                                                                              //
//      Author: Wen-xuan (Wayne) Du, November 15, 2002                          //
//              dxw@geology.wisc.edu						//
//                                                                              //
//      Last modified: February 25, 2003					//
//******************************************************************************//
#ifndef INCLUDED_SACHEADER_H
#define INCLUDED_SACHEADER_H

#include "constants.h"
#include "distaz.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;
//******************************************************************************//
//      CLASS DECLARATION: SacHeader                                            //
//******************************************************************************//
class SacHeader {

private:
	// standard sac header components
	float	d_delta,     d_depmin,    d_depmax,    d_scale,     d_odelta;    
	float	d_b,         d_e,         d_o,         d_a,         d_internal1; 
	float	d_t0,        d_t1,        d_t2,        d_t3,        d_t4;        
	float	d_t5,        d_t6,        d_t7,        d_t8,        d_t9;        
	float	d_f,         d_resp0,     d_resp1,     d_resp2,     d_resp3;     
	float	d_resp4,     d_resp5,     d_resp6,     d_resp7,     d_resp8;     
	float	d_resp9,     d_stla,      d_stlo,      d_stel,      d_stdp;      
	float	d_evla,      d_evlo,      d_evel,      d_evdp,      d_unused1;   
	float	d_user0,     d_user1,     d_user2,     d_user3,     d_user4;     
	float	d_user5,     d_user6,     d_user7,     d_user8,     d_user9;     
	float	d_dist,      d_az,        d_baz,       d_gcarc,     d_internal2; 
	float	d_internal3, d_depmen,    d_cmpaz,     d_cmpinc,    d_unused2;   
	float	d_unused3,   d_unused4,   d_unused5,   d_unused6,   d_unused7;   
	float	d_unused8,   d_unused9,   d_unused10,  d_unused11,  d_unused12;  
	int	d_nzyear,    d_nzjday,    d_nzhour,    d_nzmin,     d_nzsec;     
	int	d_nzmsec,    d_internal4, d_internal5, d_internal6, d_npts;      
	int	d_internal7, d_internal8, d_unused13,  d_unused14,  d_unused15;  
	int	d_iftype,    d_idep,      d_iztype,    d_unused16,  d_iinst;     
	int	d_istreg,    d_ievreg,    d_ievtyp,    d_iqual,     d_isynth;    
	int	d_unused17,  d_unused18,  d_unused19,  d_unused20,  d_unused21;  
	int	d_unused22,  d_unused23,  d_unused24,  d_unused25,  d_unused26;  
	int	d_leven,     d_lpspol,    d_lovrok,    d_lcalda,    d_unused27;  
	char	d_kstnm[8],  d_kevnm[16];           
	char	d_khole[8],  d_ko[8],     d_ka[8];               
	char	d_kt0[8],    d_kt1[8],    d_kt2[8];              
	char	d_kt3[8],    d_kt4[8],    d_kt5[8];              
	char	d_kt6[8],    d_kt7[8],    d_kt8[8];              
	char	d_kt9[8],    d_kf[8],     d_kuser0[8];           
	char	d_kuser1[8], d_kuser2[8], d_kcmpnm[8];           
	char	d_knetwk[8], d_kdatrd[8], d_kinst[8];

public:
	SacHeader();
	~SacHeader();

	int	getNzyear() const { return d_nzyear; }
	int	getNzjday() const { return d_nzjday; }
	int	getNzhour() const { return d_nzhour; }
	int	getNzmin() const { return d_nzmin; }
	int	getNzsec() const { return d_nzsec; }
	int	getNzmsec() const { return d_nzmsec; }
	int	getNpts() const { return d_npts; }
	float	getDelta() const { return d_delta; }
	float	getOdelta() const { return d_odelta; }
	float	getScale() const { return d_scale; }
	float	getDepmin() const { return d_depmin; }
	float	getDepmax() const { return d_depmax; }
	float	getDepmen() const { return d_depmen; }
	float	getB() const { return d_b; }
	float	getE() const { return d_e; }
	float 	getO() const { return d_o; }
	const char* getKo() const { return d_ko; }
	float	getA() const { return d_a; }
	const char* getKa() const { return d_ka; }
	float	getF() const { return d_f; }
	const char* getKf() const { return d_kf; }
	float	getT0() const { return d_t0; }
	const char* getKt0() const { return d_kt0; }
	float	getT1() const { return d_t1; }
	const char* getKt1() const { return d_kt1; }
	float	getT2() const { return d_t2; }
	const char* getKt2() const { return d_kt2; }
	float	getT3() const { return d_t3; }
	const char* getKt3() const { return d_kt3; }
	float	getT4() const { return d_t4; }
	const char* getKt4() const { return d_kt4; }
	float	getT5() const { return d_t5; }
	const char* getKt5() const { return d_kt5; }
	float	getT6() const { return d_t6; }
	const char* getKt6() const { return d_kt6; }
	float	getT7() const { return d_t7; }
	const char* getKt7() const { return d_kt7; }
	float	getT8() const { return d_t8; }
	const char* getKt8() const { return d_kt8; }
	float	getT9() const { return d_t9; }
	const char* getKt9() const { return d_kt9; }

	const char* getKnetwk() { return d_knetwk; }
	const char* getKstnm() { return d_kstnm; }
	int	getIstreg() const { return d_istreg; }
	float	getSlat() const { return d_stla; }
	float	getSlon() const { return d_stlo; }
	float	getElev() const { return d_stel; }
	float	getStla() const { return d_stla; }
	float	getStlo() const { return d_stlo; }
	float	getStel() const { return d_stel; }
	float	getStdp() const { return d_stdp; }
	float	getCmpaz() const { return d_cmpaz; }
	float	getCmpinc() const { return d_cmpinc; }
	const char* getKcmpnm() const { return d_kcmpnm; }
	const char* getKevnm() const { return d_kevnm; }
	int	getIevreg() const { return d_ievreg; }
	float	getLat() const { return d_evla; }
	float	getLon() const { return d_evlo; }
	float	getDep() const { return d_evdp; }
	float	getEvla() const { return d_evla; }
	float	getEvlo() const { return d_evlo; }
	float	getEvdp() const { return d_evdp; }
	float	getEvel() const { return d_evel; }
	float	getDist() const { return d_dist; }
	float	getAz() const { return d_az; }
	float	getBaz() const { return d_baz; }
	float	getGcarc() const { return d_gcarc; }
	float	getDistDegree();
	float	getAzimuth();
	float	getBackAzimuth();
	float	getDistKm();
	float	getUser0() { return d_user0; }
	float	getUser1() { return d_user1; }

	// modifier
	void	setNzyear(int nzyear) { d_nzyear = nzyear; }
	void	setNzjday(int nzjday) { d_nzjday = nzjday; }
	void	setNzhour(int nzhour) { d_nzhour = nzhour; }
	void	setNzmin(int nzmin) { d_nzmin = nzmin; }
	void	setNzsec(int nzsec) { d_nzsec = nzsec; }
	void	setNzmsec(int nzmsec) { d_nzmsec = nzmsec; }
	void	setO(float o) { d_o = o; }
	void	setKo(const char *ko) { strncpy(d_ko, ko, 8); }
	void	setA(float a) { d_a = a; }
	void	setKa(const char *ka) { strncpy(d_ka, ka, 8); }
	void	setT0(float t0) { d_t0 = t0; }
	void	setKt0(const char *kt0) { strncpy(d_kt0, kt0, 8); }
	void	setB(float b) { d_b = b; }
	void	setE(float e) { d_e = e; }

	void	setStla(float stla) { d_stla = stla; }
	void	setStlo(float stlo) { d_stlo = stlo; }
	void	setStel(float stel) { d_stel = stel; }
	void	setStdp(float stdp) { d_stdp = stdp; }

	void	setKevnm(const char *kevnm) { strncpy(d_kevnm,kevnm,16); }
	void	setEvla(float evla) { d_evla = evla; }
	void	setEvlo(float evlo) { d_evlo = evlo; }
	void	setEvel(float evel) { d_evel = evel; }
	void	setEvdp(float evdp) { d_evdp = evdp; }
	void	setDist(float dist) { d_dist = dist; }
	void	setAz(float az) { d_az = az; }
	void	setBaz(float baz) { d_baz = baz; }
	void	setGcarc(float gcarc) { d_gcarc = gcarc; }
	void	setUser0(float user0) { d_user0 = user0; }
	void	setUser1(float user1) { d_user1 = user1; }
	void    setUser2(float user2) { d_user2 = user2; }
	void    setUser3(float user3) { d_user3 = user3; }
	void    setUser4(float user4) { d_user4 = user4; }
	void    setUser5(float user5) { d_user5 = user5; }
	void    setUser6(float user6) { d_user6 = user6; }
	void    setUser7(float user7) { d_user7 = user7; }
	void    setUser8(float user8) { d_user8 = user8; }
	void    setUser9(float user9) { d_user9 = user9; }


	void	setDelta(float delta) { d_delta = delta; }
	void	setDepmin(float depmin) { d_depmin = depmin; }
	void    setDepmax(float depmax) { d_depmax = depmax; }
	void	setScale(float scale) { d_scale = scale; }
	void	setOdelta(float odelta) { d_odelta = odelta; }
	void	setInternal1(float internal1) { d_internal1 = internal1; }
	void    setT1(float t1) { d_t1 = t1; }
	void    setT2(float t2) { d_t2 = t2; }
	void    setT3(float t3) { d_t3 = t3; }
	void    setT4(float t4) { d_t4 = t4; }
	void    setT5(float t5) { d_t5 = t5; }
	void    setT6(float t6) { d_t6 = t6; }
	void    setT7(float t7) { d_t7 = t7; }
	void    setT8(float t8) { d_t8 = t8; }
	void    setT9(float t9) { d_t9 = t9; }
	void	setF(float f) { d_f = f; }
	void	setResp0(float resp0) { d_resp0 = resp0; }
	void	setResp1(float resp1) { d_resp1 = resp1; }
	void    setResp2(float resp2) { d_resp2 = resp2; }
	void    setResp3(float resp3) { d_resp3 = resp3; }
	void    setResp4(float resp4) { d_resp4 = resp4; }
	void    setResp5(float resp5) { d_resp5 = resp5; }
	void    setResp6(float resp6) { d_resp6 = resp6; }
	void    setResp7(float resp7) { d_resp7 = resp7; }
	void    setResp8(float resp8) { d_resp8 = resp8; }
	void    setResp9(float resp9) { d_resp9 = resp9; }
	void	setUnused1(float unused1) { d_unused1 = unused1; }
	void    setInternal2(float internal2) { d_internal2 = internal2; }
	void    setInternal3(float internal3) { d_internal3 = internal3; }
	void	setDepmen(float depmen) { d_depmen = depmen; }
	void	setCmpaz(float cmpaz) { d_cmpaz = cmpaz; }
	void	setCmpinc(float cmpinc) { d_cmpinc = cmpinc; }
	void    setUnused2(float unused2) { d_unused2 = unused2; }
	void    setUnused3(float unused3) { d_unused3 = unused3; }
	void    setUnused4(float unused4) { d_unused4 = unused4; }
	void    setUnused5(float unused5) { d_unused5 = unused5; }
	void    setUnused6(float unused6) { d_unused6 = unused6; }
	void    setUnused7(float unused7) { d_unused7 = unused7; }
	void    setUnused8(float unused8) { d_unused8 = unused8; }
	void    setUnused9(float unused9) { d_unused9 = unused9; }
	void    setUnused10(float unused10) { d_unused10 = unused10; }
	void    setUnused11(float unused11) { d_unused11 = unused11; }
	void    setUnused12(float unused12) { d_unused12 = unused12; }

	void	setInternal4(int internal4) { d_internal4 = internal4; }
	void    setInternal5(int internal5) { d_internal5 = internal5; }
	void    setInternal6(int internal6) { d_internal6 = internal6; }
	void	setNpts(int npts) { d_npts = npts; }
	void    setInternal7(int internal7) { d_internal7 = internal7; }
	void    setInternal8(int internal8) { d_internal8 = internal8; }
	void	setUnused13(int unused13) { d_unused13 = unused13; }
	void    setUnused14(int unused14) { d_unused14 = unused14; }
	void    setUnused15(int unused15) { d_unused15 = unused15; }
	void	setIftype(int iftype) { d_iftype = iftype; }
	void    setIdep(int idep) { d_idep = idep; }
	void    setIztype(int iztype) { d_iztype = iztype; }
	void    setUnused16(int unused16) { d_unused16 = unused16; }
	void    setIinst(int iinst) { d_iinst = iinst; }
	void    setIstreg(int istreg) { d_istreg = istreg; }
	void    setIevreg(int ievreg) { d_ievreg = ievreg; }
	void    setIevtyp(int ievtyp) { d_ievtyp = ievtyp; }
	void    setIqual(int iqual) { d_iqual = iqual; }
	void    setIsynth(int isynth) { d_isynth = isynth; }
	void    setUnused17(int unused17) { d_unused17 = unused17; }
	void    setUnused18(int unused18) { d_unused18 = unused18; }
	void    setUnused19(int unused19) { d_unused19 = unused19; }
	void    setUnused20(int unused20) { d_unused20 = unused20; }
	void    setUnused21(int unused21) { d_unused21 = unused21; }
	void    setUnused22(int unused22) { d_unused22 = unused22; }
	void    setUnused23(int unused23) { d_unused23 = unused23; }
	void    setUnused24(int unused24) { d_unused24 = unused24; }
	void    setUnused25(int unused25) { d_unused25 = unused25; }
	void    setUnused26(int unused26) { d_unused26 = unused26; }
	void	setLeven(int leven) { d_leven = leven; }
	void    setLpspol(int lpspol) { d_lpspol = lpspol; }
	void    setLovrok(int lovrok) { d_lovrok = lovrok; }
	void    setLcalda(int lcalda) { d_lcalda = lcalda; }
	void    setUnused27(int unused27) { d_unused27 = unused27; }

	// Input and Output
	void	ioSacHeader(FILE*, const char*);

	friend	ostream& operator<<(ostream&, const SacHeader&);

private:
	void	calcEvStaInfo();
	void    fixString(char*, int);
	int	assureOriginTime() const;	// make sure the event origin time is in the header
	int	assureRecordTime() const;	// make sure the recording time is in the header
	int	assureStationLoc() const;	// make sure the station location is in the header
	int	assureEventLoc() const;		// make sure the event location is in the header

};

#endif	// INCLUDED_SACHEADER_H
