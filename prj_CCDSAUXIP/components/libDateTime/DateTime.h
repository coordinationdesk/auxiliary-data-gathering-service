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
	Revision 5.25  2016/12/06 12:45:25  francesco.avanzi
	CSGACQ-107: implemented zStringToDateTime() function
	
	Revision 5.24  2015/11/27 09:11:34  matteo.airoldi
	Bug un MJD fixed
	
	Revision 5.23  2015/05/19 10:31:17  francesco.avanzi
	few comments revised and corrected
	
	Revision 5.22  2015/05/12 16:48:05  marpas
	UTCD::weekNumber and UTCD::dayOfTheWeek implemented (ISO)
	DateTime::dateOfWeek and DateTime::weekOfYear signature changed and reimplemented using new UTCD's methods above.
	Unitary tests upgraded and coverage checked.
	
	Revision 5.21  2015/05/11 16:44:53  francesco.avanzi
	Work In Progress on weekOfDay()
	
	Revision 5.20  2015/05/11 12:36:17  francesco.avanzi
	weekOfYear() enhanced.
	now can distinguish between 53rd week of the same year and 53rd week of prev year.
	
	Revision 5.19  2015/05/06 13:28:39  francesco.avanzi
	weekOfYear() method implemented.
	Such method returns the week of the year of a given DateTime (1 relative)
	by standard, the first week of a year is the one that contains year's first wednesday.
	
	Revision 5.18  2014/11/14 13:14:21  marpas
	new method: std::string timeString(int fracdgts, bool truncate = true, char sephms=':') const ;
	signature changed: std::ostream & timeString(std::ostream &, int fracdgts, bool truncate = true, char sephms=':') const ;
	test coverage improved
	
	Revision 5.17  2014/09/23 15:45:46  marfav
	Added rounding mode to each datetime to string function
	
	Revision 5.16  2014/09/09 14:29:05  marfav
	Fixed precision conversion
	Addedd support for reduced precision when converting JD to fraction
	as per S3PDGS-2000
	
	Revision 5.15  2014/09/05 14:42:50  marfav
	Fixed double to long conversion leading to one digit less value
	
	Revision 5.14  2014/05/27 09:50:16  marpas
	qa warning fixed
	
	Revision 5.13  2014/05/27 08:17:28  marpas
	unfilled datetime import / export regression fixed and tested
	
	Revision 5.12  2014/04/10 09:41:53  marpas
	useless method removed
	
	Revision 5.11  2014/04/08 16:37:39  marpas
	Library completely rewritten - now perfo are up to 300x faster and accuracy is guaranteed to nanosecs.
	Interface rationalized
	coverage test rewritten
	performance test rewritten
	
	Revision 5.10  2014/04/01 18:00:54  marpas
	work in progress
	
	Revision 5.9  2014/04/01 10:55:02  marpas
	starting heavy refactorying, implementing speed and accuracy
	
	Revision 5.8  2014/03/28 17:35:31  marpas
	new static method Y2ns_to_std -> YYYYMMDDhhmmssnnnnnnnnn
	new static method jd50_to_Y2ns
	Y2ns_type a POD from Y to ns
	
	Revision 5.7  2014/03/28 11:26:23  marpas
	asISO8601 overloaded
	
	Revision 5.6  2014/03/24 18:47:31  marpas
	qa warning fixed
	
	Revision 5.5  2014/03/24 07:56:11  marpas
	operator + and operator + are not longer members of DateTime class
	
	Revision 5.4  2014/03/13 11:18:22  marpas
	to and from Gps week methods added (to was already present - new signature added).
	documentation updated
	test improved
	
	Revision 5.3  2013/10/17 13:36:48  marpas
	qa warning removed
	
	Revision 5.2  2013/10/17 13:30:38  marpas
	templates to get and set values from resource set implemented
	asISO8601 overloaded to return a string
	
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
	
	Revision 2.32  2013/05/24 11:28:25  marpas
	qa rules
	
	Revision 2.31  2013/05/17 16:16:01  marpas
	stream insertion operators for DateTime
	
	Revision 2.30  2012/12/02 20:04:10  marpas
	qa rules
	
	Revision 2.29  2012/10/04 13:28:01  lucio.pulvirenti
	Datetime2timespec method added returning a filled timespec struct. Imported from PDS2 environment.
	
	Revision 2.28  2012/02/08 09:52:54  marpas
	refactoring
	
	Revision 2.27  2011/09/29 14:57:06  marpas
	getting rid of useless template (causing codecheck crash)
	
	Revision 2.26  2011/01/20 10:34:00  marpas
	boost date_time greater than 1.33 implements ptime and date default ctors in different way (they are defaulted to invalid).
	This is avoided using DATE_TIME_NO_DEFAULT_CONSTRUCTOR if the boost version is greater than 1.33
	The old libDateTime implementation was based on default ctors, this one do not longer rely on those.
	
	Revision 2.25  2010/09/14 11:14:48  marpas
	DateTime::YYYYMMDDThhmmss method added
	
	Revision 2.24  2010/09/02 12:15:20  marpas
	DateTime::num2Month added (test improved)
	
	Revision 2.23  2010/05/05 10:18:36  marpas
	DEBUG added
	
	Revision 2.22  2009/05/14 08:47:34  marpas
	DateTime::operator+(const JD50 &) and DateTime::operator-(const JD50 &)  implemented
	
	Revision 2.21  2009/05/12 10:35:11  marpas
	int DateTime::compare(const DateTime &) const method implemented and tested
	
	Revision 2.20  2009/03/23 13:55:09  marpas
	JD70 class added
	rule violations fixed
	
	Revision 2.19  2009/03/04 12:18:08  marpas
	new CODECHECK instrumentation - no regressions
	
	Revision 2.18  2009/03/02 14:02:54  marpas
	CODECHECK ifndef - no regression
	
	Revision 2.17  2008/04/30 10:10:15  marpas
	DateTime::YYYYMMDD DateTime::YYYYMMDD(char sep) methods added
	
	Revision 2.16  2008/03/28 16:24:55  enrcar
	EC:: Static method dayAndMonthFromDayOfYear renamed as dayAndMonthFromDOY. Added method: dayAndMonthFromDayOfYearZR
	
	Revision 2.15  2008/02/05 16:46:53  enrcar
	EC:: dayAndMonthFromDayOfYear static method added
	
	Revision 2.14  2007/07/19 13:42:25  giucas
	exStream include added
	
	Revision 2.13  2007/06/11 13:36:41  marpas
	dateString interface slightly changed
	
	Revision 2.12  2007/06/06 20:47:48  marpas
	getSqlStringY2D methods added
	
	Revision 2.11  2006/11/03 00:30:24  marpas
	comment changed to avoid compilation errors (thanks to Quality control imposing no ifdef to insulate the header)
	
	Revision 2.10  2006/11/03 00:24:09  marpas
	Rounding mode for SQL representation and conversions added (setRoundingMode & getRoundingMode)
	enum RoundingMode {Rint (0), Round (1 default), Trunc ( 2) } ; rules the conversion.
	setDefaultFractionDigits & getDefaultFractionDigits are used for all conversions to string but the ones devoted
	to SQL representation. setDefaultDbFractionDigits & getDefaultDbFractionDigits should be used instead.
	
	Revision 2.9  2006/10/16 10:52:07  marpas
	some make up
	documentation added
	
	Revision 2.8  2006/07/13 13:30:55  marpas
	utcs(...) methods interface now has a defaulted boolean to print or not fractional part.
	default behaviour is to print it.
	
	Revision 2.7  2006/07/13 12:40:30  marpas
	jd50 method precision improved
	
	Revision 2.6  2006/07/13 09:36:02  marpas
	getPtime() method implemented
	
	Revision 2.5  2006/06/26 10:20:39  marpas
	digits managment improved
	
	Revision 2.4  2006/06/26 10:07:50  marpas
	fractional digits management added
	
	Revision 2.3  2006/06/22 14:27:39  marpas
	Temporary version
	Uses boost::date_time
	allows microsecs management
	interface slightly changed
	
	Revision 2.2  2006/06/20 09:16:38  marpas
	YYYYMMDDhhmmss method added - MUST be removed from libSTRINGUtils
	
	Revision 2.1  2006/03/24 08:50:44  marpas
	GCC 3.4.5 complained with inline definition
	
	Revision 2.0  2006/02/28 08:40:49  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.25  2005/11/15 11:31:01  marpas
	Datetime2time_t method fixed in order to not use TZ
	
	Revision 1.24  2005/11/08 18:28:25  enrcar
	include missing. added.
	
	Revision 1.23  2005/10/22 18:02:12  marpas
	warning fixed
	
	Revision 1.22  2005/10/22 17:40:55  marpas
	unused variable removed
	
	Revision 1.21  2005/10/22 17:02:12  marpas
	streams extractors and tests
	
	Revision 1.20  2005/03/03 14:11:02  marpas
	long double support
	
	Revision 1.19  2004/08/30 09:14:07  marpas
	MJD class introduced
	
	Revision 1.18  2004/08/30 08:27:21  marpas
	MJD class name changed in MJD2000
	
	Revision 1.17  2004/08/27 15:43:41  marpas
	UTCD::daysOfMonth has an extra parameters to get correct number of days even for leap years.
	UTCD::dayOfMonth method completely rewritten and tested
	UTCD::dayOfYear method completely rewritten and tested
	
	Revision 1.16  2004/07/28 07:03:09  enrcar
	EC: Added DateTime2gpsWeek() method.
	
	Revision 1.15  2004/05/17 08:57:35  marpas
	ISO8601 method fixed. I/F allow to print msec or not
	
	Revision 1.14  2003/10/21 16:58:58  paoscu
	"operator tm() const" and "operator time_t() const" replaced by
	"tm DateTime2tm() const" and "time_t Datetime2time_t() const"
	
	Revision 1.13  2003/10/21 15:46:07  paoscu
	"operator tm() const" and "operator time_t() const" added.
	dayOfTheWeek now uses "operator tm() const".
	
	Revision 1.12  2003/09/03 13:55:03  paoscu
	Export methods are now const.
	
	Revision 1.11  2003/07/11 13:03:48  marpas
	exException::clone implemented
	
	Revision 1.10  2003/06/09 17:09:48  marpas
	New ctor added
	
	Revision 1.9  2003/05/21 17:02:42  paoscu
	sortableDateTimeString method added.
	
	Revision 1.8  2003/05/13 15:48:13  paoscu
	std namespace used explicitly.
	
	Revision 1.7  2003/04/15 18:16:57  marpas
	GCC3.2
	
	Revision 1.6  2003/03/10 10:53:48  paoscu
	dayOfTheWeek() introduced.
	
	Revision 1.5  2003/01/31 14:57:24  paoscu
	MJD2000 class added
	
	Revision 1.4  2003/01/30 11:24:07  paoscu
	ISO8601 time format supported in DateTime::operator=(const std::string &utcs).
	Export methods now write time in ISO8601
	
	Revision 1.3  2002/12/16 12:17:47  paoscu
	operators += and -= .
	
	Revision 1.2  2002/11/29 17:38:11  danalt
	exportXML added
	
	Revision 1.1.1.1  2002/11/27 17:17:19  paoscu
	Import libDateTime
		

