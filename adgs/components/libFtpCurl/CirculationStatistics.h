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
 *  Revision 5.8  2017/10/19 15:13:53  marpas
 *  getting rid of EntityBean
 *
 *  Revision 5.7  2016/03/26 21:42:30  marpas
 *  using new db I/F
 *
 *  Revision 5.6  2014/02/06 10:28:16  lucio.pulvirenti
 *  S2PDGS-625: unkn_from_host and unkn_to_host (string types) columns added to t_circulationstats table.
 *
 *  Revision 5.5  2014/02/05 17:32:04  lucio.pulvirenti
 *  Work in progress.
 *
 *  Revision 5.4  2013/10/24 18:27:59  marpas
 *  work in progress
 *
 *  Revision 5.3  2013/10/22 17:31:50  marpas
 *  work in progress
 *
 *  Revision 5.2  2013/07/22 13:47:55  marpas
 *  type reconciliation
 *
 *  Revision 5.1  2013/07/18 12:27:37  marpas
 *  id is an IdType
 *ize
 
 *  Revision 5.0  2013/06/06 18:08:19  marpas
 *  adopting linException 4.x standards
 *
 *  Revision 2.14  2013/05/29 10:53:58  marpas
 *  interface rationalization,
 *  coding best practices applied,
 *  qa rules
 *
 *  Revision 2.13  2013/04/05 09:44:15  marpas
 *  coding best practices applied
 *  qa warnings (partial)
 *  libDCUtility interface update
 *
 *  Revision 2.12  2013/04/03 15:39:23  marpas
 *  EntityBean and related class interface changed
 *
 *  Revision 2.11  2012/11/27 13:27:13  micmaz
 *  added quality tool directives.
 *
 *  Revision 2.10  2012/10/03 16:13:33  micmaz
 *  The data size must be enlarged from int to long long.
 *
 *  Revision 2.9  2012/05/22 10:01:53  micmaz
 *  IN PROGRESS - issue S1PDGS-3410: DS: Error in inserting a CirculationId with value greater than 32767
 *  http://jira.acsys.it/browse/S1PDGS-3410
 *
 *  Revision 2.8  2012/03/30 15:15:55  micmaz
 *  work in progress
 *
 *  Revision 2.7  2012/03/30 12:51:01  micmaz
 *  work in progress
 *
 *  Revision 2.6  2012/03/27 18:08:49  micmaz
 *  work in progress
 *
 *  Revision 2.5  2012/02/07 10:25:46  micmaz
 *  removed warnings .
 *
 *  Revision 2.4  2012/01/23 11:20:55  micmaz
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

#ifndef _CirculationStatistics_h_
#define _CirculationStatistics_h_


#include <acs_c++config.hpp>
#include <dbSmartPersistent.h>
#include <dbSmartQuery.h>
#include <RemoteSite.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, curl) ;
// PRQA S 4101, 4214 EOCLASS

