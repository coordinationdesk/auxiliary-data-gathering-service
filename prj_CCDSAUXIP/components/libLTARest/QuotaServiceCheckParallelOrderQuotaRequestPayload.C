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

#include <QuotaServiceCheckParallelOrderQuotaRequestPayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(QuotaServiceCheckParallelOrderQuotaRequestPayload);

const std::string QuotaServiceCheckParallelOrderQuotaRequestPayload::REQUEST_PAYLOAD_KEY_ORDERUUID = "OrderUUID";

QuotaServiceCheckParallelOrderQuotaRequestPayload::QuotaServiceCheckParallelOrderQuotaRequestPayload() :
	_mutex(),
	_orderUUID()
{ }

QuotaServiceCheckParallelOrderQuotaRequestPayload::QuotaServiceCheckParallelOrderQuotaRequestPayload(const std::string& jsstr) :
	_mutex(),
	_orderUUID()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}

QuotaServiceCheckParallelOrderQuotaRequestPayload::QuotaServiceCheckParallelOrderQuotaRequestPayload(const nlohmann::json& js) :
	_mutex(),
	_orderUUID()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}

QuotaServiceCheckParallelOrderQuotaRequestPayload::QuotaServiceCheckParallelOrderQuotaRequestPayload(const QuotaServiceCheckParallelOrderQuotaRequestPayload& data) :
	_mutex(),
	_orderUUID(data._orderUUID)
{ }

QuotaServiceCheckParallelOrderQuotaRequestPayload::~QuotaServiceCheckParallelOrderQuotaRequestPayload() ACS_NOEXCEPT { }

QuotaServiceCheckParallelOrderQuotaRequestPayload& QuotaServiceCheckParallelOrderQuotaRequestPayload::operator=(const QuotaServiceCheckParallelOrderQuotaRequestPayload& data) {
	if (this != &data) {
		bool toSet = true;
		while (toSet) {
			Lock l1(_mutex);
			TryLock l2(data._mutex);
			if (l2.locked()) {
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

void QuotaServiceCheckParallelOrderQuotaRequestPayload::fromJSON(const std::string& jsstr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	nlohmann::json js;
	try {
		js = nlohmann::json::parse(jsstr);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(QuotaServiceCheckParallelOrderQuotaRequestPayloadException(e, "Error reading JSON."));
	}
	fromJSONObject(js);
}

void QuotaServiceCheckParallelOrderQuotaRequestPayload::fromJSONObject(const nlohmann::json& json) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	Lock l(_mutex);
	
	try {
		if (json.find(REQUEST_PAYLOAD_KEY_ORDERUUID) == json.end() || json[REQUEST_PAYLOAD_KEY_ORDERUUID].is_null()) {
			ACS_THROW(QuotaServiceCheckParallelOrderQuotaRequestPayloadException("Cannot find key \"" + REQUEST_PAYLOAD_KEY_ORDERUUID + "\" into json request payload or the associated value is null. Key is mandatory"));
		}
		_orderUUID = json[REQUEST_PAYLOAD_KEY_ORDERUUID];
		if (_orderUUID.empty())
		{
			ACS_THROW(QuotaServiceCheckParallelOrderQuotaRequestPayloadException("Empty value for key \"" + REQUEST_PAYLOAD_KEY_ORDERUUID + "\" into json request payload. Value is mandatory"));
		}

	} catch (nlohmann::json::exception& e) {
		ACS_THROW(QuotaServiceCheckParallelOrderQuotaRequestPayloadException(e, "Error reading JSON Object."));
	}
}

std::string QuotaServiceCheckParallelOrderQuotaRequestPayload::toJSON() const {
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
		ACS_THROW(QuotaServiceCheckParallelOrderQuotaRequestPayloadException(e, "Error creating JSON."));
	}
	// LCOV_EXCL_STOP
	return str;
}

nlohmann::json QuotaServiceCheckParallelOrderQuotaRequestPayload::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	Lock l(_mutex);
	
	nlohmann::json js;
	try {
		js[REQUEST_PAYLOAD_KEY_ORDERUUID] = _orderUUID;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _orderUUID << "\" for key \"" << REQUEST_PAYLOAD_KEY_ORDERUUID << "\"");
	}
	// LCOV_EXCL_START
	// Defensive coding: the data validity is guaranteed by the object
	catch (nlohmann::json::exception& e) {
		ACS_THROW(QuotaServiceCheckParallelOrderQuotaRequestPayloadException(e, "Error creating JSON Object."));
	}
	// LCOV_EXCL_STOP
	return js;
}

_ACS_END_NAMESPACE
