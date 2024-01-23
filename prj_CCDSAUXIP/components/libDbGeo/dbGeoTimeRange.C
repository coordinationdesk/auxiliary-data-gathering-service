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

	$Prod: A.C.S. Database Geo Object Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2014/04/08 16:47:21  marpas
	adopting libDateTime 5.13
	
	Revision 5.0  2013/07/11 16:07:13  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.6  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.5  2012/02/13 11:15:40  marpas
	refactoring in progress
	
	Revision 2.4  2011/09/29 14:48:10  marpas
	simplifying templates for codecheck
	
	Revision 2.3  2010/05/05 10:22:26  marpas
	DEBUG added
	useless code removed
	
	Revision 2.2  2010/04/09 17:23:22  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.1  2009/06/26 09:48:18  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.10  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.9  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.8  2004/08/03 13:49:57  marfav
	dbGeoTimeRange now inherits from libDateTime TimeRange class
	
	Revision 1.7  2004/05/11 08:19:34  marpas
	isFilled implemented
	
	Revision 1.6  2004/05/10 17:15:32  ivafam
	Method isFilled throw Exception.
	
	Revision 1.5  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.4  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.3  2003/02/13 14:28:00  marpas
	contains method added
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#include <dbGeoTimeRange.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <XMLOstream.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoTimeRange) 

dbGeoTimeRange::dbGeoTimeRange() : 
	TimeRange() // _start and _stop not filled
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}



dbGeoTimeRange::dbGeoTimeRange(const TimeRange &r) : 
	TimeRange(r)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}


dbGeoTimeRange::dbGeoTimeRange(
	const DateTime &start, 
	const DateTime &stop
) :
	TimeRange (start, stop)
{  
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}

dbGeoTimeRange::dbGeoTimeRange(const dbGeoTimeRange &gtr) : TimeRange(gtr)
{  
	// empty 
}

dbGeoTimeRange::~dbGeoTimeRange()  throw()
{
}


dbGeoTimeRange &dbGeoTimeRange::operator=(const dbGeoTimeRange &gtr) 
{
	if (this != &gtr) {
		TimeRange::operator=(gtr);
	}
	
	return *this ;
}


void dbGeoTimeRange::exportObj(rsResourceSet &s) const
{
	rsPushNSpace nsp(s,"dbGeoTimeRange") ;
	{ // add _start namespace
		rsPushNSpace ns(s,"_start") ;
		start().exportObj(s) ;
	}
	{ // add _stop namespace
		rsPushNSpace ns(s,"_stop") ;
		stop().exportObj(s) ;
	}
}

void dbGeoTimeRange::importObj(rsResourceSet &s)
{
	rsPushNSpace nsp(s,"dbGeoTimeRange") ;
	{ // add _start namespace
		rsPushNSpace ns(s,"_start") ;
        DateTime dt ;
        dt.importObj(s) ;
		start(dt) ;
	}
	{ // add _stop namespace
		rsPushNSpace ns(s,"_stop") ;
        DateTime dt ;
        dt.importObj(s) ;
		stop(dt) ;
	}
}

void dbGeoTimeRange::exportXML(XMLOstream &xst) const
{
	XMLOstream::Tag txml(xst,"dbGeoTimeRange") ;
	{	// add _start tag
		XMLOstream::Tag txml2(xst,"_start") ;
		start().exportXML(xst) ;
	}
	{ // add _stop tag
		XMLOstream::Tag txml2(xst,"_stop") ;
		stop().exportXML(xst) ;
	}
}




std::istream &
operator >> (std::istream &is, dbGeoTimeRange& b) // PRQA S 4020
{
	ACS_FUNC_ANNOUNCE_DEBUG(dbGeoTimeRange,ACS_LOW_VERB) ;

	if (!is.good()) { return is ; } // PRQA S 4028
	
	is >> static_cast<TimeRange &>(b) ; // PRQA S 3081

	ACS_FUNC_WRITE_DEBUG(dbGeoTimeRange,ACS_LOW_VERB, "b.isFilled(): " << boolalpha << b.isFilled()) ;
	return is ; // PRQA S 4028
}

_ACS_END_NAMESPACE
