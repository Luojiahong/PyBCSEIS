//******************************************************************************//
//	sacHeader.c								//
//										//
//	Author: Wen-xuan (Wayne) Du, November 15, 2002				//
//	        dxw@geology.wisc.edu						//
//										//
//	Last modified: November 15, 2002					//
//******************************************************************************//
#include "sacHeader.h"

//******************************************************************************//
//	CLASS DEFINITION: SacHeader						//
//******************************************************************************//
SacHeader::SacHeader() {
	d_delta		= -12345.0;
	d_depmin	= -12345.0;
	d_depmin	= -12345.0;
	d_scale		= -12345.0;
	d_odelta	= -12345.0;
	d_b		= -12345.0;
	d_e		= -12345.0;
	d_o		= -12345.0;
	d_a		= -12345.0;
	d_internal1	= -12345.0;
	d_t0		= -12345.0;
	d_t1		= -12345.0;
	d_t2		= -12345.0;
	d_t3		= -12345.0;
	d_t4		= -12345.0;
	d_t5		= -12345.0;
	d_t6		= -12345.0;
	d_t7		= -12345.0;
	d_t8		= -12345.0;
	d_t9		= -12345.0;
	d_f		= -12345.0;
	d_resp0		= -12345.0;
	d_resp1		= -12345.0;
	d_resp2		= -12345.0;
	d_resp3		= -12345.0;
	d_resp4		= -12345.0;
	d_resp5		= -12345.0;
	d_resp6		= -12345.0;
	d_resp7		= -12345.0;
	d_resp8		= -12345.0;
	d_resp9		= -12345.0;
	d_stla		= -12345.0;
	d_stlo		= -12345.0;
	d_stel		= -12345.0;
	d_stdp		= -12345.0;
	d_evla		= -12345.0;
	d_evlo		= -12345.0;
	d_evel		= -12345.0;
	d_evdp		= -12345.0;
	d_unused1	= -12345.0;
	d_user0		= -12345.0;
	d_user1		= -12345.0;
	d_user2		= -12345.0;
	d_user3		= -12345.0;
	d_user4		= -12345.0;
	d_user5		= -12345.0;
	d_user6		= -12345.0;
	d_user7		= -12345.0;
	d_user8		= -12345.0;
	d_user9		= -12345.0;
	d_dist		= -12345.0;
	d_az		= -12345.0;
	d_baz		= -12345.0;
	d_gcarc		= -12345.0;
	d_internal2	= -12345.0;
	d_internal3	= -12345.0;
	d_depmen	= -12345.0;
	d_cmpaz		= -12345.0;
	d_cmpinc	= -12345.0;
	d_unused2	= -12345.0;
	d_unused3	= -12345.0;
	d_unused4	= -12345.0;
	d_unused5	= -12345.0;
	d_unused6	= -12345.0;
	d_unused7	= -12345.0;
	d_unused8	= -12345.0;
	d_unused9	= -12345.0;
	d_unused10	= -12345.0;
	d_unused11	= -12345.0;
	d_unused12	= -12345.0;
	
	d_nzyear	= -12345;
	d_nzjday	= -12345;
	d_nzhour	= -12345;
	d_nzmin		= -12345;
	d_nzsec		= -12345;
	d_nzmsec	= -12345;
	d_internal4	= -12345;
	d_internal5	= -12345;
	d_internal6	= -12345;
	d_npts		= -12345;
	d_internal7	= -12345;
	d_internal8	= -12345;
	d_unused13	= -12345;
	d_unused14	= -12345;
	d_unused15	= -12345;
	d_iftype	= -12345;
	d_idep		= -12345;
	d_iztype	= -12345;
	d_unused16	= -12345;
	d_iinst		= -12345;
	d_istreg	= -12345;
	d_ievreg	= -12345;
	d_ievtyp	= -12345;
	d_iqual		= -12345;
	d_isynth	= -12345;
	d_unused17	= -12345;
	d_unused18	= -12345;
	d_unused19	= -12345;
	d_unused20	= -12345;
	d_unused21	= -12345;
	d_unused22	= -12345;
	d_unused23	= -12345;
	d_unused24	= -12345;
	d_unused25	= -12345;
	d_unused26	= -12345;
	d_leven		= -12345;
	d_lpspol	= -12345;
	d_lovrok	= -12345;
	d_lcalda	= -12345;
	d_unused27	= -12345;

	strncpy(d_kstnm, "-12345", 8);
	strncpy(d_kevnm, "-12345", 16);
	strncpy(d_khole, "-12345", 8);
	strncpy(d_ko, "-12345", 8);
	strncpy(d_ka, "-12345", 8);
	strncpy(d_kt0, "-12345", 8);
	strncpy(d_kt1, "-12345", 8);
	strncpy(d_kt2, "-12345", 8);
	strncpy(d_kt3, "-12345", 8);
	strncpy(d_kt4, "-12345", 8);
	strncpy(d_kt5, "-12345", 8);
	strncpy(d_kt6, "-12345", 8);
	strncpy(d_kt7, "-12345", 8);
	strncpy(d_kt8, "-12345", 8);
	strncpy(d_kt9, "-12345", 8);
	strncpy(d_kf, "-12345", 8);
	strncpy(d_kuser0, "-12345", 8);
	strncpy(d_kuser1, "-12345", 8);
	strncpy(d_kuser2, "-12345", 8);
	strncpy(d_kcmpnm, "-12345", 8);
	strncpy(d_knetwk, "-12345", 8);
	strncpy(d_kdatrd, "-12345", 8);
	strncpy(d_kinst, "-12345", 8);

}

