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

#include <ODataOrderResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace lta;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataOrderResponsePayload);

const std::string ODataOrderResponsePayload::RESPONSE_PAYLOAD_KEY_ODATACONTEXT = "@odata.context";
const std::string ODataOrderResponsePayload::RESPONSE_PAYLOAD_KEY_ID = "Id";
const std::string ODataOrderResponsePayload::RESPONSE_PAYLOAD_KEY_STATUS = "Status";

// TODO: Check if the correct value is "$metadata#Orders" or "$metadata#OData.CSC.Order"
const std::string ODataOrderResponsePayload::RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT = "$metadata#Orders";

ODataOrderResponsePayload::ODataOrderResponsePayload() :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _id(),
  _status(ORDERSTATUS_INVALIDSTATUS)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

ODataOrderResponsePayload::ODataOrderResponsePayload(const std::string& jsstr) :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _id(),
  _status(ORDERSTATUS_INVALIDSTATUS)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

ODataOrderResponsePayload::ODataOrderResponsePayload(const nlohmann::json& js) :
  _mutex(),
  _oDataContext(RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT),
  _id(),
  _status(ORDERSTATUS_INVALIDSTATUS)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

ODataOrderResponsePayload::ODataOrderResponsePayload(
  const ODataOrderResponsePayload& data) :
  _mutex(),
  _oDataContext(data._oDataContext),
  _id(data._id),
  _status(data._status)
{
}

ODataOrderResponsePayload::~ODataOrderResponsePayload() ACS_NOEXCEPT
{
}

ODataOrderResponsePayload& ODataOrderResponsePayload::operator=(const ODataOrderResponsePayload& data)
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
        _oDataContext = data._oDataContext;
        _id = data._id;
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

void ODataOrderResponsePayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataOrderResponsePayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void ODataOrderResponsePayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  Lock l(_mutex);

  try
  {
    if (json.find(RESPONSE_PAYLOAD_KEY_ODATACONTEXT) == json.end() || json[RESPONSE_PAYLOAD_KEY_ODATACONTEXT].is_null())
    {
      ACS_LOG_WARNING("Cannot find key \"" << RESPONSE_PAYLOAD_KEY_ODATACONTEXT
                                           << "\" into json response payload or the associated value is null.");
      _oDataContext = RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT;
    }
    else
    {
      _oDataContext = json[RESPONSE_PAYLOAD_KEY_ODATACONTEXT];
    }
    if (_oDataContext != RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT)
    {
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Value \"" << _oDataContext << "\" is not what is expected (\""
                                 << RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT << "\") for \""
                                 << RESPONSE_PAYLOAD_KEY_ODATACONTEXT << "\" key into json response payload ")
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_ID) == json.end() || json[RESPONSE_PAYLOAD_KEY_ID].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_ID
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(ODataOrderResponsePayloadException(os.str()));
    }
    _id = json[RESPONSE_PAYLOAD_KEY_ID];
    if (_id.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << RESPONSE_PAYLOAD_KEY_ID << "\" into json response payload. Value is mandatory";
      ACS_THROW(ODataOrderResponsePayloadException(os.str()));
    }

    if (json.find(RESPONSE_PAYLOAD_KEY_STATUS) == json.end() ||
        json[RESPONSE_PAYLOAD_KEY_STATUS].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(ODataOrderResponsePayloadException(os.str()));
    }
    std::string statusStr = json[RESPONSE_PAYLOAD_KEY_STATUS];
    if (statusStr.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(ODataOrderResponsePayloadException(os.str()));
    }
    std::map<std::string, OrderStatus>::const_iterator it = StringToOrderStatus.find(statusStr);
    if (it == StringToOrderStatus.end())
    {
      _status = ORDERSTATUS_INVALIDSTATUS;
      std::ostringstream os;
      os << "Invalid value for key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(ODataOrderResponsePayloadException(os.str()));
    }
    _status = it->second;
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataOrderResponsePayloadException(e, "Error reading JSON Object."));
  }
}

std::string ODataOrderResponsePayload::toJSON() const
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
    ACS_THROW(ODataOrderResponsePayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP

  return str;
}

nlohmann::json ODataOrderResponsePayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    // Add members to json object
    js[RESPONSE_PAYLOAD_KEY_ODATACONTEXT] = _oDataContext;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _oDataContext << "\" for \""
                                                           << RESPONSE_PAYLOAD_KEY_ODATACONTEXT << "\" key");

    js[RESPONSE_PAYLOAD_KEY_ID] = _id;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _id << "\" for \"" << RESPONSE_PAYLOAD_KEY_ID
                                                           << "\" key");

    if (ORDERSTATUS_INVALIDSTATUS == _status)
    {
      std::ostringstream os;
      os << "Invalid value for key \"" << RESPONSE_PAYLOAD_KEY_STATUS
         << "\" into json response payload. Value is mandatory";
      ACS_THROW(ODataOrderResponsePayloadException(os.str()));
    }
    std::string statusStr = OrderStatusToString.at(_status);
    js[RESPONSE_PAYLOAD_KEY_STATUS] = statusStr;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << statusStr << "\" for \"" << RESPONSE_PAYLOAD_KEY_STATUS
                                                           << "\" key");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataOrderResponsePayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
