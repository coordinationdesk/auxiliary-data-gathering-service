/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DateTime Library $

	$Id$

	$Author$

	$Log$
	Revision 5.27  2016/12/06 14:20:58  francesco.avanzi
	zStringToDateTime changed in zuluUTCString2DateTime
	
	Revision 5.26  2016/12/06 12:45:25  francesco.avanzi
	CSGACQ-107: implemented zStringToDateTime() function
	
	Revision 5.25  2015/05/12 16:48:05  marpas
	UTCD::weekNumber and UTCD::dayOfTheWeek implemented (ISO)
	DateTime::dateOfWeek and DateTime::weekOfYear signature changed and reimplemented using new UTCD's methods above.
	Unitary tests upgraded and coverage checked.
	
	Revision 5.24  2015/05/11 16:44:53  francesco.avanzi
	Work In Progress on weekOfDay()
	
	Revision 5.23  2015/05/11 12:36:17  francesco.avanzi
	weekOfYear() enhanced.
	now can distinguish between 53rd week of the same year and 53rd week of prev year.
	
	Revision 5.22  2015/05/06 15:43:58  francesco.avanzi
	weekOfYear*() method enhanced. (no longer using DateTime(const string&) but DateTime(const UTDC&) instead
	
	Revision 5.21  2015/05/06 13:28:39  francesco.avanzi
	weekOfYear() method implemented.
	Such method returns the week of the year of a given DateTime (1 relative)
	by standard, the first week of a year is the one that contains year's first wednesday.
	
	Revision 5.20  2015/03/19 11:35:37  marpas
	accuracy and performances improved (even in negative cases).
	Test improved also.
	codind best practices applied
	qa warnings removed
	
	Revision 5.19  2015/03/18 18:15:49  marpas
	fixing inaccuracy when getting in truncate mode strings with 1 and 5 digits (e.g. 2015-03-11 10:50:35)
	
	Revision 5.18  2014/09/23 15:45:46  marfav
	Added rounding mode to each datetime to string function
	
	Revision 5.17  2014/09/09 14:29:05  marfav
	Fixed precision conversion
	Addedd support for reduced precision when converting JD to fraction
	as per S3PDGS-2000
	
	Revision 5.16  2014/09/05 14:42:49  marfav
	Fixed double to long conversion leading to one digit less value
	
	Revision 5.15  2014/06/06 14:06:43  marpas
	recognition of {UTC=}YYYY-Mmm-DD_HH:MM:SS{.[0-9]+}  done
	the date/time separator can be either '_' or ' '
	check of wrong hour/min/sec range done.
	test coverage improved
	
	Revision 5.14  2014/05/27 09:50:16  marpas
	qa warning fixed
	
	Revision 5.13  2014/05/27 08:17:28  marpas
	unfilled datetime import / export regression fixed and tested
	
	Revision 5.12  2014/04/10 09:41:53  marpas
	useless method removed
	
	Revision 5.11  2014/04/09 15:40:18  marpas
	recognition of "1965-10-29_23:59:58(.[0-9]+)", "1965/10/29T23:59:58(.[0-9]+)", "1965/10/29 23:59:58(.[0-9]+)" fixed
	
	Revision 5.10  2014/04/08 16:37:38  marpas
	Library completely rewritten - now perfo are up to 300x faster and accuracy is guaranteed to nanosecs.
	Interface rationalized
	coverage test rewritten
	performance test rewritten
	
	Revision 5.9  2014/04/01 18:00:54  marpas
	work in progress
	
	Revision 5.8  2014/04/01 10:55:02  marpas
	starting heavy refactorying, implementing speed and accuracy
	
	Revision 5.7  2014/03/28 17:35:31  marpas
    new static method Y2ns_to_std -> YYYYMMDDhhmmssnnnnnnnnn
	new static method jd50_to_Y2ns
	Y2ns_type a POD from Y to ns
	
	Revision 5.6  2014/03/24 18:47:31  marpas
	qa warning fixed
	
	Revision 5.5  2014/03/24 07:56:11  marpas
	operator + and operator + are not longer members of DateTime class
	
	Revision 5.4  2014/03/13 11:18:22  marpas
	to and from Gps week methods added (to was already present - new signature added).
	documentation updated
	test improved
	
	Revision 5.3  2014/02/13 12:15:14  marpas
	support of 19651029T235958 format
	adopting ACS_THROW and ACS_COND_THROW macros
	test improved
	
	Revision 5.2  2013/09/13 10:36:42  marpas
	DateTime supports "03 June 2013" format for input strings
	
	Revision 5.1  2013/07/23 12:08:58  marpas
	interface rationalization
	performances improved
	fixed timezone bug
	test imaproved
	
	Revision 5.0  2013/07/01 15:28:51  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.40  2013/05/17 16:16:01  marpas
	stream insertion operators for DateTime
	
	Revision 2.39  2012/12/02 20:04:10  marpas
	qa rules
	
	Revision 2.38  2012/10/04 13:28:01  lucio.pulvirenti
	Datetime2timespec method added returning a filled timespec struct. Imported from PDS2 environment.
	
	Revision 2.37  2012/02/09 13:31:36  marpas
	refactoring
	
	Revision 2.36  2012/02/08 09:52:54  marpas
	refactoring
	
	Revision 2.35  2011/09/29 14:57:06  marpas
	getting rid of useless template (causing codecheck crash)
	
	Revision 2.34  2011/01/20 10:34:00  marpas
	boost date_time greater than 1.33 implements ptime and date default ctors in different way (they are defaulted to invalid).
	This is avoided using DATE_TIME_NO_DEFAULT_CONSTRUCTOR if the boost version is greater than 1.33
	The old libDateTime implementation was based on default ctors, this one do not longer rely on those.
	
	Revision 2.33  2010/12/20 18:28:11  marpas
	more exception management added
	
	Revision 2.32  2010/12/20 16:10:03  marpas
	*** empty log message ***
	
	Revision 2.31  2010/10/18 12:29:55  marpas
	boost version checked to deal with boost exception introduced in version 2.36
	
	Revision 2.30  2010/09/02 12:19:26  marpas
	exception added
	
	Revision 2.29  2010/09/02 12:15:20  marpas
	DateTime::num2Month added (test improved)
	
	Revision 2.28  2010/05/05 10:19:06  marpas
	DEBUG added
	
	Revision 2.27  2009/10/16 08:37:23  marpas
	rounding problem fixed
	
	Revision 2.26  2009/05/14 08:47:34  marpas
	DateTime::operator+(const JD50 &) and DateTime::operator-(const JD50 &)  implemented
	
	Revision 2.25  2009/05/12 10:35:11  marpas
	int DateTime::compare(const DateTime &) const method implemented and tested
	
	Revision 2.24  2009/03/23 13:55:09  marpas
	JD70 class added
	rule violations fixed
	
	Revision 2.23  2009/03/04 12:18:08  marpas
	new CODECHECK instrumentation - no regressions
	
	Revision 2.22  2009/03/02 14:02:54  marpas
	CODECHECK ifndef - no regression
	
	Revision 2.21  2008/03/28 16:24:55  enrcar
	EC:: Static method dayAndMonthFromDayOfYear renamed as dayAndMonthFromDOY. Added method: dayAndMonthFromDayOfYearZR
	
	Revision 2.20  2008/02/05 16:46:53  enrcar
	EC:: dayAndMonthFromDayOfYear static method added
	
	Revision 2.19  2007/10/03 09:15:14  danalt
	used integer logic in fraction method
	
	Revision 2.18  2006/12/21 14:25:58  marpas
	millisec() and microsec() methods now returns always a number less than 1000 and 1000000 respectively despite the rounding mode
	
	Revision 2.17  2006/11/24 08:59:08  davcas
	Built double from long in log10() call to fix anomaly in c++ compiler of RH 3.0
	
	Revision 2.16  2006/11/22 11:18:19  marpas
	rounding problems aproximating uniots of secods fixed
	now the .9+ fraction ciphers are never rounded to the unit despite the rounding mode set
	
	Revision 2.15  2006/11/03 00:30:24  marpas
	comment changed to avoid compilation errors (thanks to Quality control imposing no ifdef to insulate the header)
	
	Revision 2.14  2006/11/03 00:24:09  marpas
	Rounding mode for SQL representation and conversions added (setRoundingMode & getRoundingMode)
	enum RoundingMode {Rint (0), Round (1 default), Trunc ( 2) } ; rules the conversion.
	setDefaultFractionDigits & getDefaultFractionDigits are used for all conversions to string but the ones devoted
	to SQL representation. setDefaultDbFractionDigits & getDefaultDbFractionDigits should be used instead.
	
	Revision 2.13  2006/10/16 10:52:07  marpas
	some make up
	documentation added
	
	Revision 2.12  2006/09/19 10:37:19  crivig
	UTC=19651029T235958000000 format handled
	
	Revision 2.11  2006/07/13 13:30:55  marpas
	utcs(...) methods interface now has a defaulted boolean to print or not fractional part.
	default behaviour is to print it.
	
	Revision 2.10  2006/07/13 12:40:30  marpas
	jd50 method precision improved
	
	Revision 2.9  2006/07/13 09:36:02  marpas
	getPtime() method implemented
	
	Revision 2.8  2006/07/12 09:40:29  marpas
	default ctor behaviour fixed
	
	Revision 2.7  2006/07/03 14:36:39  marpas
	UTC=.... format managed and tested
	
	Revision 2.6  2006/06/26 10:23:28  marpas
	another compilation error ... sorry
	
	Revision 2.5  2006/06/26 10:22:06  marpas
	compilation error fixed
	
	Revision 2.4  2006/06/26 10:20:39  marpas
	digits managment improved
	
	Revision 2.3  2006/06/26 10:07:50  marpas
	fractional digits management added
	
	Revision 2.2  2006/06/22 14:27:39  marpas
	Temporary version
	Uses boost::date_time
	allows microsecs management
	interface slightly changed
	
	Revision 2.1  2006/04/11 18:29:11  paoscu
	operator=(const string &utcs) enhanced.
	
	Revision 2.0  2006/02/28 08:40:49  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.28  2005/11/17 18:11:53  marpas
	thread safety improved
	time_t ctor now restitutes 0 msec (as it should be)
	
	Revision 1.27  2005/11/15 11:31:01  marpas
	Datetime2time_t method fixed in order to not use TZ
	
	Revision 1.26  2005/11/14 13:37:26  paoscu
	DateTime2tm method fixed: DST is always considered not used.
	
	Revision 1.25  2005/11/07 13:28:39  enrcar
	Alternative date format for DBFORMAT added
	
	Revision 1.24  2005/07/01 09:05:06  marfav
	DateTime operator equal takes in account the _filled flag too
	
	Revision 1.23  2005/03/03 14:11:02  marpas
	long double support
	
	Revision 1.22  2004/11/26 11:48:06  marpas
	DateTime(JD) ctor was bugged - fixed
	
	Revision 1.21  2004/08/30 09:36:11  marpas
	MJD bug fixed
	
	Revision 1.20  2004/08/30 09:14:07  marpas
	MJD class introduced
	
	Revision 1.19  2004/08/30 08:27:21  marpas
	MJD class name changed in MJD2000
	
	Revision 1.18  2004/08/27 15:43:41  marpas
	UTCD::daysOfMonth has an extra parameters to get correct number of days even for leap years.
	UTCD::dayOfMonth method completely rewritten and tested
	UTCD::dayOfYear method completely rewritten and tested
	
	Revision 1.17  2004/07/28 07:03:08  enrcar
	EC: Added DateTime2gpsWeek() method.
	
	Revision 1.16  2003/10/21 16:58:58  paoscu
	"operator tm() const" and "operator time_t() const" replaced by
	"tm DateTime2tm() const" and "time_t Datetime2time_t() const"
	
	Revision 1.15  2003/10/21 15:46:07  paoscu
	"operator tm() const" and "operator time_t() const" added.
	dayOfTheWeek now uses "operator tm() const".
	
	Revision 1.14  2003/09/03 13:55:03  paoscu
	Export methods are now const.
	
	Revision 1.13  2003/07/11 13:03:48  marpas
	exException::clone implemented
	
	Revision 1.12  2003/06/09 17:09:48  marpas
	New ctor added
	
	Revision 1.11  2003/04/15 18:16:57  marpas
	GCC3.2
	
	Revision 1.10  2003/03/10 10:53:48  paoscu
	dayOfTheWeek() introduced.
	
	Revision 1.9  2003/02/19 16:54:03  marpas
	comment fixed
	
	Revision 1.8  2003/02/19 16:41:37  marpas
	string method data changed into c_str
	
	Revision 1.7  2003/02/11 16:50:42  paoscu
	DB_FORMAT changed in "%04d-%02d-%02d %02d:%02d:%02d..%03d" to support libDbClient version 1.11
	
	Revision 1.6  2003/01/31 14:57:24  paoscu
	MJD2000 class added
	
	Revision 1.5  2003/01/30 11:24:07  paoscu
	ISO8601 time format supported in DateTime::operator=(const string &utcs).
	Export methods now write time in ISO8601
	
	Revision 1.4  2002/12/16 12:17:47  paoscu
	operators += and -= .
	
	Revision 1.3  2002/11/29 17:38:11  danalt
	exportXML added
	
	Revision 1.2  2002/11/28 17:30:47  paoscu
	A bug in exportObj has been fixed.
	
	Revision 1.1.1.1  2002/11/27 17:17:19  paoscu
	Import libDateTime
		

*/ 


#include <DateTime.h>
#include <Filterables.h>
#include <exException.h>
#include <rsPushNSpace.h>
#include <rsResourceSet.h>
#include <XMLOstream.h>
#include <StringUtils.h>

#include <boost/date_time/gregorian/gregorian.hpp> // PRQA S 1013

#if BOOST_VERSION > 103399 
	#define DATE_TIME_NO_DEFAULT_CONSTRUCTOR
#endif

#include <sstream>
#include <sys/time.h>       // PRQA S 1013

// library version 
#include <dtV.h>


_ACS_BEGIN_NAMESPACE(acs)

namespace {
    // to keep embedded version info
    const dtV version ; // NOSONAR
}

using std::string ;
using std::pair ;
using std::make_pair ;

const long double DateTime::JD::_jd2jd50_OFFSET = 2433282.5L ;
const long double DateTime::MJD2000::_jd502mjd2000_OFFSET = 18262.0L ;
const long double DateTime::MJD::_jd502mjd_OFFSET = 33282.0L ;
const long double DateTime::JD70::_jd502jd70_OFFSET = 7305.0L ;

//
//
//	JD50 class
//
//


void DateTime::JD50::exportObj(rsResourceSet &s) const
{
	rsPushNSpace pns(s,"JD50") ;
	s.setValue("_jd",_jd) ;
}

void DateTime::JD50::importObj(rsResourceSet &s)
{
	rsPushNSpace pns(s,"JD50") ;
	s.getValue("_jd",_jd) ;
}

void DateTime::JD50::exportXML(XMLOstream &xst) const
{
	std::ostringstream os;
	XMLOstream::Tag txml(xst, "JD50") ;
	os << std::setiosflags(std::ios::fixed) << std::setprecision(17) << _jd ; // PRQA S 4400
	XMLOstream::Tag txml2(xst, "_jd", os.str()) ;
}


//
//
//	JD70 class
//
//


void DateTime::JD70::exportObj(rsResourceSet &s) const
{
	rsPushNSpace pns(s,"JD70") ;
	s.setValue("_jd",_jd) ;
}

void DateTime::JD70::importObj(rsResourceSet &s)
{
	rsPushNSpace pns(s,"JD70") ;
	s.getValue("_jd",_jd) ;
}

void DateTime::JD70::exportXML(XMLOstream &xst) const
{
	std::ostringstream os;
	XMLOstream::Tag txml(xst, "JD70") ;
	os << std::setiosflags(std::ios::fixed) << std::setprecision(17) << _jd ; // PRQA S 4400
	XMLOstream::Tag txml2(xst, "_jd", os.str()) ;
}


//
//
//	MJD2000 class
//
//


void DateTime::MJD2000::exportObj(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"MJD2000") ;
	s.setValue("_jd",_jd) ;
}

