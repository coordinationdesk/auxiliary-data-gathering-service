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
#include <EstimationTimeServiceResponsePayload.h>
#include <FtpCurl.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace curl;

ACS_CLASS_DEFINE_DEBUG_LEVEL(EstimationTimeServiceResponsePayload);
ACS_CLASS_DEFINE_DEBUG_LEVEL(EstimationTimeServiceResponsePayloadList);

const std::string EstimationTimeServiceResponsePayload::RESPONSE_PAYLOAD_KEY_JOBRESPONSIBLE = "jobresponsible";
const std::string EstimationTimeServiceResponsePayload::RESPONSE_PAYLOAD_KEY_ESTIMATIONS = "estimations";
const std::string EstimationTimeServiceResponsePayload::EstimationResponseEntry::RESPONSE_PAYLOAD_KEY_INVENTORY_ID = "inventory_id";
const std::string EstimationTimeServiceResponsePayload::EstimationResponseEntry::RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE = "error_message";
const std::string EstimationTimeServiceResponsePayload::EstimationResponseEntry::RESPONSE_PAYLOAD_KEY_ESTIMATED_DURATION = "estimated_duration";


/*===================================================================
 * 
 *                 Estimation class definition
 *
 ====================================================================*/
	
EstimationTimeServiceResponsePayload::EstimationResponseEntry::EstimationResponseEntry() :
	inventoryId_(0),
	estimDuration_(0.0),
	errmsg_(),
	estimDurationSet_(false),
	errmsgSet_(false)
{ }

EstimationTimeServiceResponsePayload::EstimationResponseEntry::EstimationResponseEntry(const std::string& jsstr) :
	inventoryId_(0),
	estimDuration_(0.0),
	errmsg_(),
	estimDurationSet_(false),
	errmsgSet_(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}

EstimationTimeServiceResponsePayload::EstimationResponseEntry::EstimationResponseEntry(const nlohmann::json& js) :
	inventoryId_(0),
	estimDuration_(0.0),
	errmsg_(),
	estimDurationSet_(false),
	errmsgSet_(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}


EstimationTimeServiceResponsePayload::EstimationResponseEntry::EstimationResponseEntry(const EstimationTimeServiceResponsePayload::EstimationResponseEntry& e) :
	inventoryId_(e.inventoryId_),
	estimDuration_(e.estimDuration_),
	errmsg_(e.errmsg_),
	estimDurationSet_(e.estimDurationSet_),
	errmsgSet_(e.errmsgSet_)
{ }

EstimationTimeServiceResponsePayload::EstimationResponseEntry& EstimationTimeServiceResponsePayload::EstimationResponseEntry::operator=(const EstimationTimeServiceResponsePayload::EstimationResponseEntry& e) {
	if (this != &e) {
		inventoryId_ = e.inventoryId_;
		estimDuration_ = e.estimDuration_;
		errmsg_ = e.errmsg_;
		estimDurationSet_ = e.estimDurationSet_;
		errmsgSet_ = e.errmsgSet_;
	}
	return *this;
}

void EstimationTimeServiceResponsePayload::EstimationResponseEntry::fromJSON(const std::string& jsstr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	nlohmann::json js;
	try {
		js = nlohmann::json::parse(jsstr);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceResponsePayloadException(e, "Error reading JSON."));
	}
	fromJSONObject(js);
}