*/ 



#ifndef _DateTime_H_
#define _DateTime_H_ 

#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <exException.h>

// MP: this will cause the definition of BOOST_DATE_TIME_HAS_NANOSECONDS if available
#define BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG  

#include <boost/date_time/posix_time/posix_time.hpp> // PRQA S 1013 2
#include <boost/date_time/local_time/local_time.hpp>

#if BOOST_VERSION > 103399
	#define DATE_TIME_NO_DEFAULT_CONSTRUCTOR
#endif
#include <sys/time.h> // PRQA S 1013 
#include <iostream>
#include <exStream.h>



_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
class XMLOstream ;

/*! \class DateTime

	\brief The main class to deal with dates and times
*/

class DateTime 
{

public :

	static unsigned long convertDigits (unsigned long frac, unsigned short digits, unsigned short wanted);
    
	class JD50 
	{
	public :
		explicit JD50(long double d = 0) noexcept :_jd(d) { } 	
		~JD50() = default ;

		long double jd() const noexcept { return _jd ; }
        long double jd50() const noexcept { return _jd ; }
		JD50 &operator=(long double d) noexcept { _jd = d ; return *this ; }
		operator long double() const noexcept { return _jd ; }  // NOSONAR implicit cast
		JD50 & operator+=(const JD50 &r) noexcept { _jd += r._jd ; return  *this ; }
		JD50 & operator-=(const JD50 &r) noexcept { _jd -= r._jd ; return  *this ; }

