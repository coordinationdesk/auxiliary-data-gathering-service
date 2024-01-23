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
 *  Revision 7.4  2017/10/24 15:32:33  marpas
 *  findHostByName bug fixed (wrong query)
 *
 *  Revision 7.3  2017/10/19 16:18:04  marpas
 *  getting rid of EntityBean
 *
 *  Revision 7.2  2017/10/17 15:56:09  marpas
 *  getting rid of EntityBeam and rewriting all based on smart persistent
 *
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
 *  Revision 5.4  2016/03/26 21:32:47  marpas
 *  using new db I/F
 *
 *  Revision 5.3  2013/07/23 17:11:42  marpas
 *  compilations warning fixed
 *
 *  Revision 5.2  2013/07/15 17:21:12  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.1  2013/07/12 09:06:55  marpas
 *  porting circulation business intelligence into the library
 *
 *  Revision 5.0  2013/06/07 15:49:01  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.15  2013/04/05 09:51:07  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.14  2013/02/25 16:51:19  marpas
 *  coding best practices applied
 *  interface reflects changes in low level libraries
 *
 *  Revision 1.13  2012/05/23 14:04:14  micmaz
 *  updated key type
 *
 *  Revision 1.12  2012/05/23 10:51:46  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *  http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.11  2012/05/22 10:02:25  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *  http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.10  2012/02/16 17:46:26  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.9  2012/02/16 16:00:12  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.8  2012/02/07 10:25:29  micmaz
 *  removed warnings.
 *
 *  Revision 1.7  2012/02/01 16:05:00  micmaz
 *  removed loading local host from db.
 *
 *  Revision 1.6  2012/01/23 16:20:52  micmaz
 *  IN PROGRESS - issue S1PDGS-2735: PolicyGenerator: t_circulationusers population
 *  http://jira.acsys.it/browse/S1PDGS-2735
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

#include <KnownHost.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)

ACS_CLASS_DEFINE_DEBUG_LEVEL(KnownHost);

// constructor
KnownHost::KnownHost(dbConnection &conn) : dbSmartPersistent(table(), conn)
{
}


// copy constructor
KnownHost::KnownHost(const KnownHost &o):
	dbSmartPersistent(o)
{
}


KnownHost::~KnownHost() throw()
{
}


// operator =
KnownHost &KnownHost::operator=(const KnownHost &o)
{
	Lock lt(*this) ; // PRQA S 3050 2
	Lock lo(o) ;

	if(this != &o)
	{
		dbSmartPersistent::operator=(o) ;
	}
	return *this ;
}


using std::string ;

string KnownHost::str(dbConnection &conn, bool withId) const // PRQA S 4214
{
    std::ostringstream os;
    os << "{";
    os << "KnownHost";
    if (withId) {
        os << "@";
        os << getId();
    }
    bool isNull = false ; string hn = getHostName(isNull, conn);
    os << ", hostname: " << ( not isNull ? hn : string("<null>") ) ;// PRQA S 3081, 3380, 3385
    os << "}";
    return os.str();
}

KnownHost KnownHost::findHostByName(dbConnection &conn,string const& hostName) 
{
    try {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Search Host by name: " << hostName);
        const std::string &key = conn.getKey() ;
        KnownHostQuery hosts(key) ;
        {
            dbQueryParameters pp(key) ;
            pp.addParameter(dbParam(key, "t_knownhosts","host_name", dbParam::EQUAL_TO, db::toSQL(key,hostName))) ;
            hosts.addParameter(pp) ;
        }
        hosts.doQuery(conn);
        ACS_COND_THROW(hosts.empty(), // PRQA S 3081
                ExecutionError("Cannot find the Host requested with label: " + hostName));
        KnownHost host = *hosts[0];
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Host found: " << host.str(conn));
        return host;
    } catch (std::exception const& ex) {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ex.what());
    }
    ACS_THROW(ExecutionError("Cannot find the Host requested with label: " + hostName)) ; // PRQA S 3081
}


using std::vector ;

size_t getCandidateRelays(vector<KnownHost> &relays, dbConnection &conn, int SecZoneId) 
{
    const std::string &key = conn.getKey() ;
    KnownHostQuery q(key) ;
    SQLString subs(key, SQLString::SELECT) ;
    {
    	subs.addTableList("t_securityzonerelays") ;
        subs.addSelectList("t_securityzonerelays.host_id");
        dbQueryParameters pp(key) ;
        pp.addParameter(dbParam(key, "t_securityzonerelays","seczone_id", dbParam::EQUAL_TO, dbConv(key,SecZoneId))) ; // PRQA S 3050
        subs.addANDWhereParameter(pp) ;
        
    }
    {
        dbQueryParameters pp(key) ;
        pp.addParameter(dbParam(key, "t_knownhosts","id", dbParam::IN, subs)) ;
        q.addParameter(pp) ;
    }
    size_t records = q.doQuery(conn) ;
    relays.clear() ;
    for (size_t i=0 ; i < records ; ++i) {
        if (q[i]) {
            relays.push_back(*q[i]) ;  // PRQA S 3081
        }
    }
    return records ;
}
    

_ACS_END_NESTED_NAMESPACE

