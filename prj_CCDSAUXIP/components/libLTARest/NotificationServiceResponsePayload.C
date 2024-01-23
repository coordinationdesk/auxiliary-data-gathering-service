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

#include <json.hpp>
#include <NotificationServiceResponsePayload.h>
#include <FtpCurl.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace curl;

ACS_CLASS_DEFINE_DEBUG_LEVEL(NotificationServiceResponsePayload);
ACS_CLASS_DEFINE_DEBUG_LEVEL(NotificationServiceResponsePayloadList);

namespace {
	bool ResponsePayloadsPredicate (NotificationServiceResponsePayload i,NotificationServiceResponsePayload j) { return (i.getRequestId() < j.getRequestId()); }
}

const std::string NotificationServiceResponsePayload::RESPONSE_PAYLOAD_KEY_REQUEST_ID = "RequestId";
const std::string NotificationServiceResponsePayload::RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE = "ErrorMessage";

/*
 * NotificationServiceResponsePayload definition
 */

NotificationServiceResponsePayload::NotificationServiceResponsePayload(size_t id) :
	_mutex(),
	_requestId(id),
	_errmsgSet(false),
	_errmsg()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

NotificationServiceResponsePayload::NotificationServiceResponsePayload(const std::string& jsstr) :
	_mutex(),
	_requestId(0),
	_errmsgSet(false),
	_errmsg()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}

NotificationServiceResponsePayload::NotificationServiceResponsePayload(const nlohmann::json& js) :
	_mutex(),
	_requestId(0),
	_errmsgSet(false),
	_errmsg()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}

NotificationServiceResponsePayload::NotificationServiceResponsePayload(const NotificationServiceResponsePayload& data) :
	_mutex(),
	_requestId(data._requestId),
	_errmsgSet(data._errmsgSet),
	_errmsg(data._errmsg)
{ }

NotificationServiceResponsePayload::~NotificationServiceResponsePayload() ACS_NOEXCEPT { }

NotificationServiceResponsePayload& NotificationServiceResponsePayload::operator=(const NotificationServiceResponsePayload& data) {
	if (this != &data) {
		bool toSet = true;
		while (toSet) {
			Lock l1(_mutex);
			TryLock l2(data._mutex);
			if (l2.locked()) {
				_requestId = data._requestId;
				_errmsgSet = data._errmsgSet;
				_errmsg = data._errmsg;
				toSet = false;
			}
			else {
				usleep(100);
			}
		}
	}
	return *this;
}


nlohmann::json NotificationServiceResponsePayload::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	nlohmann::json js;
	try {
		js[RESPONSE_PAYLOAD_KEY_REQUEST_ID] = getRequestId();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << getRequestId() << "\" for key \"" << RESPONSE_PAYLOAD_KEY_REQUEST_ID << "\"");

		bool isset = false;
		std::string strval = getErrorMessage(&isset);
		if (isset) {
			js[RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE] = strval;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << strval << "\" for key \"" << RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE << "\" not set. It will not be written");
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceResponsePayloadException(e, "Error creating JSON Object."));
	}
	return js;
}

std::string NotificationServiceResponsePayload::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string str;
	nlohmann::json js;
	try {
		js = toJSONObject();
		str = js.dump();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transformed response data into the following json string:\n" << str);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceResponsePayloadException(e, "Error creating JSON."));
	}
	return str;
}

void NotificationServiceResponsePayload::fromJSONObject(const nlohmann::json& json) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	try {
		if (json.find(RESPONSE_PAYLOAD_KEY_REQUEST_ID) == json.end() || json[RESPONSE_PAYLOAD_KEY_REQUEST_ID].is_null()) {
			ACS_THROW(NotificationServiceResponsePayloadException("Cannot find key \"" + RESPONSE_PAYLOAD_KEY_REQUEST_ID + "\" into json response payload or the associated value is null. Key is mandatory"));
		}
		
		setRequestId(json[RESPONSE_PAYLOAD_KEY_REQUEST_ID]);
		if (json.find(RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE) != json.end() && !json[RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE].is_null()) {
			std::string val = json[RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE];
			if (!val.empty()) {
				setErrorMessage(val);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << val << "\" associated to key \"" << RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE << "\".");
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE << "\". It will not be loaded.");
			}
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE << "\" not found or the associated value was null. It will not be loaded.");
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceResponsePayloadException(e, "Error reading JSON Object."));
	}
}

void NotificationServiceResponsePayload::fromJSON(const std::string& jsstr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	nlohmann::json js;
	try {
		js = nlohmann::json::parse(jsstr);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceResponsePayloadException(e, "Error reading JSON."));
	}
	fromJSONObject(js);
}

