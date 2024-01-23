/*
 
    Copyright 1995-2006, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. Odb.H $

    $Author$

    $Id$

    $Log$
    Revision 1.3  2007/09/11 12:21:50  aleber
    EC:: LINUX: Useless X11 Include removed

    Revision 1.2  2006/11/07 16:20:36  stemic
    Fixed to compile under linux

    Revision 1.1  2006/08/24 09:39:56  enrcar
    Imported source


*/


/* Based on:  A.C.S. Odb Library - Odb.H 1.18 98/07/01 (include) */

#ifndef __LIBODBCPP_H__
#define __LIBODBCPP_H__

#if defined(__cplusplus) && defined(sgi) && !defined(ACS_LINUX_FORCE_ORB_HPP)

#include <iostream.h>
#include <fstream.h>
#include <strstream.h>

#endif /* __cplusplus */

#include <stdlib.h>
#include <ctype.h>
#ifdef sgi
/*EC:: Motif won't be used with Linux... (?) */
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#else
typedef char		Boolean;
typedef long		XtArgVal;
typedef unsigned char	XtEnum;
#endif

#include <Odb.h>

#define MAX_SPOT_SEGMENT_NB	50
#define MAX_SPOT_CHAN_SEGM_NB	50
#define MAX_SPOT_SWATH_NB	10
#define MAX_SPOT_EPHEMERIS_NB	50
#define MAX_STRING_LEN		300
#define MAX_RECORD_LEN		5000
#define EARTH_RADIUS		6378.165L
#define JD1950TO2000		18262
#define MSPERDAY		86400000	/* ms per day */
#define ODB_PRECL_LINE	130

#if defined(__cplusplus) && defined(sgi) && !defined(ACS_LINUX_FORCE_ORB_HPP)

class olCardInfo {
protected:
	Boolean		filled ;
	unsigned int	day ;		// day number of year
	unsigned int	mission ;	// mission number
	unsigned int	orbit ;		// orbit number
public:
	olCardInfo(void)			{ complete = filled = False ; day = mission = orbit = 0 ; }
	Boolean		complete ;
	Boolean GetFilled(void)			{ return filled ; }
	unsigned int GetDay(void)		{ return day ; }
	unsigned int GetMis(void)		{ return mission ; }
	unsigned int GetOrb(void)		{ return orbit ; }
	void SetFilled(Boolean f = True)	{ filled  = f ; }
	void SetDay(unsigned int d)		{ day     = d ; }
	void SetMis(unsigned int m)		{ mission = m ; }
	void SetOrb(unsigned int o)		{ orbit   = o ; }
	virtual char *GetText(void) = 0 ;
	virtual int ReadLine(istrstream&) = 0 ;
} ;

class olReservRequest : public olCardInfo { // 01
	olRequestType	req_type ;	// request type
	float		begin_lat ;	// given in degrees (-90 to 90)
	float		end_lat ;
public:
	olReservRequest(void) { req_type = (olRequestType)0 ; begin_lat = end_lat = 0. ; }
	olRequestType GetReqType(void)	{ return req_type ; }
	float GetBegLat(void)		{ return begin_lat ; }
	float GetEndLat(void)		{ return end_lat ; }
	char *GetText(void) ;
	int ReadLine(istrstream&) ;
} ;

class olRespToReserv : public olCardInfo { // 02
	olRespToReservDiagnostic	diagnostic ;	// diagnostic
	float		begin_lat ;	// given in degrees (-90 to 90)
	float		end_lat ;
public:
	olRespToReserv(void) { diagnostic = (olRespToReservDiagnostic)0 ; begin_lat = end_lat = 0. ; }
	olRespToReservDiagnostic GetDiagn(void)	{ return diagnostic ; }
	float GetBegLat(void)			{ return begin_lat ; }
	float GetEndLat(void)			{ return end_lat ; }
	char *GetText(void) ;
	int ReadLine(istrstream&) ;
} ;

class olSpotSegment {
public:
	olSpectralMode	spec_mode ;	// spectral mode
	float		cen_lon ;	// degrees (0 to 360)
	float		cen_lat ;
	olSpectralMode	spec_mode2 ;	// last 3 fields are used
	float		cen_lon2 ;	// if programming type
	float		cen_lat2 ;	// is INDEP_HRVS
} ;

