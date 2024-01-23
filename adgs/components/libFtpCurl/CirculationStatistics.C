// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2014, Advanced Computer Systems , Inc.
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
 *  Revision 5.7  2016/03/26 21:42:30  marpas
 *  using new db I/F
 *
 *  Revision 5.6  2014/12/04 09:04:05  marfav
 *  S3PDGS-2155: mergestatistics fixed when sum size flag is set
 *
 *  Revision 5.5  2014/06/20 14:52:33  marpas
 *  useless private variables removed
 *  statistics management (log) fixed
 *  statistics management (t_circulationstats) fixed
 *  operation are now an enum
 *
 *  Revision 5.4  2014/03/12 12:20:48  marpas
 *  qa warnings fixed
 *
 *  Revision 5.3  2014/02/06 10:28:16  lucio.pulvirenti
 *  S2PDGS-625: unkn_from_host and unkn_to_host (string types) columns added to t_circulationstats table.
 *
 *  Revision 5.2  2014/02/05 17:32:04  lucio.pulvirenti
 *  Work in progress.
 *
 *  Revision 5.1  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.15  2013/04/03 15:39:23  marpas
 *  EntityBean and related class interface changed
 *
 *  Revision 2.14  2012/05/22 10:01:53  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *  http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 2.13  2012/04/02 14:42:30  micmaz
 *  work in progress
 *
 *  Revision 2.12  2012/03/30 12:51:01  micmaz
 *  work in progress
 *
 *  Revision 2.11  2012/03/29 16:46:11  micmaz
 *  work in progress
 *
 *  Revision 2.10  2012/03/29 14:11:07  micmaz
 *  work in progress
 *
 *  Revision 2.9  2012/03/28 12:59:18  micmaz
 *  work in progress
 *
 *  Revision 2.8  2012/03/27 18:08:49  micmaz
 *  work in progress
 *
 *  Revision 2.7  2012/03/08 14:22:46  micmaz
 *  updated tests to cppunit
 *
 *  Revision 2.6  2012/03/08 13:11:40  micmaz
 *  added media functionality
 *
 *  Revision 2.5  2012/02/09 16:48:25  micmaz
 *  added ftps CCC  configuration parameter.
 *
 *  Revision 2.4  2012/02/07 10:25:46  micmaz
 *  removed warnings .
 *
 *  Revision 2.3  2012/01/23 11:20:55  micmaz
 *  removed unused import.
 *
 *  Revision 2.2  2012/01/23 11:16:57  micmaz
 *  RESOLVED - issue S3PDGS-542: DC problem at explicit request
 *  http://jira.acsys.it/browse/S3PDGS-542
 *
 *  Revision 2.1  2012/01/20 16:04:59  micmaz
 *  OPEN - issue S1PDGS-2685: Circulation Agent Core in transferring large files
 *  http://jira.acsys.it/browse/S1PDGS-2685
 *
 *
 */

#include <CirculationStatistics.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl)

// constructor
CirculationStatistics::CirculationStatistics(dbConnection &conn) : 
    dbSmartPersistent(table(), conn),
    _sourceSite(),
    _destSite()
{
}


// copy constructor
CirculationStatistics::CirculationStatistics(const CirculationStatistics &o):
	dbSmartPersistent(o),
    _sourceSite(o._sourceSite),
    _destSite(o._destSite)
    
{
}


CirculationStatistics::~CirculationStatistics() throw()
{
}


// operator =
CirculationStatistics &CirculationStatistics::operator=(const CirculationStatistics &o)
{
	Lock lt(*this) ; // PRQA S 3050 2
	Lock lo(o) ;

	if(this != &o)
	{
		dbSmartPersistent::operator=(o) ;
        _sourceSite = o._sourceSite ;
        _destSite = o._destSite ;
	}
	return *this ;
}


using std::string ;
using std::max ;


RemoteSite const& CirculationStatistics::getSourceSite() const throw() // PRQA S 4120
{
    return _sourceSite;
}