 		// inherited from dbExportable
		void importObj(rsResourceSet &)   ;
		void exportObj(rsResourceSet &) const;
		void exportXML(XMLOstream &) const ;

	private :
		long double	_jd ;
	} ;
	

	class JD70 
    {

	public :

		explicit JD70(long double d = 0) noexcept :_jd(d) { } 	
		~JD70() = default ;
        
		long double jd() const noexcept { return _jd ; }
        long double jd50() const noexcept { return  _jd + _jd502jd70_OFFSET ; }
		JD70 &operator=(long double d) noexcept { _jd = d ; return *this ; }
		operator long double() const noexcept { return _jd ; }  // NOSONAR implicit cast
		JD70 & operator+=(const JD70 &r) noexcept { _jd += r._jd ; return  *this ; }
		JD70 & operator-=(const JD70 &r) noexcept { _jd -= r._jd ; return  *this ; }

		void importObj(rsResourceSet &) ;
		void exportObj(rsResourceSet &) const ;
		void exportXML(XMLOstream &) const ;

		static const long double _jd502jd70_OFFSET ;        // PRQA S 2100
	private :
		long double	_jd ;
	} ;

	class JD {
	public :
		explicit JD(long double d = 0) noexcept :_jd(d) { } 	
		~JD() = default ;
        
