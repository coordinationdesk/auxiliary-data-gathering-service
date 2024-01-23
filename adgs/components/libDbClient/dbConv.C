// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.8  2016/04/29 12:15:08  marpas
	precision in floating point conversion fixed (to avoid bynary priodic artefacts)
	useless setprecision converting integral types removed
	
	Revision 5.7  2016/04/22 10:40:59  marpas
	changes to ease doxygen extraction in progress
	
	Revision 5.6  2015/03/19 14:36:25  marpas
	when converting a DateTime into a SQL string, the class now uses the DateTime::getDefaultDbFractionDigits and the limits imposed by the dialect
	
	Revision 5.5  2014/12/02 12:12:48  lucio.pulvirenti
	S2PDGS-908: dialect delimit() method used instead of getStringDelimiter() that has been removed; quoteCheck() method simplified by using dialect delimit() method.
	
	Revision 5.4  2014/02/07 18:36:52  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2013/08/28 15:52:22  marfav
	added db::toSQL to allow easier use of dbconv also with connections and pools
	QT related code removed
	
	Revision 5.2  2013/06/12 17:07:04  marpas
	coding best practices and qa rules application in progress
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 4.7  2013/05/14 18:13:41  marpas
	when using dbGeoObject, dbkey connection is mandatory now
	
	Revision 4.6  2013/01/24 19:27:58  marpas
	query stat strings fixed
	exception notification improved
	useless debug macros removed
	some stats added
	
	Revision 4.5  2012/12/14 18:38:53  marpas
	coding best practices
	qa rules
	
	Revision 4.4  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.3  2010/04/28 15:48:09  enrcar
	MARPAS:: Geoobject must NOT be surrounded by quotes in Oracle
	
	Revision 3.2  2010/04/26 10:27:58  marpas
	Preparation to ORACLE integration (needs libDbGeo ORACLE aware)
	
	Revision 3.1  2010/02/09 16:08:47  marpas
	oracle porting in progress
	
	Revision 3.0  2009/10/05 14:47:50  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.12  2009/01/27 13:16:19  marpas
	db statistics improved, some other minor changes with no regression
	
	Revision 2.11  2008/12/09 16:11:23  marpas
	dbConv::Null implemented to allow setting Null values using SQLString
	
	Revision 2.10  2008/12/03 12:23:06  marpas
	DateTime manipulation fixed for postgres
	
	Revision 2.9  2008/05/14 15:04:17  crivig
	yearToFraction6 introduced
	
	Revision 2.8  2007/01/11 17:21:35  paoscu
	sql to DateTime fixed.
	
	Revision 2.7  2006/11/21 19:05:25  paoscu
	YearToFraction3 added to DateTimeMode.
	
	Revision 2.6  2006/11/20 15:49:14  paoscu
	YEAR To FRACTION followed by (ciphers) - JIRA ACQ-342
	
	Revision 2.5  2006/11/02 18:49:04  paoscu
	Removed an ending '-' whene converting DateTime to YearToSecond string.
	
	Revision 2.4  2006/07/14 12:22:48  paoscu
	String to be converted into a datetime can have quotes.
	
	Revision 2.3  2006/06/26 12:58:14  marpas
	uses new DateTime
	
	Revision 2.2  2006/06/22 14:36:44  marpas
	temporary version, aligned with new libDateTime I/F
	
	Revision 2.1  2006/05/12 17:36:43  paoscu
	boolean conv fixed.
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2006/02/21 16:32:00  paoscu
	Added support for unsigned long long.
	
	Revision 1.8  2006/01/10 18:23:30  marpas
	string cast operators fixed
	
	Revision 1.7  2005/12/22 18:47:44  marpas
	cast operator improved
	uses string delimiter from actual dialect to convert strings
	
	Revision 1.6  2005/11/21 14:51:12  clanas
	Error changed to ConvError
	
	Revision 1.5  2005/10/06 21:12:20  paoscu
	signed long long int fixed.
	long double added
	
	Revision 1.4  2005/10/06 19:50:06  paoscu
	signed long long int added.
	
	Revision 1.3  2005/07/19 13:14:13  enrcar
	Boolean operators now got from Dialect
	
	Revision 1.2  2005/07/13 09:25:07  enrcar
	Updated to dbClient version 1.14
	
	Revision 1.14  2005/06/17 13:48:01  marpas
	YearToFraction5 mode introduced - even if it is supported up to msec (last 2 digit will be 0).
	
	Revision 1.13  2004/12/02 12:39:28  marpas
	headers fixed
	
	Revision 1.12  2004/08/26 16:22:09  marpas
	minor change in member initialization
	
	Revision 1.11  2004/04/13 08:30:54  marpas
	documentation updated
	
	Revision 1.10  2003/10/28 15:13:09  paoscu
	YearToFractionExtended mode added for DateTime.
	
	Revision 1.9  2003/07/11 13:08:02  marpas
	exException::clone implemented
	
	Revision 1.8  2003/05/13 15:05:44  paoscu
	Function convert introduced.
	
	Revision 1.7  2003/04/30 13:36:04  marpas
	using namespace std partially removed because informix headers
	
	Revision 1.6  2003/04/15 20:21:37  marpas
	GCC3.2
	
	Revision 1.5  2003/02/20 17:42:25  paoscu
	numerical check now works (integral/floating not literal)
	
	Revision 1.4  2003/02/11 17:28:35  paoscu
	Debug functionality added in dbGeoObject constructor.
	
	Revision 1.3  2003/01/24 11:33:00  paoscu
	dbRow diagnostic improved.
	
	Revision 1.2  2002/10/17 16:17:32  paoscu
	Fixed a bug in dbConv::dbConv(const unsigned long &v)
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/


