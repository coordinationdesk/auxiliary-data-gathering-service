// PRQA S 1050 EOF
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
	Revision 5.8  2014/09/23 15:45:46  marfav
	Added rounding mode to each datetime to string function
	
	Revision 5.7  2014/09/09 14:29:05  marfav
	Fixed precision conversion
	Addedd support for reduced precision when converting JD to fraction
	as per S3PDGS-2000
	
	Revision 5.6  2014/04/08 16:37:39  marpas
	Library completely rewritten - now perfo are up to 300x faster and accuracy is guaranteed to nanosecs.
	Interface rationalized
	coverage test rewritten
	performance test rewritten
	
	Revision 5.5  2014/04/01 18:00:54  marpas
	work in progress
	
	Revision 5.4  2014/04/01 10:55:02  marpas
	starting heavy refactorying, implementing speed and accuracy
	
	Revision 5.3  2014/03/28 11:26:23  marpas
	asISO8601 overloaded
	
	Revision 5.2  2014/02/13 12:13:22  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	
	Revision 5.1  2013/10/17 13:30:38  marpas
	templates to get and set values from resource set implemented
	asISO8601 overloaded to return a string
	
	Revision 5.0  2013/07/01 15:28:51  marpas
	adopting libException 5.x standards
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.17  2010/12/20 18:28:11  marpas
	more exception management added
	
	Revision 2.16  2010/09/14 11:14:48  marpas
	DateTime::YYYYMMDDThhmmss method added
	
	Revision 2.15  2009/03/23 13:55:09  marpas
	JD70 class added
	rule violations fixed
	
	Revision 2.14  2009/03/04 12:18:08  marpas
	new CODECHECK instrumentation - no regressions
	
	Revision 2.13  2009/03/02 14:02:54  marpas
	CODECHECK ifndef - no regression
	
	Revision 2.12  2008/04/30 10:10:15  marpas
	DateTime::YYYYMMDD DateTime::YYYYMMDD(char sep) methods added
	
	Revision 2.11  2007/06/11 13:36:41  marpas
	dateString interface slightly changed
	
	Revision 2.10  2007/06/10 15:55:17  marpas
	fixed in Year to day
	
	Revision 2.9  2007/06/06 20:47:48  marpas
	getSqlStringY2D methods added
	
	Revision 2.8  2007/06/05 13:28:32  marpas
	dateString method fixed (month were wrongly reported)
	
	Revision 2.7  2006/11/03 00:30:24  marpas
	comment changed to avoid compilation errors (thanks to Quality control imposing no ifdef to insulate the header)
	
	Revision 2.6  2006/11/03 00:24:09  marpas
	Rounding mode for SQL representation and conversions added (setRoundingMode & getRoundingMode)
	enum RoundingMode {Rint (0), Round (1 default), Trunc ( 2) } ; rules the conversion.
	setDefaultFractionDigits & getDefaultFractionDigits are used for all conversions to string but the ones devoted
	to SQL representation. setDefaultDbFractionDigits & getDefaultDbFractionDigits should be used instead.
	
	Revision 2.5  2006/10/16 10:52:07  marpas
	some make up
	documentation added
	
	Revision 2.4  2006/07/13 13:30:55  marpas
	utcs(...) methods interface now has a defaulted boolean to print or not fractional part.
	default behaviour is to print it.
	
	Revision 2.3  2006/06/26 10:07:50  marpas
	fractional digits management added
	
	Revision 2.2  2006/06/22 14:27:39  marpas
	Temporary version
	Uses boost::date_time
	allows microsecs management
	interface slightly changed
	
	Revision 2.1  2006/06/20 09:16:38  marpas
	YYYYMMDDhhmmss method added - MUST be removed from libSTRINGUtils
	
	Revision 2.0  2006/02/28 08:40:49  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2005/02/07 09:39:41  marpas
	headers fixed
	
	Revision 1.7  2004/05/17 08:57:35  marpas
	ISO8601 method fixed. I/F allow to print msec or not
	
	Revision 1.6  2003/10/23 15:43:09  paoscu
	method getISO8601String now prints ms with presision set to 6
	
	Revision 1.5  2003/05/21 17:02:42  paoscu
	sortableDateTimeString method added.
	
	Revision 1.4  2003/04/15 18:16:57  marpas
	GCC3.2
	
	Revision 1.3  2003/01/30 11:24:07  paoscu
	ISO8601 time format supported in DateTime::operator=(const string &utcs).
	Export methods now write time in ISO8601
	
	Revision 1.2  2002/12/16 12:17:47  paoscu
	operators += and -= .
	
	Revision 1.1.1.1  2002/11/27 17:17:19  paoscu
	Import libDateTime
		

*/


#include <DateTime.h>

#include <iomanip>


_ACS_BEGIN_NAMESPACE(acs)