		long double jd() const noexcept { return _jd ; }
        long double jd50() const noexcept { return _jd - _jd2jd50_OFFSET ; }

		JD &operator=(long double d) noexcept { _jd = d ; return *this ; }
		operator long double() const noexcept { return _jd ; }  // NOSONAR implicit cast
		JD & operator+=(const JD &r) noexcept { _jd += r._jd ; return  *this ; }
		JD & operator-=(const JD &r) noexcept { _jd -= r._jd ; return  *this ; }

		static const long double _jd2jd50_OFFSET ;        // PRQA S 2100

	private :
		long double	_jd ;
	} ;
	
	
///////////////////
// MJD2000 2000
///////////////////	
	class MJD2000 {

	public :

		explicit MJD2000(long double d = 0) noexcept :_jd(d) { } 	
		~MJD2000() = default ; 
        
		long double jd() const noexcept { return _jd ; }
		long double jd50() const noexcept { return _jd + MJD2000::_jd502mjd2000_OFFSET; }

		MJD2000 &operator=(long double d) noexcept { _jd = d ; return *this ; }
		operator long double() const noexcept { return _jd ; }  // NOSONAR implicit cast

		void importObj(rsResourceSet &) ;
		void exportObj(rsResourceSet &) const ;
		static const long double _jd502mjd2000_OFFSET ;        // PRQA S 2100

	private :
		long double	_jd ;
	} ;
	
	
	class MJD {

	public :

		explicit MJD(long double d = 0) noexcept :_jd(d) { } 	
		~MJD() = default ;
        
		long double jd() const noexcept { return _jd ; }
		long double jd50() const noexcept { return _jd - MJD::_jd502mjd_OFFSET; }

		MJD &operator=(long double d) noexcept { _jd = d ; return *this ; }
		operator long double() const noexcept { return _jd ; }  // NOSONAR implicit cast

		void importObj(rsResourceSet &) ;
		void exportObj(rsResourceSet &) const ;
		static const long double _jd502mjd_OFFSET ;        // PRQA S 2100

	private :
		long double	_jd ;
	} ;
		
	
	class UTCD {

	public :
		/*! class InvalidMonthException declaration */
		exDECLARE_EXCEPTION(InvalidMonthException,exException) ;    // PRQA S 2131, 2502

		explicit UTCD(unsigned short Y = 1950,
                     unsigned short M = 1,
                     unsigned short D = 1,
                     unsigned short h = 0,
                     unsigned short m = 0,
                     unsigned short s = 0,
                     unsigned short ms =0 ) :
       	    year(Y),
	        month(M),
	        day(D),
	        hour(h),
	        min(m),
	        sec(s),
	        frac(ms),
	        _ndigits(3)  // PRQA S 4400
        {
	        isValid(true) ; // throw Exception if not valid
        }

		UTCD(unsigned short Y, // NOSONAR - many parameters needed
             unsigned short M,
             unsigned short D, 
		     unsigned short h, 
             unsigned short m,
             unsigned short s,
			 unsigned long f, 
             unsigned short ndigits) :
	        year(Y),
	        month(M),
	        day(D),
	        hour(h),
	        min(m),
	        sec(s),
	        frac(f),
	        _ndigits(ndigits) 
        {
	        isValid(true) ;
        }
             
		~UTCD() = default ;

		bool isValid(bool genException = false) const ;
		int dayOfYear() const {	return dayOfYear(day,month,year) ; }
        int weekNumber() const { return weekNumber(day, month, year) ; }
        unsigned short dayOfTheWeek() const { return dayOfTheWeek(day, month, year) ; }
		
        static bool leapYear(unsigned short y) 
        { 
	        ACS_COND_THROW((y < 1400) || (y > 9999), exIllegalValueException("Year out of [1400,9999] range")) ; // PRQA S 3000, 3081
	        return (((y%4==0) && (y%100!=0)) || (y%400==0)) ;  // PRQA S 3000, 4400
        }
        
		bool leapYear() const { return UTCD::leapYear(year) ; }

