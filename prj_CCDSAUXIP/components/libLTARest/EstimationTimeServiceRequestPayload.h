// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA Rest Interface $


*/

#ifndef _EstimationTimeServiceRequestPayload_h_
#define _EstimationTimeServiceRequestPayload_h_

#include <acs_c++config.hpp>
#include <ThreadSafe.h>
#include <string>
#include <json.hpp>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful request sent to the LTANotificationServiceDispatcher */
class EstimationTimeServiceRequestPayload {
public:
	
	exDECLARE_EXCEPTION(EstimationTimeServiceRequestPayloadException, exException);
public:
	/** Ctors/Dtors */
	EstimationTimeServiceRequestPayload();
	explicit EstimationTimeServiceRequestPayload(const std::string& jsstr);
	explicit EstimationTimeServiceRequestPayload(const nlohmann::json& js);
	EstimationTimeServiceRequestPayload(const EstimationTimeServiceRequestPayload&);
	virtual ~EstimationTimeServiceRequestPayload() ACS_NOEXCEPT;
	
	/** Operators overload */
	EstimationTimeServiceRequestPayload& operator=(const EstimationTimeServiceRequestPayload&);
	
	/** Setters */
	inline void addInventoryId(dbPersistent::IdType id) {
		Lock l(_mutex);
		_inventoryIds.push_back(id);
	}
	
	inline void setInventoryIds(const std::vector<dbPersistent::IdType>& ids) { 
		Lock l(_mutex);
		_inventoryIds = ids;
	}
	
	inline void setJobResponsible(const std::string& jr) { 
		Lock l(_mutex);
		_jobResponsible = jr;
	}
	
	inline void setPriority(int prio) { 
		Lock l(_mutex);
		_priority = prio;
	}

	/** Getters */
	inline void getInventoryIds(std::vector<dbPersistent::IdType>& ids) const {
		ids.clear();
		
		Lock l(_mutex);
		ids = _inventoryIds;
	}
	
	inline const std::string& getJobResponsible() const {
		Lock l(_mutex);
		return _jobResponsible;
	}
	
	inline int getPriority() const {
		Lock l(_mutex);
		return _priority;
	}
	
	/** Returns the serialized json representation of this object */
	std::string toJSON() const;
	nlohmann::json toJSONObject() const;
	
	/** Initialize this object from the json string in input */
	void fromJSONObject(const nlohmann::json& json);
	void fromJSON(const std::string& json);
	
private:
	static const std::string REQUEST_PAYLOAD_KEY_INVENTORY_IDS;
	static const std::string REQUEST_PAYLOAD_KEY_JOBRESPONSIBLE;
	static const std::string REQUEST_PAYLOAD_KEY_PRIORITY;

private:
	Mutex _mutex;
	std::string _jobResponsible;
	std::vector<dbPersistent::IdType> _inventoryIds;
	int _priority;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(EstimationTimeServiceRequestPayload);
};


class EstimationTimeServiceRequestPayloadList {
public:
	exDECLARE_EXCEPTION(EstimationTimeServiceRequestPayloadListException, exException);
public:
	/** Ctors / Dtors */
	EstimationTimeServiceRequestPayloadList();
	explicit EstimationTimeServiceRequestPayloadList(const std::string& jsstr);
	explicit EstimationTimeServiceRequestPayloadList(const nlohmann::json& js);
	EstimationTimeServiceRequestPayloadList(const EstimationTimeServiceRequestPayloadList&);
	virtual ~EstimationTimeServiceRequestPayloadList() ACS_NOEXCEPT;
	
	/** Access operators overload */
	EstimationTimeServiceRequestPayload& operator[](size_t idx); 
	const EstimationTimeServiceRequestPayload& operator[](size_t idx) const; 
	
	/** Inserts the payload in the list */
	void add(const EstimationTimeServiceRequestPayload& payload);
	void add(const EstimationTimeServiceRequestPayloadList& payload);
	
	/** Returns the requests vector. */
	const std::vector<EstimationTimeServiceRequestPayload>& getRequests() const;
	
	/** In-place sort function. Sorts the payloads with respect to their request id */
	void sort();
	/** Out-of-place sort function. Sorts the payloads with respect to their request id */
	void sort(std::vector<EstimationTimeServiceRequestPayload>&) const;
	/** Clears the payload list */
	void clear();
	/** Returns the size of the list */
	size_t size() const;
	
	/** Returns the serialized json representation of this object */
	std::string toJSON() const;
	/** Returns the nlohmann object associated to this list */
	nlohmann::json toJSONObject() const;
	
	/** Initialize this object from the json string in input */
	void fromJSON(const std::string& json);
	/** Initialize this object from the nlohmann json object in input */
	void fromJSONObject(const nlohmann::json& json);
	
private:
	Mutex _mutex;
	std::vector<EstimationTimeServiceRequestPayload> _requests;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(EstimationTimeServiceRequestPayloadList);

};

_ACS_END_NAMESPACE

#endif /* _EstimationTimeServiceRequestPayload_h_ */
