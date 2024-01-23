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
	Revision 2.5  2013/05/14 18:00:34  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.4  2012/12/14 11:13:41  marpas
	coding best practices
	qa rules
	test improeved
	test coverage
	
	Revision 2.3  2012/02/13 11:15:39  marpas
	refactoring in progress
	
	Revision 2.2  2010/04/09 17:23:21  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.1  2009/06/26 09:48:17  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
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


#ifndef _dbGeoAltitude_H_
#define _dbGeoAltitude_H_

#include <acs_c++config.hpp>
#include <iostream>


_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet ;
class XMLOstream ;

class dbGeoAltitude 
{

public:

	dbGeoAltitude() throw() ;
	explicit dbGeoAltitude(double) throw() ;
	dbGeoAltitude(const dbGeoAltitude &) throw() ;
	~dbGeoAltitude() throw() ;
	
	dbGeoAltitude &operator=(const dbGeoAltitude &) throw() ;
	dbGeoAltitude &operator=(double) throw() ;
	operator double() const throw() ; // PRQA S 2181
	
	double altitude() const throw() ;
	void altitude(double) throw() ;
	
	std::ostream &getSqlString(std::ostream &) const ;

	bool isFilled() const throw() ;
	
	// inherited from dbExportable
	void exportObj(rsResourceSet &) const ;
	void importObj(rsResourceSet &) ;
	void exportXML(XMLOstream &) const ;

	friend std::ostream &operator<<(std::ostream &, const dbGeoAltitude &) ;  // PRQA S 2107 2
	friend std::istream &operator>>(std::istream &, dbGeoAltitude &) ; 
	
private :
	double _altitude ;
	bool _filled ;
} ;

_ACS_END_NAMESPACE

#endif