        /*- returns year, month and day for the 0-relative dayOfYearZR in the given year.
         *  there is no guarantee that the date falls in the same given year
         */
        static void dayAndMonthFromDayOfYearZR(unsigned short dayOfYearZR, unsigned short &year, unsigned short& day,unsigned short& month); 
        /*- returns year, month and day for the 1-relative doy1relative in the given year.
         *  there is no guarantee that the date falls in the same given year
         */
        static void dayAndMonthFromDOY(unsigned short doy1relative, unsigned short &year, unsigned short& day,unsigned short& month); 
		static int dayOfYear(unsigned short day, unsigned short month, unsigned short year) ;
        static DateTime::UTCD dayOfMonth(unsigned short dayofyear, unsigned short year) ;
		static unsigned short daysOfMonth(unsigned short m, unsigned short year) ;
        // returns the iso week number for the date
        static int weekNumber(unsigned short day, unsigned short month, unsigned short year) ;
        static unsigned short dayOfTheWeek(unsigned short day, unsigned short month, unsigned short year) ;
		unsigned long microsec() const noexcept { return convertDigits (frac, _ndigits, 6); } // PRQA S 3000, 3011, 3084, 4400 3
		unsigned long millisec() const noexcept { return convertDigits (frac, _ndigits, 3); }
		unsigned long nanosec() const noexcept {  return convertDigits (frac, _ndigits, 9); }
		void microsec(unsigned long) ;
		void millisec(unsigned long) ;
		void nanosec(unsigned long) ;
        long double jd50() const ; // can throw, because content can be invalid for boost 

        // PRQA S 2100 8
		unsigned short  year;
		unsigned short  month;
		unsigned short  day;
		unsigned short  hour;
		unsigned short  min;
		unsigned short  sec;
		unsigned long   frac;
	private:
		unsigned short _ndigits ;
	} ;



	/*! class InvalidTimeException declaration */
	exDECLARE_EXCEPTION(InvalidTimeException,exException) ;    // PRQA S 2131, 2502
	
	
	// DateTime declaration

	// the following constructor form 
	// builds a 'not filled' date time
	// the passed bool is not used - 
	// it's meaning is to differentiate the ctor signature	
	explicit DateTime(bool) noexcept : jd50_(), _filled(false) {}
	DateTime() noexcept ;                // build a time with the actual time
	explicit DateTime(time_t t) noexcept : jd50_(JD70(t/86400.0L).jd50()), _filled(true) {} // PRQA S 3011, 3081, 4400

	explicit DateTime(const boost::posix_time::ptime &) noexcept ;

    // PRQA S 2180 9
	DateTime(const  JD50 &jd50) noexcept : jd50_(jd50.jd()), _filled(true) {	} // NOSONAR implicit conversion
	DateTime(const  JD70 &jd70) noexcept : jd50_(jd70.jd50()), _filled(true) { } // NOSONAR implicit conversion
	DateTime(const  JD &jd) noexcept : jd50_(jd.jd50()), _filled(true) { } // NOSONAR implicit conversion
	DateTime(const  MJD2000 &jd2000) noexcept : jd50_(jd2000.jd50()), _filled(true) { } // NOSONAR implicit conversion
	DateTime(const  MJD &mjd) noexcept : jd50_(mjd.jd50()), _filled(true) { } // NOSONAR implicit conversion
	DateTime(const UTCD &u) : jd50_(u.jd50()), _filled(true) { } // NOSONAR implicit conversion
	DateTime(const std::string &s) : jd50_(), _filled(false) { *this = s ; } // NOSONAR implicit conversion
	DateTime(const char *s) : jd50_(), _filled(false) { *this = std::string(s) ; } // NOSONAR implicit conversion

	~DateTime() = default ;
	
    void assertIsFilled() const { ACS_COND_THROW( ! isFilled(), exIllegalValueException("Not filled") ) ; } // PRQA S 3081 

	// returns < 0 if this is less than the given
	// 0 if they are equal
	// > 0 if this is greater than the given
	int compare(const DateTime &t) const // PRQA S 4020
    {
	    ACS_COND_THROW( ! isFilled() || ! t.isFilled(), exIllegalValueException("Both args must be filled")) ; // PRQA S 3081

	    if (jd50_ > t.jd50_) { return 1 ; }
	    else if (jd50_ < t.jd50_) { return -1 ; }
	    else { return 0 ; } // they are equal by definition
    }
	
    /*! \brief add days 
     * 
     * arguments to be intended as days floating point */
	DateTime & operator+=(long double days) { jd50_ += days ; return * this ; }
    /*! \brief add days 
     * 
     * arguments to be intended as days floating point */
	DateTime & operator-=(long double days) { jd50_ -= days ; return * this ; }

	bool leapYear() const  {
        ACS_COND_THROW(_filled == false, exIllegalValueException("DateTime not filled")) ; // PRQA S 3081
        return UTCD::leapYear(jd50_to_Y2D(jd50_).Y) ;
    }

	
	//+ check if it is older than now - secs
    bool isOlder(time_t secs) const ; 
    
	// the DateTime::leapYear(int) functions returns 
	// if an year is a leap one or not
	static bool	leapYear(int y) { return UTCD::leapYear(y) ; } // PRQA S 3000, 3010
	
	/** returns in the range [1,12] the number of the month
		 passed in 3-char english language as in Jan, Feb ...
	     case are ignored
	 */
	static unsigned short month2Num(const std::string &) ;	