using std::string ;
using std::ostream ;
using std::ostringstream ;
using std::setfill ;
using std::setw ;


ostream &DateTime::utcs(ostream &os, bool frac, bool truncate) const 
{
    ACS_COND_THROW(_filled == false, exIllegalValueException("DateTime not filled")) ; // PRQA S 3081
    // emit datetime on a stream in the form 19-Jun-2006_00:00:00(.000)
    dateString(os, '-') ;
    os << '_' ;
    timeString(os, frac ? _defaultFracDgts : 0, truncate ) ; // PRQA S 3000, 3010, 3380
    return os ; // PRQA S 4028
}


exostream &DateTime::utcs(exostream &os, bool frac, bool truncate) const 
{
    ACS_COND_THROW(_filled == false, exIllegalValueException("DateTime not filled")) ; // PRQA S 3081
    // emit datetime on a exstream in the form 19-Jun-2006_00:00:00(.000)

    ostringstream o ;
    dateString(o, '-') ;
    os << '_' ;
    timeString(o, frac ? _defaultFracDgts : 0, truncate ) ; // PRQA S 3000, 3010, 3380
    os << o.str() ;
    return os ; // PRQA S 4028
}


ostream & DateTime::dateString(ostream &os, const string &sep) const
{
    ACS_COND_THROW(_filled == false, exIllegalValueException("DateTime not filled")) ; // PRQA S 3081
	// format: 01-Jan-1950 
    Y2ns_type y2ns = jd50_to_Y2D(jd50_) ; 
    return os << setw(4) << setfill('0') << y2ns.Y << sep    // PRQA S 3050, 4400 3
              << num2Month(y2ns.M) << sep // PRQA S 3000
              << setw(2) << setfill('0') << y2ns.D ;
}

ostream & DateTime::dateString(ostream &os, char sep) const
{
    ACS_COND_THROW(_filled == false, exIllegalValueException("DateTime not filled")) ; // PRQA S 3081
	// format: 01-Jan-1950 
    Y2ns_type y2ns = jd50_to_Y2D(jd50_) ; 
    return os << setw(4) << setfill('0') << y2ns.Y << sep    // PRQA S 3050, 4400 3
              << num2Month(y2ns.M) << sep // PRQA S 3000
              << setw(2) << setfill('0') << y2ns.D ;
}

ostream & DateTime::sortableDateString(ostream &os) const
{
    // emits a string in the form of 1970/01/01 over a stream
    return os << YYYYMMDD('/') ; 
}


ostream & DateTime::sortableDateTimeString(ostream &os, bool truncate) const
{
	sortableDateString(os);
	os<<' ';
	timeString(os, truncate);
	return os ; // PRQA S 4028
}



ostream & DateTime::timeString(ostream &os, bool truncate) const
{
	// 09:15:40.000 format
    return timeString(os, _defaultFracDgts, truncate) ;     // PRQA S 3000
}

ostream & DateTime::timeString(ostream &os, int fracdgts, bool truncate, char sephms) const
{
    ACS_COND_THROW(_filled == false, exIllegalValueException("DateTime not filled")) ; // PRQA S 3081
	// 09:15:40.000 format
    Y2ns_type y2ns = jd50_to_Y2ns(jd50_, fracdgts, truncate) ; 
    os << setw(2) << setfill('0') << y2ns.h << sephms    // PRQA S 4400 3
       << setw(2) << setfill('0') << y2ns.m << sephms
       << setw(2) << setfill('0') << y2ns.s ;
    if (fracdgts > 0) { // PRQA S 3000 2
        os << '.' << setw(fracdgts) << setfill('0') << DateTime::convertDigits (y2ns.ns, 9, fracdgts); // NOSONAR
    }
    return os ; // PRQA S 4028
}

string DateTime::timeString(int fracdgts, bool truncate, char sephms) const 
{
    ostringstream os ;
    timeString(os, fracdgts, truncate, sephms) ;
    return os.str() ;
}


ostream & DateTime::getSqlString(ostream &os, bool truncate) const
{
    // format: 1965-10-30 14:53:14.124689
	return asISO8601(os, _defaultDbFracDgts, ' ', truncate) ; // PRQA S 3000
}

ostream & DateTime::getSqlStringY2D(ostream &os) const
{
	// format: 1965-10-30
    return os << YYYYMMDD('-') ;
}

ostream & DateTime::getSqlString(ostream &os, int fracdgts, bool truncate) const
{
    // format: 1965-10-30 14:53:14.124689
    return asISO8601(os, fracdgts, ' ', truncate) ;
}

string DateTime::getSqlString(int fracdgts, bool truncate) const
{
	ostringstream os ;
	getSqlString(os, fracdgts, truncate) ;
	return os.str() ;
}

