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
 *  Revision 5.7  2016/08/01 08:04:14  lucio.pulvirenti
 *  S1PDGs-31765: localtransfermode_id column added.
 *
 *  Revision 5.6  2016/03/26 21:32:47  marpas
 *  using new db I/F
 *
 *  Revision 5.5  2015/10/07 12:18:30  lucio.pulvirenti
 *  APF-324: agent_node_id nullable col added.
 *
 *  Revision 5.4  2014/03/10 15:48:45  marfav
 *  Removed ACS_TABLES_FUSION_318 dependency and dead code
 *
 *  Revision 5.3  2014/01/30 17:36:50  lucio.pulvirenti
 *  String filetype added to IF. Record printed removing password from fromUrl and toUrl.
 *
 *  Revision 5.2  2013/12/11 14:00:57  lucio.pulvirenti
 *  removesource column managed (introduced as per issue S2PDGS-488).
 *
 *  Revision 5.1  2013/07/22 13:16:54  lucio.pulvirenti
 *  Changes for S2PDGS-424 implemented under ACS_TABLES_FUSION_318 ifdef.
 *  t_circulationpolicies id is serial so that CirculationPolicy derives from db::EntityBean<dbPersistent::IdType>.
 *
 *  Revision 5.0  2013/06/07 15:48:58  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.17  2013/06/07 10:54:30  lucio.pulvirenti
 *  tempnamevalue string added.
 *
 *  Revision 1.16  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.15  2013/02/25 16:51:19  marpas
 *  coding best practices applied
 *  interface reflects changes in low level libraries
 *
 *  Revision 1.14  2012/05/23 14:04:14  micmaz
 *  updated key type
 *
 *  Revision 1.13  2012/05/23 10:58:25  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *   http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.12  2012/05/22 10:02:24  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *  http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.11  2012/03/27 10:54:15  micmaz
 *  work in progress
 *
 *  Revision 1.10  2012/02/16 17:46:25  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.9  2012/02/16 16:00:12  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.8  2012/02/07 10:25:28  micmaz
 *  removed warnings.
 *
 *  Revision 1.7  2011/09/16 15:50:55  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.6  2011/08/09 11:26:26  micmaz
 *  IN PROGRESS - issue S1PDGS-1784: Remove the DataSource class in order to use the new getter/setter ConnectionPool in the libApp.
 *  http://jira.acsys.it/browse/S1PDGS-1784
 *
 *  Revision 1.5  2011/07/21 17:44:40  micmaz
 *  updated Workspace, GeneralApplication and Entities.
 *
 *  Revision 1.4  2011/06/20 16:53:19  micmaz
 *  work in progress
 *
 *  Revision 1.3  2011/06/20 16:38:42  micmaz
 *  work in progress.
 *
 *  Revision 1.2  2011/06/16 18:26:55  micmaz
 *  work in progress
 *
 *  Revision 1.1  2011/06/16 12:50:29  micmaz
 *  work in progress.
 *
 *
 *
 *
 */

#include <CirculationPolicy.h>
#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)

namespace {
using pattern::ParamConstructibleSingletonMap ;

//
//
// class OriginatorName Name
//
//

class FileTypeName:   // PRQA S 2153
    public NameIdAssociator<dbPersistent::IdType>, 
    public ParamConstructibleSingletonMap<FileTypeName, std::string>
{
    friend FileTypeName* ParamConstructibleSingletonMap<FileTypeName, std::string>::instance(const std::string &); // PRQA S 2107
public:
	virtual ~FileTypeName() throw() {}  // PRQA S 2131
	
protected:
	explicit FileTypeName(const std::string &key) :
        NameIdAssociator<dbPersistent::IdType>("t_filetypes", "id_filetype","filetype", key),
        ParamConstructibleSingletonMap<FileTypeName, std::string>()
        {
            NameIdAssociator<dbPersistent::IdType>::load() ;
        }
	
private:
	FileTypeName &operator=(const FileTypeName &) ;	// declared but not implemented
	FileTypeName (const FileTypeName &) ; 			// declared but not implemented
	FileTypeName () ; 								// declared but not implemented
} ; 

}


// constructor
CirculationPolicy::CirculationPolicy(dbConnection &conn) : dbSmartPersistent(table(), conn)
{
}


// copy constructor
CirculationPolicy::CirculationPolicy(const CirculationPolicy &o):
	dbSmartPersistent(o)
{
}


CirculationPolicy::~CirculationPolicy() throw()
{
}


// operator =
CirculationPolicy &CirculationPolicy::operator=(const CirculationPolicy &o)
{
	Lock lt(*this) ; // PRQA S 3050 2
	Lock lo(o) ;

	if(this != &o)
	{
		dbSmartPersistent::operator=(o) ;
	}
	return *this ;
}

std::string CirculationPolicy::getFileTypeString(dbConnection &conn) const // PRQA S 4214
{
    FileTypeName &ftn = *FileTypeName::instance(conn.getKey()) ;
	bool set = false ;
    dbPersistent::IdType ft = getFileType(set, conn) ;

    return ftn.name(ft) ;
}


std::string CirculationPolicy::str(dbConnection &conn, bool withId) const // PRQA S 4214
{
    std::ostringstream os;
    os << "{CirculationPolicy";
    if (withId) {
        os << "@";
        os << getId();
    }
    bool null = false ; 
    dbPersistent::IdType d = 0 ;
    d = getFileType(null, conn);
    os << ", filetype_id: " << d ;
    d = getCirculatorNode(null, conn);
    os << ", circulation_node_id: " << d ;
    d = getPriority(null, conn);
    os << ", priority: " << d ;
    d = getSourceId(null, conn);
    os << ", source_id: " << d ;
    d = getDestinationId(null, conn);
    os << ", destination_id: " << d ;
	if( dbSchemaMetaDataProvider::hasField(table(), "removesource", conn) ) {
		os << ", removesource: " << getRemoveSource(null, conn);
	}
	if( dbSchemaMetaDataProvider::hasField(table(), "agent_node_id", conn))  {
        d = getAgentNode(null, conn);
        if (not null) {
		    os << ", agent_node_id: " << d ;
        }
	}
    
    int i = getLocalTransferModeId(null, conn); 
	if( not null ) {
		os << ", localtransfermode_id: " << i ;
	}
	os << ", status_id:" << getStatus(null, conn);
	if( dbSchemaMetaDataProvider::hasField(table(), "circ_policy_name", conn) && not nullCircPolicyName(conn) )  {
		os << ", circ_policy_name:" << getCircPolicyName(null, conn);
	}
    os << "}";
    return os.str();
}

_ACS_END_NESTED_NAMESPACE