#include <dbConv.h>
#include <DateTime.h>
#include <dbGeoObject.h>
#include <SQLDialectFactory.h>
#include <SQLDialect.h>
#include <Filterables.h>
#include <sstream>
#include <iomanip>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbConv) 

dbConv::NullValue dbConv::Null ;

dbConv::dbConv() : _str() {}

dbConv::~dbConv() throw() { }


dbConv::dbConv(const dbConv &r) : _str(r._str) {}

dbConv &dbConv::operator=(const dbConv &v)
{
	if (this != &v) {
		_str = v._str ;
	}
	return *this ;
}


dbConv::dbConv(const std::string &key, const bool &v) :
	_str(v ? // PRQA S 3380
         SQLDialectFactory::sqlDialect(key).boolTrue() : 
         SQLDialectFactory::sqlDialect(key).boolFalse())
{
}
	
dbConv::dbConv(const std::string &key, const NullValue &v) :
	_str(SQLDialectFactory::sqlDialect(key).nullValue())
{
}
	
dbConv::dbConv(const std::string &key, const char *v) : _str(v)
{
	quotecheck(key) ;
}

dbConv::dbConv(const std::string &key, char v) :
    _str(SQLDialectFactory::sqlDialect(key).delimit(string(1,v))) 

{
}
	

dbConv::dbConv(const std::string &key, const string &v) : _str(v)
{
	quotecheck(key) ;
}