void DateTime::MJD2000::importObj(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"MJD2000") ;
	s.getValue("_jd",_jd) ;
}

//
//
//	MJD class
//
//


void DateTime::MJD::exportObj(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"MJD") ;
	s.setValue("_jd",_jd) ;
}

void DateTime::MJD::importObj(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"MJD") ;
	s.getValue("_jd",_jd) ;
}


//
//
//	UTCD class
//
//

void DateTime::UTCD::nanosec(unsigned long m)
{
	ACS_COND_THROW(m > 999999999,InvalidTimeException("UTCD::nanosec(nsec) cannot accept inputs greater than 999999999")) ; // PRQA S 3081
	frac = m ;
	_ndigits = 9 ; // PRQA S 4400
}

void DateTime::UTCD::microsec(unsigned long m)
{
	ACS_COND_THROW(m > 999999,InvalidTimeException("UTCD::microsec(usec) cannot accept inputs greater than 999999")) ; // PRQA S 3081
	frac = m ;
	_ndigits = 6 ; // PRQA S 4400
}

void DateTime::UTCD::millisec(unsigned long m)
{
	ACS_COND_THROW(m > 999,InvalidTimeException("UTCD::millisec(msec) cannot accept inputs greater than 999")) ; // PRQA S 3081
	frac = m ;
	_ndigits = 3 ; // PRQA S 4400
}

