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

#include <NotificationServiceRequestPayload.h>
#include <FtpCurl.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace curl;

ACS_CLASS_DEFINE_DEBUG_LEVEL(NotificationServiceRequestPayload);
ACS_CLASS_DEFINE_DEBUG_LEVEL(NotificationServiceRequestPayloadList);

namespace {
	bool RequestPayloadsPredicate (NotificationServiceRequestPayload i,NotificationServiceRequestPayload j) { return (i.getRequestId() < j.getRequestId()); }
}

const std::string NotificationServiceRequestPayload::REQUEST_PAYLOAD_KEY_REQUEST_ID = "RequestId";
const std::string NotificationServiceRequestPayload::REQUEST_PAYLOAD_KEY_PRODUCT_ID = "ProductId";
const std::string NotificationServiceRequestPayload::REQUEST_PAYLOAD_KEY_PRODUCT_NAME = "ProductName";
const std::string NotificationServiceRequestPayload::REQUEST_PAYLOAD_KEY_SUBSCRIPTION_ID = "SubscriptionId";
const std::string NotificationServiceRequestPayload::REQUEST_PAYLOAD_KEY_ORDER_ID = "OrderId";
const std::string NotificationServiceRequestPayload::REQUEST_PAYLOAD_KEY_BATCHORDER_ID = "BatchOrderId";
const std::string NotificationServiceRequestPayload::REQUEST_PAYLOAD_KEY_SUBSCRIPTION_EVENT = "SubscriptionEvent";
const std::string NotificationServiceRequestPayload::REQUEST_PAYLOAD_KEY_NOTIFICATIONDATE = "NotificationDate";
const std::string NotificationServiceRequestPayload::REQUEST_PAYLOAD_KEY_ENDPOINT = "Endpoint";
const std::string NotificationServiceRequestPayload::REQUEST_PAYLOAD_KEY_ENDPOINTUSER = "EndpointUser";
const std::string NotificationServiceRequestPayload::REQUEST_PAYLOAD_KEY_ENDPOINTPASSWORD = "EndpointPassword";

std::string NotificationServiceRequestPayload::subscriptionEvent2String(const NotificationServiceRequestPayload::SubscriptionEvent& se) {
	std::string event;
	switch (se) {
	case NotificationServiceRequestPayload::unknown:
		event = "unknown";
		break;
	case NotificationServiceRequestPayload::created:
		event = "created";
		break;
	case NotificationServiceRequestPayload::deleted:
		event = "deleted";
		break;
	default:
		std::ostringstream ss;
		ss << "Invalid subscription event having id " << static_cast<unsigned int>(se);
		ACS_THROW(NotificationServiceRequestPayloadException(ss.str()));
		break;
	}
	
	return event;
}

NotificationServiceRequestPayload::SubscriptionEvent NotificationServiceRequestPayload::string2SubscriptionEvent(const std::string& se) {
	SubscriptionEvent val = NotificationServiceRequestPayload::unknown;
	if (StringUtils::equalsNoCase(se, "unknown")) {
		val = NotificationServiceRequestPayload::unknown;
	}
	else if (StringUtils::equalsNoCase(se, "created")) {
		val = NotificationServiceRequestPayload::created;
	}
	
	else if (StringUtils::equalsNoCase(se, "deleted")) {
		val = NotificationServiceRequestPayload::deleted;
	}
	else {
		ACS_THROW(NotificationServiceRequestPayloadException("Unrecognized subscription event \"" + se + "\""));
	}
	
	return val;
}

NotificationServiceRequestPayload::NotificationServiceRequestPayload(size_t id, bool reqIdOptional) :
	_mutex(),
	_requestIdOptional(reqIdOptional),
	_productIdSet(false),
	_productNameSet(false),
	_subscriptionIdSet(false),
	_orderIdSet(false),
	_batchOrderIdSet(false),
	_subscriptionEventSet(false),
	_notificationDateSet(false),
	_endpointSet(false),
	_epuserSet(false),
	_eppasswdSet(false),
	_requestId(id),
	_productId(),
	_productName(),
	_subscriptionId(),
	_orderId(),
	_batchOrderId(),
	_subscriptionEvent(unknown),
	_notificationDate(),
	_endpoint(),
	_epuser(),
	_eppasswd()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