void EstimationTimeServiceResponsePayload::EstimationResponseEntry::fromJSONObject(const nlohmann::json& json) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	try {
		if (json.find(RESPONSE_PAYLOAD_KEY_INVENTORY_ID) == json.end() || json[RESPONSE_PAYLOAD_KEY_INVENTORY_ID].is_null()) {
			ACS_THROW(EstimationTimeServiceResponsePayloadException("Cannot find key \"" + RESPONSE_PAYLOAD_KEY_INVENTORY_ID + "\" into json response payload or the associated value is null. Key is mandatory"));
		}
		inventoryId_ = json[RESPONSE_PAYLOAD_KEY_INVENTORY_ID];
		
		if (json.find(RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE) != json.end() && !json[RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE].is_null()) {
			std::string val = json[RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE];
			if (!val.empty()) {
				errmsg_ = val;
				errmsgSet_ = true;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << errmsg_ << "\" associated to key \"" << RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE << "\".");
			} else {
				errmsgSet_ = false;
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Empty value associated to key \"" << RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE << "\". It will not be loaded.");
			}
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE << "\" not found or the associated value was null. It will not be loaded.");
		}
		
		if (json.find(RESPONSE_PAYLOAD_KEY_ESTIMATED_DURATION) != json.end() && !json[RESPONSE_PAYLOAD_KEY_ESTIMATED_DURATION].is_null()) {
			estimDuration_ = json[RESPONSE_PAYLOAD_KEY_ESTIMATED_DURATION];
			estimDurationSet_ = true;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << estimDuration_ << "\" associated to key \"" << RESPONSE_PAYLOAD_KEY_ESTIMATED_DURATION << "\".");
		} else {
			estimDurationSet_ = false;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key \"" << RESPONSE_PAYLOAD_KEY_ESTIMATED_DURATION << "\" not found or the associated value was null. It will not be loaded.");
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceResponsePayloadException(e, "Error reading JSON Object."));
	}
	
		
	ACS_COND_THROW (!errmsgSet_ && !estimDurationSet_, EstimationTimeServiceResponsePayloadException("At least one between \"" + 
						RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE + "\" and \"" + RESPONSE_PAYLOAD_KEY_ESTIMATED_DURATION + "\" must be specified. None found"));
}

std::string EstimationTimeServiceResponsePayload::EstimationResponseEntry::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string str;
	nlohmann::json js;
	try {
		js = toJSONObject();
		str = js.dump();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transformed response data into the following json string:\n" << str);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceResponsePayloadException(e, "Error creating JSON."));
	}
	return str;
}

nlohmann::json EstimationTimeServiceResponsePayload::EstimationResponseEntry::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	nlohmann::json js;
	try {
		js[RESPONSE_PAYLOAD_KEY_INVENTORY_ID] = inventoryId_;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << inventoryId_ << "\" for key \"" << RESPONSE_PAYLOAD_KEY_INVENTORY_ID << "\"");

		if (errmsgSet_) {
			js[RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE] = errmsg_;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << errmsg_ << "\" for key \"" << RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE << "\" not set. It will not be written");
		}

		if (estimDurationSet_) {
			js[RESPONSE_PAYLOAD_KEY_ESTIMATED_DURATION] = static_cast<unsigned long long>(estimDuration_);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << static_cast<unsigned long long>(estimDuration_) << "\" for key \"" << RESPONSE_PAYLOAD_KEY_ESTIMATED_DURATION << "\"");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value for key \"" << RESPONSE_PAYLOAD_KEY_ESTIMATED_DURATION << "\" not set. It will not be written");
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceResponsePayloadException(e, "Error creating JSON Object."));
	}
	return js;
}

/*===================================================================
 * 
 *   EstimationTimeServiceResponsePayload class definition
 *
 ====================================================================*/

EstimationTimeServiceResponsePayload::EstimationTimeServiceResponsePayload() :
	mutex_(),
	jobResponsible_(),
	estimations_()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

EstimationTimeServiceResponsePayload::EstimationTimeServiceResponsePayload(const std::string& jsstr) :
	mutex_(),
	jobResponsible_(),
	estimations_()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSON(jsstr);
}

EstimationTimeServiceResponsePayload::EstimationTimeServiceResponsePayload(const nlohmann::json& js) :
	mutex_(),
	jobResponsible_(),
	estimations_()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	fromJSONObject(js);
}

EstimationTimeServiceResponsePayload::EstimationTimeServiceResponsePayload(const EstimationTimeServiceResponsePayload& data) :
	mutex_(),
	jobResponsible_(data.jobResponsible_),
	estimations_(data.estimations_)
{ }

EstimationTimeServiceResponsePayload::~EstimationTimeServiceResponsePayload() ACS_NOEXCEPT { }

EstimationTimeServiceResponsePayload& EstimationTimeServiceResponsePayload::operator=(const EstimationTimeServiceResponsePayload& data) {
	if (this != &data) {
		bool toSet = true;
		while (toSet) {
			Lock l1(mutex_);
			TryLock l2(data.mutex_);
			if (l2.locked()) {
				jobResponsible_ = data.jobResponsible_;
				estimations_ = data.estimations_;
				toSet = false;
			}
			else {
				usleep(100);
			}
		}
	}
	return *this;
}


