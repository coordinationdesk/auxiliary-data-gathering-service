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

#include <QuotaServiceResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(QuotaServiceResponsePayload);

const std::string QuotaServiceResponsePayload::RESPONSE_PAYLOAD_KEY_OPERATIONUUID = "OperationUUID";
const std::string QuotaServiceResponsePayload::RESPONSE_PAYLOAD_KEY_GRANTED = "Granted";
const std::string QuotaServiceResponsePayload::RESPONSE_PAYLOAD_KEY_REASON = "Reason";
const std::string QuotaServiceResponsePayload::RESPONSE_PAYLOAD_KEY_TOKENUUID = "TokenUUID";

QuotaServiceResponsePayload::QuotaServiceResponsePayload() :
	_mutex(),
	_operationUUID(),
	_granted(false),
	_reason(),
	_tokenUUIDSet(false),
	_tokenUUID()
{ }

QuotaServiceResponsePayload::QuotaServiceResponsePayload(const std::string& jsstr) :
	_mutex(),
	_operationUUID(),
	_granted(false),
	_reason(),
	_tokenUUIDSet(false),
	_tokenUUID()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}

QuotaServiceResponsePayload::QuotaServiceResponsePayload(const nlohmann::json& js) :
	_mutex(),
	_operationUUID(),
	_granted(false),
	_reason(),
	_tokenUUIDSet(false),
	_tokenUUID()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}

QuotaServiceResponsePayload::QuotaServiceResponsePayload(const QuotaServiceResponsePayload& data) :
	_mutex(),
	_operationUUID(data._operationUUID),
	_granted(data._granted),
	_reason(data._reason),
	_tokenUUIDSet(data._tokenUUIDSet),
	_tokenUUID(data._tokenUUID)
{ }

QuotaServiceResponsePayload::~QuotaServiceResponsePayload() ACS_NOEXCEPT { }

QuotaServiceResponsePayload& QuotaServiceResponsePayload::operator=(const QuotaServiceResponsePayload& data) {
	if (this != &data) {
		bool toSet = true;
		while (toSet) {
			Lock l1(_mutex);
			TryLock l2(data._mutex);
			if (l2.locked()) {
				_operationUUID = data._operationUUID;
				_granted = data._granted;
				_reason = data._reason;
				_tokenUUIDSet = data._tokenUUIDSet;
				_tokenUUID = data._tokenUUID;
				toSet = false;
			}
			else {
				usleep(100);
			}
		}
	}
	return *this;
}

void QuotaServiceResponsePayload::fromJSON(const std::string& jsstr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	nlohmann::json js;
	try {
		js = nlohmann::json::parse(jsstr);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(QuotaServiceResponsePayloadException(e, "Error reading JSON."));
	}
	fromJSONObject(js);
}

void QuotaServiceResponsePayload::fromJSONObject(const nlohmann::json& json) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	Lock l(_mutex);
	
	try {
		if (json.find(RESPONSE_PAYLOAD_KEY_OPERATIONUUID) != json.end() && !json[RESPONSE_PAYLOAD_KEY_OPERATIONUUID].is_null()) {
			_operationUUID = json[RESPONSE_PAYLOAD_KEY_OPERATIONUUID];
		} else {
			ACS_THROW(QuotaServiceResponsePayloadException("Cannot find key \"" + RESPONSE_PAYLOAD_KEY_OPERATIONUUID + "\" into json response payload or the associated value is null. Key is mandatory."));
		}

		if (json.find(RESPONSE_PAYLOAD_KEY_GRANTED) == json.end() || json[RESPONSE_PAYLOAD_KEY_GRANTED].is_null()) {
			ACS_THROW(QuotaServiceResponsePayloadException("Cannot find key \"" + RESPONSE_PAYLOAD_KEY_GRANTED + "\" into json response payload or the associated value is null. Key is mandatory."));
		}
		_granted = json[RESPONSE_PAYLOAD_KEY_GRANTED];
		
		if (json.find(RESPONSE_PAYLOAD_KEY_REASON) != json.end() && !json[RESPONSE_PAYLOAD_KEY_REASON].is_null()) {
			_reason = json[RESPONSE_PAYLOAD_KEY_REASON];
		} else {
			ACS_THROW(QuotaServiceResponsePayloadException("Cannot find key \"" + RESPONSE_PAYLOAD_KEY_REASON + "\" into json response payload or the associated value is null. Key is mandatory."));
		}
		
		if (json.find(RESPONSE_PAYLOAD_KEY_TOKENUUID) != json.end() && !json[RESPONSE_PAYLOAD_KEY_TOKENUUID].is_null()) {
			_tokenUUID = json[RESPONSE_PAYLOAD_KEY_TOKENUUID];
			_tokenUUIDSet = true;
		} else {
			_tokenUUIDSet = false;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << RESPONSE_PAYLOAD_KEY_TOKENUUID << "\" not found or the associated value was null. It will not be loaded.");
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(QuotaServiceResponsePayloadException(e, "Error reading JSON Object."));
	}
}

std::string QuotaServiceResponsePayload::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string str;
	nlohmann::json js;
	try {
		js = toJSONObject();
		str = js.dump();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transformed response data into the following json string:\n" << str);
	}
	// LCOV_EXCL_START
	// Defensive coding: the data validity is guaranteed by the object
	catch (nlohmann::json::exception& e) {
		ACS_THROW(QuotaServiceResponsePayloadException(e, "Error creating JSON."));
	}
	// LCOV_EXCL_STOP
	return str;
}

nlohmann::json QuotaServiceResponsePayload::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
	Lock l(_mutex);
	
	nlohmann::json js;
	try {
		js[RESPONSE_PAYLOAD_KEY_OPERATIONUUID] = _operationUUID;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _operationUUID << "\" for key \"" << RESPONSE_PAYLOAD_KEY_OPERATIONUUID << "\"");

		js[RESPONSE_PAYLOAD_KEY_GRANTED] = _granted;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _granted << "\" for key \"" << RESPONSE_PAYLOAD_KEY_GRANTED << "\"");

		js[RESPONSE_PAYLOAD_KEY_REASON] = _reason;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _reason << "\" for key \"" << RESPONSE_PAYLOAD_KEY_REASON << "\"");

		if (_tokenUUIDSet) {
			js[RESPONSE_PAYLOAD_KEY_TOKENUUID] = _tokenUUID;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _tokenUUID << "\" for key \"" << RESPONSE_PAYLOAD_KEY_TOKENUUID << "\"");
		}
	}
	// LCOV_EXCL_START
	// Defensive coding: the data validity is guaranteed by the object
	catch (nlohmann::json::exception& e) {
		ACS_THROW(QuotaServiceResponsePayloadException(e, "Error creating JSON Object."));
	}
	// LCOV_EXCL_STOP
	return js;
}

_ACS_END_NAMESPACE