class olProgRequest : public olCardInfo { // 03
	olProgrammingType	prog_type ;	// programming type
	unsigned int		segments_nb ;	// number of segments
	olSpotGainType		xs_gain[3] ;
	olSpotGainType		pa_gain ;
public:
	unsigned int		line_id ;
	olSpotSegment		segments[MAX_SPOT_SEGMENT_NB] ;
	olProgrammingType GetProgType(void)	{ return prog_type ; }
	unsigned int GetSegmNb(void)		{ return segments_nb ; }
	olSpotGainType GetXsGain(int id)	{ return xs_gain[id] ;}
	olSpotGainType GetPaGain(void)		{ return pa_gain ;}
	char *GetText(void) ;
	int ReadLine(istrstream&) ;
	int Read(istrstream&) ;
	olProgRequest(void) { prog_type = (olProgrammingType)0 ; line_id = segments_nb = 0 ; pa_gain = xs_gain[0] = xs_gain[1] = xs_gain[2] = (olSpotGainType)0 ; (void)memset(segments, 0, sizeof(olSpotSegment)*MAX_SPOT_SEGMENT_NB) ; }
} ;

class olSpotSwath {
public:
	olSpectralMode	spec_mode ;	// spectral mode
	float		begin_lon ;
	float		begin_lat ;
	float		end_lon ;
	float		end_lat ;
	float		angle ;
} ;

class olRespToProg : public olCardInfo { // 04
	olRespToProgDiagnostic	diagnostic ;	// diagnostic
	unsigned int		swath_nb ;	// number of swaths
public:
	unsigned int		line_id ;
	olSpotSwath		swaths[MAX_SPOT_SWATH_NB] ;
	olRespToProgDiagnostic GetDiagn(void)	{ return diagnostic ; }
	unsigned int GetSwathNb(void)		{ return swath_nb ; }
	char *GetText(void) ;
	int ReadLine(istrstream&) ;
	olRespToProg(void) { diagnostic  = (olRespToProgDiagnostic)0 ; line_id = swath_nb = 0 ; (void)memset(swaths, 0, sizeof(olSpotSwath)*MAX_SPOT_SWATH_NB) ; }
} ;

class olOsculatingElem {
public:
	char	otype[4] ;
	double	value ;
	char	unit[3] ;
	int	checksum ;
} ;

class olEphemerisPoint {
public:
	unsigned int	point_id ;
	double		x_km ;
	double		y_km ;
	double		z_km ;
	double		x_km_s ;
	double		y_km_s ;
	double		z_km_s ;
} ;

class olEphemerisInfo {
public:
	unsigned int		epoint_nb ;
	unsigned int		jd_first_pt ;
	unsigned int		sec_first_pt ;
	olEphemerisPoint	epoint[MAX_SPOT_EPHEMERIS_NB] ;
} ;

class olAntennaPtData : public olCardInfo { // 05
	unsigned int		jday ;		// julian day date
	double			begin_sec ;	// begin seconds (jd)
	double			end_sec ;	// end seconds (jd)
	olAntennaPtDataType	data ;		// antenna-pointing data
public:
	unsigned int		line_id ;
	olOsculatingElem	oelems[6] ;	// osculating elements
	olEphemerisInfo		eph_info ;	// ephemeris data
	unsigned int GetJDay(void)		{ return jday ; }
	double GetBegSec(void)			{ return begin_sec ; }
	double GetEndSec(void)			{ return end_sec ; }
	olAntennaPtDataType GetData(void)	{ return data ; }
	char *GetText(void) ;
	int ReadLine(istrstream&) ;
	olAntennaPtData(void) { data  = (olAntennaPtDataType)0 ; line_id = jday = 0 ; begin_sec = end_sec = 0. ; (void)memset(oelems, 0, sizeof(olOsculatingElem)*6) ; (void)memset(&eph_info, 0, sizeof(olEphemerisInfo)) ; }
} ;

class olChannelSegm {
public:
	int		chan_id ;
	unsigned int	begin_counter ;
	unsigned int	end_counter ;
	int		hrv_id ;
	char		spec_mode ;
	char		segment ;
	int		test_indicator ;
	int		pa_gain ;
	int		xs_gain[3] ;
	int		ssm_angle ;
	int		status ;
	int		validity ;
	int		ref_pos ;
} ;

class olCoeff {
public:
	unsigned int		To ;
	float			to ;
	float			a ;
	unsigned long int	co ;
} ;