unsigned short DateTime::UTCD::daysOfMonth(unsigned short m, unsigned short year)
{
	boost::gregorian::date d(year,m,1) ;
	return d.end_of_month().day().as_number() ;
}




bool DateTime::UTCD::isValid(bool genException) const // PRQA S 4020
{
	// check if the structure is valid
	// emit an exception upon the argument passed
	try {
        // PRQA S 3000, 3010, 3081 6
		boost::posix_time::ptime pt(boost::gregorian::date(year,month,day),
									boost::posix_time::hours(hour)+
									boost::posix_time::minutes(min)+
									boost::posix_time::seconds(sec)+
#if defined(BOOST_DATE_TIME_HAS_NANOSECONDS)
									boost::posix_time::nanoseconds(nanosec())) ; // PRQA S 3000, 3010, 3081
#else
#warning BOOST does not defines nanoseconds
									boost::posix_time::microseconds(microsec())) ; // PRQA S 3000, 3010, 3081
#endif                        
        if (pt.is_special()) {
            ACS_THROW(InvalidTimeException() << " invalid time: " << year << "-" << month << "-" << day << ":" <<  hour << ":" << min << ":" << sec << "." << frac) ; // PRQA S 3081
        }

		return true ;
	}
	catch(const std::exception &x) { // NOSONAR boost any exception 
		// here the check if it is the case to throw an exception o just return a boolean
		if (genException) {
        	ACS_THROW(InvalidTimeException(x) << " invalid time: " << year << "-" << month << "-" << day << ":" <<  hour << ":" << min << ":" << sec << "." << frac) ;
		}
		return false ;
	}
}

unsigned short DateTime::UTCD::dayOfTheWeek(unsigned short day, unsigned short month, unsigned short year)
{
	unsigned short wd = boost::gregorian::date(year,month,day).day_of_week().as_number() ;
    return (wd > 0) ? wd : 7 ; // PRQA S 3000, 3010, 3382, 4400
}

int DateTime::UTCD::dayOfYear(unsigned short day, unsigned short month, unsigned short year)
{
	return boost::gregorian::date(year,month,day).day_of_year() ; // PRQA S 3000, 3050
}

int DateTime::UTCD::weekNumber(unsigned short day, unsigned short month, unsigned short year) 
{
	return boost::gregorian::date(year,month,day).week_number() ; // PRQA S 3000, 3050
}

void DateTime::UTCD::dayAndMonthFromDayOfYearZR(unsigned short dayOfYear,	/* IN: DELTA, i.e. 0-relative -- so 0 means Jan 1st, 1 means Jan 2nd and so on... */  // PRQA S 2017
                                                unsigned short& year, 		/* IN/OUT: year (e.g. 1987) */
                                                unsigned short& day,		/* OUT: day of month. Of course, 1-rel: 1, 2 .. 31 */
                                                unsigned short& month)		/* OUT: month. Of course, 1-rel: 1, 2 .. 12 */
{
// NB: there no guarantee that the date falls in the given year 
	return dayAndMonthFromDOY(dayOfYear+1, year, day, month) ; // PRQA S 3000, 3010
} 

void DateTime::UTCD::dayAndMonthFromDOY(unsigned short dayOfYear,	/* IN: DOY: 1-relative -- so 1 means Jan 1st, 2 means Jan 2nd and so on... */  // PRQA S 2017
                                        unsigned short& year, 		/* IN/OUT: year (e.g. 1987) */
                                        unsigned short& day,		/* OUT: day of month. Of course, 1-rel: 1, 2 .. 31 */
                                        unsigned short& month)		/* OUT: month. Of course, 1-rel: 1, 2 .. 12 */
{
// NB: there no guarantee that the date falls in the given year 
	boost::gregorian::date d(year,1,1) ;
	d += boost::gregorian::days(dayOfYear-1) ;	/* since input is DOY / 1-relative */ // PRQA S 3000, 3081
    day = d.day().as_number() ;
    month = d.month().as_number() ;
    year = d.year() ; // PRQA S 3050
} 

DateTime::UTCD DateTime::UTCD::dayOfMonth(unsigned short dayofyear,       /* DOY: INPUT IS 1-relative, so: 0 undefined, 1 means jan 1st and so on */ 
										  unsigned short year)
{
	boost::gregorian::date d(year,1,1) ;
	d += boost::gregorian::days(dayofyear-1) ;	/* since input is DOY / 1-relative */ // PRQA S 3000, 3081
	unsigned short daym = d.day().as_number() ;
	unsigned short month = d.month().as_number() ;
	
	return DateTime::UTCD(year,month,daym) ;
}


long double DateTime::UTCD::jd50() const 
{
    isValid(true) ;
    using namespace boost::gregorian ;
    const date_duration &dd = date(year,month,day) - date(1950,1,1) ;
    // use all long double constants, do not change !
    return dd.days() + hour/24.0L + min/1440.0L + sec/86400.0L + frac / (86400.0L * exp10l(_ndigits)) ; // PRQA S 3000, 3011, 3084, 4400
}                  



//
//
//	DateTime class
//
//
ACS_CLASS_DEFINE_DEBUG_LEVEL(DateTime) 

unsigned short DateTime::_defaultFracDgts = 3 ; // PRQA S 4401 3
unsigned short DateTime::_defaultDbFracDgts = 3 ;

unsigned long DateTime::convertDigits (unsigned long frac, unsigned short digits, unsigned short wanted)
{
    // do not move in global where its initialization is not guaranteed
    static std::array<unsigned long, 10> exp10_v = {
                                                   1,
                                                   10,
                                                   100,
                                                   1000,
                                                   10000,
                                                   100000,
                                                   1000000,
                                                   10000000,
                                                   100000000,
                                                   1000000000
                                                 } ;

	unsigned long result=0;
    if (digits >= wanted)
    {
        result = ( frac / exp10_v[digits - wanted] ); // PRQA S 3000, 3084
    }
    else
    {
        result = ( frac * exp10_v[wanted - digits] ); // PRQA S 3000, 3084, 3081, 3011
    }
    
    return result;
}


const string &DateTime::unfilledDate() noexcept
{
    static string unf ="not-filled" ;
    return unf ;
}