nlohmann::json EstimationTimeServiceResponsePayload::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	Lock l(mutex_);
	
	nlohmann::json js;
	try {
		js[RESPONSE_PAYLOAD_KEY_JOBRESPONSIBLE] = jobResponsible_;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << jobResponsible_ << "\" for key \"" << RESPONSE_PAYLOAD_KEY_JOBRESPONSIBLE << "\"");
		
		nlohmann::json jsarr = nlohmann::json::array();
		
		// Loop over each estimation
		size_t estimSize = estimations_.size();
		for (size_t idx = 0; idx < estimSize; ++idx) {
			jsarr.push_back(estimations_[idx].toJSONObject());
		}
		js[RESPONSE_PAYLOAD_KEY_ESTIMATIONS] = jsarr;
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceResponsePayloadException(e, "Error creating JSON Object."));
	}
	return js;
}

std::string EstimationTimeServiceResponsePayload::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	std::string str;
	nlohmann::json js;
	try {
		js = toJSONObject();
		str = js.dump();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transformed response data into the following json string:\n" << str);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceResponsePayloadException(e, "Error creating JSON."));
	}
	return str;
}

void EstimationTimeServiceResponsePayload::fromJSONObject(const nlohmann::json& json) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	Lock l(mutex_);

	try {
		if (json.find(RESPONSE_PAYLOAD_KEY_JOBRESPONSIBLE) == json.end()) {
			ACS_THROW(EstimationTimeServiceResponsePayloadException("Cannot find key \"" + RESPONSE_PAYLOAD_KEY_JOBRESPONSIBLE + "\" into json response payload or the associated value is null. Key is mandatory"));
		}
		
		jobResponsible_ = "";
		if (!json[RESPONSE_PAYLOAD_KEY_JOBRESPONSIBLE].is_null()) {
			jobResponsible_ = json[RESPONSE_PAYLOAD_KEY_JOBRESPONSIBLE];
		}
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded value \"" << jobResponsible_ << "\" associated to key \"" << RESPONSE_PAYLOAD_KEY_JOBRESPONSIBLE << "\".");

		nlohmann::json estim = json[RESPONSE_PAYLOAD_KEY_ESTIMATIONS];
		size_t estimSize = estim.size();
		for (size_t idx = 0; idx < estimSize; ++idx) {
			nlohmann::json obj = estim[idx];
			EstimationResponseEntry e(obj);
			estimations_.push_back(e);
		}

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Loaded " << estimations_.size() << " values associated to key \"" << RESPONSE_PAYLOAD_KEY_ESTIMATIONS << "\".");

	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceResponsePayloadException(e, "Error reading JSON Object."));
	}
}

void EstimationTimeServiceResponsePayload::fromJSON(const std::string& jsstr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	nlohmann::json js;
	try {
		js = nlohmann::json::parse(jsstr);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceResponsePayloadException(e, "Error reading JSON."));
	}
	fromJSONObject(js);
}


void EstimationTimeServiceResponsePayload::setJobResponsible(const std::string& jr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	jobResponsible_ = jr;
}

void EstimationTimeServiceResponsePayload::setEstimationResponseEntries(const std::vector<EstimationResponseEntry>& est) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	estimations_ = est;
}

void EstimationTimeServiceResponsePayload::addEstimationResponseEntry(const EstimationResponseEntry& e) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	estimations_.push_back(e);

}

const std::string& EstimationTimeServiceResponsePayload::getJobResponsible() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	return jobResponsible_;
}

void EstimationTimeServiceResponsePayload::getEstimationResponseEntries(std::vector<EstimationResponseEntry>& e) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex_);
	e = estimations_;
}

/*===================================================================
 * 
 *   EstimationTimeServiceResponsePayloadList class definition
 *
 ====================================================================*/

EstimationTimeServiceResponsePayloadList::EstimationTimeServiceResponsePayloadList() : 
	_mutex(),
	_responses()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

