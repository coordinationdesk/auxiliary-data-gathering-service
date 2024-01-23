// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2019, Advanced Computer Systems , Inc.
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
 *
 */

#ifndef _Circulation_H_
#define _Circulation_H_


#include <acs_c++config.hpp>
#include <dbSmartPersistent.h>
#include <dbSmartQuery.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) ;

// PRQA S 4101, 4214 EOCLASS

class Circulation : // PRQA S 2109, 2153
	public dbSmartPersistent
{
public:
	explicit Circulation(dbConnection &conn) ;
	Circulation(const Circulation &) ;

	virtual ~Circulation() throw() ;
	Circulation &operator=(const Circulation &) ; // operator =

	void setStartDate(const DateTime &v, dbConnection &c) { set("startdate",v, c) ; }
	DateTime getStartDate(bool &isNull, dbConnection &c) const { DateTime v ; get("startdate",v, isNull, c) ; return v ; }
	void unsetStartDate() { unset("startdate") ; }
    bool nullStartDate(dbConnection &conn) const { bool null ; getStartDate(null, conn) ; return null ; } 

	void setEndDate(const DateTime &v, dbConnection &c) { set("enddate",v, c) ; }
	DateTime getEndDate(bool &isNull, dbConnection &c) const { DateTime v ; get("enddate",v, isNull, c) ; return v ; }
	void unsetEndDate() { unset("enddate") ; }
    bool nullEndDate(dbConnection &conn) const { bool null ; getEndDate(null, conn) ; return null ; } 

	void setPriority(int v, dbConnection &c) { set("priority",v, c) ; }
	int getPriority(bool &isNull, dbConnection &c) const { int v ; get("priority",v, isNull, c) ; return v ; }
	void unsetPriority() { unset("priority") ; }

	void setRemark(const std::string &v, dbConnection &c) { set("remark",v, c) ; }
	std::string getRemark(bool &isNull, dbConnection &c) const { std::string v ; get("remark",v, isNull, c) ; return v ; }
	void unsetRemark() { unset("remark") ; }

	void setStatus(short v, dbConnection &c) { set("status_id",v, c) ; }
	short getStatus(bool &isNull, dbConnection &c) const { short v ; get("status_id",v, isNull, c) ; return v ; }
	void unsetStatus() { unset("status_id") ; }

	void setCreationDate(const DateTime &v, dbConnection &c) { set("creationdate",v, c) ; }
	DateTime getCreationDate(bool &isNull, dbConnection &c) const { DateTime v ; get("creationdate",v, isNull, c) ; return v ; }
	void unsetCreationDate() { unset("creationdate") ; }
    bool nullCreationDate(dbConnection &conn) const { bool null ; getCreationDate(null, conn) ; return null ; } 

	void setModificationDate(const DateTime &v, dbConnection &c) { set("modificationdate",v, c) ; }
	DateTime getModificationDate(bool &isNull, dbConnection &c) const { DateTime v ; get("modificationdate",v, isNull, c) ; return v ; }
	void unsetModificationDate() { unset("modificationdate") ; }
    bool nullModificationDate(dbConnection &conn) const { bool null ; getModificationDate(null, conn) ; return null ; } 

	void setFromURL(const std::string &v, dbConnection &c) { set("fromurl",v, c) ; }
	std::string getFromURL(bool &isNull, dbConnection &c) const { std::string v ; get("fromurl",v, isNull, c) ; return v ; }
	void unsetFromURL() { unset("fromurl") ; }

	void setToURL(const std::string &v, dbConnection &c) { set("tourl",v, c) ; }
	std::string getToURL(bool &isNull, dbConnection &c) const { std::string v ; get("tourl",v, isNull, c) ; return v ; }
	void unsetToURL() { unset("tourl") ; }

	void setRelayActivityId(int v, dbConnection &c) { set("relay_activity_id",v, c) ; }
	int getRelayActivityId(bool &isNull, dbConnection &c) const { int v ; get("relay_activity_id",v, isNull, c) ; return v ; }
	void unsetRelayActivityId() { unset("relay_activity_id") ; }

	void setRelayHostId(int v, dbConnection &c) { set("relay_host_id",v, c) ; }
	int getRelayHostId(bool &isNull, dbConnection &c) const { int v ; get("relay_host_id",v, isNull, c) ; return v ; }
	void unsetRelayHostId() { unset("relay_host_id") ; }

	void setHostId(dbPersistent::IdType v, dbConnection &c) { set("host_id",v, c) ; }
	dbPersistent::IdType getHostId(bool &isNull, dbConnection &c) const { dbPersistent::IdType v ; get("host_id",v, isNull, c) ; return v ; }
	void unsetHostId() { unset("host_id") ; }

	void setRetries(short int v, dbConnection &c) { set("retries",v, c) ; }
	short int getRetries(bool &isNull, dbConnection &c) const { short int v ; get("retries",v, isNull, c) ; return v ; }
	void unsetRetries() { unset("retries") ; }

	void setNextRetryDate(const DateTime &v, dbConnection &c) { set("nextretrydate",v, c) ; }
	DateTime getNextRetryDate(bool &isNull, dbConnection &c) const { DateTime v ; get("nextretrydate",v, isNull, c) ; return v ; }
	void unsetNextRetryDate() { unset("nextretrydate") ; }
    bool nullNextRetryDate(dbConnection &conn) const { bool null ; getModificationDate(null, conn) ; return null ; } 

	void setCirculationModeId(int v, dbConnection &c) { set("circulationmode_id",v, c) ; }
	int getCirculationModeId(bool &isNull, dbConnection &c) const { int v ; get("circulationmode_id",v, isNull, c) ; return v ; }
	void unsetCirculationModeId() { unset("circulationmode_id") ; }

	void setRemoveSource(bool v, dbConnection &c) { set("removesource",v, c) ; }
	bool getRemoveSource(bool &isNull, dbConnection &c) const { bool v ; get("removesource",v, isNull, c) ; return v ; }
	void unsetRemoveSource() { unset("removesource") ; }

	void setSynchronized(const DateTime &v, dbConnection &c) { set("synchronized",v, c) ; }
	DateTime getSynchronized(bool &isNull, dbConnection &c) const { DateTime v ; get("synchronized",v, isNull, c) ; return v ; }
	void unsetSynchronized() { unset("synchronized") ; }

	void setCirculationPolicy(dbPersistent::IdType v, dbConnection &c) { set("circulationpolicy_id",v, c) ; }
	dbPersistent::IdType getCirculationPolicy(bool &isNull, dbConnection &c) const { dbPersistent::IdType v ; get("circulationpolicy_id",v, isNull, c) ; return v ; }
	void unsetCirculationPolicy() { unset("circulationpolicy_id") ; }
    bool nullCirculationPolicy(dbConnection &conn) const { bool null ; getCirculationPolicy(null, conn) ; return null ; } 
    
	void setDestHostId(dbPersistent::IdType v, dbConnection &c) { set("dest_host_id",v, c) ; }
	dbPersistent::IdType getDestHostId(bool &isNull, dbConnection &c) const { dbPersistent::IdType v ; get("dest_host_id",v, isNull, c) ; return v ; }
	void unsetDestHostId() { unset("dest_host_id") ; }
    bool nullDestHostId(dbConnection &conn) const { bool null ; getDestHostId(null, conn) ; return null ; } 

	void setTempNameValue(const std::string &v, dbConnection &c) { set("tempnamevalue",v, c) ; }
	std::string getTempNameValue(bool &isNull, dbConnection &c) const { std::string v ; get("tempnamevalue",v, isNull, c) ; return v ; }
	void unsetTempNameValue() { unset("tempnamevalue") ; }

	void setTempNameId(int v, dbConnection &c) { set("tempname_id",v, c) ; }
	int getTempNameId(bool &isNull, dbConnection &c) const { int v ; get("tempname_id",v, isNull, c) ; return v ; }
	void unsetTempNameId() { unset("tempname_id") ; }

	void setLocalTransferModeId(int v, dbConnection &c) { set("localtransfermode_id",v, c) ; }
	int getLocalTransferModeId(bool &isNull, dbConnection &c) const { int v ; get("localtransfermode_id",v, isNull, c) ; return v ; }
	void unsetLocalTransferModeId() { unset("localtransfermode_id") ; }

	void setDestProtocolId(short int v, dbConnection &c) { set("dest_protocol_id",v, c) ; }
	short int getDestProtocolId(bool &isNull, dbConnection &c) const { short int v ; get("dest_protocol_id",v, isNull, c) ; return v ; }
	void unsetDestProtocolId() { unset("dest_protocol_id") ; }

	void setStreamerId(dbPersistent::IdType id, dbConnection &c) { set("streamer_id", id, c) ; }
	bool getStreamerId(bool &isNull, dbConnection &c) const { dbPersistent::IdType id; get("streamer_id", id, isNull, c) ; return id ; }
	void unsetStreamerId() { unset("streamer_id") ; }
	bool nullStreamerId(dbConnection &conn) const {  bool null; getStreamerId(null, conn); return null; }

    std::string str(dbConnection &,bool withId = false) const;
    
    static std::string table() { return "t_circulationsqueue" ; } 

private:
    Circulation() ; // not implemented
private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(Circulation)
} ;
// PRQA L:EOCLASS

typedef db::SmartQuery<Circulation> CirculationQuery ;

_ACS_END_NESTED_NAMESPACE;


#endif /* _Circulation_H_ */
