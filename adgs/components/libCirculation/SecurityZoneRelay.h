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
 *  $Prod: A.C.S. libCirculation Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 7.1  2017/10/17 15:56:09  marpas
 *  getting rid of EntityBeam and rewriting all based on smart persistent
 *
 *  Revision 7.0  2016/10/12 10:08:27  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.1  2013/07/15 17:21:12  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
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
 *  Revision 1.8  2012/02/16 17:46:26  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.7  2012/02/16 16:00:12  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.6  2012/01/17 16:50:52  micmaz
 *  OPEN - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 1.5  2011/09/16 15:50:56  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.4  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
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

#ifndef SECURITYZONERELAY_H_
#define SECURITYZONERELAY_H_


#include <acs_c++config.hpp>
#include <dbSmartPersistent.h>
#include <dbSmartQuery.h>
#include<SecurityZone.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)

// PRQA S 4101, 4214 EOCLASS

class SecurityZoneRelay : // PRQA S 2109, 2153
	public dbSmartPersistent
{
public:
	explicit SecurityZoneRelay(dbConnection &conn) ;
	SecurityZoneRelay(const SecurityZoneRelay &) ;

	virtual ~SecurityZoneRelay() throw() ;
	SecurityZoneRelay &operator=(const SecurityZoneRelay &) ; // operator =

	void setHost(int v, dbConnection &c) { set("host_id",v, c) ; }
	int getHost(bool &isNull, dbConnection &c) const { int v ; get("host_id",v, isNull, c) ; return v ; }
	void unsetHost() { unset("host_id") ; }

	void setZone(int v, dbConnection &c) { set("seczone_id",v, c) ; }
	int getZone(bool &isNull, dbConnection &c) const { int v ; get("seczone_id",v, isNull, c) ; return v ; }
	void unsetZone() { unset("seczone_id") ; }

    std::string str(dbConnection &,bool withId = false) const;
    
    static std::string table() { return "t_securityzonerelays" ; } 

private:
    SecurityZoneRelay() ; // not implemented
} ;
// PRQA L:EOCLASS

typedef db::SmartQuery<SecurityZoneRelay> SecurityZoneRelayQuery ;


_ACS_END_NESTED_NAMESPACE

#endif /* SECURITYZONERELAY_H_ */