NotificationServiceRequestPayload::NotificationServiceRequestPayload(const std::string& jsstr, bool reqIdOptional) :
	_mutex(),
	_requestIdOptional(reqIdOptional),
	_productIdSet(false),
	_productNameSet(false),
	_subscriptionIdSet(false),
	_orderIdSet(false),
	_batchOrderIdSet(false),
	_subscriptionEventSet(false),
	_notificationDateSet(false),
	_endpointSet(false),
	_epuserSet(false),
	_eppasswdSet(false),
	_requestId(),
	_productId(),
	_productName(),
	_subscriptionId(),
	_orderId(),
	_batchOrderId(),
	_subscriptionEvent(unknown),
	_notificationDate(),
	_endpoint(),
	_epuser(),
	_eppasswd()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}

NotificationServiceRequestPayload::NotificationServiceRequestPayload(const nlohmann::json& js, bool reqIdOptional) :
	_mutex(),
	_requestIdOptional(reqIdOptional),
	_productIdSet(false),
	_productNameSet(false),
	_subscriptionIdSet(false),
	_orderIdSet(false),
	_batchOrderIdSet(false),
	_subscriptionEventSet(false),
	_notificationDateSet(false),
	_endpointSet(false),
	_epuserSet(false),
	_eppasswdSet(false),
	_requestId(),
	_productId(),
	_productName(),
	_subscriptionId(),
	_orderId(),
	_batchOrderId(),
	_subscriptionEvent(unknown),
	_notificationDate(),
	_endpoint(),
	_epuser(),
	_eppasswd()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}

NotificationServiceRequestPayload::NotificationServiceRequestPayload(const NotificationServiceRequestPayload& data) :
	_mutex(),
	_requestIdOptional(data._requestIdOptional),
	_productIdSet(data._productIdSet),
	_productNameSet(data._productNameSet),
	_subscriptionIdSet(data._subscriptionIdSet),
	_orderIdSet(data._orderIdSet),
	_batchOrderIdSet(data._batchOrderIdSet),
	_subscriptionEventSet(data._subscriptionEventSet),
	_notificationDateSet(data._notificationDateSet),
	_endpointSet(data._endpointSet),
	_epuserSet(data._epuserSet),
	_eppasswdSet(data._eppasswdSet),
	_requestId(data._requestId),
	_productId(data._productId),
	_productName(data._productName),
	_subscriptionId(data._subscriptionId),
	_orderId(data._orderId),
	_batchOrderId(data._batchOrderId),
	_subscriptionEvent(data._subscriptionEvent),
	_notificationDate(data._notificationDate),
	_endpoint(data._endpoint),
	_epuser(data._epuser),
	_eppasswd(data._eppasswd)
{ }

NotificationServiceRequestPayload::~NotificationServiceRequestPayload() ACS_NOEXCEPT { }

NotificationServiceRequestPayload& NotificationServiceRequestPayload::operator=(const NotificationServiceRequestPayload& data) {
	if (this != &data) {
		bool toSet = true;
		while (toSet) {
			Lock l1(_mutex);
			TryLock l2(data._mutex);
			if (l2.locked()) {
				_requestIdOptional = data._requestIdOptional;
				_productIdSet = data._productIdSet;
				_productNameSet = data._productNameSet;
				_subscriptionIdSet = data._subscriptionIdSet;
				_orderIdSet = data._orderIdSet;
				_batchOrderIdSet = data._batchOrderIdSet;
				_subscriptionEventSet = data._subscriptionEventSet;
				_notificationDateSet = data._notificationDateSet;
				_endpointSet = data._endpointSet;
				_epuserSet = data._epuserSet;
				_eppasswdSet = data._eppasswdSet;
				_requestId = data._requestId;
				_productId = data._productId;
				_productName = data._productName;
				_subscriptionId = data._subscriptionId;
				_orderId = data._orderId;
				_batchOrderId = data._batchOrderId;
				_subscriptionEvent = data._subscriptionEvent;
				_notificationDate = data._notificationDate;
				_endpoint = data._endpoint;
				_epuser = data._epuser;
				_eppasswd = data._eppasswd;
				toSet = false;
			}
			else {
				usleep(100);
			}
		}
	}
	return *this;
}