SacHeader::~SacHeader() {
}

float SacHeader::getDistDegree() {
	if (d_gcarc == -12345.0)
		calcEvStaInfo();
	return d_gcarc;
}

float SacHeader::getAzimuth() {
	if (d_az == -12345.0)
		calcEvStaInfo();
	return d_az;
}

float SacHeader::getBackAzimuth() {
	if (d_baz == -12345.0)
		calcEvStaInfo();
	return d_baz;
}

float SacHeader::getDistKm() {
	if (d_dist == -12345.0)
		calcEvStaInfo();
	return d_dist;
}

void SacHeader::ioSacHeader(FILE *fp, const char *mode) {
	int	fs = sizeof(float);
	int	ls = sizeof(int);
	int	ss = 8*sizeof(char);
	if (strcmp(mode, "r") == 0) {
		fread(&d_delta, fs, 1, fp);
		fread(&d_depmin, fs, 1, fp);
		fread(&d_depmax, fs, 1, fp);
		fread(&d_scale, fs, 1, fp);
		fread(&d_odelta, fs, 1, fp);
		fread(&d_b, fs, 1, fp);
		fread(&d_e, fs, 1, fp);
		fread(&d_o, fs, 1, fp);
		fread(&d_a, fs, 1, fp);
		fread(&d_internal1, fs, 1, fp);
		fread(&d_t0, fs, 1, fp);
		fread(&d_t1, fs, 1, fp);
		fread(&d_t2, fs, 1, fp);
		fread(&d_t3, fs, 1, fp);
		fread(&d_t4, fs, 1, fp);
		fread(&d_t5, fs, 1, fp);
		fread(&d_t6, fs, 1, fp);
		fread(&d_t7, fs, 1, fp);
		fread(&d_t8, fs, 1, fp);
		fread(&d_t9, fs, 1, fp);
		fread(&d_f, fs, 1, fp);
		fread(&d_resp0, fs, 1, fp);
		fread(&d_resp1, fs, 1, fp);
		fread(&d_resp2, fs, 1, fp);
		fread(&d_resp3, fs, 1, fp);
		fread(&d_resp4, fs, 1, fp);
		fread(&d_resp5, fs, 1, fp);
		fread(&d_resp6, fs, 1, fp);
		fread(&d_resp7, fs, 1, fp);
		fread(&d_resp8, fs, 1, fp);
		fread(&d_resp9, fs, 1, fp);
		fread(&d_stla, fs, 1, fp);
		fread(&d_stlo, fs, 1, fp);
		fread(&d_stel, fs, 1, fp);
		fread(&d_stdp, fs, 1, fp);
		fread(&d_evla, fs, 1, fp);
		fread(&d_evlo, fs, 1, fp);
		fread(&d_evel, fs, 1, fp);
		fread(&d_evdp, fs, 1, fp);
		fread(&d_unused1, fs, 1, fp);
		fread(&d_user0, fs, 1, fp);
		fread(&d_user1, fs, 1, fp);
		fread(&d_user2, fs, 1, fp);
		fread(&d_user3, fs, 1, fp);
		fread(&d_user4, fs, 1, fp);
		fread(&d_user5, fs, 1, fp);
		fread(&d_user6, fs, 1, fp);
		fread(&d_user7, fs, 1, fp);
		fread(&d_user8, fs, 1, fp);
		fread(&d_user9, fs, 1, fp);
		fread(&d_dist, fs, 1, fp);
		fread(&d_az, fs, 1, fp);
		fread(&d_baz, fs, 1, fp);
		fread(&d_gcarc, fs, 1, fp);
		fread(&d_internal2, fs, 1, fp);
		fread(&d_internal3, fs, 1, fp);
		fread(&d_depmen, fs, 1, fp);
		fread(&d_cmpaz, fs, 1, fp);
		fread(&d_cmpinc, fs, 1, fp);
		fread(&d_unused2, fs, 1, fp);
		fread(&d_unused3, fs, 1, fp);
		fread(&d_unused4, fs, 1, fp);
		fread(&d_unused5, fs, 1, fp);
		fread(&d_unused6, fs, 1, fp);
		fread(&d_unused7, fs, 1, fp);
		fread(&d_unused8, fs, 1, fp);
		fread(&d_unused9, fs, 1, fp);
		fread(&d_unused10, fs, 1, fp);
		fread(&d_unused11, fs, 1, fp);
		fread(&d_unused12, fs, 1, fp);

		fread(&d_nzyear, ls, 1, fp);
		fread(&d_nzjday, ls, 1, fp);
		fread(&d_nzhour, ls, 1, fp);
		fread(&d_nzmin, ls, 1, fp);
		fread(&d_nzsec, ls, 1, fp);
		fread(&d_nzmsec, ls, 1, fp);
		fread(&d_internal4, ls, 1, fp);
		fread(&d_internal5, ls, 1, fp);
		fread(&d_internal6, ls, 1, fp);
		fread(&d_npts, ls, 1, fp);
		fread(&d_internal7, ls, 1, fp);
		fread(&d_internal8, ls, 1, fp);
		fread(&d_unused13, ls, 1, fp);
		fread(&d_unused14, ls, 1, fp);
		fread(&d_unused15, ls, 1, fp);
		fread(&d_iftype, ls, 1, fp);
		fread(&d_idep, ls, 1, fp);
		fread(&d_iztype, ls, 1, fp);
		fread(&d_unused16, ls, 1, fp);
		fread(&d_iinst, ls, 1, fp);
		fread(&d_istreg, ls, 1, fp);
		fread(&d_ievreg, ls, 1, fp);
		fread(&d_ievtyp, ls, 1, fp);
		fread(&d_iqual, ls, 1, fp);
		fread(&d_isynth, ls, 1, fp);
		fread(&d_unused17, ls, 1, fp);
		fread(&d_unused18, ls, 1, fp);
		fread(&d_unused19, ls, 1, fp);
		fread(&d_unused20, ls, 1, fp);
		fread(&d_unused21, ls, 1, fp);
		fread(&d_unused22, ls, 1, fp);
		fread(&d_unused23, ls, 1, fp);
		fread(&d_unused24, ls, 1, fp);
		fread(&d_unused25, ls, 1, fp);
		fread(&d_unused26, ls, 1, fp);
		fread(&d_leven, ls, 1, fp);
		fread(&d_lpspol, ls, 1, fp);
		fread(&d_lovrok, ls, 1, fp);
		fread(&d_lcalda, ls, 1, fp);
		fread(&d_unused27, ls, 1, fp);
	
		fread(d_kstnm, ss, 1, fp);
		fread(d_kevnm, 2*ss, 1, fp);
		fread(d_khole, ss, 1, fp);
		fread(d_ko, ss, 1, fp);
		fread(d_ka, ss, 1, fp);
		fread(d_kt0, ss, 1, fp);
		fread(d_kt1, ss, 1, fp);
		fread(d_kt2, ss, 1, fp);
		fread(d_kt3, ss, 1, fp);
		fread(d_kt4, ss, 1, fp);
		fread(d_kt5, ss, 1, fp);
		fread(d_kt6, ss, 1, fp);
		fread(d_kt7, ss, 1, fp);
		fread(d_kt8, ss, 1, fp);
		fread(d_kt9, ss, 1, fp);
		fread(d_kf, ss, 1, fp);
		fread(d_kuser0, ss, 1, fp);
		fread(d_kuser1, ss, 1, fp);
		fread(d_kuser2, ss, 1, fp);
		fread(d_kcmpnm, ss, 1, fp);
		fread(d_knetwk, ss, 1, fp);
		fread(d_kdatrd, ss, 1, fp);
		fread(d_kinst, ss, 1, fp);

		// fix the strings because fread() could not handle it properly
		fixString(d_kstnm, 8);
		fixString(d_kevnm, 16);
		fixString(d_khole, 8);
		fixString(d_ko, 8);
		fixString(d_ka, 8);
		fixString(d_kt0, 8);
		fixString(d_kt1, 8);
		fixString(d_kt2, 8);
		fixString(d_kt3, 8);
		fixString(d_kt4, 8);
		fixString(d_kt5, 8);
		fixString(d_kt6, 8);
		fixString(d_kt7, 8);
		fixString(d_kt8, 8);
		fixString(d_kt9, 8);
		fixString(d_kf, 8);
		fixString(d_kuser0, 8);
		fixString(d_kuser1, 8);
		fixString(d_kuser2, 8);
		fixString(d_kcmpnm, 8);
		fixString(d_knetwk, 8);
		fixString(d_kdatrd, 8);
		fixString(d_kinst, 8);
	} else if (strcmp(mode, "w") == 0) {
                fwrite(&d_delta, fs, 1, fp);
                fwrite(&d_depmin, fs, 1, fp);
                fwrite(&d_depmax, fs, 1, fp);
                fwrite(&d_scale, fs, 1, fp);
                fwrite(&d_odelta, fs, 1, fp);
                fwrite(&d_b, fs, 1, fp);
                fwrite(&d_e, fs, 1, fp);
                fwrite(&d_o, fs, 1, fp);
                fwrite(&d_a, fs, 1, fp);
                fwrite(&d_internal1, fs, 1, fp);
                fwrite(&d_t0, fs, 1, fp);
                fwrite(&d_t1, fs, 1, fp);
                fwrite(&d_t2, fs, 1, fp);
                fwrite(&d_t3, fs, 1, fp);
                fwrite(&d_t4, fs, 1, fp);
                fwrite(&d_t5, fs, 1, fp);
                fwrite(&d_t6, fs, 1, fp);
                fwrite(&d_t7, fs, 1, fp);
                fwrite(&d_t8, fs, 1, fp);
                fwrite(&d_t9, fs, 1, fp);
                fwrite(&d_f, fs, 1, fp);
                fwrite(&d_resp0, fs, 1, fp);
                fwrite(&d_resp1, fs, 1, fp);
                fwrite(&d_resp2, fs, 1, fp);
                fwrite(&d_resp3, fs, 1, fp);
                fwrite(&d_resp4, fs, 1, fp);
                fwrite(&d_resp5, fs, 1, fp);
                fwrite(&d_resp6, fs, 1, fp);
                fwrite(&d_resp7, fs, 1, fp);
                fwrite(&d_resp8, fs, 1, fp);
                fwrite(&d_resp9, fs, 1, fp);
                fwrite(&d_stla, fs, 1, fp);
                fwrite(&d_stlo, fs, 1, fp);
                fwrite(&d_stel, fs, 1, fp);
                fwrite(&d_stdp, fs, 1, fp);
                fwrite(&d_evla, fs, 1, fp);
                fwrite(&d_evlo, fs, 1, fp);
                fwrite(&d_evel, fs, 1, fp);
                fwrite(&d_evdp, fs, 1, fp);
                fwrite(&d_unused1, fs, 1, fp);
                fwrite(&d_user0, fs, 1, fp);
                fwrite(&d_user1, fs, 1, fp);
                fwrite(&d_user2, fs, 1, fp);
                fwrite(&d_user3, fs, 1, fp);
                fwrite(&d_user4, fs, 1, fp);
                fwrite(&d_user5, fs, 1, fp);
                fwrite(&d_user6, fs, 1, fp);
                fwrite(&d_user7, fs, 1, fp);
                fwrite(&d_user8, fs, 1, fp);
                fwrite(&d_user9, fs, 1, fp);
                fwrite(&d_dist, fs, 1, fp);
                fwrite(&d_az, fs, 1, fp);
                fwrite(&d_baz, fs, 1, fp);
                fwrite(&d_gcarc, fs, 1, fp);
                fwrite(&d_internal2, fs, 1, fp);
                fwrite(&d_internal3, fs, 1, fp);
                fwrite(&d_depmen, fs, 1, fp);
                fwrite(&d_cmpaz, fs, 1, fp);
                fwrite(&d_cmpinc, fs, 1, fp);
                fwrite(&d_unused2, fs, 1, fp);
                fwrite(&d_unused3, fs, 1, fp);
                fwrite(&d_unused4, fs, 1, fp);
                fwrite(&d_unused5, fs, 1, fp);
                fwrite(&d_unused6, fs, 1, fp);
                fwrite(&d_unused7, fs, 1, fp);
                fwrite(&d_unused8, fs, 1, fp);
                fwrite(&d_unused9, fs, 1, fp);
                fwrite(&d_unused10, fs, 1, fp);
                fwrite(&d_unused11, fs, 1, fp);
                fwrite(&d_unused12, fs, 1, fp);

                fwrite(&d_nzyear, ls, 1, fp);
                fwrite(&d_nzjday, ls, 1, fp);
                fwrite(&d_nzhour, ls, 1, fp);
                fwrite(&d_nzmin, ls, 1, fp);
                fwrite(&d_nzsec, ls, 1, fp);
                fwrite(&d_nzmsec, ls, 1, fp);
                fwrite(&d_internal4, ls, 1, fp);
                fwrite(&d_internal5, ls, 1, fp);
                fwrite(&d_internal6, ls, 1, fp);
                fwrite(&d_npts, ls, 1, fp);
                fwrite(&d_internal7, ls, 1, fp);
                fwrite(&d_internal8, ls, 1, fp);
                fwrite(&d_unused13, ls, 1, fp);
                fwrite(&d_unused14, ls, 1, fp);
                fwrite(&d_unused15, ls, 1, fp);
                fwrite(&d_iftype, ls, 1, fp);
                fwrite(&d_idep, ls, 1, fp);
                fwrite(&d_iztype, ls, 1, fp);
                fwrite(&d_unused16, ls, 1, fp);
                fwrite(&d_iinst, ls, 1, fp);
                fwrite(&d_istreg, ls, 1, fp);
                fwrite(&d_ievreg, ls, 1, fp);
                fwrite(&d_ievtyp, ls, 1, fp);
                fwrite(&d_iqual, ls, 1, fp);
                fwrite(&d_isynth, ls, 1, fp);
                fwrite(&d_unused17, ls, 1, fp);
                fwrite(&d_unused18, ls, 1, fp);
                fwrite(&d_unused19, ls, 1, fp);
                fwrite(&d_unused20, ls, 1, fp);
                fwrite(&d_unused21, ls, 1, fp);
                fwrite(&d_unused22, ls, 1, fp);
                fwrite(&d_unused23, ls, 1, fp);
                fwrite(&d_unused24, ls, 1, fp);
                fwrite(&d_unused25, ls, 1, fp);
                fwrite(&d_unused26, ls, 1, fp);
                fwrite(&d_leven, ls, 1, fp);
                fwrite(&d_lpspol, ls, 1, fp);
                fwrite(&d_lovrok, ls, 1, fp);
                fwrite(&d_lcalda, ls, 1, fp);
                fwrite(&d_unused27, ls, 1, fp);

                fwrite(d_kstnm, ss, 1, fp);
                fwrite(d_kevnm, 2*ss, 1, fp);
                fwrite(d_khole, ss, 1, fp);
                fwrite(d_ko, ss, 1, fp);
                fwrite(d_ka, ss, 1, fp);
                fwrite(d_kt0, ss, 1, fp);
                fwrite(d_kt1, ss, 1, fp);
                fwrite(d_kt2, ss, 1, fp);
                fwrite(d_kt3, ss, 1, fp);
                fwrite(d_kt4, ss, 1, fp);
                fwrite(d_kt5, ss, 1, fp);
                fwrite(d_kt6, ss, 1, fp);
                fwrite(d_kt7, ss, 1, fp);
                fwrite(d_kt8, ss, 1, fp);
                fwrite(d_kt9, ss, 1, fp);
                fwrite(d_kf, ss, 1, fp);
                fwrite(d_kuser0, ss, 1, fp);
                fwrite(d_kuser1, ss, 1, fp);
                fwrite(d_kuser2, ss, 1, fp);
                fwrite(d_kcmpnm, ss, 1, fp);
                fwrite(d_knetwk, ss, 1, fp);
                fwrite(d_kdatrd, ss, 1, fp);
                fwrite(d_kinst, ss, 1, fp);
	} else {
		cerr << "Error in SacHeader::iosacHeader(): mode is not either 'r' or 'w'. Exit!" << endl;
		exit (-1);
	}
}

