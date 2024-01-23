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
 *  Revision 1.11  2012/05/23 14:04:14  micmaz
 *  updated key type
 *
 *  Revision 1.10  2012/05/23 10:51:46  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *  http://jira.acsys.it/browse/S1PDGS-3410
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
 *  Revision 1.6  2012/02/01 16:05:00  micmaz
 *  removed loading local host from db.
 *
 *  Revision 1.5  2012/01/23 16:20:52  micmaz
 *  IN PROGRESS - issue S1PDGS-2735: PolicyGenerator: t_circulationusers population
 *  http://jira.acsys.it/browse/S1PDGS-2735
 *
 *  Revision 1.4  2011/09/16 15:50:55  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.3  2011/07/21 17:44:40  micmaz
 *  updated Workspace, GeneralApplication and Entities.
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

#ifndef KNOWNHOST_H_
#define KNOWNHOST_H_

#include <acs_c++config.hpp>
#include <dbSmartPersistent.h>
#include <dbSmartQuery.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)

// PRQA S 4101, 4214 EOCLASS

class KnownHost : // PRQA S 2109, 2153
	public dbSmartPersistent
{
public:
	explicit KnownHost(dbConnection &conn) ;
	KnownHost(const KnownHost &) ;

	virtual ~KnownHost() throw() ;
	KnownHost &operator=(const KnownHost &) ; // operator =

	void setHostName(const std::string &v, dbConnection &c) { set("host_name",v, c) ; }
	std::string getHostName(bool &isNull, dbConnection &c) const { std::string v ; get("host_name",v, isNull, c) ; return v ; } // PRQA S 4214
	void unsetHostName() { unset("host_name") ; }

    std::string str(dbConnection &,bool withId = false) const;
    
    static std::string table() { return "t_knownhosts" ; } 
    static KnownHost findHostByName(dbConnection &, std::string const& hostName) ;

private:
    KnownHost() ; // not implemented
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(KnownHost)
} ;
// PRQA L:EOCLASS

typedef db::SmartQuery<KnownHost> KnownHostQuery ;

size_t getCandidateRelays(std::vector<KnownHost> &, dbConnection &, int SecZoneId) ;


_ACS_END_NESTED_NAMESPACE

#endif /* KNOWNHOST_H_ */