class olImgSchedPlan : public olCardInfo { // 06
	olIspDiagnostic	diagnostic ;	// diagnostic
	unsigned int	chan1_segm_nb ;
	unsigned int	chan2_segm_nb ;
	unsigned long	obt ;	// on board time of pass
public:
	olCoeff		coeff ;
	unsigned int	line_id ;
	olChannelSegm	chan1_segm[MAX_SPOT_CHAN_SEGM_NB] ;
	olChannelSegm	chan2_segm[MAX_SPOT_CHAN_SEGM_NB] ;
	olIspDiagnostic GetDiagn(void)		{ return diagnostic ; }
	unsigned int GetChan1SegNb(void)	{ return chan1_segm_nb ; }
	unsigned int GetChan2SegNb(void)	{ return chan2_segm_nb ; }
	unsigned long int GetObt(void)		{ return obt ; }
	void SetChan1SegNb(unsigned int n)	{ chan1_segm_nb = n ; }
	void SetChan2SegNb(unsigned int n)	{ chan2_segm_nb = n ; }
	char *GetText(void) ;
	int ReadLine(istrstream&) ;
	olImgSchedPlan(void) { diagnostic  = (olIspDiagnostic)0 ; line_id = chan1_segm_nb = chan2_segm_nb = 0 ; obt = (unsigned long)0 ; (void)memset(&coeff, 0, sizeof(olCoeff)) ; (void)memset(chan1_segm, 0, sizeof(olChannelSegm)*MAX_SPOT_CHAN_SEGM_NB) ; (void)memset(chan2_segm, 0, sizeof(olChannelSegm)*MAX_SPOT_CHAN_SEGM_NB) ; }
} ;

class olPostPassEphem : public olCardInfo { // 07
public:
	unsigned int	line_id ;
	olCoeff		coeff ;
	olEphemerisInfo	eph_info ;
	char *GetText(void) ;
	int ReadLine(istrstream&) ;
	olPostPassEphem(void) { line_id = 0 ; (void)memset(&coeff, 0, sizeof(olCoeff)) ; (void)memset(&eph_info, 0, sizeof(olEphemerisInfo)) ; }
} ;

class olRecord {
	static olFormat	format ;	// defined in olRecord.c++
	static int	swap ;		// defined in olRecord.c++
	double		jdt ;
	olComeFrom	where ;
protected:
	long		inspos ;
public:
	olRecord(void) { jdt = 0 ; where = (olComeFrom)0 ; }
	double GetJdt(void)			{ return jdt ; }
	olComeFrom GetFromWhere(void)		{ return where ; }
	olFormat GetFormat(void)		{ return format ; }
	int GetSwapFlag(void)			{ return swap ; }
	void SetJdt(double j)			{ jdt = j ; }
	void SetFromWhere(olComeFrom w)		{ where = w ; }
	static void SetFormat(olFormat f)	{ format = f ; }
	static void SetSwapFlag(int s)		{ swap = s ; }
	int CompPosition(olRecord *, LIST *) ;
	int Add(LIST&) ;
	int Delete(LIST&) ;
	long CheckUTCD(const UTCD, UTCD *, long fd = 1) ;
	virtual int GetOrbit(void) = 0 ;
	virtual int ComputeJdt(void) = 0 ;
	virtual int Read(istream&) = 0 ;
	virtual int Write(ofstream&) = 0 ;
	virtual int BuildString(char *) = 0 ;
	virtual int Show(Widget, int) = 0 ;
	virtual Boolean GetFilled(void) = 0 ;
	virtual olRecord *Copy(void) = 0 ;
	virtual int SetValues(Widget, int) = 0 ;
	virtual int GetDay(void) = 0 ;
	virtual int GetMsec(void) = 0 ;
} ;

class olPrclState {
public:
	char	reckey[6] ;
	char	start[6] ;
	char	end[6] ;
	char	obstyp[6] ;
	char	obslev[6] ;
	char	modid[2] ;
	char	relid[2] ;
	char	rmsfit[4] ;
	char	sigpos[4] ;
	char	sigvel[4] ;
	char	qualit[1] ;
	char	tdtutc[5] ;
	char	cmmnt[78] ;
} ;

