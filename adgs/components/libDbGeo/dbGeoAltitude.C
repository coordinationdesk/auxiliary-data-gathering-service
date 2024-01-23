// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
	Revision 5.0  2013/07/11 16:07:11  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.5  2013/05/14 18:00:34  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.4  2012/02/13 11:15:39  marpas
	refactoring in progress
	
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
	
	Revision 1.3  2003/04/15 18:35:04  marpas
	GCC3.2
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/



#include <dbGeoAltitude.h>
#include <iomanip>
#include <sstream>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


dbGeoAltitude::dbGeoAltitude() throw() : 
	_altitude(0),
	_filled(0)
{  
}

dbGeoAltitude::dbGeoAltitude(double v) throw()  : 
	_altitude(v),
	_filled(1)
{
}

dbGeoAltitude::dbGeoAltitude(const dbGeoAltitude &v) throw() :
    _altitude(v._altitude),
    _filled(v._filled)
{
	//empty 
}



dbGeoAltitude::~dbGeoAltitude() throw() 
{
}


dbGeoAltitude &dbGeoAltitude::operator=(const dbGeoAltitude &v) throw() 
{ 
	if (this != &v) {
		_altitude = v._altitude ; 
		_filled = v._filled ;
	}
	
	return *this ;
}


dbGeoAltitude &dbGeoAltitude::operator=(double v) throw() 
{ 	
	_altitude = v ; 
	_filled = true ; 
	return *this ; 
}



void dbGeoAltitude::altitude(double v) throw() 
{ 
	_altitude = v ; 
	_filled = true ;
}



ostream &dbGeoAltitude::getSqlString(ostream &os) const 
{
	if(_filled) {
		os << fixed << setprecision(8) << _altitude << "::GeoAltitude" ; // PRQA S 4400
    }
	else {
		os << "ANY" ; // no altitude
    }
	return os ; // PRQA S 4028
}


void dbGeoAltitude::exportObj(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"dbGeoAltitude") ;	
	s.setValue("_altitude",_altitude) ;
	s.setValue("_filled",_filled) ;
}

void dbGeoAltitude::importObj(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"dbGeoAltitude") ;
	s.getValue("_altitude",_altitude) ;
	s.getValue("_filled",_filled) ;
}

void dbGeoAltitude::exportXML(XMLOstream &xst) const
{
	XMLOstream::Tag txml(xst,"dbGeoAltitude") ;	
	{
		ostringstream os;
		os << fixed << setprecision(17) << _altitude ; // PRQA S 4400
		XMLOstream::Tag txml2(xst,"_altitude",os.str()) ;	
	}
	{
		XMLOstream::Tag txml2(xst,"_filled",(_filled ? "true" : "false")) ; // PRQA S 3380	
	}
}

bool dbGeoAltitude::isFilled() const throw() { return _filled ;} // PRQA S 4120

double dbGeoAltitude::altitude() const throw() {return _altitude ;} // PRQA S 4120
dbGeoAltitude::operator double() const throw() { return _altitude ; } 


_ACS_END_NAMESPACE