class CirculationStatistics : // PRQA S 2109, 2153
	public dbSmartPersistent
{
public:
	explicit CirculationStatistics(dbConnection &conn) ;
	CirculationStatistics(const CirculationStatistics &) ;

	virtual ~CirculationStatistics() throw() ;
	CirculationStatistics &operator=(const CirculationStatistics &) ; // operator =

	void setCreationDate(const DateTime &v, dbConnection &c) { set("creationdate",v, c) ; }
	DateTime getCreationDate(bool &set, dbConnection &c) const { DateTime v ; get("creationdate",v, set, c) ; return v ; }
	void unsetCreationDate() { unset("creationdate") ; }
    bool nullCreationDate(dbConnection &conn) const { bool null ; getCreationDate(null, conn) ; return null ; } 

	void setFromHostId(dbPersistent::IdType v, dbConnection &c) { set("from_host_id",v, c) ; }
	dbPersistent::IdType getFromHostId(bool &set, dbConnection &c) const { dbPersistent::IdType v ; get("from_host_id",v, set, c) ; return v ; }
	void unsetFromHostId() { unset("from_host_id") ; }
    bool nullFromHostId(dbConnection &conn) const { bool null ; getFromHostId(null, conn) ; return null ; } 

	void setToHostId(dbPersistent::IdType v, dbConnection &c) { set("to_host_id",v, c) ; }
	dbPersistent::IdType getToHostId(bool &set, dbConnection &c) const { dbPersistent::IdType v ; get("to_host_id",v, set, c) ; return v ; }
	void unsetToHostId() { unset("to_host_id") ; }
    bool nullToHostId(dbConnection &conn) const { bool null ; getToHostId(null, conn) ; return null ; } 

	void setNameLookUpTime(double v, dbConnection &c) { set("name_lookup_time",v, c) ; }
	double getNameLookUpTime(bool &set, dbConnection &c) const { double v ; get("name_lookup_time",v, set, c) ; return v ; }
	void unsetNameLookUpTime() { unset("name_lookup_time") ; }
    bool nullNameLookUpTime(dbConnection &conn) const { bool null ; getNameLookUpTime(null, conn) ; return null ; } 

	void setConnectionTime(double v, dbConnection &c) { set("connection_time",v, c) ; }
	double getConnectionTime(bool &set, dbConnection &c) const { double v ; get("connection_time",v, set, c) ; return v ; }
	void unsetConnectionTime() { unset("connection_time") ; }
    bool nullConnectionTime(dbConnection &conn) const { bool null ; getConnectionTime(null, conn) ; return null ; } 

	void setAppConnectionTime(double v, dbConnection &c) { set("app_connection_time",v, c) ; }
	double getAppConnectionTime(bool &set, dbConnection &c) const { double v ; get("app_connection_time",v, set, c) ; return v ; }
	void unsetAppConnectionTime() { unset("app_connection_time") ; }
    bool nullAppConnectionTime(dbConnection &conn) const { bool null ; getAppConnectionTime(null, conn) ; return null ; } 

	void setPreTransfTime(double v, dbConnection &c) { set("pre_transf_time",v, c) ; }
	double getPreTransfTime(bool &set, dbConnection &c) const { double v ; get("pre_transf_time",v, set, c) ; return v ; }
	void unsetPreTransfTime() { unset("pre_transf_time") ; }
    bool nullPreTransfTime(dbConnection &conn) const { bool null ; getPreTransfTime(null, conn) ; return null ; } 

	void setStartTransfTime(double v, dbConnection &c) { set("start_transf_time",v, c) ; }
	double getStartTransfTime(bool &set, dbConnection &c) const { double v ; get("start_transf_time",v, set, c) ; return v ; }
	void unsetStartTransfTime() { unset("start_transf_time") ; }
    bool nullStartTransfTime(dbConnection &conn) const { bool null ; getStartTransfTime(null, conn) ; return null ; } 

	void setTotalTime(double v, dbConnection &c) { set("total_time",v, c) ; }
	double getTotalTime(bool &set, dbConnection &c) const { double v ; get("total_time",v, set, c) ; return v ; }
	void unsetTotalTime() { unset("total_time") ; }
    bool nullTotalTime(dbConnection &conn) const { bool null ; getTotalTime(null, conn) ; return null ; } 

	void setRedirectTime(double v, dbConnection &c) { set("redirect_time",v, c) ; }
	double getRedirectTime(bool &set, dbConnection &c) const { double v ; get("redirect_time",v, set, c) ; return v ; }
	void unsetRedirectTime() { unset("redirect_time") ; }
    bool nullRedirectTime(dbConnection &conn) const { bool null ; getRedirectTime(null, conn) ; return null ; } 

	void setDataSize(long long v, dbConnection &c) { set("datasize",v, c) ; }
	long long getDataSize(bool &set, dbConnection &c) const { long long v ; get("datasize",v, set, c) ; return v ; }
	void unsetDataSize() { unset("datasize") ; }
    bool nullDataSize(dbConnection &conn) const { bool null ; getDataSize(null, conn) ; return null ; } 

	void setCirculationsQueueId(dbPersistent::IdType v, dbConnection &c) { set("circulationsqueue_id",v, c) ; }
	dbPersistent::IdType getCirculationsQueueId(bool &set, dbConnection &c) const { dbPersistent::IdType v ; get("circulationsqueue_id",v, set, c) ; return v ; }
	void unsetCirculationsQueueId() { unset("circulationsqueue_id") ; }
    bool nullCirculationsQueueId(dbConnection &conn) const { bool null ; getCirculationsQueueId(null, conn) ; return null ; } 

// the following columns have been added after jira issue S2PDGS-625

	void setUnknFromHost(const std::string &v, dbConnection &c) { set("unkn_from_host",v, c) ; }
	std::string getUnknFromHost(bool &set, dbConnection &c) const { std::string v ; get("unkn_from_host",v, set, c) ; return v ; }
	void unsetUnknFromHost() { unset("unkn_from_host") ; }
    bool nullUnknFromHost(dbConnection &conn) const { bool null ; getUnknFromHost(null, conn) ; return null ; } 

	void setUnknToHost(const std::string &v, dbConnection &c) { set("unkn_to_host",v, c) ; }
	std::string getUnknToHost(bool &set, dbConnection &c) const { std::string v ; get("unkn_to_host",v, set, c) ; return v ; }
	void unsetUnknToHost() { unset("unkn_to_host") ; }
    bool nullUnknToHost(dbConnection &conn) const { bool null ; getUnknToHost(null, conn) ; return null ; } 


    RemoteSite const& getSourceSite() const throw() ;
    RemoteSite const& getDestSite() const throw() ;
    void setSourceSite(RemoteSite const& sourceSite);
    void setDestSite(RemoteSite const& destSite);

    void mergeStatistics(dbConnection &, CirculationStatistics const& other, bool sumSize=true);

    std::string str(dbConnection &,bool withId = false) const;
    
    static std::string table() { return "t_circulationstats" ; } 

private:
    CirculationStatistics() ; // not implemented
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(CirculationStatistics)
    RemoteSite _sourceSite;
    RemoteSite _destSite;
} ;

// PRQA L:EOCLASS

typedef db::SmartQuery<CirculationStatistics> CirculationStatisticsQuery ;

_ACS_END_NESTED_NAMESPACE;

#endif /* _CirculationStatistics_h_ */