class olPrclStterr {
public:
	char	reckey[6] ;
	char	satid[7] ;
	char	orbtyp[1] ;
	char	ttagd[6] ;
	char	ttagms[11] ;
	char	xsat[12] ;
	char	ysat[12] ;
	char	zsat[12] ;
	char	xdsat[11] ;
	char	ydsat[11] ;
	char	zdsat[11] ;
	char	roll[6] ;
	char	pitch[6] ;
	char	yaw[6] ;
	char	ascarc[2] ;
	char	check[3] ;
	char	quali[1] ;
	char	radcor[4] ;
	char	spare[2] ;
} ;

class olErsSvInfo : public olRecord {
        struct {
            int     day ;           // julian date
            int     ms ;            // msec after midnight
            int     orbit ;         // orbit number
            double  x ;             // position X [KM]
            double  y ;             // position Y [KM]
            double  z ;             // position Z [KM]
            double  vx ;            // velocity X [KM/SEC]
            double  vy ;            // velocity Y [KM/SEC]
            double  vz ;            // velocity Z [KM/SEC]
            int     tai ;           // tai coefficient
        } record ;
	static olPrclState	state ;	// defined in olErsSv.c++
public:
	int GetDay(void)		{ return record.day ; }
	int GetMsec(void)		{ return record.ms ; }
	int GetOrbit(void)		{ return record.orbit ; }
	double GetX(void)		{ return record.x ; }
	double GetY(void)		{ return record.y ; }
	double GetZ(void)		{ return record.z ; }
	double GetVX(void)		{ return record.vx ; }
	double GetVY(void)		{ return record.vy ; }
	double GetVZ(void)		{ return record.vz ; }
	int GetTai(void)        	{ return record.tai ; }
	char *GetStateUtc(void)		{ return state.tdtutc ; }
	void SetDay(int day)		{ record.day   = day ; }
	void SetMsec(int ms)		{ record.ms    = ms ; }
	void SetOrbit(int orbit)	{ record.orbit = orbit ; }
	void SetX(double x)		{ record.x     = x ; }
	void SetY(double y)		{ record.y     = y ; }
	void SetZ(double z)		{ record.z     = z ; }
	void SetVX(double vx)		{ record.vx    = vx ; }
	void SetVY(double vy)		{ record.vy    = vy ; }
	void SetVZ(double vz)		{ record.vz    = vz ; }
	void SetTai(int tai)		{ record.tai   = tai ; }
	void SetStateUtc(char *utc)	{ (void)strcpy(state.tdtutc, utc) ; }
	int BuildString(char *) ;
	int Show(Widget, int) ;
	int ReadFAscii(istream&) ;
	int ReadFAsciiPrcl(istream&) ;
	int ReadFBinary(istream&) ;
	int Read(istream&) ;
	int Write(ofstream&) ;
	int ComputeJdt(void) ;
	Boolean GetFilled(void) { return False ; }
	olRecord *Copy(void) ;
	int SetValues(Widget, int) ;
} ;

class olErsTcInfo : public olRecord {
        struct {
                int             day ;           // julian date
                int             ms ;            // msec after midnight
                int             orbit ;         // orbit number
                unsigned        BinTime ;       // binary time
                unsigned        ClockPeriod ;   // clock period
        } record ;
public:
        int GetDay(void)			{ return record.day ; }
        int GetMsec(void)			{ return record.ms ; }
        int GetOrbit(void)			{ return record.orbit ; }
        unsigned GetBinTime(void)		{ return record.BinTime ; }
        unsigned GetClockPeriod(void)		{ return record.ClockPeriod ; }
        void SetDay(int day)			{ record.day         = day ; }
        void SetMsec(int ms)			{ record.ms          = ms ; }
        void SetOrbit(int orbit)		{ record.orbit       = orbit ; }
        void SetBinTime(unsigned BinTime)	{ record.BinTime     = BinTime ; }
        void SetClockPeriod(unsigned ClockPeriod){ record.ClockPeriod = ClockPeriod ; }
	int BuildString(char *) ;
	int Show(Widget, int) ;
        int ReadFBinary(istream&) ;
        int Read(istream&) ;
        int Write(ofstream&) ;
	int ComputeJdt(void) ;
	Boolean GetFilled(void) { return False ; }
	olRecord *Copy(void) ;
	int SetValues(Widget, int) ;
} ;