	/** returns the  3-char english language month name as in Jan, Feb ...
		 passing an integer [1-12]
	 */
	static const std::string &num2Month(int) ;	

	/**
	 *	Return day of the week. 1=Monday, 7=Sunday
	 */
	unsigned short dayOfTheWeek() const;
    
    /**
     * returns Week of the Year (p.first) and the year it belongs (p.second)
     * e.g that date could be the first week of the next year, or, vice versa, the last week
     * of the previous year
     */
    std::pair<unsigned short, unsigned int> weekOfYear() const ;

    /**
     * returns the date associated to the first day (Mon) of a given week of a given year.
     * 
     */
    static DateTime dateOfWeek(const std::pair<unsigned short, unsigned int> &p) ;
    
    /**
     * returns the DateTime associated to the zulu-like string
     * 
     * 
     */
    static DateTime zuluUTCString2DateTime(const std::string &pzd) ;
    
    
	std::string &utcs(std::string &, bool frac=true, bool truncate=true) const ;
	std::ostream &utcs(std::ostream &, bool frac=true, bool truncate=true) const ;
	exostream &utcs(exostream &, bool frac=true, bool truncate=true) const ;
	void toUTCD(UTCD &utcd) const ;

	/**
	 *	returns a struct tm filled (note UTC - do not uses TZ)
	 */
	tm DateTime2tm(long *ns = nullptr) const ;
	
	/**
	 *	returns UTC absolute time (do not uses TZ)
	 */
 	time_t Datetime2time_t(long *ns = nullptr) const ;
	
	/**
	 *	returns a struct timespec filled (note UTC - do not uses TZ)
	 */
	timespec Datetime2timespec() const ;
	
	long double jd50() const noexcept { return jd50_ ; }

	/**
     * \brief gps week computation (to)
	 * 
     * returns the Gps week of the datetime
     * \see fromGpsWeek, toGpsWeek
	 */
	unsigned short int Datetime2gpsWeek() const { assertIsFilled() ;
        return static_cast<unsigned short int>( (jd50() - 10962.0 )/7.0 ) ; /* JD50 to GPS WEEK */ // PRQA S 3081, 4400
    }
	/**
     * \brief gps week computation (to)
	 * 
     * returns the Gps week of the datetime
     * \see fromGpsWeek, Datetime2gpsWeek
	 */
    unsigned short int toGpsWeek() const { return Datetime2gpsWeek() ; }

    
    
	/**
     * \brief gps week computation (from)
	 * 
     * returns the starting DateTime of the given Gps week
     * \see Datetime2gpsWeek, toGpsWeek
	 */
    static DateTime fromGpsWeek(unsigned short int) ;
    // PRQA S 2181 7
    operator UTCD() const { UTCD u ; toUTCD(u) ; return u ; }   // NOSONAR implicit cast
	operator JD50() const { assertIsFilled() ; return JD50(jd50()) ; }   // NOSONAR implicit cast
	operator JD70() const { assertIsFilled() ; return JD70(jd50() - JD70::_jd502jd70_OFFSET) ; }   // NOSONAR implicit cast
	operator JD() const { assertIsFilled() ; return JD(jd50() + JD::_jd2jd50_OFFSET) ; }   // NOSONAR implicit cast
	operator MJD2000() const { assertIsFilled() ; return  MJD2000(jd50() - MJD2000::_jd502mjd2000_OFFSET) ; }   // NOSONAR implicit cast
	operator MJD() const { assertIsFilled() ; return  MJD(jd50() + MJD::_jd502mjd_OFFSET) ; }   // NOSONAR implicit cast

	std::string getSqlString(bool truncate=true) const ;
	std::string getSqlStringY2D() const ;
	std::string getSqlString(int fracdgts, bool truncate=true) const ;
	std::ostream &getSqlString(std::ostream &, bool truncate=true) const ;
	std::ostream &getSqlStringY2D(std::ostream &) const ;
	std::ostream &getSqlString(std::ostream &, int fracdgts, bool truncate=true) const ;
	std::ostream &asISO8601(std::ostream &, int fracDgts, const std::string &sep, bool truncate=true) const ;
	std::ostream &asISO8601(std::ostream &, int fracDgts, char sep = 'T', bool truncate=true) const ;
	std::string asISO8601(int fracDgts, bool truncate=true) const ;
	std::string asISO8601(int fracDgts, const std::string & sep , bool truncate=true) const ;
	std::string asISO8601(int fracDgts, char sep, bool truncate=true ) const ;
	
	std::ostream & sortableDateString(std::ostream &) const ; 
	std::ostream & sortableDateTimeString(std::ostream &, bool truncate = true) const ; 
	std::ostream & dateString(std::ostream &, const std::string &sep) const ; 
	std::ostream & dateString(std::ostream &, const char sep='-') const ; 
	std::ostream & timeString(std::ostream &, bool truncate = true) const ; 
	std::ostream & timeString(std::ostream &, int fracdgts, bool truncate = true, char sephms=':') const ;
	std::string timeString(int fracdgts, bool truncate = true, char sephms=':') const ;