ostream& operator<<(ostream& o, const SacHeader& sacHeader) {
	o.setf(ios::fixed, ios::floatfield);
	o.precision(5);
	if (sacHeader.d_delta != -12345.0) o << "delta\t\t= " << sacHeader.d_delta << endl;
	if (sacHeader.d_depmin != -12345.0) o << "depmin\t\t= " << sacHeader.d_depmin << endl;
	if (sacHeader.d_depmax != -12345.0) o << "depmax\t\t= " << sacHeader.d_depmax << endl;
	if (sacHeader.d_scale != -12345.0) o << "scale\t\t= " << sacHeader.d_scale << endl;
	if (sacHeader.d_odelta != -12345.0) o << "odelta\t\t= " << sacHeader.d_odelta << endl;
	if (sacHeader.d_b != -12345.0) o << "b\t\t= " << sacHeader.d_b << endl;
	if (sacHeader.d_e != -12345.0) o << "e\t\t= " << sacHeader.d_e << endl;
	if (sacHeader.d_o != -12345.0) o << "o\t\t= " << sacHeader.d_o << endl;
	if (sacHeader.d_a != -12345.0) o << "a\t\t= " << sacHeader.d_a << endl;
	if (sacHeader.d_internal1 != -12345.0) o << "internal1\t= " << sacHeader.d_internal1 << endl;
	if (sacHeader.d_t0 != -12345.0) o << "t0\t\t= " << sacHeader.d_t0 << endl;
	if (sacHeader.d_t1 != -12345.0) o << "t1\t\t= " << sacHeader.d_t1 << endl;
	if (sacHeader.d_t2 != -12345.0) o << "t2\t\t= " << sacHeader.d_t2 << endl;
	if (sacHeader.d_t3 != -12345.0) o << "t3\t\t= " << sacHeader.d_t3 << endl;
	if (sacHeader.d_t4 != -12345.0) o << "t4\t\t= " << sacHeader.d_t4 << endl;
	if (sacHeader.d_t5 != -12345.0) o << "t5\t\t= " << sacHeader.d_t5 << endl;
	if (sacHeader.d_t6 != -12345.0) o << "t6\t\t= " << sacHeader.d_t6 << endl;
	if (sacHeader.d_t7 != -12345.0) o << "t7\t\t= " << sacHeader.d_t7 << endl;
	if (sacHeader.d_t8 != -12345.0) o << "t8\t\t= " << sacHeader.d_t8 << endl;
	if (sacHeader.d_t9 != -12345.0) o << "t9\t\t= " << sacHeader.d_t9 << endl;
	if (sacHeader.d_f != -12345.0) o << "f\t\t= " << sacHeader.d_f << endl;
	if (sacHeader.d_resp0 != -12345.0) o << "resp0\t\t= " << sacHeader.d_resp0 << endl;
	if (sacHeader.d_resp1 != -12345.0) o << "resp1\t\t= " << sacHeader.d_resp1 << endl;
	if (sacHeader.d_resp2 != -12345.0) o << "resp2\t\t= " << sacHeader.d_resp2 << endl;
	if (sacHeader.d_resp3 != -12345.0) o << "resp3\t\t= " << sacHeader.d_resp3 << endl;
	if (sacHeader.d_resp4 != -12345.0) o << "resp4\t\t= " << sacHeader.d_resp4 << endl;
	if (sacHeader.d_resp5 != -12345.0) o << "resp5\t\t= " << sacHeader.d_resp5 << endl;
	if (sacHeader.d_resp6 != -12345.0) o << "resp6\t\t= " << sacHeader.d_resp6 << endl;
	if (sacHeader.d_resp7 != -12345.0) o << "resp7\t\t= " << sacHeader.d_resp7 << endl;
	if (sacHeader.d_resp8 != -12345.0) o << "resp8\t\t= " << sacHeader.d_resp8 << endl;
	if (sacHeader.d_resp9 != -12345.0) o << "resp9\t\t= " << sacHeader.d_resp9 << endl;
	if (sacHeader.d_stla != -12345.0) o << "stla\t\t= " << sacHeader.d_stla << endl;
	if (sacHeader.d_stlo != -12345.0) o << "stlo\t\t= " << sacHeader.d_stlo << endl;
	if (sacHeader.d_stel != -12345.0) o << "stel\t\t= " << sacHeader.d_stel << endl;
	if (sacHeader.d_stdp != -12345.0) o << "stdp\t\t= " << sacHeader.d_stdp << endl;
	if (sacHeader.d_evla != -12345.0) o << "evla\t\t= " << sacHeader.d_evla << endl;
	if (sacHeader.d_evlo != -12345.0) o << "evlo\t\t= " << sacHeader.d_evlo << endl;
	if (sacHeader.d_evel != -12345.0) o << "evel\t\t= " << sacHeader.d_evel << endl;
	if (sacHeader.d_evdp != -12345.0) o << "evdp\t\t= " << sacHeader.d_evdp << endl;
	if (sacHeader.d_unused1 != -12345.0) o << "unused1\t\t= " << sacHeader.d_unused1 << endl;
	if (sacHeader.d_user0 != -12345.0) o << "user0\t\t= " << sacHeader.d_user0 << endl;
	if (sacHeader.d_user1 != -12345.0) o << "user1\t\t= " << sacHeader.d_user1 << endl;
	if (sacHeader.d_user2 != -12345.0) o << "user2\t\t= " << sacHeader.d_user2 << endl;
	if (sacHeader.d_user3 != -12345.0) o << "user3\t\t= " << sacHeader.d_user3 << endl;
	if (sacHeader.d_user4 != -12345.0) o << "user4\t\t= " << sacHeader.d_user4 << endl;
	if (sacHeader.d_user5 != -12345.0) o << "user5\t\t= " << sacHeader.d_user5 << endl;
	if (sacHeader.d_user6 != -12345.0) o << "user6\t\t= " << sacHeader.d_user6 << endl;
	if (sacHeader.d_user7 != -12345.0) o << "user7\t\t= " << sacHeader.d_user7 << endl;
	if (sacHeader.d_user8 != -12345.0) o << "user8\t\t= " << sacHeader.d_user8 << endl;
	if (sacHeader.d_user9 != -12345.0) o << "user9\t\t= " << sacHeader.d_user9 << endl;
	if (sacHeader.d_dist != -12345.0) o << "dist\t\t= " << sacHeader.d_dist << endl;
	if (sacHeader.d_az != -12345.0) o << "az\t\t= " << sacHeader.d_az << endl;
	if (sacHeader.d_baz != -12345.0) o << "baz\t\t= " << sacHeader.d_baz << endl;
	if (sacHeader.d_gcarc != -12345.0) o << "gcarc\t\t= " << sacHeader.d_gcarc << endl;
	if (sacHeader.d_internal2 != -12345.0) o << "internal2\t= " << sacHeader.d_internal2 << endl;
	if (sacHeader.d_internal3 != -12345.0) o << "internal3\t= " << sacHeader.d_internal3 << endl;
	if (sacHeader.d_depmen != -12345.0) o << "depmen\t\t= " << sacHeader.d_depmen << endl;
	if (sacHeader.d_cmpaz != -12345.0) o << "cmpaz\t\t= " << sacHeader.d_cmpaz << endl;
	if (sacHeader.d_cmpinc != -12345.0) o << "cmpinc\t\t= " << sacHeader.d_cmpinc << endl;
	if (sacHeader.d_unused2 != -12345.0) o << "unused2\t\t= " << sacHeader.d_unused2 << endl;
	if (sacHeader.d_unused3 != -12345.0) o << "unused3\t\t= " << sacHeader.d_unused3 << endl;
	if (sacHeader.d_unused4 != -12345.0) o << "unused4\t\t= " << sacHeader.d_unused4 << endl;
	if (sacHeader.d_unused5 != -12345.0) o << "unused5\t\t= " << sacHeader.d_unused5 << endl;
	if (sacHeader.d_unused6 != -12345.0) o << "unused6\t\t= " << sacHeader.d_unused6 << endl;
	if (sacHeader.d_unused7 != -12345.0) o << "unused7\t\t= " << sacHeader.d_unused7 << endl;
	if (sacHeader.d_unused8 != -12345.0) o << "unused8\t\t= " << sacHeader.d_unused8 << endl;
	if (sacHeader.d_unused9 != -12345.0) o << "unused9\t\t= " << sacHeader.d_unused9 << endl;
	if (sacHeader.d_unused10 != -12345.0) o << "unused10\t\t= " << sacHeader.d_unused10 << endl;
	if (sacHeader.d_unused11 != -12345.0) o << "unused11\t\t= " << sacHeader.d_unused11 << endl;
	if (sacHeader.d_unused12 != -12345.0) o << "unused12\t\t= " << sacHeader.d_unused12 << endl;
	o.unsetf(ios::floatfield);

	if (sacHeader.d_nzyear != -12345) o << "nzyear\t\t= " << sacHeader.d_nzyear << endl;
	if (sacHeader.d_nzjday != -12345) o << "nzjday\t\t= " << sacHeader.d_nzjday << endl;
	if (sacHeader.d_nzhour != -12345) o << "nzhour\t\t= " << sacHeader.d_nzhour << endl;
	if (sacHeader.d_nzmin != -12345) o << "nzmin\t\t= " << sacHeader.d_nzmin << endl;
	if (sacHeader.d_nzsec != -12345) o << "nzsec\t\t= " << sacHeader.d_nzsec << endl;
	if (sacHeader.d_nzmsec != -12345) o << "nzmsec\t\t= " << sacHeader.d_nzmsec << endl;
	if (sacHeader.d_internal4 != -12345) o << "internal4\t= " << sacHeader.d_internal4 << endl;
	if (sacHeader.d_internal5 != -12345) o << "internal5\t= " << sacHeader.d_internal5 << endl;
	if (sacHeader.d_internal6 != -12345) o << "internal6\t= " << sacHeader.d_internal6 << endl;
	if (sacHeader.d_npts != -12345) o << "npts\t\t= " << sacHeader.d_npts << endl;
	if (sacHeader.d_internal7 != -12345) o << "internal7\t= " << sacHeader.d_internal7 << endl;
	if (sacHeader.d_internal8 != -12345) o << "internal8\t= " << sacHeader.d_internal8 << endl;
	if (sacHeader.d_unused13 != -12345) o << "unused13\t= " << sacHeader.d_unused13 << endl;
	if (sacHeader.d_unused14 != -12345) o << "unused14\t= " << sacHeader.d_unused14 << endl;
	if (sacHeader.d_unused15 != -12345) o << "unused15\t= " << sacHeader.d_unused15 << endl;
	if (sacHeader.d_iftype != -12345) o << "iftype\t\t= " << sacHeader.d_iftype << endl;
	if (sacHeader.d_idep != -12345) o << "idep\t\t= " << sacHeader.d_idep << endl;
	if (sacHeader.d_iztype != -12345) o << "iztype\t\t= " << sacHeader.d_iztype << endl;
	if (sacHeader.d_unused16 != -12345) o << "unused16\t\t= " << sacHeader.d_unused16 << endl;
	if (sacHeader.d_iinst != -12345) o << "iinst\t\t= " << sacHeader.d_iinst << endl;
	if (sacHeader.d_istreg != -12345) o << "istreg\t\t= " << sacHeader.d_istreg << endl;
	if (sacHeader.d_ievreg != -12345) o << "ievreg\t\t= " << sacHeader.d_ievreg << endl;
	if (sacHeader.d_ievtyp != -12345) o << "ievtyp\t\t= " << sacHeader.d_ievtyp << endl;
	if (sacHeader.d_iqual != -12345) o << "iqual\t\t= " << sacHeader.d_iqual << endl;
	if (sacHeader.d_isynth != -12345) o << "isynth\t\t= " << sacHeader.d_isynth << endl;
	if (sacHeader.d_unused17 != -12345) o << "unused17\t= " << sacHeader.d_unused17 << endl;
	if (sacHeader.d_unused18 != -12345) o << "unused18\t= " << sacHeader.d_unused18 << endl;
	if (sacHeader.d_unused19 != -12345) o << "unused19\t= " << sacHeader.d_unused19 << endl;
	if (sacHeader.d_unused20 != -12345) o << "unused20\t= " << sacHeader.d_unused20 << endl;
	if (sacHeader.d_unused21 != -12345) o << "unused21\t= " << sacHeader.d_unused21 << endl;
	if (sacHeader.d_unused22 != -12345) o << "unused22\t= " << sacHeader.d_unused22 << endl;
	if (sacHeader.d_unused23 != -12345) o << "unused23\t= " << sacHeader.d_unused23 << endl;
	if (sacHeader.d_unused24 != -12345) o << "unused24\t= " << sacHeader.d_unused24 << endl;
	if (sacHeader.d_unused25 != -12345) o << "unused25\t= " << sacHeader.d_unused25 << endl;
	if (sacHeader.d_unused26 != -12345) o << "unused26\t= " << sacHeader.d_unused26 << endl;
	if (sacHeader.d_leven != -12345) o << "leven\t\t= " << sacHeader.d_leven << endl;
	if (sacHeader.d_lpspol != -12345) o << "lpspol\t\t= " << sacHeader.d_lpspol << endl;
	if (sacHeader.d_lovrok != -12345) o << "lovrok\t\t= " << sacHeader.d_lovrok << endl;
	if (sacHeader.d_lcalda != -12345) o << "lcalda\t\t= " << sacHeader.d_lcalda << endl;
	if (sacHeader.d_unused27 != -12345) o << "unused27\t= " << sacHeader.d_unused27 << endl;

	if (strcmp(sacHeader.d_kstnm, "-12345") != 0) o << "kstnm\t\t= " << sacHeader.d_kstnm << endl;
	if (strcmp(sacHeader.d_kevnm, "-12345") != 0) o << "kevnm\t\t= " << sacHeader.d_kevnm << endl;
	if (strcmp(sacHeader.d_khole, "-12345") != 0) o << "khole\t\t= " << sacHeader.d_khole << endl;
	if (strcmp(sacHeader.d_ko, "-12345") != 0) o << "ko\t\t= " << sacHeader.d_ko << endl;
	if (strcmp(sacHeader.d_ka, "-12345") != 0) o << "ka\t\t= " << sacHeader.d_ka << endl;
	if (strcmp(sacHeader.d_kt0, "-12345") != 0) o << "kt0\t\t= " << sacHeader.d_kt0 << endl;
	if (strcmp(sacHeader.d_kt1, "-12345") != 0) o << "kt1\t\t= " << sacHeader.d_kt1 << endl;
	if (strcmp(sacHeader.d_kt2, "-12345") != 0) o << "kt2\t\t= " << sacHeader.d_kt2 << endl;
	if (strcmp(sacHeader.d_kt3, "-12345") != 0) o << "kt3\t\t= " << sacHeader.d_kt3 << endl;
	if (strcmp(sacHeader.d_kt4, "-12345") != 0) o << "kt4\t\t= " << sacHeader.d_kt4 << endl;
	if (strcmp(sacHeader.d_kt5, "-12345") != 0) o << "kt5\t\t= " << sacHeader.d_kt5 << endl;
	if (strcmp(sacHeader.d_kt6, "-12345") != 0) o << "kt6\t\t= " << sacHeader.d_kt6 << endl;
	if (strcmp(sacHeader.d_kt7, "-12345") != 0) o << "kt0\t\t= " << sacHeader.d_kt0 << endl;
	if (strcmp(sacHeader.d_kt8, "-12345") != 0) o << "kt8\t\t= " << sacHeader.d_kt8 << endl;
	if (strcmp(sacHeader.d_kt9, "-12345") != 0) o << "kt9\t\t= " << sacHeader.d_kt9 << endl;
	if (strcmp(sacHeader.d_kf, "-12345") != 0) o << "kf\t\t= " << sacHeader.d_kf << endl;
	if (strcmp(sacHeader.d_kuser0, "-12345") != 0) o << "kuser0\t\t= " << sacHeader.d_kuser0 << endl;
	if (strcmp(sacHeader.d_kuser1, "-12345") != 0) o << "kuser1\t\t= " << sacHeader.d_kuser1 << endl;
	if (strcmp(sacHeader.d_kuser2, "-12345") != 0) o << "kuser2\t\t= " << sacHeader.d_kuser2 << endl;
	if (strcmp(sacHeader.d_kcmpnm, "-12345") != 0) o << "kcmpnm\t\t= " << sacHeader.d_kcmpnm << endl;
	if (strcmp(sacHeader.d_knetwk, "-12345") != 0) o << "knetwk\t\t= " << sacHeader.d_knetwk << endl;
	if (strcmp(sacHeader.d_kdatrd, "-12345") != 0) o << "kdatrd\t\t= " << sacHeader.d_kdatrd << endl;
	if (strcmp(sacHeader.d_kinst, "-12345") != 0) o << "kinst\t\t= " << sacHeader.d_kinst << endl;

	return o;
}

void SacHeader::calcEvStaInfo() {
	if (!assureStationLoc() || !assureEventLoc())
		exit (-1);
	distaz(getLat(), getLon(), getSlat(), getSlon(), &d_gcarc, &d_az, &d_baz, &d_dist);
}

void SacHeader::fixString(char *s, int size) {
	if (*(s+size-1) != ' ') {
		*(s+size-1) = '\0';
	} else {
		for (int i = (size-2); i >= 0; i--) {
			if (*(s+i) != ' ') {
				*(s+i+1) = '\0';
				break;
			}
			if (i == 0)	// pure blank string
				*s = '\0';
		}
	}
}

int SacHeader::assureStationLoc() const {
        if (getSlat() == 0 && getSlon() == 0) {
                cerr << "Warning: No station location in the header!" << endl;
                return (0);
        }
        return (1);
}

int SacHeader::assureEventLoc() const {
        if (getLat() == 0 && getLon() == 0) {
                cerr << "Warning: No event location in the header!" << endl;
                return (0);
        }
        return (1);
}