unsigned short DateTime::month2Num(const string &month) // NOSONAR 
{    
// computed complexity is high, nevertheless it is a trivial
// implementation of a trie walking to a 3-char string representing 
// english months (Jan ... Dec)
    ACS_COND_THROW(month.length() != 3, exIllegalValueException("Unrecognized month: " + month)) ; // PRQA S 3081
    switch(month[0]) {
    case 'J': // 1, 6, 7
    case 'j': 
        switch(month[1]) {
        case 'A': // 1
        case 'a': 
            if (toupper(month[2]) == 'N') { // 1 - found // PRQA S 4400 L1
                return 1 ;
            }
            ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
            
        case 'U':
        case 'u': // 6,7 
            switch(month[2]) {
            case 'N':
            case 'n': // 6 - found
                return 6 ;
            case 'L':
            case 'l': // 7 - found
                return 7 ;
            default:
                ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
            }
            
        default:
            ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
        }
        
    case 'F': // 2
    case 'f':
        if ((toupper(month[1]) == 'E') && (toupper(month[2]) == 'B')) {
            return 2 ;
        }
        ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
        
    case 'M': // 3, 5
    case 'm':
        if (toupper(month[1]) == 'A' ) {
            switch(month[2]) {
            case 'R':
            case 'r': // 3 - found
                return 3 ;
            case 'Y':
            case 'y': // 5 - found
                return 5 ;
            default:
                ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
            }
        }
        ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
        
    case 'A': // 4, 8 
    case 'a':    
        switch(month[1]) {
        case 'P': // 
        case 'p': 
            if (toupper(month[2]) == 'R') { // 4 - found
                return 4 ;
            }
            ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
        case 'U': // 
        case 'u': 
            if (toupper(month[2]) == 'G') { // 8 - found
                return 8 ;
            }
            ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
        default:
            ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
        }
        
    case 'S': // 9
    case 's':
        if ((toupper(month[1]) == 'E') && (toupper(month[2]) == 'P')) {
            return 9 ;
        }
        ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
    case 'O': // 10
    case 'o':
        if ((toupper(month[1]) == 'C') && (toupper(month[2]) == 'T')) {
            return 10 ;
        }
        ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
        
    case 'N': // 11
    case 'n':
        if ((toupper(month[1]) == 'O') && (toupper(month[2]) == 'V')) {
            return 11 ;
        }
        ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
        
    case 'D': // 12
    case 'd':
        if ((toupper(month[1]) == 'E') && (toupper(month[2]) == 'C')) {
            return 12 ; // PRQA L:L1
        }
        ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
        
    default:
        ACS_THROW(exIllegalValueException("Unrecognized month: " + month)) ;
    }
}



const string &DateTime::num2Month(int m)
{
    static std::array<string,12> months = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    if ((m<1) || (m>12)) { // PRQA S 4400
        ACS_THROW(InvalidTimeException() << "Cannot convert int(" << m << ") to month") // PRQA S 3081
    }
    return months[m-1] ;
}

unsigned short DateTime::dayOfTheWeek() const
{
	unsigned short wd = jd50_to_ptime(jd50_).date().day_of_week().as_number() ;
    return (wd > 0) ? wd : 7 ; // PRQA S 3000, 3010, 3382, 4400
}

pair<unsigned short, unsigned int> DateTime::weekOfYear() const // PRQA S 4020
{
    DateTime::UTCD utcd;
    toUTCD(utcd) ; 
    int woy = utcd.weekNumber() ; // PRQA S 3000, 3010
    switch(woy) { // PRQA S 3000
    case 1:
        if (utcd.month == 12) { // PRQA S 3000, 4400 2
            return make_pair(woy, utcd.year+1) ; // week #1 could be from previous year
        }
        else {
            return make_pair(woy, utcd.year) ;    
        }
    case 52: // PRQA S 4402 2
    case 53:
        if (utcd.month == 1) { // PRQA S 3000 2
             return make_pair(woy, utcd.year-1) ; // week #52 or 53 (last) could be of next year
        }
        else {
            return make_pair(woy, utcd.year) ;    
        }
    default:
        return make_pair(woy, utcd.year) ;  
    }
}


DateTime DateTime::dateOfWeek(const pair<unsigned short, unsigned int> &p)  
{
    ACS_COND_THROW(p.first > 53, InvalidTimeException("Invalid week number (>53)")) // PRQA S 3000, 3081

    auto year = ushort(p.second) ; // PRQA S 3010
    int o = p.first * 7 + 1 - ( UTCD::dayOfTheWeek(4, 1, year) + 3 ) ; // PRQA S 3000, 3010, 4400
    int diy = UTCD::dayOfYear(31,12,year) ; // PRQA S 3010
    if (o < 1) {
        --year ;
        o += UTCD::dayOfYear(31, 12, year) ; // PRQA S 3010, 3084, 4400
    }
    else if (o > diy) {
        o -= diy ;
        ++year ;
    }
    else {
        // nothing
    }
    unsigned short day = 0 ;
    unsigned short month = 0 ;
    UTCD::dayAndMonthFromDOY(o, year, day, month); // NOSONAR

    return DateTime::UTCD(year, month, day, 0,0,0,0);

}
DateTime DateTime::zuluUTCString2DateTime(const std::string &pzd) 
{
    return ((* pzd.rbegin()  == 'Z') || (* pzd.rbegin() == 'z')) ? pzd.substr( 0 , pzd.size() - 1 ) : pzd ; // PRQA S 3084, 3383
}

namespace { // PRQA S 2134 L5

    inline bool is_underscore(char c) { return c == '_' ; }
    inline bool is_dash(char c) { return c == '-' ; }
    inline bool is_slash(char c) { return c == '/' ; }
    inline bool is_colon(char c) { return c == ':' ; }
    inline bool is_dot(char c) { return c == '.' ; }
    inline bool is_T(char c) { return c == 'T' ; }
    inline bool is_space(char c) { return c == ' ' ; }
    
    inline bool isDateSep(char c) { return is_dash(c) || is_slash(c) ; } 
    inline bool isDateTimeSep(char c) { return is_T(c) || is_underscore(c) || is_space(c) ; }
    inline bool isDateTimeSep2(char c) { return is_underscore(c) || is_space(c) ; }
    inline bool isTimeSep(char c) { return is_colon(c) ; }
    inline bool isFractionSep(char c) { return is_dot(c) ; }

    inline unsigned short toNum(const string &month) { // PRQA S 2134, 4020
        // decisional tree: maximize efficiency
        ACS_COND_THROW(month.empty(),exIllegalValueException("Empty month")) ; // PRQA S 3081
        ushort m = 0 ;
        bool g = false ;
        switch(month[0]) {
        case 'j':
        case 'J': // January, June, July
            if (month.length() < 4) { // PRQA S 4400 L5
                ACS_THROW(exIllegalValueException("Unrecognized month: \"" + month + "\"")) ; // PRQA S 3081
            }
            switch(month[3]) {
            case 'U': // janUary
            case 'u': 
                g = StringUtils::equalsNoCase(&month[1], "anuary") ;
                m = 1 ;
                break ;
            case 'E': // junE
            case 'e': 
                g = StringUtils::equalsNoCase(&month[1], "une") ;
                m = 6 ;
                break ;
            case 'Y': // julY
            case 'y': 
                g = StringUtils::equalsNoCase(&month[1], "uly") ;
                m = 7 ;
                break ;
            default:
                ACS_THROW(exIllegalValueException("Unrecognized month: \"" + month + "\"")) ; // PRQA S 3081
            }
            break ;

        case 'f':
        case 'F': // February
            g = StringUtils::equalsNoCase(&month[1], "ebruary") ;
            m = 2 ;
            break ;

        case 'm':
        case 'M': // March, May
            if (month.length() < 3) {
                ACS_THROW(exIllegalValueException("Unrecognized month: \"" + month + "\"")) ; // PRQA S 3081
            }
            if (toupper(month[2]) == 'R') {
                g = StringUtils::equalsNoCase(&month[1], "arch") ;
                m = 3 ;
            }
            else {
                g = StringUtils::equalsNoCase(&month[1], "ay") ;
                m = 5 ;
            }
            break ;                    

        case 'a':
        case 'A': // April, August
             if (toupper(month[1]) == 'P') {
                 g = StringUtils::equalsNoCase(&month[1], "pril") ;
                 m = 4 ;
             }
             else {
                 g = StringUtils::equalsNoCase(&month[1], "ugust") ;
                 m = 8 ;
             }
             break ;                    

        case 's':
        case 'S': // September
             g = StringUtils::equalsNoCase(&month[1], "eptember") ;
             m = 9 ;
             break ;

        case 'o':
        case 'O': // October
             g = StringUtils::equalsNoCase(&month[1], "ctober") ;
             m = 10 ;
             break ;

        case 'n':
        case 'N': // November
             g = StringUtils::equalsNoCase(&month[1], "ovember") ;
             m = 11 ;
             break ;

        case 'd':
        case 'D': // December
             g = StringUtils::equalsNoCase(&month[1], "ecember") ;
             m = 12 ;
             break ;
             
        default: 
            ACS_THROW(exIllegalValueException("Unrecognized month: \"" + month + "\"")) ; // PRQA S 3081
        }
        ACS_COND_THROW(not g,exIllegalValueException("Unrecognized month: \"" + month + "\"")) ; // PRQA S 3081
        return m ;
    }
    
