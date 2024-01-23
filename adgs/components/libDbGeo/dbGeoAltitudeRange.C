// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Database Geo Object Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2014/05/09 11:39:54  marpas
	coding best practices applied
	qa & c++ warnings fixed
	last throwing macros for exceptions adopted
	
	Revision 5.0  2013/07/11 16:07:11  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.6  2013/05/14 18:00:34  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.5  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.4  2011/09/29 14:48:09  marpas
	simplifying templates for codecheck
	
	Revision 2.3  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.2  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.1  2009/03/02 12:38:18  marpas
	CODECHECK warning and errors fixed - no regressions
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.6  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.5  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.4  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.3  2003/04/15 20:40:43  marpas
	GCC3.2
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#include <dbGeoAltitudeRange.h>
#include <iomanip>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

dbGeoAltitudeRange::dbGeoAltitudeRange() throw() :
	_lower(),
	_upper()  
{ 
}

dbGeoAltitudeRange::dbGeoAltitudeRange(double l, double u) throw() :
	_lower(l),
	_upper(u)  
{  
}


dbGeoAltitudeRange::~dbGeoAltitudeRange() throw() { }

bool operator==(const dbGeoAltitudeRange & a, const dbGeoAltitudeRange & b) throw() // PRQA S 4020
{
    if ((a.lower() > b.lower()) || (a.lower() < b.lower())) { return false ; } 
    if ((a.upper() > b.upper()) || (a.upper() < b.upper())) { return false ; } 
	return true ;
}

bool operator!=(const dbGeoAltitudeRange & a, const dbGeoAltitudeRange & b) throw() 
{
	return not (a == b) ;
}


dbGeoAltitudeRange::dbGeoAltitudeRange(const dbGeoAltitudeRange &v) throw() :
    _lower(v._lower),
	_upper(v._upper)

{  
	// empty
}

dbGeoAltitudeRange &dbGeoAltitudeRange::operator=(const dbGeoAltitudeRange &v) throw() 
{  
	if (this != &v) {
		_lower = v._lower ;
		_upper = v._upper ;
	}
	
	return *this ;
}


ostream & dbGeoAltitudeRange::getSqlString(ostream &os) const 
{
	if(_upper.isFilled() && _lower.isFilled()) { // both filled

		os << '(' << fixed << setprecision(8) << minAltitude(_lower,_upper) // PRQA S 4400
		   << ", " 
		   << maxAltitude(_upper,_lower) 
		   << ')' ;
    }
	else if(_lower.isFilled()) { // only one  
		os << fixed << setprecision(8) << '(' << _lower.altitude() << ')' ; // PRQA S 4400
    }
	else if(_upper.isFilled()) { // only one
		os << fixed << setprecision(8) << '(' << _upper.altitude() << ')' ; // PRQA S 4400
    }
	else { // none
		os << "ANY" ;
    }
		
	return os ; // PRQA S 4028
}

void dbGeoAltitudeRange::exportObj(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"dbGeoAltitudeRange") ;	
	{
		rsPushNSpace ns(s,"_lower") ; // lower alt		
		_lower.exportObj(s) ;
	}
	{
		rsPushNSpace ns(s,"_upper") ; // upper alt		
		_upper.exportObj(s) ;
	}
}

void dbGeoAltitudeRange::importObj(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"dbGeoAltitudeRange") ;
	{
		rsPushNSpace ns(s,"_lower") ; // lower alt		
		_lower.importObj(s) ;
	}
	{
		rsPushNSpace ns(s,"_upper") ; // upper alt		
		_upper.importObj(s) ;
	}
}

void dbGeoAltitudeRange::exportXML(XMLOstream &xst) const
{
	XMLOstream::Tag txml(xst,"dbGeoAltitudeRange") ;	
	{
		XMLOstream::Tag txml2(xst,"_lower") ; // lower alt	
		_lower.exportXML(xst) ;
	}
	{
		XMLOstream::Tag txml3(xst,"_upper") ; // upper alt	
		_upper.exportXML(xst) ;
	}
}


double dbGeoAltitudeRange::minAltitude(const dbGeoAltitude &a, const dbGeoAltitude &b) throw() 
{ 
	return (a.altitude() < b.altitude()) ? a.altitude() : b.altitude() ; // PRQA S 3382
}

double dbGeoAltitudeRange::maxAltitude(const dbGeoAltitude &a, const dbGeoAltitude &b) throw() 
{ 
	return (a.altitude() > b.altitude()) ? a.altitude() : b.altitude() ; // PRQA S 3382 
}

std::istream &
operator >> (std::istream &is, dbGeoAltitudeRange& ar) // PRQA S 4020
{
	if (!is.good()) { return is ; } // PRQA S 4028
	
	char ell ; // PRQA S 4101
	is >> ell ; 
	if (ell == '(') {
		char comma ; // PRQA S 4101 3
		double l; 
        double L ;
		is >> l >> comma >> L >> ell ;
		if (!is.fail()) {
			ar.lower(l) ;
			ar.upper(L) ;
		}
		return is ; // PRQA S 4028
	}
	
	if ((ell == 'A') || (ell == 'a')) {
		char n ; // PRQA S 4101
		is.get(n) ;
		if (is.fail()) {
			return is ; // PRQA S 4028
        }
		if (!((n == 'N') || (n=='n'))) {
			is.putback(n) ;
			is.setstate(std::ios::failbit) ;
		}
			
		char y ; // PRQA S 4101
		is.get(y) ;
		if (is.fail()) {
			return is ; // PRQA S 4028
        }
		if (!((y == 'Y') || (y=='y'))) {
			is.putback(y) ;
			is.setstate(std::ios::failbit) ;
		}
		
		ar = dbGeoAltitudeRange() ;
		return is ; // PRQA S 4028
	}
	
	is.putback(ell) ;
	is.setstate(std::ios::failbit) ;
	return is ; // PRQA S 4028
}

_ACS_END_NAMESPACE