	std::string YYYYMMDDhhmmss () const ;
	std::string YYYYMMDDThhmmss () const ;
	std::string YYYYMMDD () const ;
	std::string YYYYMMDD (char sep) const ;
	
	DateTime &operator=(const std::string &) ;
	DateTime &operator=(const UTCD &u) { jd50_ = u.jd50() ;	_filled = true ; return *this ; }
	DateTime &operator=(const JD50 &jd50) noexcept { jd50_ = jd50.jd50() ; _filled = true ; return *this ; }
	DateTime &operator=(const JD70 &jd70) noexcept { jd50_ = jd70.jd50() ; _filled = true ; return *this ; }
	DateTime &operator=(const JD &jd) noexcept { jd50_ = jd.jd50() ;	_filled = true ; return *this ; }
	DateTime &operator=(const MJD2000 &jd2000) noexcept { jd50_ = jd2000.jd50() ; _filled = true ; return *this ; }
	DateTime &operator=(const MJD &mjd) noexcept { jd50_ = mjd.jd50() ; _filled = true ; return *this ; }

	bool isFilled() const noexcept { return _filled ; }
	void reset() noexcept { _filled = false ; }
	long double diffDay(const DateTime &d) const noexcept {  return jd50_ - d.jd50_ ; }

	// inherited from dbExportable
    void exportObj(rsResourceSet &) const;
    void importObj(rsResourceSet &)  ;
    void exportXML(XMLOstream &) const;
	
	static void setDefaultFractionDigits(unsigned short) ;
	static void setDefaultDbFractionDigits(unsigned short) ;
	static unsigned short getDefaultFractionDigits() ;
	static unsigned short getDefaultDbFractionDigits() ;
    
    struct Y2ns_type {
		unsigned short  Y ;
		unsigned short  M ;
		unsigned short  D ;
		unsigned short  h ;
		unsigned short  m ;
		unsigned short  s ;
		unsigned long   ns ; // nanosec
    } ;
    static Y2ns_type jd50_to_Y2ns(long double, int digits=9, bool truncate=true) ; 
    static Y2ns_type jd50_to_Y2D(long double) ; 
    static long double Y2ns_to_jd50(const Y2ns_type &) ;
    static boost::posix_time::ptime jd50_to_ptime(long double) ; 
    static long double ptime_to_jd50(const boost::posix_time::ptime &)  ;
    static const std::string &unfilledDate() noexcept ;
// data section
private:
	long double jd50_ ; 

	bool _filled ;
	static unsigned short _defaultFracDgts ;
	static unsigned short _defaultDbFracDgts ;
   
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DateTime) ;
} ;

std::istream &operator >> (std::istream &, DateTime& ) ;

exostream & operator << (exostream &, const DateTime&) ; // PRQA S 2072
std::ostream & operator << (std::ostream &,const DateTime&) ;

template<>
inline
void rsResourceSet::setValue<DateTime>(const std::string &res, const DateTime &v) // PRQA S 2133, 2134
{
    Lock lock(_mutex) ;
    findOrInsertValue(res)->set(v.isFilled() ? v.asISO8601(9) : DateTime::unfilledDate()) ; // PRQA S 3380, 3385, 4400
}

template<>
inline
void rsResourceSet::getValue(const std::string &res, DateTime &v) const  // PRQA S 2133, 2134
{
    Lock lock(_mutex) ;
    std::string dt ;
	getValue(res)->get(dt) ;
    if (dt == DateTime::unfilledDate()) {
        v = DateTime(false) ; // PRQA S 3081
    }
    else {
        // Using zuluUTCString2DateTime allows to transform also zulu strings in DateTime. 
        v = DateTime::zuluUTCString2DateTime(dt) ; // PRQA S 3081
    }
} 
/*! \brief add days 
 * 
 * arguments to be intended as days floating point */
inline DateTime operator+(const DateTime &d, long double days) // PRQA S 2134
{
	return DateTime(d) += days ; // PRQA S 3081
}

/*! \brief add days 
 * 
 * arguments to be intended as days floating point */
inline DateTime operator-(const DateTime &d, long double days)  // PRQA S 2134
{
	return DateTime(d) -= days ; // PRQA S 3081
}

