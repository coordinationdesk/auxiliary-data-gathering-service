// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2018, Advanced Computer Systems , Inc.
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
 *  Revision 7.3  2018/07/24 15:50:17  lucio.pulvirenti
 *  S3EVOL-159: status_id added.
 *
 *  Revision 7.2  2017/10/19 16:18:04  marpas
 *  getting rid of EntityBean
 *
 *  Revision 7.1  2017/10/17 15:56:09  marpas
 *  getting rid of EntityBeam and rewriting all based on smart persistent
 *
 *  Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.9  2016/08/01 08:04:14  lucio.pulvirenti
 *  S1PDGs-31765: localtransfermode_id column added.
 *
 *  Revision 5.8  2015/10/07 12:18:30  lucio.pulvirenti
 *  APF-324: agent_node_id nullable col added.
 *
 *  Revision 5.7  2014/03/10 15:48:45  marfav
 *  Removed ACS_TABLES_FUSION_318 dependency and dead code
 *
 *  Revision 5.6  2014/01/30 17:36:50  lucio.pulvirenti
 *  String filetype added to IF. Record printed removing password from fromUrl and toUrl.
 *
 *  Revision 5.5  2013/12/17 17:49:24  marpas
 *  qa warnings
 *
 *  Revision 5.4  2013/12/11 14:00:57  lucio.pulvirenti
 *  removesource column managed (introduced as per issue S2PDGS-488).
 *
 *  Revision 5.3  2013/10/28 17:33:52  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/07/22 13:16:54  lucio.pulvirenti
 *  Changes for S2PDGS-424 implemented under ACS_TABLES_FUSION_318 ifdef.
 *  t_circulationpolicies id is serial so that CirculationPolicy derives from db::EntityBean<dbPersistent::IdType>.
 *
 *  Revision 5.1  2013/07/15 17:21:11  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:48:58  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.18  2013/06/07 10:54:30  lucio.pulvirenti
 *  tempnamevalue string added.
 *
 *  Revision 1.17  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.16  2013/02/25 16:51:19  marpas
 *  coding best practices applied
 *  interface reflects changes in low level libraries
 *
 *  Revision 1.15  2012/05/23 14:04:14  micmaz
 *  updated key type
 *
 *  Revision 1.14  2012/05/23 10:58:25  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *   http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.13  2012/05/22 10:02:24  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *  http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.12  2012/03/27 10:54:16  micmaz
 *  work in progress
 *
 *  Revision 1.11  2012/02/16 18:06:02  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.10  2012/01/17 16:50:52  micmaz
 *  OPEN - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 1.9  2011/09/16 15:50:55  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.8  2011/08/29 10:24:29  micmaz
 *  IN PROGRESS - issue S1PDGS-1604: CirculationAgent does not use t_circulationtempnames
 *  http://jira.acsys.it/browse/S1PDGS-1604
 *
 *  Revision 1.7  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
 *
 *  Revision 1.6  2011/07/21 17:44:40  micmaz
 *  updated Workspace, GeneralApplication and Entities.
 *
 *  Revision 1.5  2011/06/20 16:53:19  micmaz
 *  work in progress
 *
 *  Revision 1.4  2011/06/20 16:40:21  micmaz
 *  work in progress.
 *
 *  Revision 1.3  2011/06/20 16:38:42  micmaz
 *  work in progress.
 *
 *  Revision 1.2  2011/06/20 16:01:24  micmaz
 *  work in progress.
 *
 *  Revision 1.1  2011/06/16 12:50:29  micmaz
 *  work in progress.
 *
 *
 *
 *
 */

#ifndef _CirculationPolicy_H_
#define _CirculationPolicy_H_

#include <acs_c++config.hpp>
#include <dbSmartPersistent.h>
#include <dbSmartQuery.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)

// PRQA S 4101, 4214 EOCLASS

