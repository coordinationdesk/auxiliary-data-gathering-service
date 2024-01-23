// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2016, Advanced Computer Systems , Inc.
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
 *  Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.11  2016/07/25 15:45:35  lucio.pulvirenti
 *  S1PDGS-31765: localtransfermode_id col added.
 *
 *  Revision 5.10  2016/04/07 15:47:54  marpas
 *  initialization at ctor and copy ctor fixed
 *
 *  Revision 5.9  2016/03/26 21:32:47  marpas
 *  using new db I/F
 *
 *  Revision 5.8  2015/10/07 12:19:29  lucio.pulvirenti
 *  APF-324: Added HostId print in str() method.
 *
 *  Revision 5.7  2015/08/27 16:02:12  damdec
 *  S2PDGS-1247: check on not null fields introduced.
 *
 *  Revision 5.6  2015/08/21 14:55:39  marfav
 *  S2PDGS-1295 passwords are hidden in str()
 *
 *  Revision 5.5  2014/10/03 14:02:50  lucio.pulvirenti
 *  S2PDGS-869: host_id no more nullable.
 *
 *  Revision 5.4  2014/08/07 10:52:00  lucio.pulvirenti
 *  Bug fixed in str(): try-catch block added when getting both fromUrl and toUrl not to throw exception.
 *
 *  Revision 5.3  2014/01/30 17:38:18  lucio.pulvirenti
 *  In print method, toUrl and fromUrl printed with no password.
 *
 *  Revision 5.2  2013/07/15 17:21:11  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.1  2013/06/12 13:43:47  lucio.pulvirenti
 *  tempnamevalue now nullable.
 *
 *  Revision 5.0  2013/06/07 15:48:57  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.22  2013/06/07 15:23:35  lucio.pulvirenti
 *  tempnamevalue and tempname_id column added.
 *
 *  Revision 1.21  2013/05/29 12:03:56  marpas
 *  a t_circulationsqueue record has a serial id, dbPersistent::IdType typedef used
 *
 *  Revision 1.20  2013/04/05 09:51:06  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.19  2012/05/23 14:04:14  micmaz
 *  updated key type
 *
 *  Revision 1.18  2012/05/23 10:58:25  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *   http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.17  2012/05/22 10:02:24  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *  http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.16  2012/02/16 17:46:25  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.15  2012/02/16 16:00:12  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.14  2012/02/07 10:25:28  micmaz
 *  removed warnings.
 *
 *  Revision 1.13  2012/02/06 12:51:15  marpas
 *  macros for assignement used
 *
 *  Revision 1.12  2012/01/18 18:30:15  micmaz
 *  The distructor method should be virtual
 *
 *  Revision 1.11  2011/09/16 15:50:55  micmaz
 *  reworked namespaces.
 *
 *  Revision 1.10  2011/07/21 17:44:40  micmaz
 *  updated Workspace, GeneralApplication and Entities.
 *
 *  Revision 1.9  2011/07/13 17:06:03  micmaz
 *  REOPENED - issue S1PDGS-1655: CirculationAgent maxNumberOfRetries problem
 *  http://jira.acsys.it/browse/S1PDGS-1655
 *
 *  Revision 1.8  2011/07/06 13:00:15  micmaz
 *  *** empty log message ***
 *
 *  Revision 1.7  2011/06/23 15:49:20  micmaz
 *  work in progress
 *
 *  Revision 1.6  2011/06/07 17:20:32  micmaz
 *  work in progress
 *
 *  Revision 1.5  2011/05/31 13:15:28  micmaz
 *  work in progress...
 *
 *  Revision 1.4  2011/04/29 17:22:14  micmaz
 *  if cannot create a log file, redirect to stderr
 *
 *  Revision 1.3  2011/04/01 16:15:20  micmaz
 *  work in progress...
 *
 *  Revision 1.2  2011/04/01 14:48:19  micmaz
 *  implemented the Circulations WP
 *
 *  Revision 1.1  2011/03/31 13:45:47  micmaz
 *  work in progress
 *
 *  Revision 1.1.1.1  2011/03/29 15:09:09  micmaz
 *  “Start”
 *
 *
 *
 *
 */

#include <Circulation.h>
#include <RemoteSite.h>
#include <RemoteSiteFactory.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;

// constructor
Circulation::Circulation(dbConnection &conn) : dbSmartPersistent(table(), conn)
{
}


// copy constructor
Circulation::Circulation(const Circulation &o):
	dbSmartPersistent(o)
{
}


Circulation::~Circulation() throw()
{
}


// operator =
Circulation &Circulation::operator=(const Circulation &o)
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


string Circulation::str(dbConnection &conn, bool withId) const // PRQA S 4214
{
    using acs::curl::RemoteSiteFactory ;
    std::ostringstream os;
    os << "{";
    os << "Circulation";
    if (withId) {
        os << "@";
        os << getId();
    }
    bool null = false ;
    if (not nullStartDate(conn)) {
        os << ", StartDate:" << getStartDate(null, conn).YYYYMMDDThhmmss();
    }
    if (not nullEndDate(conn)) {
        os << ", EndDate:" << getEndDate(null, conn).YYYYMMDDThhmmss();
    }
    os << ", Priority:" << getPriority(null, conn);
    os << ", Remark:" << getRemark(null, conn);
    os << ", Status_id:" << getStatus(null, conn);
    if (not nullCreationDate(conn)) {
        os << ", CreationDate:" << getCreationDate(null, conn).YYYYMMDDThhmmss();
    }
    if (not nullModificationDate(conn)) {
        os << ", ModificationDate:" << getModificationDate(null, conn).YYYYMMDDThhmmss();
    }
    
	string fromUrl;
	try {
		// S2PDGS-1295 Using asterisks instead of the password
		fromUrl = RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(getFromURL(null, conn), true),false);
	}
	catch( std::exception &e )
	{
		ACS_LOG_ERROR("Wrong fromUrl format: \"" << getFromURL(null, conn) << '\"');
	}
    os << ", FromURL:" << fromUrl;
	
	string toUrl;
	try {
		// S2PDGS-1295 Using asterisks instead of the password
		toUrl = RemoteSiteFactory::toURL(RemoteSiteFactory::makeSite(getToURL(null, conn), true),false);
	}
	catch( std::exception &e )
	{
		ACS_LOG_ERROR("Wrong toUrl format: \"" << getToURL(null, conn) << '\"');
	}
    os << ", ToURL:" << toUrl;
    os << ", RelayHostId:" << getRelayHostId(null, conn);
    os << ", HostId:" << getHostId(null, conn);
    os << ", Retries:" << getRetries(null, conn);
    os << ", Removable:" << getRemoveSource(null, conn);
    os << ", Policy:" << getCirculationPolicy(null, conn);
    os << ", Dest Host:" << getDestHostId(null, conn);
    os << ", tempnamevalue: \"" << getTempNameValue(null, conn) << "\"";
    os << ", tempname_id:" << getTempNameId(null, conn);
    os << ", localtransfermode_id:" << getLocalTransferModeId(null, conn);
    if ( dbSchemaMetaDataProvider::hasField("t_circulationsqueue", "streamer_id", conn) && not nullStreamerId(conn)) {
        os << ", StreamerId:" << getStreamerId(null, conn);
    }
    
    os << "}";
    return os.str();
}

_ACS_END_NESTED_NAMESPACE;
