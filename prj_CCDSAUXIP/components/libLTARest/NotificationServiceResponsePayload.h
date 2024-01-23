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

#ifndef _NotificationServiceResponsePayload_h_
#define _NotificationServiceResponsePayload_h_

#include <acs_c++config.hpp>
#include <ThreadSafe.h>
#include <string>
#include <json.hpp>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTANotificationServiceDispatcher */
class NotificationServiceResponsePayload {
public:
	exDECLARE_EXCEPTION(NotificationServiceResponsePayloadException, exException);

public:
	/** Ctors/Dtors */
	explicit NotificationServiceResponsePayload(size_t requestId);
	explicit NotificationServiceResponsePayload(const std::string& jsstr);
	explicit NotificationServiceResponsePayload(const nlohmann::json& json);
	NotificationServiceResponsePayload(const NotificationServiceResponsePayload&);
	virtual ~NotificationServiceResponsePayload() ACS_NOEXCEPT;
	
	/** Operators overload */
	NotificationServiceResponsePayload& operator=(const NotificationServiceResponsePayload&);
	
	/** Setters/Getters */
	inline void setErrorMessage(const std::string& errmsg) {
		Lock l(_mutex);
		_errmsgSet = true;
		_errmsg = errmsg;
	}

	inline size_t getRequestId() const {
		Lock l(_mutex);
		return _requestId;
	}
	
	inline const std::string& getErrorMessage(bool* isset = 0) const {
		Lock l(_mutex);
		if (isset != 0) {
			*isset = _errmsgSet;
		}
		return _errmsg;
	}

	/** Returns the serialized json representation of this object */
	std::string toJSON() const;
	nlohmann::json toJSONObject() const;
	
	/** Initialize this object from the json string in input */
	void fromJSONObject(const nlohmann::json& json);
	void fromJSON(const std::string& jsstr);

private:
	inline void setRequestId(size_t id) {
		Lock l(_mutex);
		_requestId = id;
	}

private:
	static const std::string RESPONSE_PAYLOAD_KEY_REQUEST_ID;
	static const std::string RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE;

private:
	Mutex _mutex;
	size_t _requestId;
	bool _errmsgSet;
	std::string _errmsg;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(NotificationServiceResponsePayload);
};

class NotificationServiceResponsePayloadList {
public:
	exDECLARE_EXCEPTION(NotificationServiceResponsePayloadListException, exException);
public:
	/** Ctors / Dtors */
	NotificationServiceResponsePayloadList();
	explicit NotificationServiceResponsePayloadList(const std::string& jsstr);
	NotificationServiceResponsePayloadList(const NotificationServiceResponsePayloadList&);
	virtual ~NotificationServiceResponsePayloadList() ACS_NOEXCEPT;

	/** Operators overload */
	NotificationServiceResponsePayload& operator[](size_t idx);
	const NotificationServiceResponsePayload& operator[](size_t idx) const;

	/** Insert the payload in the list at the position specified by payload.requestId */
	void add(const NotificationServiceResponsePayload& payload);
	void add(const NotificationServiceResponsePayloadList& payload);
	void sort();
	void sort(std::vector<NotificationServiceResponsePayload>&) const;
	void clear();
	size_t size() const;
	
	const std::vector<NotificationServiceResponsePayload>& getResponses() const;
	
	/** Returns the serialized json representation of this object */
	std::string toJSON() const;
	nlohmann::json toJSONObject() const;
	
	/** Initialize this object from the json string in input */
	void fromJSON(const std::string& json);
	void fromJSONObject(const nlohmann::json& json);

private:
	Mutex _mutex;
	std::vector<NotificationServiceResponsePayload> _responses;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(NotificationServiceResponsePayloadList);

};

_ACS_END_NAMESPACE

#endif /* _NotificationServiceResponsePayload_h_ */