class olSpotInfo : public olRecord {
	static char		line[MAX_STRING_LEN] ;	// defined in olSpot.c++
	static unsigned int	last_orbit ;		// defined in olSpot.c++
public:
	olReservRequest	data1 ;
	olRespToReserv	data2 ;
	olProgRequest	data3 ;
	olRespToProg	data4 ;
	olAntennaPtData	data5 ;
	olImgSchedPlan	data6 ;
	olPostPassEphem	data7 ;
	int GetOrbit(void) ;
	int GetDay(void) ;
	int GetMsec(void) ;
	char *GetText(void) ;
	int BuildString(char *) ;
	int Show(Widget, int) ;
	int Read(istream&) ;
	int Write(ofstream&) ;
	int ComputeJdt(void) ;
	Boolean GetFilled(void) ;
	olRecord *Copy(void) ;
	int SetValues(Widget, int) ;
	void Merge(const olSpotInfo&) ;
} ;

class olJLHeader {
public:
	int	MagicNum ; /* magic number */
	int	NumOfRec ; /* number of records inside the file */
}  ;

class olJLSvInfo : public olRecord {
	static int		ins_mode ;	// defined in olJLSv.c++
	static olJLHeader	header ;	// defined in olJLSv.c++
        struct {
		UTCD		UtTime ;			/* UT time */
		JD50		JdTime ;			/* Julian date time */
		olPropagation	PropagationFlag ;		/* propagation flag */
		olValidation	SetDataValid ;			/* validation of data set   			*/
		double		SemiMajorAxis ;			/* semimajor axis of orbit	    (Km)	*/
		double		Eccentricity ;			/* eccentricity			    (deg)	*/
		double		Inclination ;			/* inclination			    (deg)	*/
		double		LongAscNode ;			/* longitude of the ascending node  (deg)	*/
		double		RateLongAscNode ;		/* longitude of the asc. node rate  (deg/day)	*/
		double		ArgPerigee ;			/* argument of perigee		    (deg)	*/
		double		RateArgPerigee ;		/* argument of perigee rate	    (deg/day)	*/
		double		MeanAnomaly ;			/* mean anomaly			    (deg)	*/
		double		Period ;			/* actual orbital period	    (min)	*/
		double		PerigeeHeight ;			/* height of perigee		    (Km)	*/
		double		ApogeeHeight ;			/* height of apogee		    (Km)	*/
		double		PerigeeSpeed ;			/* velocity at the perigee	    (Km/sec)	*/
		double		ApogeeSpeed ;			/* velocity at the apogee	    (Km/sec)	*/
		double		PerigeeLatitude ;		/* perigee latitude		    (deg.dec)	*/
		double		EastLongitude ;			/* east longitude		    (deg.dec)	*/
		double		GeodeticLat ;			/* geodetic latitude		    (deg.dec)	*/
		double		HeightSat ;			/* height of satellite at ...	    (Km)	*/
		double		x ;				/* ECITOD x coord		    (Km)	*/
		double		y ;				/* ECITOD y coord		    (Km)	*/
		double		z ;				/* ECITOD z coord		    (Km)	*/
		double		vx ;				/* ECITOD vx coord		    (Km/sec)	*/
		double		vy ;				/* ECITOD vy coord		    (Km/sec)	*/
		double		vz ;				/* ECITOD vz coord		    (Km/sec)	*/
        } record ;
public:
	void SetHeader(int recs_nb)	{ header.NumOfRec = recs_nb ; }
	void SetInsMode(int mode)	{ ins_mode = mode ; }
	int GetInsMode(void)		{ return ins_mode ; }
	int GetDay(void) ;
	int GetMsec(void) ;
	int GetOrbit(void)      	{ return 0 ; }
	UTCD GetUtTime(void)		{ return record.UtTime ; }
	double GetSemiMajAxis(void)	{ return record.SemiMajorAxis ; }
	double GetEccentricity(void)	{ return record.Eccentricity ; }
	double GetInclination(void)	{ return record.Inclination ; }
	double GetLongAscNode(void)	{ return record.LongAscNode ; }
	double GetArgPerigee(void)	{ return record.ArgPerigee ; }
	double GetMeanAnomaly(void)	{ return record.MeanAnomaly ; }
	double GetX(void)		{ return record.x ; }
	double GetY(void)		{ return record.y ; }
	double GetZ(void)		{ return record.z ; }
	double GetVX(void)		{ return record.vx ; }
	double GetVY(void)		{ return record.vy ; }
	double GetVZ(void)		{ return record.vz ; }
	void SetX(double x)		{ record.x     = x ; }
	void SetY(double y)		{ record.y     = y ; }
	void SetZ(double z)		{ record.z     = z ; }
	void SetVX(double vx)		{ record.vx    = vx ; }
	void SetVY(double vy)		{ record.vy    = vy ; }
	void SetVZ(double vz)		{ record.vz    = vz ; }
	int BuildString(char *) ;
	int Show(Widget, int) ;
	int Read(istream&) ;
	int ReadHeader(istream&) ;
	int Write(ofstream&) ;
	int WriteHeader(ofstream&) ;
	int ComputeJdt(void) ;
	Boolean GetFilled(void) { return False ; }
	olRecord *Copy(void) ;
	int SetValues(Widget, int) ;
} ;

