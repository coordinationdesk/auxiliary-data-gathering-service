// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	Revision 5.5  2016/04/11 13:11:41  marpas
	toSQL now accept a const char * also (overloaded)
	
	Revision 5.4  2013/09/17 15:26:38  marpas
	qa warnings fixed
	
	Revision 5.3  2013/08/28 15:52:22  marfav
	added db::toSQL to allow easier use of dbconv also with connections and pools
	QT related code removed
	
	Revision 5.2  2013/07/12 08:57:18  marpas
	deprecating methods with no dbkey
	
	Revision 5.1  2013/06/11 17:02:59  marpas
	adopting libException 5.x standards - work in progress
	
	Revision 5.0  2013/06/06 18:52:58  marpas
	adopting libException 5.x standards
	
	Revision 4.5  2013/05/14 18:13:41  marpas
	when using dbGeoObject, dbkey connection is mandatory now
	
	Revision 4.4  2012/12/11 17:30:11  marpas
	qa rules
	
	Revision 4.3  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.1  2012/02/17 13:55:14  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:14  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:51  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.9  2009/06/22 12:36:35  marpas
	robustness improved
	got rid of useless ifdef and warning
	useless code removed
	test compilation fixed
	
	Revision 2.8  2009/03/05 11:35:07  marpas
	CODECHECK parsing: no regressions
	
	Revision 2.7  2009/03/04 17:50:04  marpas
	CODECHECK parsing improved
	
	Revision 2.6  2009/03/02 14:03:30  marpas
	CODECHECK parsing, no regressions
	
	Revision 2.5  2009/02/18 11:50:19  marpas
	template convenience method nullConv specialized for DateTime
	
	Revision 2.4  2009/02/18 11:37:57  marpas
	istatic template convenience method added
	
	Revision 2.3  2008/12/09 16:11:23  marpas
	dbConv::Null implemented to allow setting Null values using SQLString
	
	Revision 2.2  2008/05/14 15:04:17  crivig
	yearToFraction6 introduced
	
	Revision 2.1  2006/11/21 19:05:25  paoscu
	YearToFraction3 added to DateTimeMode.
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2006/02/21 16:32:00  paoscu
	Added support for unsigned long long.
	
	Revision 1.7  2006/01/10 18:23:30  marpas
	string cast operators fixed
	
	Revision 1.6  2005/12/22 18:47:44  marpas
	cast operator improved
	uses string delimiter from actual dialect to convert strings
	
	Revision 1.5  2005/11/21 14:51:12  clanas
	Error changed to ConvError
	
	Revision 1.4  2005/10/06 21:12:20  paoscu
	signed long long int fixed.
	long double added
	
	Revision 1.3  2005/10/06 19:50:06  paoscu
	signed long long int added.
	
	Revision 1.2  2005/07/13 09:24:43  enrcar
	updated to dbClient version 1.9
	
	Revision 1.9  2005/06/17 13:48:01  marpas
	YearToFraction5 mode introduced - even if it is supported up to msec (last 2 digit will be 0).
	
	Revision 1.8  2004/12/02 12:39:28  marpas
	headers fixed
	
	Revision 1.7  2003/10/28 15:13:09  paoscu
	YearToFractionExtended mode added for DateTime.
	
	Revision 1.6  2003/07/11 13:08:02  marpas
	exException::clone implemented
	
	Revision 1.5  2003/05/13 15:05:44  paoscu
	Function convert introduced.
	
	Revision 1.4  2003/04/30 13:36:04  marpas
	using namespace std partially removed because informix headers
	
	Revision 1.3  2003/02/11 17:28:35  paoscu
	Debug functionality added in dbGeoObject constructor.
	
	Revision 1.2  2003/01/24 11:33:00  paoscu
	dbRow diagnostic improved.
	
	Revision 1.1.1.1  2002/10/02 09:58:02  marpas
	Import libDbClient
	

*/


#ifndef _dbConv_H_
#define _dbConv_H_ 

#include <dbConnection.h>

#include <exException.h>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

//
//
//
// class dbConv
//
//
//

class dbGeoObject ;
class DateTime ;

class dbConv // PRQA S 2109 
{
public:

	/*! class ConvError declaration */
	exDECLARE_EXCEPTION(ConvError,exException) ;    // PRQA S 2131, 2502

	class NullValue { // convenience class to define a Null type 
	
	} ;

	enum DateTimeMode{
		
		JD50,
		Year,
		YearToMonth,
		YearToDay,
		YearToSecond,

		/*! Number of digits is automatically evaluated */
		YearToFraction,

		/**
		 * Number of digits is automatically evaluated DATETIME constructor called explicitly.
		 * Probably dangerous to use.
		 */
		YearToFractionExtended,
		
		YearToFraction5 ,
		YearToFraction3 ,
		YearToFraction6
	} ;
		
	dbConv();
	dbConv& operator=(const dbConv &);	
	dbConv(const dbConv &);	//copy constructor

	dbConv(const std::string& key, const NullValue &);
	dbConv(const std::string& key, const bool&);
	dbConv(const std::string& key, const char *);
	dbConv(const std::string& key, char);
	
	dbConv(const std::string &key, const std::string&);
	dbConv(const std::string &key, const DateTime &v ,DateTimeMode mode = JD50);
	dbConv(const std::string &key, const dbGeoObject &);	
	
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
	explicit __attribute__((deprecated)) dbConv(const NullValue &);  
	explicit __attribute__((deprecated)) dbConv(const bool&);
	explicit __attribute__((deprecated)) dbConv(const char *);
	explicit __attribute__((deprecated)) dbConv(char);
	explicit __attribute__((deprecated)) dbConv(const std::string&);
	explicit __attribute__((deprecated)) dbConv(const DateTime &v ,DateTimeMode mode = JD50);
#endif	
	
