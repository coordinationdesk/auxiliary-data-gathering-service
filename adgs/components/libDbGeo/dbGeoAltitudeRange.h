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
	Revision 5.0  2013/07/11 16:07:11  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.7  2013/05/14 18:00:34  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.6  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.5  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.4  2011/09/29 14:48:09  marpas
	simplifying templates for codecheck
	
	Revision 2.3  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.2  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.1  2006/03/30 10:41:58  marpas
	namespace does not need to be declared again inside
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.6  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.5  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.4  2003/09/03 13:57:43  paoscu
	Export methods are now const.
	
	Revision 1.3  2003/04/30 13:07:28  marpas
	using namespace std no longer in includes
	
	Revision 1.2  2002/11/29 17:48:46  danalt
	add exportXML, corrected some bugs
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#ifndef _dbGeoAltitudeRange_H_
#define _dbGeoAltitudeRange_H_

#include <dbGeoAltitude.h>
#include <iostream>


_ACS_BEGIN_NAMESPACE(acs)
class dbGeoAltitudeRange { // PRQA S 2109

public:

	dbGeoAltitudeRange() throw() ;
	dbGeoAltitudeRange(double,double) throw() ;
	dbGeoAltitudeRange(const dbGeoAltitudeRange &) throw() ;
	dbGeoAltitudeRange &operator=(const dbGeoAltitudeRange &) throw() ;
	~dbGeoAltitudeRange() throw() ;
	
	double lower() const throw() { return _lower ; } // PRQA S 3050
	void lower(double v ) throw() { _lower = v ; }
	
	double upper() const throw() { return _upper ; } // PRQA S 3050
	void upper(double v ) throw() { _upper = v ; }
	
	std::ostream &getSqlString(std::ostream &) const ;
	
	// inherited from dbExportable
	void exportObj(rsResourceSet &) const;
	void importObj(rsResourceSet &) ;
	void exportXML(XMLOstream &) const ;

private:
	static double minAltitude(const dbGeoAltitude&, const dbGeoAltitude&) throw() ;
	static double maxAltitude(const dbGeoAltitude&, const dbGeoAltitude&) throw() ;
	
private :
	dbGeoAltitude	_lower ;
	dbGeoAltitude	_upper ;
} ;



std::istream &operator >> (std::istream &, dbGeoAltitudeRange& ar) ;
bool operator==(const dbGeoAltitudeRange &, const dbGeoAltitudeRange &) throw() ;
bool operator!=(const dbGeoAltitudeRange &, const dbGeoAltitudeRange &) throw() ;

_ACS_END_NAMESPACE






#endif // _dbGeoAltitudeRange_H_

