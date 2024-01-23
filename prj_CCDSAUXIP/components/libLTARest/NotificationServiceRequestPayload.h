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

#ifndef _NotificationServiceRequestPayload_h_
#define _NotificationServiceRequestPayload_h_

#include <acs_c++config.hpp>
#include <ThreadSafe.h>
#include <string>
#include <json.hpp>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful request sent to the LTANotificationServiceDispatcher */
class NotificationServiceRequestPayload {
public:
	
	exDECLARE_EXCEPTION(NotificationServiceRequestPayloadException, exException);

	enum SubscriptionEvent {
		unknown,
		created,
		deleted
	};

	static std::string subscriptionEvent2String(const SubscriptionEvent& se);
	static SubscriptionEvent string2SubscriptionEvent(const std::string& se);
	
public:
	/** Ctors/Dtors */
	explicit NotificationServiceRequestPayload(size_t id, bool reqIdOptional = false);
	explicit NotificationServiceRequestPayload(const std::string& jsstr, bool reqIdOptional = false);
	explicit NotificationServiceRequestPayload(const nlohmann::json& js, bool reqIdOptional = false);
	NotificationServiceRequestPayload(const NotificationServiceRequestPayload&);
	virtual ~NotificationServiceRequestPayload() ACS_NOEXCEPT;
	
	/** Operators overload */
	NotificationServiceRequestPayload& operator=(const NotificationServiceRequestPayload&);
	
	/** Setters */
	inline void setProductId(const std::string& id) { 
		Lock l(_mutex);
		_productId = id;
		_productIdSet = true;
	}
	
	inline void setProductName(const std::string& name) { 
		Lock l(_mutex);
		_productName = name;
		_productNameSet = true;
	}
	
	inline void setNotificationDate(const std::string& se) {
		Lock l(_mutex);
		_notificationDate = se;
		_notificationDateSet = true;
	}

	inline void setSubscriptionId(const std::string& sid) {
		Lock l(_mutex);
		_subscriptionId = sid;
		_subscriptionIdSet = true;
	}
	
	inline void setOrderId(const std::string& oid) {
		Lock l(_mutex);
		_orderId = oid;
		_orderIdSet = true;
	}
	
	inline void setBatchOrderId(const std::string& boid) {
		Lock l(_mutex);
		_batchOrderId = boid;
		_batchOrderIdSet = true;
	}
	
	inline void setSubscriptionEvent(const SubscriptionEvent& se) {
		Lock l(_mutex);
		_subscriptionEvent = se;
		_subscriptionEventSet = true;
	}
	
	inline void setEndpoint(const std::string& ep) {
		Lock l(_mutex);
		_endpoint = ep;
		_endpointSet = true;
	}
	
	inline void setEndpointUser(const std::string& epusr) {
		Lock l(_mutex);
		_epuser = epusr;
		_epuserSet = true;
	}
	
	inline void setEndpointPassword(const std::string& eppasswd) {
		Lock l(_mutex);
		_eppasswd = eppasswd;
		_eppasswdSet = true;
	}

	/** Getters */
	inline size_t getRequestId() const {
		Lock l(_mutex);
		return _requestId;
	}
	
	inline const std::string& getProductId(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _productIdSet;
		}
		return _productId;
	}
	
	inline const std::string& getProductName(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _productIdSet;
		}
		
		return _productName;
	}
	
	inline std::string getNotificationDate(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _notificationDateSet;
		}
		
		return _notificationDate;
	}

	inline const std::string& getSubscriptionId(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _subscriptionIdSet;
		}
		
		return _subscriptionId;
	}
	
	inline const std::string& getOrderId(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _orderIdSet;
		}
		
		return _orderId;
	}
	
	inline const std::string& getBatchOrderId(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _batchOrderIdSet;
		}
		
		return _batchOrderId;
	}
	
	inline SubscriptionEvent getSubscriptionEvent(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _subscriptionEventSet;
		}
		
		return _subscriptionEvent;
	}
	
	inline const std::string& getEndpoint(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _endpointSet;
		}
		
		return _endpoint;
	}
	
	inline const std::string& getEndpointUser(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _epuserSet;
		}
		
		return _epuser;
	}
	
	inline const std::string& getEndpointPassword(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _eppasswdSet;
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
	void setRequestIdOptional(bool flag);

private:
	inline void setRequestId(size_t id) {
		Lock l(_mutex);
		_requestId = id;
	}
	
private:
	static const std::string REQUEST_PAYLOAD_KEY_REQUEST_ID;
	static const std::string REQUEST_PAYLOAD_KEY_PRODUCT_ID;
	static const std::string REQUEST_PAYLOAD_KEY_PRODUCT_NAME;
	static const std::string REQUEST_PAYLOAD_KEY_SUBSCRIPTION_ID;
	static const std::string REQUEST_PAYLOAD_KEY_ORDER_ID;
	static const std::string REQUEST_PAYLOAD_KEY_BATCHORDER_ID;
	static const std::string REQUEST_PAYLOAD_KEY_SUBSCRIPTION_EVENT;
	static const std::string REQUEST_PAYLOAD_KEY_NOTIFICATIONDATE;
	static const std::string REQUEST_PAYLOAD_KEY_ENDPOINT;
	static const std::string REQUEST_PAYLOAD_KEY_ENDPOINTUSER;
	static const std::string REQUEST_PAYLOAD_KEY_ENDPOINTPASSWORD;

private:
	Mutex _mutex;
	
	bool _requestIdOptional;
	
	bool _productIdSet;
	bool _productNameSet;
	bool _subscriptionIdSet;
	bool _orderIdSet;
	bool _batchOrderIdSet;
	bool _subscriptionEventSet;
	bool _notificationDateSet;
	bool _endpointSet;
	bool _epuserSet;
	bool _eppasswdSet;
	
	size_t _requestId;
	std::string _productId;
	std::string _productName;
	std::string _subscriptionId;
	std::string _orderId;
	std::string _batchOrderId;
	SubscriptionEvent _subscriptionEvent;
	std::string _notificationDate;
	std::string _endpoint;
	std::string _epuser;
	std::string _eppasswd;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(NotificationServiceRequestPayload);
};


class NotificationServiceRequestPayloadList {
public:
	exDECLARE_EXCEPTION(NotificationServiceRequestPayloadListException, exException);
public:
	/** Ctors / Dtors */
	NotificationServiceRequestPayloadList();
	explicit NotificationServiceRequestPayloadList(const std::string& jsstr);
	explicit NotificationServiceRequestPayloadList(const nlohmann::json& js);
	NotificationServiceRequestPayloadList(const NotificationServiceRequestPayloadList&);
	virtual ~NotificationServiceRequestPayloadList() ACS_NOEXCEPT;
	
	/** Access operators overload */
	NotificationServiceRequestPayload& operator[](size_t idx); 
	const NotificationServiceRequestPayload& operator[](size_t idx) const; 
	
	/** Inserts the payload in the list */
	void add(const NotificationServiceRequestPayload& payload);
	void add(const NotificationServiceRequestPayloadList& payload);
	
	/** Returns the requests vector. */
	const std::vector<NotificationServiceRequestPayload>& getRequests() const;
	
	/** In-place sort function. Sorts the payloads with respect to their request id */
	void sort();
	/** Out-of-place sort function. Sorts the payloads with respect to their request id */
	void sort(std::vector<NotificationServiceRequestPayload>&) const;
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
	std::vector<NotificationServiceRequestPayload> _requests;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(NotificationServiceRequestPayloadList);

};

_ACS_END_NAMESPACE

#endif /* _NotificationServiceRequestPayload_h_ */