    inline bool is_h(unsigned short h) { return h < 24 ; } // nb is unsigned // PRQA S 3000 3
    inline bool is_m(unsigned short m) { return m < 60 ; } // nb is unsigned
    inline bool is_s(unsigned short s) { return s < 60 ; } // nb is unsigned
    
    inline ushort strtoushort(const char *nptr, char **endptr, int base) 
    {
        return ushort(strtoul(nptr, endptr, base)) ;    
    }
    
    inline // 13 June 2013[ 23:59:58(.[0-9]+)] case 7
    DateTime::Y2ns_type from_D_month_Y_h_m_s_f(const string &s) { // PRQA S 4020
        DateTime::Y2ns_type y2ns ; // PRQA S 4102 L5
        char *end = nullptr ;
        const char *bgn = &s[0] ;
        y2ns.D = strtoushort(bgn, &end, 10) ; // PRQA S 3010, 4400 L5
        ACS_COND_THROW((end - bgn != 2) || not is_space(*end), exIllegalValueException("Unrecognized day or too short")) ; // PRQA S 3081
        size_t som = end-bgn+1 ; // PRQA S 3000 L5
        size_t eom = s.find(' ', end-bgn+1) ;
        y2ns.M = toNum(s.substr(som,eom-som)) ; // PRQA S 3084 L5
        bgn = &s[0] + eom +1 ;
        y2ns.Y = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 4), exIllegalValueException("Unrecognized year or too short")) ; // PRQA S 3081
        if (*end == 0) { // end
            y2ns.h = 0 ; 
            y2ns.m = 0 ; 
            y2ns.s = 0 ; 
            y2ns.ns = 0 ; 
            return y2ns;
        }
        ACS_COND_THROW(not is_space(*end), exIllegalValueException("Unrecognized date time separator")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.h = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not isTimeSep(*end) || not is_h(y2ns.h), exIllegalValueException("Unrecognized hour or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.m = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not isTimeSep(*end) || not is_m(y2ns.m), exIllegalValueException("Unrecognized minutes or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.s = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2)|| not is_s(y2ns.s), exIllegalValueException("Unrecognized seconds or too short")) ; // PRQA S 3081
        if (*end == 0) { // end
            y2ns.ns = 0 ; 
            return y2ns;
        }
        ACS_COND_THROW(not isFractionSep(*end), exIllegalValueException("Unrecognized seconds or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        unsigned long ns = strtoul(bgn, &end, 10) ;
        ssize_t nsl = end - bgn ;
        ACS_COND_THROW((nsl < 1) || (nsl > 9) || (*end != 0), exIllegalValueException("Unrecognized fraction or too long")) ; // PRQA S 3081
        
        y2ns.ns = DateTime::convertDigits (ns, nsl, 9);  // NOSONAR
        
        return y2ns ;
    }
    
          // 0....5....0....  5
    inline // 19651029T235958([0-9]+) used for cases 1,6
    DateTime::Y2ns_type from_YMD_hmsf(const string &s) { // PRQA S 4020
        DateTime::Y2ns_type y2ns ; // PRQA S 4102 L7
        char *end = nullptr ;
        y2ns.Y = strtoushort(s.substr(0,4).c_str(), &end, 10) ; // PRQA S 3010, 4400 L7
        ACS_COND_THROW(*end != 0, exIllegalValueException("Unrecognized year")) ; // PRQA S 3081 L7
        y2ns.M = strtoushort(s.substr(4,2).c_str(), &end, 10) ;
        ACS_COND_THROW(*end != 0, exIllegalValueException("Unrecognized month")) ;
        y2ns.D = strtoushort(s.substr(6,2).c_str(), &end, 10) ;
        ACS_COND_THROW(*end != 0, exIllegalValueException("Unrecognized day")) ;
        ACS_COND_THROW(not is_T(s[8]), exIllegalValueException("Invalid date separator")) ;
        y2ns.h = strtoushort(s.substr(9,2).c_str(), &end, 10) ; 
        ACS_COND_THROW((*end != 0) || not is_h(y2ns.h), exIllegalValueException("Unrecognized hours")) ;
        y2ns.m = strtoushort(s.substr(11,2).c_str(), &end, 10) ;
        ACS_COND_THROW((*end != 0) || not is_m(y2ns.m), exIllegalValueException("Unrecognized minutes")) ; 
        y2ns.s = strtoushort(s.substr(13,2).c_str(), &end, 10) ;
        ACS_COND_THROW((*end != 0) || not is_s(y2ns.s), exIllegalValueException("Unrecognized seconds")) ;
        ACS_COND_THROW(s[13] == 0 || s[14]== 0, exIllegalValueException("Unrecognized seconds")) ;
        if (s[15] == 0) { // end 
            y2ns.ns = 0 ;
            return y2ns ;
        } 
        ACS_COND_THROW(not isFractionSep(s[15]), exIllegalValueException("Unrecognized fraction separator")) ; // PRQA S 3081
        ACS_COND_THROW(s[16] == 0, exIllegalValueException("Unrecognized fraction")) ;

        const char * bgn = &s[0]+16 ;
        unsigned long ns = strtoul(bgn, &end, 10) ;
        ssize_t nsl = end - bgn ;
        ACS_COND_THROW((nsl < 1) || (nsl > 9) || (*end != 0), exIllegalValueException("Unrecognized fraction or too long")) ; // PRQA S 3081
        y2ns.ns = DateTime::convertDigits (ns, nsl, 9); // NOSONAR
        return y2ns ; // PRQA L:L7
    }

    inline // 1965-10-29T23:59:58(.[0-9]+)	cases 2,3,4,5
    DateTime::Y2ns_type from_Y_M_D_h_m_s_f(const string &s) { // PRQA S 4020
        DateTime::Y2ns_type y2ns ; // PRQA S 4102
        char *end = nullptr ;
        const char *bgn = &s[0] ;
        y2ns.Y = strtoushort(bgn, &end, 10) ; // PRQA S 3010, 4400 L5
        ACS_COND_THROW((end - bgn != 4) || not isDateSep(*end), exIllegalValueException("Unrecognized year or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.M = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not isDateSep(*end), exIllegalValueException("Unrecognized month or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.D = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2), exIllegalValueException("Unrecognized day")) ; // PRQA S 3081
        ACS_COND_THROW(not isDateTimeSep(*end), exIllegalValueException("Unrecognized date time separator")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.h = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not isTimeSep(*end) || not is_h(y2ns.h), exIllegalValueException("Unrecognized hour or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.m = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not isTimeSep(*end) || not is_m(y2ns.m), exIllegalValueException("Unrecognized minutes or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.s = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not is_s(y2ns.s), exIllegalValueException("Unrecognized seconds or too short")) ; // PRQA S 3081
        if (*end == 0) { // end
            y2ns.ns = 0 ; 
            return y2ns;
        }
        ACS_COND_THROW(not isFractionSep(*end), exIllegalValueException("Unrecognized seconds or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        unsigned long ns = strtoul(bgn, &end, 10) ;
        ssize_t nsl = end - bgn ;
        ACS_COND_THROW((nsl < 1) || (nsl > 9) || (*end != 0), exIllegalValueException("Unrecognized fraction or too long")) ; // PRQA S 3081
        y2ns.ns = DateTime::convertDigits (ns, nsl, 9); // NOSONAR

        return y2ns ;
    }
    
    inline // 29-Oct-1965_23:59:58(.[0-9]+) cases 8,9
    DateTime::Y2ns_type from_D_mon_Y_h_m_s_f(const string &s) { // PRQA S 4020
        DateTime::Y2ns_type y2ns ; // PRQA S 4102
        char *end = nullptr ;
        const char *bgn = &s[0] ;
        y2ns.D = strtoushort(bgn, &end, 10) ; // PRQA S 3010, 4400 L5
        ACS_COND_THROW((end - bgn != 2) || not is_dash(*end), exIllegalValueException("Unrecognized day or too short")) ; // PRQA S 3081
        y2ns.M = DateTime::month2Num(s.substr(3,3)) ; // PRQA S 3000 L5

        ACS_COND_THROW(not is_dash(s[6]), exIllegalValueException("Unrecognized month or too short")) ; // PRQA S 3081
        bgn = &s[6]+1 ; // after 2nd dash: YYYY
        y2ns.Y = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 4) || not isDateTimeSep2(*end), exIllegalValueException("Unrecognized year or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.h = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not isTimeSep(*end) || not is_h(y2ns.h), exIllegalValueException("Unrecognized hour or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.m = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not isTimeSep(*end) || not is_m(y2ns.m), exIllegalValueException("Unrecognized minutes or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.s = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not is_s(y2ns.s), exIllegalValueException("Unrecognized seconds or too short")) ; // PRQA S 3081
        if (*end == 0) { // end
            y2ns.ns = 0 ; 
            return y2ns;
        }
        ACS_COND_THROW(not isFractionSep(*end), exIllegalValueException("Unrecognized seconds or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        unsigned long ns = strtoul(bgn, &end, 10) ;
        ssize_t nsl = end - bgn ;
        ACS_COND_THROW((nsl < 1) || (nsl > 9) || (*end != 0), exIllegalValueException("Unrecognized fraction or too long")) ; // PRQA S 3081
        y2ns.ns = DateTime::convertDigits (ns, nsl, 9);  // NOSONAR

        return y2ns ;
    }

    inline // 1965-Oct-15_23:59:58(.[0-9]+) cases 13, 14
    DateTime::Y2ns_type from_Y_mon_D_h_m_s_f(const string &s) { // PRQA S 4020
        DateTime::Y2ns_type y2ns ; // PRQA S 4102
        char *end = nullptr ;
        const char *bgn = &s[0] ;
        y2ns.Y = strtoushort(bgn, &end, 10) ; // PRQA S 3010, 4400 L5
        ACS_COND_THROW((end - bgn != 4) || not is_dash(*end), exIllegalValueException("Unrecognized year or too short")) ; // PRQA S 3081
        y2ns.M = DateTime::month2Num(s.substr(5,3)) ; // PRQA S 3000 L5

        ACS_COND_THROW(not is_dash(s[8]), exIllegalValueException("Unrecognized month or too short")) ; // PRQA S 3081
        bgn = &s[8]+1 ;
        y2ns.D = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not isDateTimeSep2(*end), exIllegalValueException("Unrecognized day or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.h = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not isTimeSep(*end) || not is_h(y2ns.h), exIllegalValueException("Unrecognized hour or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.m = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not isTimeSep(*end) || not is_m(y2ns.m), exIllegalValueException("Unrecognized minutes or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        y2ns.s = strtoushort(bgn, &end, 10) ;
        ACS_COND_THROW((end - bgn != 2) || not is_s(y2ns.s), exIllegalValueException("Unrecognized seconds or too short")) ; // PRQA S 3081
        if (*end == 0) { // end
            y2ns.ns = 0 ; 
            return y2ns;
        }
        ACS_COND_THROW(not isFractionSep(*end), exIllegalValueException("Unrecognized seconds or too short")) ; // PRQA S 3081
        bgn = end+1 ;
        unsigned long ns = strtoul(bgn, &end, 10) ;
        ssize_t nsl = end - bgn ;
        ACS_COND_THROW((nsl < 1) || (nsl > 9) || (*end != 0), exIllegalValueException("Unrecognized fraction or too long")) ; // PRQA S 3081
        y2ns.ns = DateTime::convertDigits (ns, nsl, 9);  // NOSONAR

        return y2ns ;
    }
} // PRQA L:L5



DateTime &DateTime::operator=(const string &utcs) // PRQA S 4020
{
//  1) UTC=19651029T235958([0-9]+)	
//  2) UTC=1965-10-29T23:59:58(.[0-9]+)	
//  3) 1965-10-29T23:59:58(.[0-9]+) 
//  4) 1965-10-29 23:59:58(.[0-9]+)
//  5) 1965/10/29 23:59:58(.[0-9]+)	
//  6) 19651029T235958([0-9]+)
//  7) 13 June 2013 23:59:58(.[0-9]+) 
//  8) 29-Oct-1965_23:59:58(.[0-9]+)
//  9) 29-Oct-1965 23:59:58(.[0-9]+)
// 10) 1965-10-29_23:59:58(.[0-9]+) 
// 11) 1965/10/29T23:59:58(.[0-9]+)	
// 12) 1965/10/29 23:59:58(.[0-9]+)	
// 13) 2014-Jun-05_11:02:30(.[0-9]+)
// 14) UTC=2014-Jun-05_11:02:30(.[0-9]+)

	static const string allowedmsg = string(" was not recognized. Allowed formats are:\n") + // PRQA S 3081
                                   "29-Oct-1965[_ ]23:59:58\n" +
                                   "(UTC=)1965[-/]10[-/]29[T_]23:59:58\n" +
                                   "(UTC=)1965[-/]Jan[-/]29[_]23:59:58\n" +
		                           "UTC=19651029T235958\n" +
		                           "19651029T235958\n" +
		                           "Optionally followed by a fractional part as in UTC=1965-10-29_23:59:58.123456789" ;
    size_t len = utcs.length() ;
    ACS_COND_THROW(len < 10, InvalidTimeException("\"" + utcs + "\"" + allowedmsg)) ; // PRQA S 3081
    try {
        if (utcs.find("UTC=") == 0 ) { // case 1 or 2 or 14 
            if (utcs[8] == '-') { // case 2 or 14 // PRQA S 4400 L6
                if (isalpha(utcs[9]) != 0 ) { // case 14
	                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "case 14") ;
                    jd50_ = Y2ns_to_jd50(from_Y_mon_D_h_m_s_f(utcs.substr(4))) ; 
                    _filled = true ;
                    return *this ; 
                }
                else { // case 2 
	                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "case 2") ;
                    jd50_ = Y2ns_to_jd50(from_Y_M_D_h_m_s_f(utcs.substr(4))) ; 
                    _filled = true ;
                    return *this ; 
                }
            }
            else { // case 1 
	            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "case 1") ;
                jd50_ = Y2ns_to_jd50(from_YMD_hmsf(utcs.substr(4))) ; 
                _filled = true ;
                return *this ; 
            }
        }
        else if (utcs[4] == '-' ) { // case 3, 4, 10 or 13 
            if (isalpha(utcs[5]) != 0) { // case 13
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "13") ;
                jd50_ = Y2ns_to_jd50(from_Y_mon_D_h_m_s_f(utcs)) ; 
                _filled = true ;
                return *this ;
            }
            else {
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "case 3,4 or 10") ;
                jd50_ = Y2ns_to_jd50(from_Y_M_D_h_m_s_f(utcs)) ; 
                _filled = true ;
                return *this ;
            } 
        }
        else if (utcs[4] == '/' ) { // case 5, 11, 12
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "case 5, 11, 12") ;
            jd50_ = Y2ns_to_jd50(from_Y_M_D_h_m_s_f(utcs)) ; 
            _filled = true ;
            return *this ; 
        }
        else if (isalpha(utcs[3]) != 0) { // case 7 or 8 or 9 
            if (utcs[2] == '-' ) { // case 8 
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "case 8 or 9") ;
                jd50_ = Y2ns_to_jd50(from_D_mon_Y_h_m_s_f(utcs)) ;
                _filled = true ;
                return *this ; 
            }
            else {
                ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "case 7") ;
                jd50_ = Y2ns_to_jd50(from_D_month_Y_h_m_s_f(utcs)) ;
                _filled = true ;
                return *this ; 
            }
        }
        else if (isdigit(utcs[4]) != 0) { // case 6
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "case 6") ;
            jd50_ = Y2ns_to_jd50(from_YMD_hmsf(utcs)) ; 
            _filled = true ;
            return *this ; 
        }
        else {
            ACS_THROW(exIllegalValueException("Unrecognized date format")) ; // PRQA S 3081
        }
    }
    catch(const std::exception &x) {
        ACS_THROW(InvalidTimeException(x, "\"" + utcs + "\"" + allowedmsg)) ; // PRQA S 3081
    }
    
    // PRQA L:L6
    
}

