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
	Revision 5.0  2013/07/11 16:07:12  marpas
	libException 5.x standards adopted
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.4  2013/05/14 18:00:35  marpas
	implementig PostGRES/PostGIS support
	
	Revision 2.3  2012/02/13 11:15:40  marpas
	refactoring in progress
	
	Revision 2.2  2010/04/09 17:23:22  marpas
	rules applied in preparation for ORACLE SPATIAL porting
	
	Revision 2.1  2009/06/26 09:48:18  crivig
	intersection improved; no exception from destructor; closed flag passed to intersect methods
	
	Revision 2.0  2006/02/28 08:43:59  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/10/22 19:26:14  marpas
	dbGeoObject  parser implemented
	
	Revision 1.4  2004/12/30 21:07:35  marpas
	header fixed
	
	Revision 1.3  2003/04/30 17:21:18  marpas
	using namespace std no longer into acs includes
	
	Revision 1.2  2003/04/15 18:35:04  marpas
	GCC3.2
	
	Revision 1.1.1.1  2002/10/30 17:08:20  paoscu
	Import libDbGeo
	

*/


#include <dbGeoStationSet.h>
#include <dbGeoStation.h>
#include <rsResourceSet.h>
#include <Filterables.h>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/  dbGeoStationSet   _/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//  _/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbGeoStationSet)

dbGeoStationSet::dbGeoStationSet() :_stations()
{
}


dbGeoStationSet::~dbGeoStationSet() throw() { }

void dbGeoStationSet::clear()
{
	_stations.clear() ;
}


void dbGeoStationSet::fill(const rsResourceSet &rs) // PRQA S 4020
{
	string *st = 0 ;
	try {
		// get the repository .. 
		string path ;
		rs.getValue("stationsRepository", path) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "stationsRepository: " << path) ;
		// ... and the lenght of the stations array
		size_t nst = rs.getArraySize("station") ; // PRQA S 4412
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "stations: " << nst) ;
		if (0 == nst)  {
			return ;
        }
			
		// allocate an array of strings 
		st = new string[nst] ;
		// fill it
		rs.getArray("station",st,nst) ;
		
		// and now load each station
		for (size_t i=0; i < nst; i++) { // PRQA S 3000
			try {
				dbGeoStation curr = dbGeoStation(st[i]) ; // PRQA S 3081
				curr.load(path+"/"+st[i]) ;
				_stations.push_back(curr) ;
			}
			catch(exException &e) {
				ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
					ACS_LOG_NOTIFY_EX(e) ;
				ACS_CLASS_END_DEBUG

				// ignore it ;
			}
		}
		
		delete [] st ;
	}
	catch(exception &e) {
		delete [] st ;
		clear() ;
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
            ACS_LOG_NOTIFY_EX(e) ;
        ACS_CLASS_END_DEBUG
		
		// ignore it ;
	}
}


size_t dbGeoStationSet::size() const 
{
	return _stations.size() ;
}

const dbGeoStation &dbGeoStationSet::operator[](size_t idx) const 
{
	if (idx >= size()) { // out of array
		ostringstream os ;
		os << "dbGeoStationSet::operator[]" << idx << " greater or equal to " << size() ;
		
		ACS_THROW(exBoundsException(os.str())) ; // PRQA S 3081
	}
	
	// return the station
	return _stations[idx] ;
}

	
dbGeoStation &dbGeoStationSet::operator[](size_t idx) 
{
	if (idx >= size()) {
		ostringstream os ;
		os << "dbGeoStationSet::operator[]" << idx << " greater or equal to " << size() ;
		
		ACS_THROW(exBoundsException(os.str())) ; // PRQA S 3081
	}
	
	// return the station
	return _stations[idx] ;
}


_ACS_END_NAMESPACE