class olDate {
public:
	int	Year ;
	int	Month ;
	int	Day ;
} ;

class olJersTcInfo : public olRecord {
	static olJLHeader	header ;	// defined in olJersTc.c++
        struct {
		olDate	RegistrationDate ; /* registration date of time error */
		JD50	JulianRegDate ;    /* julian registration date */
		UTCD	PresetGroundTime ; /* preset ground    time (UTC) */
		int	PresetSatTime ;    /* preset satellite time (counts) */
		int	TimeError ;        /* time error (millisec) */
        } record ;
public:
	void SetHeader(int recs_nb)	{ header.NumOfRec = recs_nb ; }
	int GetDay(void) ;
	int GetMsec(void) ;
	int GetOrbit(void)      	{ return 0 ; }
	olDate GetRegDate(void)		{ return record.RegistrationDate ; }
	UTCD GetPresGrTime(void)	{ return record.PresetGroundTime ; }
	int GetPresSatTime(void)	{ return record.PresetSatTime ; }
	int GetTimeError(void)		{ return record.TimeError ; }
	int GetJRegDate(void)		{ return record.JulianRegDate ; }
	int BuildString(char *) ;
	int Show(Widget, int) ;
	int Read(istream&) ;
	int ReadHeader(istream&) ;
	int Write(ofstream&) ;
	int WriteHeader(ofstream&) ;
	int ComputeJdt(void) ;
	Boolean GetFilled(void)		{ return False ; }
	olRecord *Copy(void) ;
	int SetValues(Widget, int) ;
} ;

class olBaseList {
protected:
	int				sat_id ;
	int				miss_id ;
	int				db_type ;
	char			db_name[MAX_STRING_LEN] ;
	long			man_rec_nb ;
	Boolean			invdate ;
	Boolean			fromfile ;
	unsigned int	exist_rec_id ;
public:
	olBaseList() { invdate = False ; fromfile = False ; }
	LIST	db ;
	LIST	disp ;
	int GetSatellite(void)				{ return sat_id ; }
	int GetMission(void)				{ return miss_id ; }
	int GetDbType(void)					{ return db_type ; }
	char *GetDbPath(void)				{ return db_name ; }
	int GetDbRecNb(void)				{ return db.getElmNumTot() ; }
	int GetDispRecNb(void)				{ return disp.getElmNumTot() ; }
	int GetManRecNb(void)				{ return man_rec_nb ; }
	Boolean GetLoadFlag(void)			{ return True ; }
	Boolean GetDateFlag(void)			{ return invdate ; }
	Boolean GetFromFileFlag(void)		{ return fromfile ; }
	unsigned int GetExistRecId(void)	{ return exist_rec_id ; }
	void SetSatellite(int sat)			{ sat_id = sat ; }
	void SetDbType(int type)			{ db_type = type ; }
	void SetMission(int miss)			{ miss_id = miss ; }
	void SetDbPath(char *name)			{ if (name) (void)strcpy(db_name, name) ; }
	void SetLoadFlag(Boolean)			{}
	void SetDateFlag(Boolean i)			{ invdate = i ; }
	void SetFromFileFlag(Boolean i)		{ fromfile = i ; }
	void AddManRecNb(void)				{ man_rec_nb++ ; }
	void DelManRecNb(void)				{ man_rec_nb-- ; }
	int CompDbName(void) ;
	int CompDbDateName(JD50) ;
	olRecord *GetRecNum(long) ;
	int GetRecArray(olComeFrom, int *) ;
	int FilterDb(olFilterType, int, double, double) ;
	int Sort(void) ;
	int CheckDate(JD50) ;
	virtual olFormat GetFormat(olComeFrom) = 0 ;
	virtual void Empty(void) = 0 ;
	virtual int LoadDb(olComeFrom) = 0 ;
	virtual int SaveDb(void) = 0 ;
	virtual char *GetSatName(void) = 0 ;
	virtual int CompDbDir(char *) = 0 ;
} ;