nlohmann::json NotificationServiceRequestPayload::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	nlohmann::json js;
	try {
		// Add members to json object
		std::string strval;
		bool set = false;
		
		if (!_requestIdOptional)
		{
			js[REQUEST_PAYLOAD_KEY_REQUEST_ID] = getRequestId();
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << getRequestId() << "\" for key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_ID << "\"");
		}
		strval = getProductId(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_PRODUCT_ID] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_ID << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_ID << "\" not set. It will not be written");
		}
		
		strval = getProductName(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_PRODUCT_NAME] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_NAME << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_NAME << "\" not set. It will not be written");
		}
		
		strval = getSubscriptionId(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_SUBSCRIPTION_ID] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_SUBSCRIPTION_ID << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_SUBSCRIPTION_ID << "\" not set. It will not be written");
		}
		
		strval = getOrderId(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_ORDER_ID] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_ID << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_ID << "\" not set. It will not be written");
		}
		
		strval = getBatchOrderId(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_BATCHORDER_ID] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_BATCHORDER_ID << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_BATCHORDER_ID << "\" not set. It will not be written");
		}
		
		strval = subscriptionEvent2String(getSubscriptionEvent(&set));
		if (set) {
			js[REQUEST_PAYLOAD_KEY_SUBSCRIPTION_EVENT] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_SUBSCRIPTION_EVENT << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_SUBSCRIPTION_EVENT << "\" not set. It will not be written");
		}
		
		strval = getNotificationDate(&set);
		if (set)
		{
			js[REQUEST_PAYLOAD_KEY_NOTIFICATIONDATE] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_NOTIFICATIONDATE << "\"");
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_NOTIFICATIONDATE << "\" not set. It will not be written");
		}

		strval = getEndpoint(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_ENDPOINT] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_ENDPOINT << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_ENDPOINT << "\" not set. It will not be written");
		}
		
		strval = getEndpointUser(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_ENDPOINTUSER] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_ENDPOINTUSER << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_ENDPOINTUSER << "\" not set. It will not be written");
		}
		
		strval = getEndpointPassword(&set);
		if (set) {
			js[REQUEST_PAYLOAD_KEY_ENDPOINTPASSWORD] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << REQUEST_PAYLOAD_KEY_ENDPOINTPASSWORD << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << REQUEST_PAYLOAD_KEY_ENDPOINTPASSWORD << "\" not set. It will not be written");
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceRequestPayloadException(e, "Error creating JSON Object."));
	}
	return js;
}

std::string NotificationServiceRequestPayload::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	std::string jsstr;
	nlohmann::json js;
	try {
		js = toJSONObject();
		jsstr = js.dump();
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceRequestPayloadException(e, "Error creating JSON."));
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transformed request data into the following json string:\n" << jsstr);
	
	return jsstr;
}

void NotificationServiceRequestPayload::fromJSON(const std::string& jsstr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	nlohmann::json js;
	try {
		js = nlohmann::json::parse(jsstr);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceRequestPayloadException(e, "Error reading JSON."));
	}
	fromJSONObject(js);
}

