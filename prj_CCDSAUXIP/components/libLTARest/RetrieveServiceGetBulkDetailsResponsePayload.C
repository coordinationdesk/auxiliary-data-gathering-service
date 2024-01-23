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

#include <RetrieveServiceGetBulkDetailsResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

/***********
 * ELEMENT *
 **********/

ACS_CLASS_DEFINE_DEBUG_LEVEL(RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement);

const std::string RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement::
  RESPONSE_PAYLOAD_KEY_BATCHID = "batch_id";
const std::string RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement::
  RESPONSE_PAYLOAD_KEY_STATUS = "status";

RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement::BatchesDetailsElement() :
  _mutex(),
  _batchID(0),
  _status()
{
}

RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement::BatchesDetailsElement(
  const std::string& jsstr) :
  _mutex(),
  _batchID(0),
  _status()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement::BatchesDetailsElement(
  const nlohmann::json& js) :
  _mutex(),
  _batchID(0),
  _status()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement::BatchesDetailsElement(
  const BatchesDetailsElement& data) :
  _mutex(),
  _batchID(data._batchID),
  _status(data._status)
{
}

RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement::~BatchesDetailsElement()
  ACS_NOEXCEPT
{
}

RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement&
RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement::operator=(
  const BatchesDetailsElement& data)
{
  if (this != &data)
  {
    bool toSet = true;
    while (toSet)
    {
      Lock l1(_mutex);
      TryLock l2(data._mutex);
      if (l2.locked())
      {
        _batchID = data._batchID;
        _status = data._status;
        toSet = false;
      }
      else
      {
        usleep(100);
      }
    }
  }
  return *this;
}

void RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement::fromJSON(
  const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchesDetailsElementException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement::fromJSONObject(
  const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  try
  {
    if (json.find(RESPONSE_PAYLOAD_KEY_BATCHID) == json.end() || json[RESPONSE_PAYLOAD_KEY_BATCHID].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_BATCHID
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(BatchesDetailsElementException(os.str()));
    }
    _batchID = json[RESPONSE_PAYLOAD_KEY_BATCHID];

    if (json.find(RESPONSE_PAYLOAD_KEY_STATUS) != json.end() && !json[RESPONSE_PAYLOAD_KEY_STATUS].is_null())
    {
      _status = json[RESPONSE_PAYLOAD_KEY_STATUS];
    }
    else
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(BatchesDetailsElementException(os.str()));
    }
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchesDetailsElementException(e, "Error reading JSON Object."));
  }
}

std::string RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement::toJSON() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string str;
  nlohmann::json js;
  try
  {
    js = toJSONObject();
    str = js.dump();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Response data transformed data into the following json string: \"\n"
                                          << str << "\"");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchesDetailsElementException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement::toJSONObject()
  const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    js[RESPONSE_PAYLOAD_KEY_BATCHID] = _batchID;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _batchID << "\" for \""
                                                           << RESPONSE_PAYLOAD_KEY_BATCHID << "\" key");

    js[RESPONSE_PAYLOAD_KEY_STATUS] = _status;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _status << "\" for \""
                                                           << RESPONSE_PAYLOAD_KEY_STATUS << "\" key");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchesDetailsElementException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

/***********
 *  LIST   *
 **********/

namespace
{
bool compareFunction(const RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement& a,
                     const RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement& b)
{
  return (a.getBatchID() < b.getBatchID());
}
}  // namespace

ACS_CLASS_DEFINE_DEBUG_LEVEL(RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList);

RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsList() :
  _mutex(),
  _batchesDetails()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsList(const std::string& jsstr) :
  _mutex(),
  _batchesDetails()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsList(const nlohmann::json& js) :
  _mutex(),
  _batchesDetails()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsList(const BatchesDetailsList& list) :
  _mutex(),
  _batchesDetails(list._batchesDetails)
{
}

RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::~BatchesDetailsList() ACS_NOEXCEPT {}

const RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement&
RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::operator[](size_t idx) const
{
  Lock l(_mutex);
  if (idx >= _batchesDetails.size())
  {
    std::ostringstream ss;
    ss << "Invalid index " << idx << ". Size is " << _batchesDetails.size();
    ACS_THROW(exBoundsException(ss.str()));
  }

  return _batchesDetails[idx];
}

RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::BatchesDetailsElement&
RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::operator[](size_t idx)
{
  const BatchesDetailsList& me = (*this);
  const BatchesDetailsElement& e = me[idx];      // calls const version
  return const_cast<BatchesDetailsElement&>(e);  // cast away constness
}

void RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::add(const BatchesDetailsElement& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  _batchesDetails.push_back(payload);
}

void RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::add(const BatchesDetailsList& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);
  add(payload.getBatchesDetails());
}

void RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::add(
  const std::vector<BatchesDetailsElement>& payload)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);
  _batchesDetails.insert(_batchesDetails.end(), payload.begin(), payload.end());
}

void RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::sort()
{
  Lock l(_mutex);
  std::sort(_batchesDetails.begin(), _batchesDetails.end(), compareFunction);
}

void RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::sort(std::vector<BatchesDetailsElement>& out)
  const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  out = _batchesDetails;
  std::sort(out.begin(), out.end(), compareFunction);
}

void RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::clear()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  _batchesDetails.clear();
}

size_t RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::size() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  return _batchesDetails.size();
}

void RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::fromJSON(const std::string& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(json);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchesDetailsListException(e, "Error reading JSON."));
  }

  fromJSONObject(js);
}

void RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  try
  {
    _batchesDetails.clear();

    if (json.is_null() || !json.is_array())
    {
      ACS_THROW(BatchesDetailsListException("Json response payload is not an array or the associated value is null"));
    }

    size_t listSize = json.size();
    for (size_t idx = 0; idx < listSize; ++idx)
    {
      BatchesDetailsElement element(json[idx]);
      add(element);
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the is_null and is_array checks, other check are in
  // BatchesDetailsElement fromJSONObject functions
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchesDetailsListException(e, "Error reading JSON Object."));
  }
  // LCOV_EXCL_STOP
}

std::string RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::toJSON() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string jsdumped;
  nlohmann::json js;
  try
  {
    js = toJSONObject();
    jsdumped = js.dump();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Generated the following json: " << jsdumped)
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchesDetailsListException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return jsdumped;
}