RemoteSite const& CirculationStatistics::getDestSite() const throw() // PRQA S 4120
{
    return _destSite;
}

void CirculationStatistics::setSourceSite(RemoteSite const& sourceSite) // PRQA S 4121
{
    _sourceSite = sourceSite;
}
void CirculationStatistics::setDestSite(RemoteSite const& destSite) // PRQA S 4121 
{
    _destSite = destSite;
}

void CirculationStatistics::mergeStatistics(dbConnection &conn, CirculationStatistics const& other, bool sumSize) {
    bool null = false ;
    setNameLookUpTime(max(getNameLookUpTime(null, conn),other.getNameLookUpTime(null, conn)), conn);
    setConnectionTime(max(getConnectionTime(null, conn),other.getConnectionTime(null, conn)), conn);
    setAppConnectionTime(max(getAppConnectionTime(null, conn),other.getAppConnectionTime(null, conn)), conn);
    setPreTransfTime(max(getPreTransfTime(null, conn),other.getPreTransfTime(null, conn)), conn);
    setStartTransfTime(max(getStartTransfTime(null, conn),other.getStartTransfTime(null, conn)), conn);
    setRedirectTime(max(getRedirectTime(null, conn),other.getRedirectTime(null, conn)), conn);
    if (sumSize)
    {
        setTotalTime((getTotalTime(null, conn) + other.getTotalTime(null, conn)), conn);
        setDataSize((getDataSize(null, conn) + other.getDataSize(null, conn)), conn);
    }
    else
    {
        setTotalTime(max(getTotalTime(null, conn),other.getTotalTime(null, conn)), conn);
        setDataSize(max(getDataSize(null, conn),other.getDataSize(null, conn)), conn);
    }
}

string CirculationStatistics::str(dbConnection &conn, bool withId) const // PRQA S 4214
{
    std::ostringstream os;
    os << "{";
    os << "CirculationStatistics";
    if (withId) {
        os << "@";
        os << getId();
    }
    
    bool null = false ;
    if (not nullCreationDate(conn)) {
        os << ", CreationDate:" << getCreationDate(null, conn).YYYYMMDDThhmmss();
    }
    if (not nullFromHostId(conn)) {
        os << ", FromHostId:" << getFromHostId(null, conn);
    }
    if (not nullToHostId(conn)) {
        os << ", ToHostId:" << getToHostId(null, conn);
    }
    if (not nullTotalTime(conn)) {
        os << ", TotalTime:" << getTotalTime(null, conn);
    }
    if (not nullNameLookUpTime(conn)) {
        os << ", Name lookup time:" << getNameLookUpTime(null, conn);
    }
    if (not nullConnectionTime(conn)) {
        os << ", Connection time:" << getConnectionTime(null, conn);
    }
    if (not nullAppConnectionTime(conn)) {
        os << ", App Connection time:" << getAppConnectionTime(null, conn);
    }
    if (not nullPreTransfTime(conn)) {
        os << ", Pre Transf time:" << getPreTransfTime(null, conn);
    }
    if (not nullStartTransfTime(conn)) {
        os << ", Start Transf time:" << getStartTransfTime(null, conn);
    }
    if (not nullRedirectTime(conn)) {
        os << ", Redirect time:" << getRedirectTime(null, conn);
    }
    if (not nullDataSize(conn)) {
        os << ", Data Size:" << getDataSize(null, conn) << " (bytes)";
    }
    os << ", Circ:" << getCirculationsQueueId(null, conn);
    if ( dbSchemaMetaDataProvider::hasField("t_circulationstats", "unkn_from_host", conn) && not nullUnknFromHost(conn) ) { // PRQA S 3230
        os << ", UnknFromHost:" << getUnknFromHost(null, conn);
    }
    if ( dbSchemaMetaDataProvider::hasField("t_circulationstats", "unkn_to_host", conn) && not nullUnknToHost(conn) ) { // PRQA S 3230
        os << ", UnknToHost:" << getUnknToHost(null, conn);
    }
    os << "}";
    return os.str();
}

_ACS_END_NESTED_NAMESPACE;