void NotificationServiceRequestPayload::fromJSONObject(const nlohmann::json& js) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	try {
		
		if (!_requestIdOptional)
		{
			ACS_COND_THROW((js.find(REQUEST_PAYLOAD_KEY_REQUEST_ID) == js.end()) || (js[REQUEST_PAYLOAD_KEY_REQUEST_ID].is_null()),
						NotificationServiceRequestPayloadException("Key \"" + REQUEST_PAYLOAD_KEY_REQUEST_ID + "\" not found into json request payload or the associated value is null. RequestId key is mandatory"));

			setRequestId(js[REQUEST_PAYLOAD_KEY_REQUEST_ID]);
		}
		
		std::string val;
		if (js.find(REQUEST_PAYLOAD_KEY_PRODUCT_ID) != js.end() && !js[REQUEST_PAYLOAD_KEY_PRODUCT_ID].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_PRODUCT_ID];
			if (!val.empty()) {
				setProductId(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_ID << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_ID << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_ID << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_PRODUCT_NAME) != js.end() && !js[REQUEST_PAYLOAD_KEY_PRODUCT_NAME].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_PRODUCT_NAME];
			if (!val.empty()) {
				setProductName(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_NAME << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_NAME << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_PRODUCT_NAME << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_SUBSCRIPTION_ID) != js.end() && !js[REQUEST_PAYLOAD_KEY_SUBSCRIPTION_ID].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_SUBSCRIPTION_ID];
			if (!val.empty()) {
				setSubscriptionId(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_SUBSCRIPTION_ID << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_SUBSCRIPTION_ID << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_SUBSCRIPTION_ID << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_ORDER_ID) != js.end() && !js[REQUEST_PAYLOAD_KEY_ORDER_ID].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_ORDER_ID];
			if (!val.empty()) {
				setOrderId(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_ORDER_ID << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_ORDER_ID << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_ORDER_ID << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_BATCHORDER_ID) != js.end() && !js[REQUEST_PAYLOAD_KEY_BATCHORDER_ID].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_BATCHORDER_ID];
			if (!val.empty()) {
				setBatchOrderId(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_BATCHORDER_ID << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_BATCHORDER_ID << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_BATCHORDER_ID << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_SUBSCRIPTION_EVENT) != js.end() && !js[REQUEST_PAYLOAD_KEY_SUBSCRIPTION_EVENT].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_SUBSCRIPTION_EVENT];
			if (!val.empty()) {
				setSubscriptionEvent(string2SubscriptionEvent(val));
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_SUBSCRIPTION_EVENT << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_SUBSCRIPTION_EVENT << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_SUBSCRIPTION_EVENT << "\" not found or the associated value was null. It will not be loaded.");
		}
		if (js.find(REQUEST_PAYLOAD_KEY_NOTIFICATIONDATE) != js.end() && !js[REQUEST_PAYLOAD_KEY_NOTIFICATIONDATE].is_null())
		{
			std::string val = js[REQUEST_PAYLOAD_KEY_NOTIFICATIONDATE];
			if (!val.empty()) {
  				setNotificationDate(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_NOTIFICATIONDATE << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_NOTIFICATIONDATE << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_NOTIFICATIONDATE << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_ENDPOINT) != js.end() && !js[REQUEST_PAYLOAD_KEY_ENDPOINT].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_ENDPOINT];
			if (!val.empty()) {
				setEndpoint(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_ENDPOINT << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_ENDPOINT << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_ENDPOINT << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_ENDPOINTUSER) != js.end() && !js[REQUEST_PAYLOAD_KEY_ENDPOINTUSER].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_ENDPOINTUSER];
			if (!val.empty()) {
				setEndpointUser(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_ENDPOINTUSER << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_ENDPOINTUSER << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_ENDPOINTUSER << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (js.find(REQUEST_PAYLOAD_KEY_ENDPOINTPASSWORD) != js.end() && !js[REQUEST_PAYLOAD_KEY_ENDPOINTPASSWORD].is_null()) {
			std::string val = js[REQUEST_PAYLOAD_KEY_ENDPOINTPASSWORD];
			if (!val.empty()) {
				setEndpointPassword(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << REQUEST_PAYLOAD_KEY_ENDPOINTPASSWORD << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << REQUEST_PAYLOAD_KEY_ENDPOINTPASSWORD << "\". It will not be loaded.");
			}
		} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << REQUEST_PAYLOAD_KEY_ENDPOINTPASSWORD << "\" not found or the associated value was null. It will not be loaded.");
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceRequestPayloadException(e, "Error reading JSON Object."));
	}
}
void NotificationServiceRequestPayload::setRequestIdOptional(bool flag) 
{ 
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	_requestIdOptional = flag;

}

NotificationServiceRequestPayloadList::NotificationServiceRequestPayloadList() : 
	_mutex(),
	_requests()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

NotificationServiceRequestPayloadList::NotificationServiceRequestPayloadList(const std::string& jsstr) : 
	_mutex(),
	_requests()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}