long double DateTime::Y2ns_to_jd50(const Y2ns_type &y2ns) 
{
    using namespace boost::gregorian ;
    long days = (date(y2ns.Y, y2ns.M, y2ns.D) - date(1950,1,1)).days() ;
    
    // note use all long double contants: do not change
    return days + y2ns.h/24.0L + y2ns.m/1440.0L + y2ns.s/86400.0L + y2ns.ns / (86400.0L * 1000000000.0L) ; // PRQA S 3000, 3011, 3084, 4400
}

namespace {
    struct interval_day {
        long d ;
		unsigned short  h ;
		unsigned short  m ;
		unsigned short  s ;
		long double ps ; // nanosec
    } ;

    inline 
    interval_day fromAbsJD50(long double jd50) // PRQA S 2134
    {
        interval_day i ; // PRQA S 4102
        long double j = fabsl(jd50) ;
		i.d = long(j) ; // PRQA S 3011 L2
		long double fracdays = j - i.d ; 
		long double h = fracdays*24 ;
        i.h = ushort(h) ;
		long double m = (h - int(h)) * 60.0 ; // PRQA S 3081, 4400 L2
        i.m = ushort(m) ;
		long double s = (m - int(m)) * 60.0 ; 
        i.s = ushort(s) ;
        i.ps = ((s-int(s)) * 1e12L) ;
        return i ;
    }
}

