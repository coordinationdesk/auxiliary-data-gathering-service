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
 *  Revision 5.1  2016/03/26 21:32:47  marpas
 *  using new db I/F
 *
 *  Revision 5.0  2013/06/07 15:49:01  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.6  2013/04/05 09:51:07  marpas
 *  libDCUtility interface change propagation, especially for what is related to dbConnection
 *  flow control redesign
 *  coding best practices applied
 *  qa rules (partial)
 *
 *  Revision 1.5  2012/05/23 14:04:15  micmaz
 *  updated key type
 *
 *  Revision 1.4  2012/05/23 10:58:25  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *   http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.3  2012/05/22 10:02:24  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *  http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 1.2  2012/02/16 17:46:25  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *  Revision 1.1  2012/02/16 16:00:12  micmaz
 *  IN PROGRESS - issue S3PDGS-518: Gestione thread paralleli di DC su base ricevitore
 *  http://jira.acsys.it/browse/S3PDGS-518
 *
 *
 */

#include <KnownHostLimits.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)

using namespace std;
using namespace acs::db;


// constructor
KnownHostLimits::KnownHostLimits(dbConnection &conn) : dbSmartPersistent("t_knownhosts_limits", conn)
{
}


// copy constructor
KnownHostLimits::KnownHostLimits(const KnownHostLimits &o):
	dbSmartPersistent(o)
{
}


KnownHostLimits::~KnownHostLimits() throw()
{
}


// operator =
KnownHostLimits &KnownHostLimits::operator=(const KnownHostLimits &o)
{
	Lock lt(*this) ; // PRQA S 3050 2
	Lock lo(o) ;

	if(this != &o)
	{
		dbSmartPersistent::operator=(o) ;
	}
	return *this ;
}

string KnownHostLimits::str(dbConnection &conn, bool withId) const // PRQA S 4214
{
    std::ostringstream os;
    os << "{";
    os << "KnownHostLimits";
    if (withId) {
        os << "@";
        os << getId();
    }
    //not nullable: no null pointer core dump
    bool s = false ;
    os << ", destHost: " << getDestHost(s, conn) ;
    //not nullable: no null pointer core dump
    os << ", circulatorNode: " << getCirculatorNode(s, conn) ;
    os << ", maxUploadSlots: " << getMaxUploadSlots(s, conn) ;
    os << "}";
    return os.str();

}


_ACS_END_NESTED_NAMESPACE;