EstimationTimeServiceResponsePayloadList::EstimationTimeServiceResponsePayloadList(const std::string& jsstr) : 
	_mutex(),
	_responses()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	fromJSON(jsstr);
}

EstimationTimeServiceResponsePayloadList::EstimationTimeServiceResponsePayloadList(const EstimationTimeServiceResponsePayloadList& list) :
	_mutex(),
	_responses(list._responses)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

EstimationTimeServiceResponsePayloadList::~EstimationTimeServiceResponsePayloadList() ACS_NOEXCEPT { }

EstimationTimeServiceResponsePayload& EstimationTimeServiceResponsePayloadList::operator[](size_t idx) {
	Lock l(_mutex);
	if (idx >= _responses.size()) {
		std::ostringstream ss;
		ss << "Invalid index " << idx << ". Size is " << _responses.size();
		ACS_THROW(exBoundsException(ss.str()));
	}
	
	return _responses[idx];
}

const EstimationTimeServiceResponsePayload& EstimationTimeServiceResponsePayloadList::operator[](size_t idx) const {
	Lock l(_mutex);
	if (idx >= _responses.size()) {
		std::ostringstream ss;
		ss << "Invalid index " << idx << ". Size is " << _responses.size();
		ACS_THROW(exBoundsException(ss.str()));
	}
	
	return _responses[idx];
}

void EstimationTimeServiceResponsePayloadList::add(const EstimationTimeServiceResponsePayload& payload) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	_responses.push_back(payload);
}

void EstimationTimeServiceResponsePayloadList::add(const EstimationTimeServiceResponsePayloadList& payload) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	const std::vector<EstimationTimeServiceResponsePayload>& p = payload.getResponses();
	_responses.insert(_responses.end(), p.begin(), p.end());
}

const std::vector<EstimationTimeServiceResponsePayload>& EstimationTimeServiceResponsePayloadList::getResponses() const {
	Lock l(_mutex);
	return _responses;
}

void EstimationTimeServiceResponsePayloadList::clear() {
	Lock l(_mutex);
	_responses.clear();
}


size_t EstimationTimeServiceResponsePayloadList::size() const {
	return _responses.size();
}

nlohmann::json EstimationTimeServiceResponsePayloadList::toJSONObject() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(_mutex);
	nlohmann::json js;
	try {
		js["estimation_time_response"] = nlohmann::json::array(); // create the empty array
		
		size_t resSize = _responses.size();
		for (size_t idx = 0; idx < resSize; ++idx) {
			const EstimationTimeServiceResponsePayload& payload = _responses[idx];
			nlohmann::json resjs = payload.toJSONObject();

			js["estimation_time_response"].push_back(resjs);
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceResponsePayloadListException(e, "Error creating JSON Object."));
	}
	return js;
}

std::string EstimationTimeServiceResponsePayloadList::toJSON() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	std::string jsdumped;
	nlohmann::json js;
	try {
		js = toJSONObject();
		jsdumped = js.dump();
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generated the following json: " << jsdumped);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceResponsePayloadListException(e, "Error creating JSON."));
	}
	return jsdumped;
}

void EstimationTimeServiceResponsePayloadList::fromJSON(const std::string& json) {
	nlohmann::json js;
	try {
		js = nlohmann::json::parse(json);
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceResponsePayloadListException(e, "Error reading JSON."));
	}
	fromJSONObject(js);
}

void EstimationTimeServiceResponsePayloadList::fromJSONObject(const nlohmann::json& js) {
	Lock l(_mutex);
	try {
		if (js.find("estimation_time_response") == js.end()) {
			ACS_THROW(EstimationTimeServiceResponsePayloadListException("Cannot find key \"estimation_time_response\" into json response payload."));
		}
		
		size_t listSize = js["estimation_time_response"].size();
		for (size_t idx = 0; idx < listSize; ++idx) {
			nlohmann::json jsobj = js["estimation_time_response"][idx];
			EstimationTimeServiceResponsePayload res(jsobj);
			add(res);
		}
	} catch (nlohmann::json::exception& e) {
		ACS_THROW(EstimationTimeServiceResponsePayloadListException(e, "Error reading JSON Object."));
	}
}

_ACS_END_NAMESPACE
