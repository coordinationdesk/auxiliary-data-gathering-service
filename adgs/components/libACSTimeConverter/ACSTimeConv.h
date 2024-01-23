// PRQA S 1050 EOF
/* 
	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Authors: $

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.5  2014/04/14 13:59:24  marpas
	coding best practices applied
	performances boost (up to 100x)
	interface rationalization
	coverage test
	
	Revision 2.4  2014/04/10 18:18:51  marpas
	work in progress: do not use
	
	Revision 2.3  2014/04/10 11:14:57  marpas
	refactoring in progress - do not use
	
	Revision 2.2  2014/04/09 16:31:30  marpas
	refactoring in progress - do not use for stable
	
	Revision 2.1  2014/02/03 14:19:38  giucas
	New macros adopted
	
	Revision 2.0  2006/03/01 16:29:25  serfol
	Exception 2.0 I/F adapted
	
	Revision 1.18  2006/03/01 16:28:43  serfol
	nothing
	
	Revision 1.17  2005/10/12 11:02:02  seralb
	Added ascii cosmo compact
	
	Revision 1.16  2005/09/16 11:41:37  seralb
	Added conversion for Ers Millisec Ascii Format
	
	Revision 1.15  2005/03/03 14:38:31  seralb
	Cosmo format and long double support
	
	Revision 1.14  2004/12/17 10:29:47  seralb
	Now jd50 instead of jd
	
	Revision 1.13  2004/12/13 17:08:46  seralb
	Configuration file for reference conversion and JD instead of JD2000
	
	Revision 1.12  2004/12/06 16:09:36  seralb
	Upgrade
	
	Revision 1.11  2004/12/06 11:31:57  seralb
	Added UT1 conversion
	
	Revision 1.10  2004/12/03 15:47:40  seralb
	Added method and GPS conversion implemented
	
	Revision 1.9  2004/12/03 13:22:25  seralb
	Header fixed
	
	Revision 1.8  2004/12/02 16:07:18  seralb
	Bug solved
	
	Revision 1.7  2004/12/01 16:36:07  seralb
	Some names changed and a bug solved
	
	Revision 1.6  2004/12/01 15:09:47  seralb
	Added constructor and similar for the structure
	
	Revision 1.5  2004/12/01 14:20:36  seralb
	Now there is just one public method
	
	Revision 1.4  2004/11/30 16:09:26  seralb
	now int are long int
	
	Revision 1.3  2004/11/29 16:57:17  seralb
	Now methods are global
	
	Revision 1.2  2004/11/26 14:14:28  seralb
	Upgrade
	
	Revision 1.1.1.1  2004/11/25 16:27:46  seralb
	Import libACSTimeConverter
	
 */	




#ifndef _ACSTimeConv_H_
#define _ACSTimeConv_H_ 