	/* Key is not actally used in the constructors below -- the "keyed" version is provided just for compatibility with templates */
	explicit __attribute__((deprecated)) dbConv(const short&);
	explicit __attribute__((deprecated)) dbConv(const unsigned short&);
	explicit __attribute__((deprecated)) dbConv(const int&);
	explicit __attribute__((deprecated)) dbConv(const unsigned int&);
	explicit __attribute__((deprecated)) dbConv(const long&);
	explicit __attribute__((deprecated)) dbConv(const signed long long int&);
	explicit __attribute__((deprecated)) dbConv(const unsigned long long int&);
	explicit __attribute__((deprecated)) dbConv(const unsigned long&);
	explicit __attribute__((deprecated)) dbConv(const float&);
	explicit __attribute__((deprecated)) dbConv(const double&);
	explicit __attribute__((deprecated)) dbConv(const long double&);
	dbConv(const std::string &key, const short&);
	dbConv(const std::string &key, const unsigned short&);
	dbConv(const std::string &key, const int&);
	dbConv(const std::string &key, const unsigned int&);
	dbConv(const std::string &key, const long&);
	dbConv(const std::string &key, const signed long long int&);
	dbConv(const std::string &key, const unsigned long long int&);
	dbConv(const std::string &key, const unsigned long&);
	dbConv(const std::string &key, const float&);
	dbConv(const std::string &key, const double&);
	dbConv(const std::string &key, const long double&);
	
	
	operator const std::string &() const ; // PRQA S 2181
	
	bool operator==(const dbConv &) const ; // PRQA S 2070 2
	bool operator!=(const dbConv &) const ;

	dbConv(dbConv&);
	
	~dbConv() throw() ;

	 
	static NullValue Null ;     // PRQA S 2100
	
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
	template <typename T> static std::string __attribute__((deprecated)) nullConv(const std::string &, bool, const T &) ;
#endif	
	template <typename T> static std::string nullConv(const std::string &key, const std::string &, bool, const T &) ;

private:
	void quotecheck(const std::string &key) ;
	
private:
	std::string _str ; 
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbConv) ;
} ; 
// PRQA S 2134 L1
#ifndef ACS_DBCLIENT_PREVENT_DEFAULTKEY
template <typename T> inline 
std::string dbConv::nullConv(const std::string & pre, bool vSet, const T &v) { 
	const std::string def_key = "" ; 
	return nullConv(def_key, pre, vSet, v) ;
}
#endif

template <typename T> 
inline 
std::string dbConv::nullConv(const std::string &key, const std::string & pre, bool vSet, const T &v) 
{ 
    return pre + (vSet ? std::string(dbConv(key, v)) : std::string("NULL")) + " " ; // PRQA S 3050, 3081, 3383, 3385
}


template <> 
inline 
std::string dbConv::nullConv<DateTime>(const std::string &key, const std::string & pre, bool vSet, const DateTime &v) // PRQA S 2133
{ 
    return pre + (vSet ? std::string(dbConv(key, v,YearToFraction)) : std::string("NULL")) + " " ; // PRQA S 3050, 3081, 3383, 3385
} 

_ACS_BEGIN_NAMESPACE(db)


/*! \addtogroup db 
 *  @{
 */

template <typename convType__> 
inline
const std::string nullConv (const std::string& key, const std::string & pre, bool vSet, const convType__& element )
{
    return dbConv::nullConv (key, pre, vSet, element);
}

template <typename keyType__, typename convType__>
inline
const std::string nullConv (const keyType__& keyContainer, const std::string & pre, bool vSet, const convType__& element )
{
    return dbConv::nullConv (keyContainer.getKey(), pre, vSet, element);
}

template <typename keyType__, typename convType__>
inline
const std::string toSQL (const keyType__& keyContainer, const convType__& element )
{
    return dbConv (keyContainer.getKey(), element); // PRQA S 3050
}

template <typename convType__>
inline
const std::string toSQL (const std::string& key, const convType__& element )
{
    return dbConv (key, element); // PRQA S 3050
}

template <typename convType__>
inline
const std::string toSQL (const char *k, const convType__& element ) // PRQA S 4020
{
    if (0 == k) {
        return dbConv (std::string(""), element); // PRQA S 3050, 3081
    }
    else {
        return dbConv (std::string(k), element); // PRQA S 3050, 3081
    }
}

inline 
const std::string toSQL (const std::string &key, const DateTime &v ,dbConv::DateTimeMode mode = dbConv::JD50) 
{
	return dbConv (key, v, mode); // PRQA S 3050
}

inline 
const std::string toSQL (const char *k, const DateTime &v ,dbConv::DateTimeMode mode = dbConv::JD50) // PRQA S 4020 
{
    if (0 == k) {
	    return dbConv (std::string(""), v, mode); // PRQA S 3050, 3081
    }
    else {
	    return dbConv (std::string(k), v, mode); // PRQA S 3050, 3081
    }
}

template <typename keyType__>
inline
const std::string toSQL (const keyType__& keyContainer, const DateTime &v , dbConv::DateTimeMode mode = dbConv::JD50)
{
	return dbConv (keyContainer.getKey(), v, mode); // PRQA S 3050
}
/*! @}*/


// PRQA L:L1
_ACS_END_NAMESPACE //db
_ACS_END_NAMESPACE //acs

#endif // _dbConv_H_
