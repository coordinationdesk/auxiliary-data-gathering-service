// PRQA S 1050 EOF
/*

	Copyright 2020, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	LTA Rest Interface


*/

#ifndef _ProductOrderResponsePayload_h_
#define _ProductOrderResponsePayload_h_

#include <acs_c++config.hpp>
#include <ThreadSafe.h>
#include <string>
#include <DateTime.h>
#include <json.hpp>

_ACS_BEGIN_NAMESPACE(acs)
using json = nlohmann::json;

/** This object implements the RESTful response received by LTADownloader */
class ProductOrderResponsePayload {
public:

	exDECLARE_EXCEPTION(ProductOrderResponsePayloadException, exException);

	enum JobStatus {
		unknown,
		queued,
		in_progress,
		completed,
		failed,
		cancelled 
	};

	static std::string jobStatus2String(const JobStatus& se);
	static JobStatus string2JobStatus(const std::string& se);

public:
	/** Ctors/Dtors */
	ProductOrderResponsePayload();
	ProductOrderResponsePayload(const std::string& jsstr);
	ProductOrderResponsePayload(const nlohmann::json& js);
	ProductOrderResponsePayload(const ProductOrderResponsePayload&);
	virtual ~ProductOrderResponsePayload() ACS_NOEXCEPT;
	
	/** Operators overload */
	ProductOrderResponsePayload& operator=(const ProductOrderResponsePayload&);

	/** Setters */
	inline void setOrderId(const std::string& id) { 
		Lock l(_mutex);
		_orderId = id;
		_orderIdSet = true;
	}

	inline void setStatus(const JobStatus& status) { 
		Lock l(_mutex);
		_status = status;
		_statusSet = true;
	}

	inline void setStatusMessage(const std::string& sm) {
		Lock l(_mutex);
		_statusMessage = sm;
		_statusMessageSet = true;
	}

	inline void setOrderSize(const int& os) {
		Lock l(_mutex);
		_orderSize = os;
		_orderSizeSet = true;
	}

	inline void setSubmissionDate(const std::string& sd) {
		Lock l(_mutex);
		_submissionDate = sd;
		_submissionDateSet = true;
	}

	inline void setEstimatedDate(const std::string& ed) {
		Lock l(_mutex);
		_estimatedDate = ed;
		_estimatedDateSet = true;
	}
	inline void setCompletedDate(const std::string& cd) {
		Lock l(_mutex);
		_completedDate = cd;
		_completedDateSet = true;
	}
	inline void setEvictionDate(const std::string& ed) {
		Lock l(_mutex);
		_evictionDate = ed;
		_evictionDateSet = true;
	}
	inline void setPriority(const int& p) {
		Lock l(_mutex);
		_priority = p;
		_prioritySet = true;
	}
	inline void setNotificationEndpoint(const std::string& ep) {
		Lock l(_mutex);
		_endpoint = ep;
		_endpointSet = true;
	}
	inline void setNotificationEpUsername(const std::string& eu) {
		Lock l(_mutex);
		_epuser = eu;
		_epuserSet = true;
	}
	inline void setNotificationEpPassword(const std::string& ep) {
		Lock l(_mutex);
		_eppasswd = ep;
		_eppasswdSet = true;
	}
	
	/** Getters */
	inline const std::string& getOrderId(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _orderIdSet;
		}
		return _orderId;
	}
	
	inline const JobStatus& getStatus(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _statusSet;
		}
		
		return _status;
	}
	

	inline std::string getStatusMessage(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _statusMessageSet;
		}
		
		return _statusMessage;
	}
	
	inline const int& getOrderSize(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _orderSizeSet;
		}
		
		return _orderSize;
	}
	
	inline std::string getSubmissionDate(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _submissionDateSet;
		}
		
		return _submissionDate;
	}

	inline std::string getEstimatedDate(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _estimatedDateSet;
		}
		
		return _estimatedDate;
	}
	inline std::string getCompletedDate(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _completedDateSet;
		}
		
		return _completedDate;
	}
	inline std::string getEvictionDate(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _evictionDateSet;
		}
		
		return _evictionDate;
	}
	inline int getPriority(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _prioritySet;
		}
		
		return _priority;
	}
	inline std::string getNotificationEndpoint(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _endpointSet;
		}
		
		return _endpoint;
	}
	inline std::string getNotificationEpUsername(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset =_epuserSet;
		}
		
		return _epuser;
	}
	inline std::string getNotificationEpPassword(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset =_eppasswdSet;
		}
		
		return _eppasswd;
	}

	/** Returns the serialized json representation of this object */
	std::string toJSON() const;
	nlohmann::json toJSONObject() const;
	
	/** Initialize this object from the json string in input */
	void fromJSONObject(const nlohmann::json& json);
	void fromJSON(const std::string& json);
protected:
	Mutex _mutex;
private:
	
	bool _orderIdSet;
	bool _statusSet;
	bool _statusMessageSet;
	bool _orderSizeSet;
	bool _submissionDateSet;
	bool _estimatedDateSet;
	bool _completedDateSet;
	bool _evictionDateSet;
	bool _prioritySet;
	bool _endpointSet;
	bool _epuserSet;
	bool _eppasswdSet;
	
	std::string 	_orderId;
	JobStatus 	_status;
	std::string 	_statusMessage;
	int 		_orderSize;
	std::string 	_submissionDate;
	std::string 	_estimatedDate;
	std::string 	_completedDate;
	std::string 	_evictionDate;
	int 		_priority;
	std::string 	_endpoint;
	std::string 	_epuser;
	std::string 	_eppasswd;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ProductOrderResponsePayload);
};


// void to_json(json& j, const ProductOrderResponsePayload& p);
// void from_json(const json& j, ProductOrderResponsePayload& payload);
// 
class ProductOrderResponsePayloadList {
public:
	exDECLARE_EXCEPTION(ProductOrderResponsePayloadListException, exException);
public:
	/** Ctors / Dtors */
	ProductOrderResponsePayloadList();
	explicit ProductOrderResponsePayloadList(const std::string& jsstr);
	explicit ProductOrderResponsePayloadList(const nlohmann::json& js);
	ProductOrderResponsePayloadList(const ProductOrderResponsePayloadList&);
	virtual ~ProductOrderResponsePayloadList() ACS_NOEXCEPT;
	
	/** Access operators overload */
	ProductOrderResponsePayload& operator[](size_t idx); 
	const ProductOrderResponsePayload& operator[](size_t idx) const; 
	
	/** Inserts the payload in the list */
	void add(const ProductOrderResponsePayload& payload);
	void add(const ProductOrderResponsePayloadList& payload);
	
	/** Returns the requests vector. */
	const std::vector<ProductOrderResponsePayload>& getResponses() const;
	
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
	std::vector<ProductOrderResponsePayload> _response;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ProductOrderResponsePayloadList);

};


_ACS_END_NAMESPACE

#endif /* _ProductOrderResponsePayload_h_ */