// NB: this is a very tricky method: it deals with precision computation, 
// rounding and negative values
//
// eg: a jd50 value x and -x should be always symmetric wrt to the origin 
// as in (Jan 1, 1950 - midnight).
// so 1 ns after is just 1 ns after midnight, while 1 ns BEFORE (negative) is the last ns of Dec 31, 1949
DateTime::Y2ns_type DateTime::jd50_to_Y2ns(long double jd50, int digits, bool truncate) // NOSONAR
{ 
    // do not move in global namespace where its initialization is not guaranteed
    static std::array<unsigned long, 10> exp10_v = {
                                                  1,         // 0
                                                  10,        // 1
                                                  100,       // 2
                                                  1000,      // 3
                                                  10000,     // 4
                                                  100000,    // 5
                                                  1000000,   // 6
                                                  10000000,  // 7
                                                  100000000, // 8
                                                  1000000000 // 9
                                                } ;
    digits = std::max(0,digits) ; // take care of domain: digits >= 0
	try {
        interval_day i = fromAbsJD50(jd50) ;
        Y2ns_type c ; // PRQA S 4102
        if (jd50 >=0) 
        {
            c.ns = rintl(i.ps/1000.0L) ; // NOSONAR to integral type

            if (digits < 9) // take care of domain: digits < 9
            {
                if (truncate)
                {
                    c.ns = c.ns / exp10_v[9 - digits] ; // PRQA S 3084
                    // was c.ns = c.ns / truncl(exp10l(9 - digits) but generates a periodic number in binary
                    
                }
                else
                {
                    c.ns = rintl (c.ns / exp10l (9 - digits) ); // NOSONAR to integral type
                }
                c.ns = c.ns * exp10_v[9 - digits] ; // PRQA S 3084
            }
            
            bool f = false ;
            if (c.ns >= 1000000000) {
                c.ns = 0 ;
                f = true ;
            }
            c.s = i.s + (f ? 1 : 0) ; // PRQA S 3000, 3010, 3380 L2
            short c_m = 0 ;
            if (c.s > 59) { c.s = 0; c_m = 1 ; }
            c.m = ushort(i.m + c_m) ;
            short c_h = 0 ;
            if (c.m > 59) { c.m = 0; c_h = 1 ; }
            c.h = ushort(i.h + c_h) ;
            short c_d = 0 ;
            if (c.h > 23) { c.h = 0 ; c_d = 1 ; }
		    using namespace boost::gregorian ;
            const date& dt = date(1950,1,1)+date_duration(i.d+c_d) ;
            const date::ymd_type &ymd = dt.year_month_day() ;
            c.Y = ymd.year ; // PRQA S 3050 L2
            c.M = ymd.month ;
            c.D = ymd.day ;
        }
        else 
        { 
            // negative values
            bool f = i.ps > 0 ;
            long negns = rintl (i.ps / 1000L);  // NOSONAR to long

            // negative branch: truncation occurs in other direction (simmetry)
            // refer to comment in the beginning of the method.

            // PRQA S 3000, 3084 2
            if (digits < 9) // take care of domain: digits < 9 
            {
                // PRQA S 3000, 3084 2
                if (truncate)
                {
                    negns = negns / exp10_v[9 - digits] ; // PRQA S 3084
                    // was ns = truncl (ns / exp10l (9 - digits) ); // NOSONAR

                }
                else
                {
                    negns = rintl (negns / exp10l (9 - digits) );  // NOSONAR to long
                }
                negns = negns * exp10_v[9 - digits] ; // PRQA S 3084
            }

            // here is the simmetry
            long ns = 1000000000L - negns ;

            if (ns >= 1000000000) {
                ns = 0 ;
                f = false ;
            }
            short correct_s = 0 ;
            if (f ) { correct_s = 1 ; }
            long s = 60 - i.s - correct_s ; 
            short correct_m = 0 ;
            if (s > 59) {
                s = 0 ;
            }
            if (f || (s > 0)) { correct_m = 1 ; f = true ; }
            long m = 60 - i.m - correct_m ; 
            short correct_h = 0 ;
            if (m > 59) {
                m = 0 ; 
            }
            if ( f || (m > 0) ) { correct_h = 1 ; f = true ;  }
            long h = 24 - i.h - correct_h ; 
            short correct_d = 0 ;
            if (h > 23) {
                h = 0 ; 
            }
            if ( f || (h > 0)) { correct_d = 1 ;  }
		    using namespace boost::gregorian ;
            const date& dt = date(1950,1,1)-date_duration(i.d+correct_d) ;
            const date::ymd_type &ymd = dt.year_month_day() ;
            c.ns = ns ;
            c.s = ushort(s) ;
            c.m = ushort(m) ;
            c.h = ushort(h) ;
            c.Y = ymd.year ; // PRQA S 3050 L2
            c.M = ymd.month ;
            c.D = ymd.day ;
            
        }
        return c ;
        // PRQA L:L2
	}
	catch(const std::exception &x) {
		ACS_THROW(InvalidTimeException(x)<< "Invalid JD50: " << std::setprecision(12) << jd50 << " [JD50]" ) ; 
	}
}

DateTime::Y2ns_type DateTime::jd50_to_Y2D(long double jd50) 
{
	try {
		// assignement with a JD50
		// all other similar assignement use 
		// this one with a specific offset	
		auto dd = long(jd50) ; // keep the sign // PRQA S 3011
        if ((jd50 < 0) && (dd == 0)) { dd -=1 ; } // the day before
		using namespace boost::gregorian ;
        const date& dt = date(1950,1,1)+date_duration(dd) ; // PRQA S 3081
        const date::ymd_type &ymd = dt.year_month_day() ;
        
        Y2ns_type c ; // PRQA S 4102
        c.Y = ymd.year ; // PRQA S 3050 L3
        c.M = ymd.month ;
        c.D = ymd.day ;
        c.h = c.m = c.s = c.ns = 0 ; // PRQA S 3010
        return c ;
        // PRQA L:L3
	}
	catch(const std::exception &x) {
		ACS_THROW(InvalidTimeException(x) << "Invalid JD50: " << std::setprecision(12) << jd50 << " [JD50]") ;
	}
}