dbConv::dbConv(const std::string &key, const DateTime &v, DateTimeMode mode) :
    _str()
{
	ostringstream os  ;
	switch(mode) {
		
	case JD50: // a JD50 is a double
		{
			DateTime::JD50 jd = v ; // PRQA S 3050
			os << setprecision(24) << jd.jd() ; // PRQA S 4400
			break ;
		}
	case Year: // only the year as in '2009'
		{
			DateTime::UTCD utcd = v ; // PRQA S 3050
			os << "'" << setw(4) << setfill('0') << utcd.year << "'" ; // PRQA S 4400
			break ;
		}
	case YearToMonth: // year and month as in '2009-12'
		{
			DateTime::UTCD utcd = v ; // PRQA S 3050
			os << "'" << setw(4) << setfill('0') << utcd.year << '-'  // PRQA S 4400 2
			   << setw(2) << utcd.month << "'" ;
			break ;
		}
	case YearToDay: // year, month and day as in '2009-12-31'
		{
			DateTime::UTCD utcd = v ; // PRQA S 3050
			os << "'" << setw(4) << setfill('0') << utcd.year   // PRQA S 4400 3
			   << '-' << setw(2) << utcd.month 
			   << '-' << setw(2) << utcd.day << "'" ;
			break ;
		}
	case YearToSecond: // up to seconds 
		{
			os << "'" << v.getSqlString(0) << "'" ;
			break ;
		}
	case YearToFraction: // up to fraction of seconds
		{
            unsigned short maxf = SQLDialectFactory::sqlDialect(key).getTimeStampMaxDecimalDigits() ;
			unsigned short frac = DateTime::getDefaultDbFractionDigits() ;
			if (frac > maxf) { // limit to maxf
				frac = maxf ;
            }
			os << "'" << v.getSqlString(frac) << "'" ; // PRQA S 3000
			break ;
		}
	case YearToFraction5: 
		{
			os << "'" << v.getSqlString(5) << "'" ; // PRQA S 4400
			break ;
		}
	case YearToFraction6:
		{
			os << "'" << v.getSqlString(6) << "'" ; // PRQA S 4400
			break ;
		}
	case YearToFraction3:
		{
			os << "'" << v.getSqlString(3) << "'" ; // PRQA S 4400
			break ;
		}
	case YearToFractionExtended:
        {
        
            unsigned short maxf = SQLDialectFactory::sqlDialect(key).getTimeStampMaxDecimalDigits() ;
			unsigned short frac = DateTime::getDefaultDbFractionDigits() ;
			if (frac > maxf) { // limit to 6 // PRQA S 3000, 4400 2
				frac = maxf ;
            }

            string dialect = SQLDialectFactory::dialectName(key) ;
			if( dialect == "Postgres8" || dialect == "Oracle11" ) {
				// Postgres: TIMESTAMP ' ... '
				os << "TIMESTAMP '" << v.getSqlString(frac) << "'" ; // PRQA S 3000
			}
			else if (dialect == "Informix9") {
				// Informix: DATETIME( ... ) YEAR TO FRACTION(..)
				os << "DATETIME(" << v.getSqlString(frac) << ") YEAR TO FRACTION(" << frac << ")" ; // PRQA S 3000
			}
			else {
				ACS_THROW( exIllegalValueException(string("Unknown db: ")+dialect )); // PRQA S 3081
            }
            break ;
		}
	}		
	_str = os.str() ; // the final string
}


dbConv::dbConv(const string &key, const dbGeoObject &v) :
    _str()
{
	ostringstream os ;
    const string &dialect = SQLDialectFactory::dialectName(key) ;
	if (dialect != "Oracle11") {
		os << '\'' ;
    }
	v.getSqlString(os,SQLDialectFactory::dialectName(key)) ;
	if (dialect != "Oracle11") {
		os << '\'' ;
    }
	_str = os.str() ; // the complete geoobject surronded by single quotes (not on Oracle)
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "dbGeoObject, _str is\"" << _str << "\" .") ;
}


#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY

/* These code is deeply REDUNDANT -- FIXME */

dbConv::dbConv(const bool &v) :
    _str( v ? // PRQA S 3380
          SQLDialectFactory::sqlDialect("").boolTrue() : 
          SQLDialectFactory::sqlDialect("").boolFalse())
{
}
	
dbConv::dbConv(const NullValue &v) :
    _str (SQLDialectFactory::sqlDialect("").nullValue())
{
}
	
dbConv::dbConv(const char *v) : _str(v)
{
	quotecheck("") ; // default key
}

dbConv::dbConv(char v):
    _str(SQLDialectFactory::sqlDialect("").delimit(string(1,v))) 
{}
	

