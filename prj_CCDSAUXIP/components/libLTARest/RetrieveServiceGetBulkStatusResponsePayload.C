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

#include <RetrieveServiceGetBulkStatusResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RetrieveServiceGetBulkStatusResponsePayload);

const std::string RetrieveServiceGetBulkStatusResponsePayload::RESPONSE_PAYLOAD_KEY_STATUS = "status";
const std::string RetrieveServiceGetBulkStatusResponsePayload::RESPONSE_PAYLOAD_KEY_ERRORMSG = "errormsg";
const std::string RetrieveServiceGetBulkStatusResponsePayload::RESPONSE_PAYLOAD_KEY_ERRORCODE = "error_code";

RetrieveServiceGetBulkStatusResponsePayload::RetrieveServiceGetBulkStatusResponsePayload() :
  _mutex(),
  _status(),
  _errormsg(),
  _errorCode(RetrieveServiceResponseErrorCode::RETRIEVESERVICE_INTERNAL_ERROR),
  _errorCodeSet(false)
{
}

RetrieveServiceGetBulkStatusResponsePayload::RetrieveServiceGetBulkStatusResponsePayload(const std::string& jsstr) :
  _mutex(),
  _status(),
  _errormsg(),
  _errorCode(RetrieveServiceResponseErrorCode::RETRIEVESERVICE_INTERNAL_ERROR),
  _errorCodeSet(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

RetrieveServiceGetBulkStatusResponsePayload::RetrieveServiceGetBulkStatusResponsePayload(const nlohmann::json& js) :
  _mutex(),
  _status(),
  _errormsg(),
  _errorCode(RetrieveServiceResponseErrorCode::RETRIEVESERVICE_INTERNAL_ERROR),
  _errorCodeSet(false)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

RetrieveServiceGetBulkStatusResponsePayload::RetrieveServiceGetBulkStatusResponsePayload(
  const RetrieveServiceGetBulkStatusResponsePayload& data) :
  _mutex(),
  _status(data._status),
  _errormsg(data._errormsg),
  _errorCode(data._errorCode),
  _errorCodeSet(data._errorCodeSet)
{
}

RetrieveServiceGetBulkStatusResponsePayload::~RetrieveServiceGetBulkStatusResponsePayload() ACS_NOEXCEPT
{
}

RetrieveServiceGetBulkStatusResponsePayload& RetrieveServiceGetBulkStatusResponsePayload::operator=(
  const RetrieveServiceGetBulkStatusResponsePayload& data)
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

void RetrieveServiceGetBulkStatusResponsePayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBulkStatusResponsePayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void RetrieveServiceGetBulkStatusResponsePayload::fromJSONObject(const nlohmann::json& json)
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
      ACS_THROW(RetrieveServiceGetBulkStatusResponsePayloadException(os.str()));
    }
    _status = json[RESPONSE_PAYLOAD_KEY_STATUS];

    if (json.find(RESPONSE_PAYLOAD_KEY_ERRORMSG) != json.end() && !json[RESPONSE_PAYLOAD_KEY_ERRORMSG].is_null())
    {
      _errormsg = json[RESPONSE_PAYLOAD_KEY_ERRORMSG];
    }
    else
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_ERRORMSG
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(RetrieveServiceGetBulkStatusResponsePayloadException(os.str()));
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
        ACS_THROW(RetrieveServiceGetBulkStatusResponsePayloadException(os.str()));
      }
    }
    else
    {
      _errorCodeSet = false;
    }
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(RetrieveServiceGetBulkStatusResponsePayloadException(e, "Error reading JSON Object."));
  }
}

std::string RetrieveServiceGetBulkStatusResponsePayload::toJSON() const
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
    ACS_THROW(RetrieveServiceGetBulkStatusResponsePayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json RetrieveServiceGetBulkStatusResponsePayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    js[RESPONSE_PAYLOAD_KEY_STATUS] = _status;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _status << "\" for \"" << RESPONSE_PAYLOAD_KEY_STATUS
                                                           << "\" key");

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
    ACS_THROW(RetrieveServiceGetBulkStatusResponsePayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