class olErsSvList : public olBaseList {
public:
	olFormat GetFormat(olComeFrom) ;
	void Empty(void) ;
	int LoadDb(olComeFrom) ;
	int SaveDb(void) ;
	char *GetSatName(void)		{ return "ERS" ; }
	int CompDbDir(char *) ;
	int CompFileDir(char *) ;
	int CompDbSearchMask(char *) ;
	int CompFileSearchMask(char *) ;
	int GetNearestRecByDate(JD50, olErsSvInfo **) ;
	int GetRecsByRange(JD50, JD50, olErsSvInfo **) ;
	~olErsSvList()				{ Empty() ; }
	Boolean CheckIfExist(olErsSvInfo *) ;
} ;

class olErsTcList : public olBaseList {
public:
	olFormat GetFormat(olComeFrom) ;
	void Empty(void) ;
	int LoadDb(olComeFrom) ;
	int SaveDb(void) ;
	char *GetSatName(void)		{ return "ERS" ; }
	int CompDbDir(char *) ;
	int CompFileDir(char *) ;
	int CompFileSearchMask(char *) ;
	int GetNearestRecByDate(JD50, olErsTcInfo **) ;
	int GetRecsByRange(JD50, JD50, olErsTcInfo **) ;
	~olErsTcList()				{ Empty() ; }
	Boolean CheckIfExist(olErsTcInfo *) ;
} ;

class olJLSvList : public olBaseList {
public:
	olFormat GetFormat(olComeFrom)	{ return Binary ; }
	void Empty(void) ;
	int LoadDb(olComeFrom) ;
	int SaveDb(void) ;
	char *GetSatName(void)		{ return (db_type == ODB_LANDSAT_DB_TYPE) ? "LANDSAT" : "JERS" ; }
	int CompDbDir(char *) ;
	int GetRecsByDate(UTCD, olJLSvInfo **) ;
	int GetNearestRecByDate(UTCD, olJLSvInfo **) ;
	~olJLSvList()				{ Empty() ; }
	Boolean CheckIfExist(olJLSvInfo *) ;
} ;

class olJersTcList : public olBaseList {
public:
	olFormat GetFormat(olComeFrom)	{ return Binary ; }
	void Empty(void) ;
	int LoadDb(olComeFrom) ;
	int SaveDb(void) ;
	char *GetSatName(void)		{ return "JERS" ; }
	int CompDbDir(char *) ;
	~olJersTcList()				{ Empty() ; }
	Boolean CheckIfExist(olJersTcInfo *) ;
} ;

class olSpotList : public olBaseList {
public:
	void SetMission(int miss) ;
	Boolean CheckIfExist(olSpotInfo *) ;
	olFormat GetFormat(olComeFrom) ;
	void Empty(void) ;
	int LoadDb(olComeFrom) ;
	int SaveDb(void) ;
	int SplitDb(void) ;
	char *GetSatName(void)		{ return "SPOT" ; }
	int CompDbDir(char *) ;
	int CompFileDir(char *) ;
	int CompDbSearchMask(char *) ;
	int CompFileSearchMask(char *) ;
	int GetRecByDateAndOrbit(int, unsigned, olSpotInfo **) ;
	int GetRecsByDate(int, unsigned **, olSpotInfo **) ;
	~olSpotList()				{ Empty() ; }
} ;

void olInitialize(void) ;
unsigned long olSwp4(unsigned long   *) ;
int olSwp4I(int *) ;
char *olGetMonthS(int) ;
void olGetCurrDate(int *, int *, int *) ;
int olCompare(const void *, const void *) ;

#define olGetFileName(a)	(strrchr(a, '/') ? strrchr(a, '/') + 1 : (a))
#define olEnvCpy(s, e)		(getenv(e) ? (void)strcpy((s), getenv(e)) : (void)strcpy((s), ""))

#endif /* defined(__cplusplus) && defined(sgi) && !defined(ACS_LINUX_FORCE_ORB_HPP) */


#endif /*__LIBODBCPP_H__ */