/*
 * NotificationServiceRequestPayloadList definition
 */

NotificationServiceResponsePayloadList::NotificationServiceResponsePayloadList() : 
	_mutex(),
	_responses()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

NotificationServiceResponsePayloadList::NotificationServiceResponsePayloadList(const std::string& jsstr) : 
	_mutex(),
	_responses()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	fromJSON(jsstr);
	sort();
}

NotificationServiceResponsePayloadList::NotificationServiceResponsePayloadList(const NotificationServiceResponsePayloadList& list) :
	_mutex(),
	_responses(list._responses)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

NotificationServiceResponsePayloadList::~NotificationServiceResponsePayloadList() ACS_NOEXCEPT { }

NotificationServiceResponsePayload& NotificationServiceResponsePayloadList::operator[](size_t idx) {
	Lock l(_mutex);
	if (idx >= _responses.size()) {
		std::ostringstream ss;
		ss << "Invalid index " << idx << ". Size is " << _responses.size();
		ACS_THROW(exBoundsException(ss.str()));
	}
	
	return _responses[idx];
}

const NotificationServiceResponsePayload& NotificationServiceResponsePayloadList::operator[](size_t idx) const {
	Lock l(_mutex);
	if (idx >= _responses.size()) {
		std::ostringstream ss;
		ss << "Invalid index " << idx << ". Size is " << _responses.size();
		ACS_THROW(exBoundsException(ss.str()));
	}
	
	return _responses[idx];
}

void NotificationServiceResponsePayloadList::add(const NotificationServiceResponsePayload& payload) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	_responses.push_back(payload);
}

void NotificationServiceResponsePayloadList::add(const NotificationServiceResponsePayloadList& payload) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	const std::vector<NotificationServiceResponsePayload>& p = payload.getResponses();
	_responses.insert(_responses.end(), p.begin(), p.end());
}

const std::vector<NotificationServiceResponsePayload>& NotificationServiceResponsePayloadList::getResponses() const {
	Lock l(_mutex);
	return _responses;
}

void NotificationServiceResponsePayloadList::sort() {
	Lock l(_mutex);
	std::sort(_responses.begin(), _responses.end(), ResponsePayloadsPredicate);
}

void NotificationServiceResponsePayloadList::sort(std::vector<NotificationServiceResponsePayload>& out) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	out = _responses;
	std::sort(out.begin(), out.end(), ResponsePayloadsPredicate);
}

void NotificationServiceResponsePayloadList::clear() {
	Lock l(_mutex);
	_responses.clear();
}


size_t NotificationServiceResponsePayloadList::size() const {
	return _responses.size();
}

nlohmann::json NotificationServiceResponsePayloadList::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	nlohmann::json js;
	try {
		js["notification_responses"] = nlohmann::json::array(); // create the empty array
		
		std::vector<NotificationServiceResponsePayload> res = _responses;
		sort(res);

		size_t resSize = res.size();
		for (size_t idx = 0; idx < resSize; ++idx) {
			const NotificationServiceResponsePayload& payload = res[idx];
			nlohmann::json resjs = payload.toJSONObject();

			js["notification_responses"].push_back(resjs);
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceResponsePayloadListException(e, "Error creating JSON Object."));
	}
	return js;
}

std::string NotificationServiceResponsePayloadList::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	std::string jsdumped;
	nlohmann::json js;
	try {
		js = toJSONObject();
		jsdumped = js.dump();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generated the following json: " << jsdumped);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceResponsePayloadListException(e, "Error creating JSON."));
	}
	return jsdumped;
}

void NotificationServiceResponsePayloadList::fromJSON(const std::string& json) {
	nlohmann::json js;
	try {
		js = nlohmann::json::parse(json);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceResponsePayloadListException(e, "Error reading JSON."));
	}
	fromJSONObject(js);
}

void NotificationServiceResponsePayloadList::fromJSONObject(const nlohmann::json& js) {
	Lock l(_mutex);
	try {
		if (js.find("notification_responses") == js.end()) {
			ACS_THROW(NotificationServiceResponsePayloadListException("Cannot find key \"notification_responses\" into json response payload. RequestId key is mandatory"));
		}
		
		size_t listSize = js["notification_responses"].size();
		for (size_t idx = 0; idx < listSize; ++idx) {
			nlohmann::json jsobj = js["notification_responses"][idx];
			NotificationServiceResponsePayload res(jsobj);
			add(res);
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(NotificationServiceResponsePayloadListException(e, "Error reading JSON Object."));
	}
}

_ACS_END_NAMESPACE
