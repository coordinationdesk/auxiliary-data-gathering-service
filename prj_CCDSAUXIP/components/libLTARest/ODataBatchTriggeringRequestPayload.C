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

#include <ODataBatchTriggeringRequestPayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ODataBatchTriggeringRequestPayload);

const std::string ODataBatchTriggeringRequestPayload::REQUEST_PAYLOAD_KEY_PRIORITY = "Priority";

ODataBatchTriggeringRequestPayload::ODataBatchTriggeringRequestPayload() :
  _mutex(),
  _prioritySet(false),
  _priority(0)
{
}

ODataBatchTriggeringRequestPayload::ODataBatchTriggeringRequestPayload(const std::string& jsstr) :
  _mutex(),
  _prioritySet(false),
  _priority(0)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

ODataBatchTriggeringRequestPayload::ODataBatchTriggeringRequestPayload(const nlohmann::json& js) :
  _mutex(),
  _prioritySet(false),
  _priority(0)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

ODataBatchTriggeringRequestPayload::ODataBatchTriggeringRequestPayload(
  const ODataBatchTriggeringRequestPayload& data) :
  _mutex(),
  _prioritySet(data._prioritySet),
  _priority(data._priority)
{
}

ODataBatchTriggeringRequestPayload::~ODataBatchTriggeringRequestPayload() ACS_NOEXCEPT
{
}

ODataBatchTriggeringRequestPayload& ODataBatchTriggeringRequestPayload::operator=(
  const ODataBatchTriggeringRequestPayload& data)
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
        _prioritySet = data._prioritySet;
        _priority = data._priority;
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

void ODataBatchTriggeringRequestPayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataBatchTriggeringRequestPayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void ODataBatchTriggeringRequestPayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  try
  {
    if (json.find(REQUEST_PAYLOAD_KEY_PRIORITY) != json.end() && !json[REQUEST_PAYLOAD_KEY_PRIORITY].is_null())
    {
      _priority = json[REQUEST_PAYLOAD_KEY_PRIORITY];
      _prioritySet = true;
    }
    else
    {
      _priority = 0;
      _prioritySet = false;
    }
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataBatchTriggeringRequestPayloadException(e, "Error reading JSON Object."));
  }
}

std::string ODataBatchTriggeringRequestPayload::toJSON() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Prevents generating an empty JSON object
  if (!_prioritySet)
  {
    return "";
  }
  
  std::string str;
  nlohmann::json js;
  try
  {
    js = toJSONObject();
    str = js.dump();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Request data transformed data into the following json string: \"\n"
                                          << str << "\"");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataBatchTriggeringRequestPayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json ODataBatchTriggeringRequestPayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  Lock l(_mutex);

  nlohmann::json js;
  try
  {
    if (_prioritySet)
    {
      js[REQUEST_PAYLOAD_KEY_PRIORITY] = _priority;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _priority << "\" for \""
                                                             << REQUEST_PAYLOAD_KEY_PRIORITY << "\" key");
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(ODataBatchTriggeringRequestPayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