string DateTime::getSqlString(bool truncate) const
{
	ostringstream os ;
	getSqlString(os, truncate) ;
	return os.str() ;
}

string DateTime::getSqlStringY2D() const
{
	ostringstream os ;
	getSqlStringY2D(os) ;
	return os.str() ;
}

ostream & DateTime::asISO8601(ostream &os, int fracDgts, const string &sep, bool truncate) const
{
    ACS_COND_THROW(_filled == false, exIllegalValueException("DateTime not filled")) ; // PRQA S 3081
	// format: 1965-10-30T14:53:14.124689
    Y2ns_type y2ns = jd50_to_Y2ns(jd50_, fracDgts, truncate) ; 
    os << setw(4) << setfill('0') << y2ns.Y << '-' // PRQA S 4400 9
       << setw(2) << setfill('0') << y2ns.M << '-'
       << setw(2) << setfill('0') << y2ns.D << sep
       << setw(2) << setfill('0') << y2ns.h << ':'
       << setw(2) << setfill('0') << y2ns.m << ':'
       << setw(2) << setfill('0') << y2ns.s ;
    if (fracDgts > 0) { // PRQA S 3000 2
        os << '.' << setw(fracDgts) << setfill('0') << DateTime::convertDigits (y2ns.ns, 9, fracDgts); // NOSONAR
    }
    return os ; // PRQA S 4028
}


ostream & DateTime::asISO8601(ostream &os, int fracDgts, char sep, bool truncate) const
{
    return asISO8601 (os, fracDgts, string("") + sep, truncate); // PRQA S 3081
}


string DateTime::asISO8601(int fracDgts, bool truncate) const
{
    ostringstream os ;
    asISO8601(os, fracDgts, "T", truncate) ;
    return os.str() ;
}

string DateTime::asISO8601(int fracDgts, const string &sep, bool truncate) const
{
    ostringstream os ;
    asISO8601(os, fracDgts, sep, truncate) ;
    return os.str() ;
}

string DateTime::asISO8601(int fracDgts, char sep, bool truncate) const
{
    ostringstream os ;
    asISO8601(os, fracDgts, sep, truncate) ;
    return os.str() ;
}

string DateTime::YYYYMMDDhhmmss () const 
{
    ACS_COND_THROW(_filled == false, exIllegalValueException("DateTime not filled")) ; // PRQA S 3081
	// format: 19651030145314
    Y2ns_type y2ns = jd50_to_Y2ns(jd50_) ; 
    ostringstream os ;
    os << setw(4) << setfill('0') << y2ns.Y // PRQA S 4400 6
       << setw(2) << setfill('0') << y2ns.M 
       << setw(2) << setfill('0') << y2ns.D 
       << setw(2) << setfill('0') << y2ns.h 
       << setw(2) << setfill('0') << y2ns.m 
       << setw(2) << setfill('0') << y2ns.s ;
    return os.str() ;
}

string DateTime::YYYYMMDDThhmmss () const 
{
    ACS_COND_THROW(_filled == false, exIllegalValueException("DateTime not filled")) ; // PRQA S 3081
	// format: 19651030T145314
    Y2ns_type y2ns = jd50_to_Y2ns(jd50_) ; 
    ostringstream os ;
    os << setw(4) << setfill('0') << y2ns.Y // PRQA S 4400 6
       << setw(2) << setfill('0') << y2ns.M 
       << setw(2) << setfill('0') << y2ns.D << 'T'
       << setw(2) << setfill('0') << y2ns.h 
       << setw(2) << setfill('0') << y2ns.m 
       << setw(2) << setfill('0') << y2ns.s ;
    return os.str() ;
}

string DateTime::YYYYMMDD () const 
{
    ACS_COND_THROW(_filled == false, exIllegalValueException("DateTime not filled")) ; // PRQA S 3081
	// format: 19651030
    Y2ns_type y2ns = jd50_to_Y2D(jd50_) ; 
    ostringstream os ;
    os << setw(4) << setfill('0') << y2ns.Y // PRQA S 4400 3 
       << setw(2) << setfill('0') << y2ns.M 
       << setw(2) << setfill('0') << y2ns.D ;
    return os.str() ;
}

string DateTime::YYYYMMDD (char sep) const 
{
    ACS_COND_THROW(_filled == false, exIllegalValueException("DateTime not filled")) ; // PRQA S 3081
		// as above but with a specific separator - by argument 
		// YYYYMMDD('^') --> 1965^10^31
    Y2ns_type y2ns = jd50_to_Y2D(jd50_) ; 
    ostringstream os ;
    os << setw(4) << setfill('0') << y2ns.Y << sep // PRQA S 4400 3 
       << setw(2) << setfill('0') << y2ns.M << sep 
       << setw(2) << setfill('0') << y2ns.D ;
    return os.str() ;
}


_ACS_END_NAMESPACE