inline bool operator>(const DateTime::JD50 &a, const DateTime::JD50 &b) noexcept { return a.jd() > b.jd() ; } // PRQA S 2134 EOF
inline bool operator>=(const DateTime::JD50 &a, const DateTime::JD50 &b) noexcept { return a.jd() >= b.jd() ; }
inline bool operator<=(const DateTime::JD50 &a, const DateTime::JD50 &b) noexcept { return ! (a > b) ; }
inline bool operator<(const DateTime::JD50 &a, const DateTime::JD50 &b) noexcept { return ! (a >= b) ; }
inline bool operator==(const DateTime::JD50 &a, const DateTime::JD50 &b) noexcept { return ! (a < b) && ! (a > b) ; }
inline bool operator!=(const DateTime::JD50 &a, const DateTime::JD50 &b) noexcept { return ! (a == b) ; }


inline bool operator>(const DateTime::JD70 &a, const DateTime::JD70 &b) noexcept { return a.jd() > b.jd() ; }
inline bool operator>=(const DateTime::JD70 &a, const DateTime::JD70 &b) noexcept { return a.jd() >= b.jd() ; }
inline bool operator<=(const DateTime::JD70 &a, const DateTime::JD70 &b) noexcept { return ! (a > b) ; }
inline bool operator<(const DateTime::JD70 &a, const DateTime::JD70 &b) noexcept { return ! (a >= b) ; }
inline bool operator==(const DateTime::JD70 &a, const DateTime::JD70 &b) noexcept { return ! (a < b) && ! (a > b) ; }
inline bool operator!=(const DateTime::JD70 &a, const DateTime::JD70 &b) noexcept { return ! (a == b) ; }

inline bool operator>(const DateTime::JD &a, const DateTime::JD &b) noexcept { return a.jd() > b.jd() ; }
inline bool operator>=(const DateTime::JD &a, const DateTime::JD &b) noexcept { return a.jd() >= b.jd() ; }
inline bool operator<=(const DateTime::JD &a, const DateTime::JD &b) noexcept { return ! (a > b) ; }
inline bool operator<(const DateTime::JD &a, const DateTime::JD &b) noexcept { return ! (a >= b) ; }
inline bool operator==(const DateTime::JD &a, const DateTime::JD &b) noexcept { return ! (a < b) && ! (a > b) ; }
inline bool operator!=(const DateTime::JD &a, const DateTime::JD &b) noexcept { return ! (a == b) ; }

inline bool operator>(const DateTime::MJD2000 &a, const DateTime::MJD2000 &b) noexcept { return a.jd() > b.jd() ; }
inline bool operator>=(const DateTime::MJD2000 &a, const DateTime::MJD2000 &b) noexcept { return a.jd() >= b.jd() ; }
inline bool operator<=(const DateTime::MJD2000 &a, const DateTime::MJD2000 &b) noexcept { return ! (a > b) ; }
inline bool operator<(const DateTime::MJD2000 &a, const DateTime::MJD2000 &b) noexcept { return ! (a >= b) ; }
inline bool operator==(const DateTime::MJD2000 &a, const DateTime::MJD2000 &b) noexcept { return ! (a < b) && ! (a > b) ; }
inline bool operator!=(const DateTime::MJD2000 &a, const DateTime::MJD2000 &b) noexcept { return ! (a == b) ; }

inline bool operator>(const DateTime::MJD &a, const DateTime::MJD &b) noexcept { return a.jd() > b.jd() ; }
inline bool operator>=(const DateTime::MJD &a, const DateTime::MJD &b) noexcept { return a.jd() >= b.jd() ; }
inline bool operator<=(const DateTime::MJD &a, const DateTime::MJD &b) noexcept { return ! (a > b) ; }
inline bool operator<(const DateTime::MJD &a, const DateTime::MJD &b) noexcept { return ! (a >= b) ; }
inline bool operator==(const DateTime::MJD &a, const DateTime::MJD &b) noexcept { return ! (a < b) && ! (a > b) ; }
inline bool operator!=(const DateTime::MJD &a, const DateTime::MJD &b) noexcept { return ! (a == b) ; }

inline bool operator>(const DateTime &a, const DateTime &b) { return a.jd50() > b.jd50() ; }
inline bool operator<(const DateTime &a, const DateTime &b) { return a.jd50() < b.jd50() ; }
inline bool operator>=(const DateTime &a, const DateTime &b) { return a.jd50() >= b.jd50() ; }
inline bool operator<=(const DateTime &a, const DateTime &b) { return a.jd50() <= b.jd50() ; }
inline bool operator==(const DateTime &a, const DateTime &b) noexcept // PRQA S 4020
{
    if (a.isFilled() && b.isFilled()) { // both are filled 
	    return a.compare(b) == 0 ;
    }
    else {
        return a.isFilled() == b.isFilled() ; // both must be not filled
    }
}

inline bool operator!=(const DateTime &a, const DateTime &b) noexcept { return !(a == b) ; }

_ACS_END_NAMESPACE

#endif
