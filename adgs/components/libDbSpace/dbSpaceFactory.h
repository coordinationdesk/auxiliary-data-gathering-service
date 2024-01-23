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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.7  2013/07/22 17:51:06  marpas
	aligned to new libDbData interface
	
	Revision 1.6  2013/02/06 16:12:24  marpas
	some quality best practice applied
	some qa rule fixed
	
	Revision 1.5  2012/11/19 14:16:56  marpas
	HEAD REALIGNEMENT
	
	Revision 1.3  2012/02/13 16:50:12  marpas
	refactoring in progress
	
	Revision 1.2  2005/06/08 16:20:05  marpas
	headers fixed
	
	Revision 1.1  2002/11/06 13:27:20  danalt
	allineamento con dbCryosat
	

*/

#ifndef _dbSpaceFactory_H_
#define _dbSpaceFactory_H_ 

#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent;
class dbGeneralQuery;

class dbSpaceFactory // PRQA S 2109
{
public:
	dbSpaceFactory();
	~dbSpaceFactory() throw() ;

	static dbPersistent* buildDbSatRelation(const std::string &dbKey);
	static dbGeneralQuery* buildDbSatRelationQuery(const std::string &dbKey);

	static dbPersistent* buildDbStation(const std::string &dbKey);
	static dbGeneralQuery* buildDbStationQuery(const std::string &dbKey);

	static dbPersistent* buildDbSensor(const std::string &dbKey);
	static dbGeneralQuery* buildDbSensorQuery(const std::string &dbKey);

	static dbPersistent* buildDbSatellite(const std::string &dbKey);
	static dbGeneralQuery* buildDbSatelliteQuery(const std::string &dbKey);
private:
	dbSpaceFactory(const dbSpaceFactory &); // not implemented
	dbSpaceFactory& operator= (const dbSpaceFactory &); // not implemented

private:
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbSpaceFactory)

};

_ACS_END_NAMESPACE


#endif // _dbSpaceFactory_H_