dbConv::dbConv(const string &v) : _str(v)
{
	quotecheck("") ; // default key
}


dbConv::dbConv(const DateTime &v, DateTimeMode mode):
    _str()
{
	const string def_key = "" ; 

	ostringstream os  ;
	switch(mode) {
		
	case JD50: // a JD50 is a double
	{
		DateTime::JD50	jd = v ; // PRQA S 3050
		os << setprecision(24) << jd.jd() ; // PRQA S 4400
		break ;
	}
	case Year: // only the year as in '2009'
		{
			DateTime::UTCD utcd = v ; // PRQA S 3050
			os << "'" << setw(4) << setfill('0') << utcd.year << "'" ; // PRQA S 4400
		    break ;
		}
	case YearToMonth: // year and month as in '2009-12'
		{
			DateTime::UTCD utcd = v ; // PRQA S 3050
			os << "'" << setw(4) << setfill('0') << utcd.year << '-'  // PRQA S 4400 2
			   << setw(2) << utcd.month << "'" ;
		    break ;
		}
	case YearToDay: // year, month and day as in '2009-12-31'
		{
			DateTime::UTCD utcd = v ; // PRQA S 3050
			os << "'" << setw(4) << setfill('0') << utcd.year  // PRQA S 4400 3
			   << '-' << setw(2) << utcd.month 
			   << '-' << setw(2) << utcd.day << "'" ;
		    break ;
		}
	case YearToSecond: // up to seconds 
		{
			os << "'" << v.getSqlString(0) << "'" ;
		    break ;
		}
	case YearToFraction: // up to fraction of seconds
		{
            unsigned short maxf = SQLDialectFactory::sqlDialect(def_key).getTimeStampMaxDecimalDigits() ;
			unsigned short frac = DateTime::getDefaultDbFractionDigits() ;
			if (frac > maxf) { // limit to maxf
				frac = maxf ;
            }
			os << "'" << v.getSqlString(frac) << "'" ; // PRQA S 3000
		    break ;
		}
	case YearToFraction5: 
		{
			os << "'" << v.getSqlString(5) << "'" ; // PRQA S 4400
		    break ;
		}
	case YearToFraction6:
		{
			os << "'" << v.getSqlString(6) << "'" ; // PRQA S 4400
			break ;
	    }
	case YearToFraction3:
		{
			os << "'" << v.getSqlString(3) << "'" ; // PRQA S 4400
			break ;
	    }
	case YearToFractionExtended:
        {
            unsigned short maxf = SQLDialectFactory::sqlDialect(def_key).getTimeStampMaxDecimalDigits() ;
			unsigned short frac = DateTime::getDefaultDbFractionDigits() ;
			if (frac > maxf) { // limit to 6 // PRQA S 3000, 4400 2
				frac = maxf ;
            }

            string dialect = SQLDialectFactory::dialectName(def_key) ;
			if( dialect == "Postgres8" || dialect == "Oracle11" ) {
				// Postgres: TIMESTAMP ' ... '
				os << "TIMESTAMP '" << v.getSqlString(frac) << "'" ; // PRQA S 3000
			}
			else if (dialect == "Informix9" ) {
				// Informix: DATETIME( ... ) YEAR TO FRACTION(..)
				os << "DATETIME(" << v.getSqlString(frac) << ") YEAR TO FRACTION(" << frac << ")" ;// PRQA S 3000
			}
			else {
				ACS_THROW( exIllegalValueException(string("Unknown db: ")+dialect )); // PRQA S 3081
            }
            break ;
        }
	}		
	_str = os.str() ; // the final string
}

#endif

// PRQA S 2017 EOF

/* In the following constructors, the key is actually not needed: */

dbConv::dbConv(const short &v):
    _str()
{
	ostringstream os  ;
	os << v ;
	_str = os.str() ;
}