boost::posix_time::ptime DateTime::jd50_to_ptime(long double jd50) 
{
	try {
		// assignement with a JD50
		// all other similar assignement use 
		// this one with a specific offset	
		auto dd = int(jd50) ; 
		long double fracdays = jd50 - dd ; 

		long double h = fracdays*24 ;
		boost::posix_time::time_duration td= boost::posix_time::hours(int(h)) ; // PRQA S 3072, 3081
		long double m = (h - int(h)) * 60.0 ; // PRQA S 3081, 4400  L4
  		td += boost::posix_time::minutes(int(m)) ; // PRQA S 3081
		long double s = (m - int(m)) * 60.0 ; 
		td += boost::posix_time::seconds(int(s)) ; // PRQA S 3081
       
        auto nsecs = long((s-int(s)) * 1000000000 + 0.5) ;
        if (nsecs > 999999999) { nsecs = 0 ; }
        
#if defined(BOOST_DATE_TIME_HAS_NANOSECONDS)
		td += boost::posix_time::nanoseconds(nsecs) ; // PRQA S 3010, 3081
#else
#warning BOOST does not defines nanoseconds
		td += boost::posix_time::microseconds(nsecs/1000) ; // PRQA S 3010, 3081
#endif      
		using namespace boost::gregorian ;
		return boost::posix_time::ptime(date(1950,1,1)+date_duration(dd),td) ; // PRQA S 3081, 4400
        // PRQA L:L4                  
	}
	catch(const std::exception &x) {
		ACS_THROW(InvalidTimeException(x) << "Invalid JD50: " << std::setprecision(12) << jd50 << " [JD50]") ; 
	}
}

long double DateTime::ptime_to_jd50(const boost::posix_time::ptime &p) 
{
    const boost::posix_time::time_duration &t = p.time_of_day() ;
    return (p.date() - boost::gregorian::date(1950,1,1)).days() + t.total_nanoseconds() / 86400000000000.0L ; // PRQA S 3011, 4400
} 


// public

DateTime::DateTime() noexcept : 
    jd50_(ptime_to_jd50(boost::posix_time::microsec_clock::universal_time())), 
    _filled(true)
{
}

DateTime::DateTime(const boost::posix_time::ptime &pt) noexcept : 
    jd50_(ptime_to_jd50(pt)), 
    _filled(!pt.is_special())
{
}

bool DateTime::isOlder(time_t secs) const { return *this < DateTime(time(nullptr) - secs) ; } // PRQA S 3081

string &DateTime::utcs(string &s, bool frac, bool truncate) const 
{
	std::ostringstream os ;
	utcs(os, frac, truncate) ;
	s = os.str() ;
	return s ; // PRQA S 4028
}


void DateTime::toUTCD(UTCD &utcd) const 
{
    assertIsFilled() ;
	try {
		using namespace boost::gregorian ;
		using namespace boost::posix_time ;
        Y2ns_type y2ns = jd50_to_Y2ns(jd50_) ;

		utcd = UTCD (y2ns.Y,y2ns.M,y2ns.D,y2ns.h,y2ns.m,y2ns.s,y2ns.ns,9) ; // PRQA S 3000, 3010, 4400 
	}
	catch(const std::exception &x) {
		ACS_THROW(InvalidTimeException(x,"Can't convert to toUTCD")) ; // PRQA S 3081
	}
}




tm DateTime::DateTime2tm(long *ns) const
{
	try {
        Y2ns_type y2ns = jd50_to_Y2ns(jd50_) ;
		boost::posix_time::ptime pt(boost::gregorian::date(y2ns.Y,y2ns.M,y2ns.D),
									boost::posix_time::hours(y2ns.h)+ // PRQA S 3081 8
									boost::posix_time::minutes(y2ns.m)+
									boost::posix_time::seconds(y2ns.s)) ;
                                    
        // because tm has no fractions of seconds, there's no need to use them
        // the above computation stops at seconds resolution.
		struct tm tmStruct = boost::posix_time::to_tm(pt) ;

		if(::mktime(&tmStruct) == time_t(-1)) // PRQA S 3081
		{
			ACS_THROW(InvalidTimeException("Can't convert to tm")) ; // PRQA S 3081
		}
        if (ns) { *ns = y2ns.ns ; } // PRQA S 3000
		return tmStruct ;
	}
	catch(const std::exception &x) {
		ACS_THROW(InvalidTimeException(x,"Can't convert to tm")) ; // PRQA S 3081
	}
}


time_t DateTime::Datetime2time_t(long *ns) const
{
	struct tm tmStruct = DateTime2tm(ns) ;
	return 	::timegm(&tmStruct) ; // do not use mktime because it uses TZ variable
}

timespec DateTime::Datetime2timespec() const
{
    timespec ts ; // PRQA S 4102
    long ns = 0 ;
	ts.tv_sec  = Datetime2time_t(&ns);
	ts.tv_nsec = ns ; 
    return ts ;
}


DateTime DateTime::fromGpsWeek(unsigned short int w)
{
    boost::posix_time::ptime pt(boost::gregorian::date(1980,1,6)) ; // PRQA S 4400
    pt += boost::gregorian::days(w*7) ; // PRQA S 3000, 3081,  4400
    return DateTime(pt) ; // PRQA S 3081
}


void DateTime::exportObj(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"DateTime") ;
	s.setValue("time",isFilled() ? asISO8601(9) : unfilledDate()) ; // PRQA S 3380, 3385, 4400
	s.setValue("_filled",_filled) ;
}

void DateTime::importObj(rsResourceSet &s)
{
	string time;

	rsPushNSpace nsp(s,"DateTime") ;
	s.getValue("_filled",_filled) ;
    if (_filled) {
	    s.getValue("time",time) ;
        *this = time ;
	}
}

void DateTime::exportXML(XMLOstream &xst) const
{
	// exports in XML in the format ISO8601
	XMLOstream::Tag txml(xst,"DateTime") ;
	XMLOstream::Tag timeTag(xst, "time", isFilled() ? asISO8601(9) : unfilledDate() ) ; // PRQA S 3380, 3385, 4400
	timeTag.close();
	XMLOstream::Tag filledTag(xst, "_filled", (_filled ? "true" : "false")) ; // PRQA S 3380
	filledTag.close();
}



void DateTime::setDefaultFractionDigits(unsigned short dgts) 
{
	ACS_COND_THROW(dgts > 6, exIllegalValueException("Too many digits for DateTime representation: max is 6")) ; // PRQA S 3000, 3081
	_defaultFracDgts = dgts ;
}

unsigned short DateTime::getDefaultFractionDigits() 
{
	return _defaultFracDgts ;
}

void DateTime::setDefaultDbFractionDigits(unsigned short dgts) 
{
	ACS_COND_THROW(dgts > 6, exIllegalValueException("Too many digits for Db DateTime representation: max is 6")) ; // PRQA S 3000, 3081
	_defaultDbFracDgts = dgts ;
}

unsigned short DateTime::getDefaultDbFractionDigits() 
{
	return _defaultDbFracDgts ;
}

//
//
//	DateTime
//
//

std::istream &operator >> (std::istream &is, DateTime& d)
{
/* 2005-10-22 16:35:11.[0-9]+ */
	if (is.good()) {
        using std::locale ; 
		boost::posix_time::ptime pt(boost::posix_time::from_time_t(0)) ;
        // the facet just created with new is passed to a locale template ctor
        // as per stl doc, it is responsibility of the locale dtor to delete the facet
		auto *input_facet = new boost::local_time::local_time_input_facet() ; // NOSONAR
		locale oldloc = is.getloc() ;
        // here the input_facet is given to a localte template ctor
		is.imbue(locale(locale::classic(), input_facet)) ;
		input_facet->format("%Y-%m-%d %H:%M:%S%F") ;

		is >> pt ;
		is.imbue(oldloc) ;

		d = DateTime(pt) ; // PRQA S 3081
	}
	return is ;     // PRQA S 4028
}




exostream & operator << (exostream &os , const DateTime &d )  // PRQA S 4222
{
    os << d.asISO8601(9) ; // PRQA S 4400
    return os ; // PRQA S 4028
}

std::ostream & operator << (std::ostream &os ,const DateTime &d ) 
{
    os << d.asISO8601(9) ; // PRQA S 4400
    return os ; // PRQA S 4028
}

_ACS_END_NAMESPACE