nlohmann::json RetrieveServiceGetBulkDetailsResponsePayload::BatchesDetailsList::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);
  nlohmann::json js;
  try
  {
    js = nlohmann::json::array();  // create the empty array

    size_t rqSize = _batchesDetails.size();
    for (size_t idx = 0; idx < rqSize; ++idx)
    {
      js.push_back(_batchesDetails[idx].toJSONObject());
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(BatchesDetailsListException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

/***********
 * PAYLOAD *
 **********/

ACS_CLASS_DEFINE_DEBUG_LEVEL(RetrieveServiceGetBulkDetailsResponsePayload);

const std::string RetrieveServiceGetBulkDetailsResponsePayload::RESPONSE_PAYLOAD_KEY_STATUS = "status";
const std::string RetrieveServiceGetBulkDetailsResponsePayload::RESPONSE_PAYLOAD_KEY_BATCHESDETAILS = "batches_details";
const std::string RetrieveServiceGetBulkDetailsResponsePayload::RESPONSE_PAYLOAD_KEY_ERRORMSG = "errormsg";
const std::string RetrieveServiceGetBulkDetailsResponsePayload::RESPONSE_PAYLOAD_KEY_ERRORCODE = "error_code";

RetrieveServiceGetBulkDetailsResponsePayload::RetrieveServiceGetBulkDetailsResponsePayload() :
  _mutex(),
  _status(),
  _batchesDetails(),
  _errormsg(),
  _errorCode(RetrieveServiceResponseErrorCode::RETRIEVESERVICE_INTERNAL_ERROR),
  _errorCodeSet(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RetrieveServiceGetBulkDetailsResponsePayload::RetrieveServiceGetBulkDetailsResponsePayload(const std::string& jsstr) :
  _mutex(),
  _status(),
  _batchesDetails(),
  _errormsg(),
  _errorCode(RetrieveServiceResponseErrorCode::RETRIEVESERVICE_INTERNAL_ERROR),
  _errorCodeSet(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

RetrieveServiceGetBulkDetailsResponsePayload::RetrieveServiceGetBulkDetailsResponsePayload(const nlohmann::json& js) :
  _mutex(),
  _status(),
  _batchesDetails(),
  _errormsg(),
  _errorCode(RetrieveServiceResponseErrorCode::RETRIEVESERVICE_INTERNAL_ERROR),
  _errorCodeSet(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

RetrieveServiceGetBulkDetailsResponsePayload::RetrieveServiceGetBulkDetailsResponsePayload(
  const RetrieveServiceGetBulkDetailsResponsePayload& data) :
  _mutex(),
  _status(data._status),
  _batchesDetails(data._batchesDetails),
  _errormsg(data._errormsg),
  _errorCode(data._errorCode),
  _errorCodeSet(data._errorCodeSet)
{
}

RetrieveServiceGetBulkDetailsResponsePayload::~RetrieveServiceGetBulkDetailsResponsePayload() ACS_NOEXCEPT
{
}

RetrieveServiceGetBulkDetailsResponsePayload& RetrieveServiceGetBulkDetailsResponsePayload::operator=(
  const RetrieveServiceGetBulkDetailsResponsePayload& data)
{
  if (this != &data)
  {
    bool toSet = true;
    while (toSet)
    {
      Lock l1(_mutex);
      TryLock l2(data._mutex);
      if (l2.locked())
      {
        _status = data._status;
        _batchesDetails = data._batchesDetails;
        _errormsg = data._errormsg;
        _errorCode = data._errorCode;
        _errorCodeSet = data._errorCodeSet;
        toSet = false;
      }
      else
      {
        usleep(100);
      }
    }
  }
  return *this;
}

void RetrieveServiceGetBulkDetailsResponsePayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBulkDetailsResponsePayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void RetrieveServiceGetBulkDetailsResponsePayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);

  try
  {
    if (json.find(RESPONSE_PAYLOAD_KEY_STATUS) == json.end() || json[RESPONSE_PAYLOAD_KEY_STATUS].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(RetrieveServiceGetBulkDetailsResponsePayloadException(os.str()));
    }
    _status = json[RESPONSE_PAYLOAD_KEY_STATUS];

    if (json.find(RESPONSE_PAYLOAD_KEY_BATCHESDETAILS) == json.end() ||
        json[RESPONSE_PAYLOAD_KEY_BATCHESDETAILS].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_BATCHESDETAILS
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(RetrieveServiceGetBulkDetailsResponsePayloadException(os.str()));
    }
    _batchesDetails.fromJSONObject(json[RESPONSE_PAYLOAD_KEY_BATCHESDETAILS]);

    if (json.find(RESPONSE_PAYLOAD_KEY_ERRORMSG) != json.end() && !json[RESPONSE_PAYLOAD_KEY_ERRORMSG].is_null())
    {
      _errormsg = json[RESPONSE_PAYLOAD_KEY_ERRORMSG];
    }
    else
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_ERRORMSG
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(RetrieveServiceGetBulkDetailsResponsePayloadException(os.str()));
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_ERRORCODE) != json.end())
    {
      if (!json[RESPONSE_PAYLOAD_KEY_ERRORCODE].is_null())
      {
        _errorCode = json[RESPONSE_PAYLOAD_KEY_ERRORCODE];
        _errorCodeSet = true;
      }
      else
      {
        _errorCodeSet = false;
        std::ostringstream os;
        os << "Null value for key \"" << RESPONSE_PAYLOAD_KEY_ERRORCODE << "\"";
        ACS_THROW(RetrieveServiceGetBulkDetailsResponsePayloadException(os.str()));
      }
    }
    else
    {
      _errorCodeSet = false;
    }
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBulkDetailsResponsePayloadException(e, "Error reading JSON Object."));
  }
}

std::string RetrieveServiceGetBulkDetailsResponsePayload::toJSON() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  std::string str;
  nlohmann::json js;
  try
  {
    js = toJSONObject();
    str = js.dump();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Response data transformed data into the following json string: \"\n"
                                          << str << "\"");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBulkDetailsResponsePayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP

  return str;
}

nlohmann::json RetrieveServiceGetBulkDetailsResponsePayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    // Add members to json object
    js[RESPONSE_PAYLOAD_KEY_STATUS] = _status;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _status << "\" for \""
                                                           << RESPONSE_PAYLOAD_KEY_STATUS << "\" key");

    js[RESPONSE_PAYLOAD_KEY_BATCHESDETAILS] = _batchesDetails.toJSONObject();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting Batches Details JSON Object for \""
                                          << RESPONSE_PAYLOAD_KEY_BATCHESDETAILS << "\" key");

    js[RESPONSE_PAYLOAD_KEY_ERRORMSG] = _errormsg;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _errormsg << "\" for \"" << RESPONSE_PAYLOAD_KEY_ERRORMSG
                                                           << "\" key");

    if (_errorCodeSet)
    {
      js[RESPONSE_PAYLOAD_KEY_ERRORCODE] = _errorCode;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << static_cast<int>(_errorCode) << "\" for \""
                                                             << RESPONSE_PAYLOAD_KEY_ERRORCODE << "\" key");
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBulkDetailsResponsePayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
