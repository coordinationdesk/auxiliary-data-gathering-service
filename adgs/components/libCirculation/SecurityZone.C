// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2017, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 7.1  2017/10/16 16:49:00  marpas
 *  getting rid of libDCUtility
 *  using db::SmartQuery and rewriting persistent objects inheriting from dbSmartPersistent
 *
 *  Revision 7.0  2016/10/12 10:08:27  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.7  2016/03/26 21:32:47  marpas
 *  using new db I/F
 *
 *  Revision 5.6  2014/02/07 18:53:36  marpas
 *  adopting ACS_THROW and ACS_COND_THROW macros
 *
 *  Revision 5.5  2013/07/23 17:13:13  marpas
 *  compilations warning fixed
 *
 *  Revision 5.4  2013/07/18 12:29:51  marpas
 *  coding best practices application in progress
 *  type mismatch fix in progress
 *
 *  Revision 5.3  2013/07/15 17:21:12  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.2  2013/07/12 09:06:55  marpas
 *  porting circulation business intelligence into the library
 *
 *  Revision 5.1  2013/07/05 11:29:33  marpas
 *  findHostSecurityZone function added
 *  isHostInSecurityZone function added
 *  getHostSecurityZoneId function added
 *
 *  Revision 5.0  2013/06/07 15:49:02  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.13  2013/04/05 09:51:07  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.12  2013/02/25 16:51:19  marpas
 *  coding best practices applied
 *  interface reflects changes in low level libraries
 *
 *  Revision 1.11  2012/05/23 14:04:15  micmaz
 *  updated key type
 *
 *  Revision 1.10  2012/05/23 10:58:26  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *   http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.9  2012/05/22 10:02:25  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *  http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.8  2012/02/16 17:46:25  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.7  2012/02/16 16:00:12  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.6  2012/02/07 10:25:29  micmaz
 *  removed warnings.
 *
 *  Revision 1.5  2011/09/16 15:50:55  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.4  2011/07/21 17:44:40  micmaz
 *  updated Workspace, GeneralApplication and Entities.
 *
 *  Revision 1.3  2011/06/16 18:26:55  micmaz
 *  work in progress
 *
 *  Revision 1.2  2011/06/09 15:38:16  micmaz
 *  work in progress
 *
 *  Revision 1.1  2011/06/08 17:41:48  micmaz
 *  work in progress
 *
 *
 *
 *
 */

#include <SecurityZone.h>
#include <dbQuery.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)

using std::string ;

// constructor
SecurityZone::SecurityZone(dbConnection &conn) : dbSmartPersistent(table(), conn)
{
}


// copy constructor
SecurityZone::SecurityZone(const SecurityZone &o):
	dbSmartPersistent(o)
{
}


SecurityZone::~SecurityZone() throw()
{
}


// operator =
SecurityZone &SecurityZone::operator=(const SecurityZone &o)
{
	Lock lt(*this) ; // PRQA S 3050 2
	Lock lo(o) ;

	if(this != &o)
	{
		dbSmartPersistent::operator=(o) ;
	}
	return *this ;
}

string SecurityZone::str(dbConnection & conn, bool withId) const // PRQA S 4214
{
    std::ostringstream os;
    os << "{";
    os << "SecurityZone";
    if (withId) {
        os << "@";
        os << getId();
    }
    
    bool isNull = false ;
    string s ;
    int i = 0 ;
    s = getIpCidr(isNull, conn);
    os << ", ip_cidr: " << (isNull ? string("<null>") : s) ; // PRQA S 3081, 3380, 3385
    i = getLevel(isNull, conn);
    os << ", level: " << i ; 
    s = getSecurityzone(isNull, conn);
    os << ", zone: " << (isNull ? string("<null>") : s) ;  // PRQA S 3081, 3380, 3385
    os << "}";
    return os.str();
}








SecurityZone findHostSecurityZone(const std::string & ip_address, dbConnection &conn) 
{
    SecurityZoneQuery query(conn.getKey()) ;
	size_t items = query.doQuery(conn) ;
	// if here t_securityzones is empty
	ACS_THROW(dbQuery::QueryNoDataFound("no security zones configured")) ; // PRQA S 3081
    for (size_t i=0 ; i < items; ++i) {
        SecurityZone *ap = query[i] ;
        
        bool null = false ;
        string ip_cidr = ap->getIpCidr(null, conn) ;
        if (StringUtils::ipBelongsToSubnet(ip_cidr, ip_address))
		{
			return *ap ;
		}
    }

	ACS_THROW(dbQuery::QueryNoDataFound("no security zone found for host = "+ip_address)); // PRQA S 3081
}


dbPersistent::IdType getNeededSecurityZoneId(const std::string & ip_address, int hostLevel, dbConnection &conn) // PRQA S 4020 
{
    const dbSmartPersistent& secZone = findHostSecurityZone(ip_address, conn) ;
    int secZoneLevel = -1 ;
    bool null = false ;
    secZone.get("level", secZoneLevel, null, conn) ;
    if (not isHostInSecurityZone(hostLevel, secZoneLevel)) {
        return  secZone.getId() ;
    }
    return 0 ;
    
}

_ACS_END_NESTED_NAMESPACE