class CirculationPolicy : // PRQA S 2109, 2153
	public dbSmartPersistent
{
public:
	explicit CirculationPolicy(dbConnection &conn) ;
	CirculationPolicy(const CirculationPolicy &) ;

	virtual ~CirculationPolicy() throw() ;
	CirculationPolicy &operator=(const CirculationPolicy &) ; // operator =

	void setFileType(dbPersistent::IdType v, dbConnection &c) { set("filetype_id",v, c) ; }
	dbPersistent::IdType getFileType(bool &set, dbConnection &c) const { dbPersistent::IdType v ; get("filetype_id",v, set, c) ; return v ; }
	void unsetFileType() { unset("filetype_id") ; }

	void setDestinationId(dbPersistent::IdType v, dbConnection &c) { set("destination_id",v, c) ; }
	dbPersistent::IdType getDestinationId(bool &set, dbConnection &c) const { dbPersistent::IdType v ; get("destination_id",v, set, c) ; return v ; }
	void unsetDestinationId() { unset("destination_id") ; }

	void setCirculatorNode(dbPersistent::IdType v, dbConnection &c) { set("circulator_node_id",v, c) ; }
	dbPersistent::IdType getCirculatorNode(bool &set, dbConnection &c) const { dbPersistent::IdType v ; get("circulator_node_id",v, set, c) ; return v ; }
	void unsetCirculatorNode() { unset("circulator_node_id") ; }

	void setSourceId(dbPersistent::IdType v, dbConnection &c) { set("source_id",v, c) ; }
	dbPersistent::IdType getSourceId(bool &set, dbConnection &c) const { dbPersistent::IdType v ; get("source_id",v, set, c) ; return v ; }
	void unsetSourceId() { unset("source_id") ; }

	void setAgentNode(dbPersistent::IdType v, dbConnection &c) { set("agent_node_id",v, c) ; }
	dbPersistent::IdType getAgentNode(bool &set, dbConnection &c) const { dbPersistent::IdType v ; get("agent_node_id",v, set, c) ; return v ; }
	void unsetAgentNode() { unset("agent_node_id") ; }
    bool nullAgentNode(dbConnection &c) const { bool n ; getAgentNode(n, c) ; return n; }

	void setRemoveSource(bool v, dbConnection &c) { set("removesource",v, c) ; }
	bool getRemoveSource(bool &set, dbConnection &c) const { bool v ; get("removesource",v, set, c) ; return v ; }
	void unsetRemoveSource() { unset("removesource") ; }

	void setPriority(int v, dbConnection &c) { set("priority",v, c) ; }
	int getPriority(bool &set, dbConnection &c) const { int v ; get("priority",v, set, c) ; return v ; }
	void unsetPriority() { unset("priority") ; }

	void setLocalTransferModeId(int v, dbConnection &c) { set("localtransfermode_id",v, c) ; }
	int getLocalTransferModeId(bool &set, dbConnection &c) const { int v ; get("localtransfermode_id",v, set, c) ; return v ; }
	void unsetLocalTransferModeId() { unset("localtransfermode_id") ; }
    bool nullLocalTransferModeId(dbConnection &c) const { bool n ; getLocalTransferModeId(n, c) ; return n; }

	void setStatus(short v, dbConnection &c) { set("status_id",v, c) ; }
	short getStatus(bool &set, dbConnection &c) const { short v ; get("status_id",v, set, c) ; return v ; }
	void unsetStatus() { unset("status_id") ; }

	void setCircPolicyName(std::string v, dbConnection &c) { set("circ_policy_name",v, c) ; }
	std::string getCircPolicyName(bool &set, dbConnection &c) const { std::string v ; get("circ_policy_name",v, set, c) ; return v ; }
	void unsetCircPolicyName() { unset("circ_policy_name") ; }
    bool nullCircPolicyName(dbConnection &conn) const { bool null ; getCircPolicyName(null, conn) ; return null ; } 
	
    std::string str(dbConnection &,bool withId = false) const;
    
    static std::string table() { return "t_circulationpolicies" ; } 
    
    std::string getFileTypeString(dbConnection &conn) const ;
    
private:
    CirculationPolicy() ; // not implemented
} ;
// PRQA L:EOCLASS

typedef db::SmartQuery<CirculationPolicy> CirculationPolicyQuery ;

_ACS_END_NESTED_NAMESPACE

#endif /* _CirculationPolicy_H_ */