NotificationServiceRequestPayloadList::NotificationServiceRequestPayloadList(const nlohmann::json& js) : 
	_mutex(),
	_requests()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}

NotificationServiceRequestPayloadList::NotificationServiceRequestPayloadList(const NotificationServiceRequestPayloadList& list) :
	_mutex(),
	_requests(list._requests)
{ }

NotificationServiceRequestPayload& NotificationServiceRequestPayloadList::operator[](size_t idx) {
	Lock l(_mutex);
	if (idx >= _requests.size()) {
		std::ostringstream ss;
		ss << "Invalid index " << idx << ". Size is " << _requests.size();
		ACS_THROW(exBoundsException(ss.str()));
	}
	
	return _requests[idx];
}

const NotificationServiceRequestPayload& NotificationServiceRequestPayloadList::operator[](size_t idx) const {
	Lock l(_mutex);
	if (idx >= _requests.size()) {
		std::ostringstream ss;
		ss << "Invalid index " << idx << ". Size is " << _requests.size();
		ACS_THROW(exBoundsException(ss.str()));
	}
	
	return _requests[idx];
}

NotificationServiceRequestPayloadList::~NotificationServiceRequestPayloadList() ACS_NOEXCEPT { }

void NotificationServiceRequestPayloadList::add(const NotificationServiceRequestPayload& payload) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	_requests.push_back(payload);
}

void NotificationServiceRequestPayloadList::add(const NotificationServiceRequestPayloadList& payload) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	Lock l(_mutex);
	const std::vector<NotificationServiceRequestPayload>& p = payload.getRequests();
	_requests.insert(_requests.end(), p.begin(), p.end());
}

const std::vector<NotificationServiceRequestPayload>& NotificationServiceRequestPayloadList::getRequests() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	return _requests;
}

void NotificationServiceRequestPayloadList::sort() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	std::sort(_requests.begin(), _requests.end(), RequestPayloadsPredicate);
}

void NotificationServiceRequestPayloadList::sort(std::vector<NotificationServiceRequestPayload>& out) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	out = _requests;
	std::sort(out.begin(), out.end(), RequestPayloadsPredicate);
}

void NotificationServiceRequestPayloadList::clear() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	_requests.clear();
}

size_t NotificationServiceRequestPayloadList::size() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	return _requests.size();
}

nlohmann::json NotificationServiceRequestPayloadList::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	nlohmann::json js;
	try {
		
		js["notification_requests"] = nlohmann::json::array(); // create the empty array

		std::vector<NotificationServiceRequestPayload> rq = _requests;
		sort(rq);

		size_t rqSize = rq.size();
		for (size_t idx = 0; idx < rqSize; ++idx) {
			const NotificationServiceRequestPayload& payload = rq[idx];
			nlohmann::json rqjs = payload.toJSONObject();
			js["notification_requests"].push_back(rqjs);
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceRequestPayloadListException(e, "Error creating JSON Object."));
	}
	 
	return js;
}

std::string NotificationServiceRequestPayloadList::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string jsdumped;
	nlohmann::json js;
	try {
		js = toJSONObject();
		jsdumped = js.dump();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generated the following json: " << jsdumped)
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceRequestPayloadListException(e, "Error creating JSON."));
	}
	return jsdumped;
}

void NotificationServiceRequestPayloadList::fromJSON(const std::string& json) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	nlohmann::json js;
	try {
		js = nlohmann::json::parse(json);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceRequestPayloadListException(e, "Error reading JSON."));
	}
	
	fromJSONObject(js);
}

void NotificationServiceRequestPayloadList::fromJSONObject(const nlohmann::json& js) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	try {
		if (js.find("notification_requests") == js.end()) {
			ACS_THROW(NotificationServiceRequestPayloadListException("Cannot find key \"notification_requests\" into json response payload. RequestId key is mandatory"));
		}
		size_t listSize = js["notification_requests"].size();
		for (size_t idx = 0; idx < listSize; ++idx) {
			nlohmann::json jsobj = js["notification_requests"][idx];
			NotificationServiceRequestPayload rq(jsobj);
			add(rq);
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceRequestPayloadListException(e, "Error reading JSON Object."));
	}
}

_ACS_END_NAMESPACE
