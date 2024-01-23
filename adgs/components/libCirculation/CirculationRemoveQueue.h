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
 *  Revision 7.1  2017/10/17 15:56:09  marpas
 *  getting rid of EntityBeam and rewriting all based on smart persistent
 *
 *  Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.1  2013/07/15 17:21:11  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:48:58  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.9  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.8  2013/02/25 16:51:19  marpas
 *  coding best practices applied
 *  interface reflects changes in low level libraries
 *
 *  Revision 1.7  2012/05/23 14:04:14  micmaz
 *  updated key type
 *
 *  Revision 1.6  2012/05/23 10:58:25  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *   http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.5  2012/05/22 10:02:25  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *  http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.4  2012/02/16 16:00:12  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.3  2011/09/16 15:50:56  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.2  2011/09/05 16:27:41  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *  Revision 1.1  2011/09/05 16:16:41  micmaz
 *  IN PROGRESS - issue S1PDGS-1605: CirculationAgent in implicit mode can not circulate the same files to more than one host
 *  http://jira.acsys.it/browse/S1PDGS-1605
 *
 *
 *
 *
 */

#ifndef CIRCULATIONREMOVEQUEUE_H_
#define CIRCULATIONREMOVEQUEUE_H_

#include <acs_c++config.hpp>
#include <dbSmartPersistent.h>
#include <dbSmartQuery.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)

class CirculationRemoveQueue : // PRQA S 2109, 2153
	public dbSmartPersistent
{
public:
	explicit CirculationRemoveQueue(dbConnection &conn) ;
	CirculationRemoveQueue(const CirculationRemoveQueue &) ;

	virtual ~CirculationRemoveQueue() throw() ;
	CirculationRemoveQueue &operator=(const CirculationRemoveQueue &) ; // operator =

	void setURL(const std::string &v, dbConnection &c) { set("url",v, c) ; }
	std::string getURL(bool &set, dbConnection &c) const { std::string v ; get("url",v, set, c) ; return v ; } // PRQA S 4214
	void unsetURL() { unset("url") ; }

	void setHostId(dbPersistent::IdType v, dbConnection &c) { set("host_id",v, c) ; }
	dbPersistent::IdType getHostId(bool &set, dbConnection &c) const { dbPersistent::IdType v ; get("host_id",v, set, c) ; return v ; } // PRQA S 4214
	void unsetHostId() { unset("host_id") ; }

    std::string str(dbConnection &,bool withId = false) const;
    
    static std::string table() { return "t_circulation_remove_queue" ; } 

private:
    CirculationRemoveQueue() ; // not implemented
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(CirculationRemoveQueue)
} ;

typedef db::SmartQuery<CirculationRemoveQueue> CirculationRemoveQueueQuery ;

_ACS_END_NESTED_NAMESPACE

#endif /* CIRCULATIONREMOVEQUEUE_H_ */
