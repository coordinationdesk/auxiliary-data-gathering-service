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

#include <QuotaServiceCancelOrderRequestPayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(QuotaServiceCancelOrderRequestPayload);

const std::string QuotaServiceCancelOrderRequestPayload::REQUEST_PAYLOAD_KEY_USERID = "UserID";
const std::string QuotaServiceCancelOrderRequestPayload::REQUEST_PAYLOAD_KEY_ORDERUUID = "OrderUUID";


QuotaServiceCancelOrderRequestPayload::QuotaServiceCancelOrderRequestPayload() :
	_mutex(),
	_userID(0),
	_orderUUID()
{ }

QuotaServiceCancelOrderRequestPayload::QuotaServiceCancelOrderRequestPayload(const std::string& jsstr) :
	_mutex(),
	_userID(0),
	_orderUUID()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}

QuotaServiceCancelOrderRequestPayload::QuotaServiceCancelOrderRequestPayload(const nlohmann::json& js) :
	_mutex(),
	_userID(0),
	_orderUUID()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}

QuotaServiceCancelOrderRequestPayload::QuotaServiceCancelOrderRequestPayload(const QuotaServiceCancelOrderRequestPayload& data) :
	_mutex(),
	_userID(data._userID),
	_orderUUID(data._orderUUID)
{ }

QuotaServiceCancelOrderRequestPayload::~QuotaServiceCancelOrderRequestPayload() ACS_NOEXCEPT { }

QuotaServiceCancelOrderRequestPayload& QuotaServiceCancelOrderRequestPayload::operator=(const QuotaServiceCancelOrderRequestPayload& data) {
	if (this != &data) {
		bool toSet = true;
		while (toSet) {
			Lock l1(_mutex);
			TryLock l2(data._mutex);
			if (l2.locked()) {
				_userID = data._userID;
				_orderUUID = data._orderUUID;
				toSet = false;
			}
			else {
				usleep(100);
			}
		}
	}
	return *this;
}

void QuotaServiceCancelOrderRequestPayload::fromJSON(const std::string& jsstr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	nlohmann::json js;
	try {
		js = nlohmann::json::parse(jsstr);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(QuotaServiceCancelOrderRequestPayloadException(e, "Error reading JSON."));
	}
	fromJSONObject(js);
}

void QuotaServiceCancelOrderRequestPayload::fromJSONObject(const nlohmann::json& json) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	Lock l(_mutex);
	
	try {
		if (json.find(REQUEST_PAYLOAD_KEY_USERID) == json.end() || json[REQUEST_PAYLOAD_KEY_USERID].is_null()) {
			ACS_THROW(QuotaServiceCancelOrderRequestPayloadException("Cannot find key \"" + REQUEST_PAYLOAD_KEY_USERID + "\" into json request payload or the associated value is null. Key is mandatory"));
		}
		_userID = json[REQUEST_PAYLOAD_KEY_USERID];

		if (json.find(REQUEST_PAYLOAD_KEY_ORDERUUID) == json.end() || json[REQUEST_PAYLOAD_KEY_ORDERUUID].is_null()) {
			ACS_THROW(QuotaServiceCancelOrderRequestPayloadException("Cannot find key \"" + REQUEST_PAYLOAD_KEY_ORDERUUID + "\" into json request payload or the associated value is null. Key is mandatory"));
		}
		_orderUUID = json[REQUEST_PAYLOAD_KEY_ORDERUUID];
		if (_orderUUID.empty())
		{
			ACS_THROW(QuotaServiceCancelOrderRequestPayloadException("Empty value for key \"" + REQUEST_PAYLOAD_KEY_ORDERUUID + "\" into json request payload. Value is mandatory"));
		}

	} catch (nlohmann::json::exception& e) {
		ACS_THROW(QuotaServiceCancelOrderRequestPayloadException(e, "Error reading JSON Object."));
	}
}

std::string QuotaServiceCancelOrderRequestPayload::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string str;
	nlohmann::json js;
	try {
		js = toJSONObject();
		str = js.dump();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transformed request data into the following json string:\n" << str);
	}
	// LCOV_EXCL_START
	// Defensive coding: the data validity is guaranteed by the object
	catch (nlohmann::json::exception& e) {
		ACS_THROW(QuotaServiceCancelOrderRequestPayloadException(e, "Error creating JSON."));
	}
	// LCOV_EXCL_STOP
	return str;
}

nlohmann::json QuotaServiceCancelOrderRequestPayload::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	Lock l(_mutex);
	
	nlohmann::json js;
	try {
		js[REQUEST_PAYLOAD_KEY_USERID] = _userID;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _userID << "\" for key \"" << REQUEST_PAYLOAD_KEY_USERID << "\"");

		js[REQUEST_PAYLOAD_KEY_ORDERUUID] = _orderUUID;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _orderUUID << "\" for key \"" << REQUEST_PAYLOAD_KEY_ORDERUUID << "\"");
	}
	// LCOV_EXCL_START
	// Defensive coding: the data validity is guaranteed by the object
	catch (nlohmann::json::exception& e) {
		ACS_THROW(QuotaServiceCancelOrderRequestPayloadException(e, "Error creating JSON Object."));
	}
	// LCOV_EXCL_STOP
	return js;
}

_ACS_END_NAMESPACE