#include <acs_c++config.hpp>
#include <DateTime.h>
#include <exException.h>
#include <exMacros.h>
#include <RegExp.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class ACSTimeConv // PRQA S 2109
{
public:

	/*! class ACSTimeConvException declaration */
	exDECLARE_EXCEPTION(ACSTimeConvException,exException) ; // PRQA S 2131, 2502


	/**
	 * \brief  Class constructor 
	 *
	 */	
	ACSTimeConv();

	/**
	 * \brief  Class destructor   
	 */ 
	~ACSTimeConv() throw() {} 



	enum TimeFormatDefines
	{

	  PROCESSING					    = 1,	 // JD50

	  TRANSPORT_DAY_SEC_USEC 			= 11,	 //Days, Seconds, Microseconds
	  TRANSPORT_DAY_MSEC_USEC 			= 12,	 //Days, MilliSeconds, Microseconds

	  ASCII_STD					        = 31,	 //yyyy-mm-dd_hh:nn:ss
	  ASCII_STD_REF					    = 32,	 //RRR=yyyy-mm-dd_hh:nn:ss
	  ASCII_STD_MICROSEC				= 33,	 //yyyy-mm-dd_hh:nn:ss.uuuuuu
	  ASCII_STD_REF_MICROSEC			= 34,	 //RRR=yyyy-mm-dd_hh:nn:ss.uuuuuu

	  ASCII_COMPACT					    = 35,	 //yyyymmdd_hhnnss
	  ASCII_COMPACT_REF 				= 36,	 //RRR=yyyymmdd_hhnnss
	  ASCII_COMPACT_MICROSEC			= 37,	 //yyyymmdd_hhnnssuuuuuu
	  ASCII_COMPACT_REF_MICROSEC        = 38,	 //RRR=yyyymmdd_hhnnssuuuuuu

	  ASCII_ENVI					    = 39,	 //dd-mmm-yyyy hh:nn:ss
	  ASCII_ENVI_REF				    = 40,	 //RRR=dd-mmm-yyyy hh:nn:ss
	  ASCII_ENVI_MICROSEC				= 41,	 //dd-mmm-yyyy hh:nn:ss.uuuuuu
	  ASCII_ENVI_REF_MICROSEC			= 42,	 //RRR=dd-mmm-yyyy hh:nn:ss.uuuuuu

	  ASCII_CCSDSA					    = 43,	 //yyyy-mm-ddThh:nn:ss
	  ASCII_CCSDSA_REF  				= 44,	 //RRR=yyyy-mm-ddThh:nn:ss
	  ASCII_CCSDSA_MICROSEC 			= 45,	 //yyyy-mm-ddThh:nn:ss.uuuuuu
	  ASCII_CCSDSA_REF_MICROSEC			= 46,	 //RRR=yyyy-mm-ddThh:nn:ss.uuuuuu
	  ASCII_CCSDSA_COMPACT  			= 47,	 //yyyymmddThhnss
	  ASCII_CCSDSA_COMPACT_REF			= 48,	 //RRR=yyyymmddThhnnss
	  ASCII_CCSDSA_COMPACT_MICROSEC     = 49,	 //yyyymmddThhnnssuuuuuu
	  ASCII_CCSDSA_COMPACT_REF_MICROSEC = 50,	 //RRR=yyyymmddThhnnssuuuuuu

	  ASCII_COSMO_NANOSEC			   	= 51,    //yyyy-mm-dd hh:mm:ss.nnnnnnnnn
	  ASCII_COSMO_MICROSEC			   	= 52,    //yyyy-mm-dd hh:mm:ss.nnnnnn
	  ASCII_COSMO_MILLISEC			   	= 53,    //yyyy-mm-dd hh:mm:ss.nnn
	  ASCII_COSMO_SEC				    = 54,    //yyyy-mm-dd hh:mm:ss
	  ASCII_COSMO_COMPACT				= 55,    //yyyymmddhhmmss

	  ASCII_ERS_MILLISEC 				= 61	 //dd-mmm-yyyy_hh:nn:ss.uuu	


	} ;
    

	enum TimeReferenceDefines
	{

	  TAI	= 1,
	  UTC	= 2,
	  UT1	= 3,
	  GPS	= 4

	} ;

	struct TimeConv // PRQA S 2173, 2175
	{

		TimeConv() :
			 ref(),
			 format(),
 			 timeS(),
			 timeD(),
             timeI(3) //PRQA S 4400
        {}
        
		~TimeConv() throw() {}
		TimeConv(const TimeConv &r ) :
			 ref(r.ref),
			 format(r.format),
 			 timeS(r.timeS),
			 timeD(r.timeD),
             timeI(r.timeI)
        {}
		TimeConv &operator=(const TimeConv &r )
        {
            if (this != &r) {
                ref = r.ref ;
                format = r.format ;
                timeS = r.timeS ;
                timeD = r.timeD ;
                timeI = r.timeI ;
            }
            return *this ;
        }
        
		int ref; // PRQA S 2100 7
		int format;
		std::string timeS;
		long double timeD;
		std::vector<long> timeI ; // always 3 !!!
	};

	/*
	 * \brief Given a TimeConv structure, 
	 * converts from input reference&format 
	 * to output reference&format 
	 * and fill as output a TimeConv structure 
	 */
	void convertTime(const TimeConv&, TimeConv&) const ;	

    static std::istream & read(std::istream &, TimeConv &) ; // PRQA S 2502
       
	std::vector < std::pair<std::string, ssize_t> > getLeapSecondTimeDifferences() const ;    /* AS: JD50, total leap seconds to date */

private:

	ACSTimeConv(const ACSTimeConv & ); // not implemented
	ACSTimeConv &operator=(const ACSTimeConv &);

    long double toJD50(const ACSTimeConv::TimeConv& in) const ;
    void convertTimeInternal(long double, TimeConv& out) const ;
	//methods

    std::string toAsciiCompact(long double, TimeReferenceDefines, bool, unsigned short) const ;
    std::string toAsciiStd(long double, TimeReferenceDefines, bool, unsigned short) const ;
    std::string toAsciiEnvi(long double, TimeReferenceDefines, bool, unsigned short) const ;
    std::string toCcsdsa(long double, TimeReferenceDefines, bool, unsigned short) const ;
    std::string toCcsdsaCompact(long double, TimeReferenceDefines, bool, unsigned short) const ;
    std::string toCosmo(long double, bool, unsigned short) const ;
    std::string toAsciiErsMillisec(long double) const ;

	long double fromTransportTime(const std::vector<long> &, bool secs) const ; // secs? (days sec usec) : (days msec usec)
	long double fromAscii(const std::string &) const ;

	void toTransportTimeDSU(long double, std::vector<long> &) const ;
	void convert_StandardFormat_TransportTimeDSU(const std::string&,  long &,  long &,  long &) const ; // MP: remove
	void toTransportTimeDMU(long double, std::vector<long> &) const ;

    /*- value is in secs */
	long double convert_s(TimeReferenceDefines, TimeReferenceDefines, long double) const ;
    /*- value is in days */
    long double convert(TimeReferenceDefines in, TimeReferenceDefines out, long double v) const ;

	void loadConfiguration();

private:		

	//attributes
#ifdef TEST
public:
#else
private:
#endif
	struct TimeDifferences // PRQA S 2173, 2175
	{
		TimeDifferences() : ref(), start(), value() {}
		~TimeDifferences() throw() {}
		TimeDifferences(const TimeDifferences & r) : ref(r.ref), start(r.start), value(r.value) {}
		TimeDifferences &operator=(const TimeDifferences &r ) {
            if (this != &r) {
                ref = r.ref; start = r.start; value = r.value ;
            }
            return *this ;
        } 

		TimeReferenceDefines ref; // PRQA S 2100 3
		long double start;
		long double value;

	} ;
#ifdef TEST
private:
#endif

    TimeDifferences _tD ;
	std::vector<TimeDifferences> _timeDiff;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ACSTimeConv) ;

};
	
inline bool operator==(const ACSTimeConv::TimeConv &a, const ACSTimeConv::TimeConv &b) // PRQA S 2134
{
    return (a.ref == b.ref) &&
           (a.format == b.format) && 
           (a.timeS == b.timeS) && 
           (not (a.timeD < b.timeD) && (not (a.timeD > b.timeD))) && 
           (a.timeI == b.timeI) ;
}

inline bool operator!=(const ACSTimeConv::TimeConv &a, const ACSTimeConv::TimeConv &b)  // PRQA S 2134
{
    return not operator==(a,b) ;
}

std::istream & operator >> (std::istream &, ACSTimeConv::TimeConv &) ;

std::ostream & operator << (std::ostream &, const ACSTimeConv::TimeConv &) ;
exostream & operator << (exostream &, const ACSTimeConv::TimeConv &) ; // PRQA S 2072
_ACS_END_NAMESPACE

#endif //_ACSTimeConv_H_