dbConv::dbConv(const unsigned short &v):
    _str()
{
	ostringstream os  ;
	os << v ;
	_str = os.str() ;
}

dbConv::dbConv(const int &v):
    _str()
{
	ostringstream os  ;
	os << v ;
	_str = os.str() ;
}

dbConv::dbConv(const unsigned int &v):
    _str()
{
	ostringstream os  ;
	os << v ;
	_str = os.str() ;
}

dbConv::dbConv(const float &v):
    _str()
{
	ostringstream os  ;
	os << setprecision(8) << v ; // PRQA S 4400
	_str = os.str() ;
}

dbConv::dbConv(const long &v):
    _str()
{
	ostringstream os  ;
	os << v ; 
	_str = os.str() ;
}

dbConv::dbConv(const signed long long int &v):
    _str()
{
	ostringstream os  ;
	os << v ; 
	_str = os.str() ;
}


dbConv::dbConv(const unsigned long long int &v):
    _str()
{
	ostringstream os  ;
	os << v ; 
	_str = os.str() ;
}

dbConv::dbConv(const unsigned long &v):
    _str()
{
	ostringstream os  ;
	os << v ; 
	_str = os.str() ;
}

dbConv::dbConv(const double &v):
    _str()
{
	ostringstream os  ;
	os << setprecision(16) << v ; // PRQA S 4400
	_str = os.str() ;
}

dbConv::dbConv(const long double &v):
    _str()
{
	ostringstream os  ;
	os << setprecision(24) << v ; // PRQA S 4400
	_str = os.str() ;
}

dbConv::dbConv(const string &key_unused, const short &v):
    _str()
{
	ostringstream os  ;
	os << v ;
	_str = os.str() ;
}

dbConv::dbConv(const string &key_unused, const unsigned short &v):
    _str()
{
	ostringstream os  ;
	os << v ;
	_str = os.str() ;
}

dbConv::dbConv(const string &key_unused, const int &v):
    _str()
{
	ostringstream os  ;
	os << v ;
	_str = os.str() ;
}

dbConv::dbConv(const string &key_unused, const unsigned int &v):
    _str()
{
	ostringstream os  ;
	os << v ;
	_str = os.str() ;
}

dbConv::dbConv(const string &key_unused, const float &v):
    _str()
{
	ostringstream os  ;
	os << setprecision(8) << v ; // PRQA S 4400
	_str = os.str() ;
}

dbConv::dbConv(const string &key_unused, const long &v):
    _str()
{
	ostringstream os  ;
	os << v ; // PRQA S 4400
	_str = os.str() ;
}

dbConv::dbConv(const string &key_unused, const signed long long int &v):
    _str()
{
	ostringstream os  ;
	os << v ; // PRQA S 4400
	_str = os.str() ;
}


dbConv::dbConv(const string &key_unused, const unsigned long long int &v):
    _str()
{
	ostringstream os  ;
	os << v ; // PRQA S 4400
	_str = os.str() ;
}

dbConv::dbConv(const string &key_unused, const unsigned long &v):
    _str()
{
	ostringstream os  ;
	os << v ; // PRQA S 4400
	_str = os.str() ;
}

dbConv::dbConv(const string &key_unused, const double &v):
    _str()
{
	ostringstream os  ;
	os << setprecision(16) << v ; // PRQA S 4400
	_str = os.str() ;
}

dbConv::dbConv(const string &key_unused, const long double &v) :
    _str()
{
	ostringstream os  ;
	os << setprecision(24) << v ; // PRQA S 4400
	_str = os.str() ;
}


void dbConv::quotecheck(const string &key) 
{
    _str= SQLDialectFactory::sqlDialect(key).delimit(_str) ; 
}


dbConv::operator const std::string &() const { return _str ; } 


	
	
bool dbConv::operator==(const dbConv &v) const { return _str == v._str ; }
bool dbConv::operator!=(const dbConv &v) const { return _str != v._str ; }



_ACS_END_NAMESPACE

