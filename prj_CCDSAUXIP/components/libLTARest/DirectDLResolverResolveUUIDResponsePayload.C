/*

  Copyright 2022, Exprivia - DFDA-AS
  Via Della Bufalotta, 378 - 00139 Roma - Italy
  http://www.exprivia.it

  All Rights Reserved.

  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
  the contents of this file may not be disclosed to third parties, copied or
  duplicated in any form, in whole or in part, without the prior written
  permission of Exprivia SpA

  $Prod: LTA Rest Interface $

*/

#include <DirectDLResolverResolveUUIDResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(DirectDLResolverResolveUUIDResponsePayload);

namespace
{
constexpr char RESPONSE_PAYLOAD_KEY_DOWNLOAD_URL[] = "downloadurl";
constexpr char RESPONSE_PAYLOAD_KEY_MESSAGE[] = "message";
}  // namespace

DirectDLResolverResolveUUIDResponsePayload::DirectDLResolverResolveUUIDResponsePayload(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

DirectDLResolverResolveUUIDResponsePayload::DirectDLResolverResolveUUIDResponsePayload(const nlohmann::json& js)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

void DirectDLResolverResolveUUIDResponsePayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (const nlohmann::json::exception& e)
  {
    ACS_THROW(DirectDLResolverResolveUUIDResponsePayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void DirectDLResolverResolveUUIDResponsePayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  try
  {
    ACS_COND_THROW(json.find(RESPONSE_PAYLOAD_KEY_DOWNLOAD_URL) == json.end() ||
                     json[RESPONSE_PAYLOAD_KEY_DOWNLOAD_URL].is_null(),
                   DirectDLResolverResolveUUIDResponsePayloadException()
                     << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_DOWNLOAD_URL
                     << "\" into json response payload or the associated value is null. Key is mandatory.");

    _downloadURL = json[RESPONSE_PAYLOAD_KEY_DOWNLOAD_URL];
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Extracted value \"" << _downloadURL << "\" for key \""
                                                             << RESPONSE_PAYLOAD_KEY_DOWNLOAD_URL << "\"");

    if (json.find(RESPONSE_PAYLOAD_KEY_MESSAGE) != json.end() && !json[RESPONSE_PAYLOAD_KEY_MESSAGE].is_null())
    {
      _messageSet = true;
      _message = json[RESPONSE_PAYLOAD_KEY_MESSAGE];

      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Extracted value \"" << _message << "\" for key \""
                                                               << RESPONSE_PAYLOAD_KEY_MESSAGE << "\"");
    }
    else
    {
      _messageSet = false;
      _message = "";

      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No value extracted for key \"" << RESPONSE_PAYLOAD_KEY_MESSAGE << "\"");
    }
  }
  catch (const nlohmann::json::exception& e)
  {
    ACS_THROW(DirectDLResolverResolveUUIDResponsePayloadException(e, "Error reading JSON Object."));
  }
}

std::string DirectDLResolverResolveUUIDResponsePayload::toJSON() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string str;
  try
  {
    str = toJSONObject().dump();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Transformed response data into the following json string:\n"
                                          << str);
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (const nlohmann::json::exception& e)
  {
    ACS_THROW(DirectDLResolverResolveUUIDResponsePayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json DirectDLResolverResolveUUIDResponsePayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js[RESPONSE_PAYLOAD_KEY_DOWNLOAD_URL] = _downloadURL;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _downloadURL << "\" for key \""
                                                           << RESPONSE_PAYLOAD_KEY_DOWNLOAD_URL << "\"");

    if (_messageSet)
    {
      js[RESPONSE_PAYLOAD_KEY_MESSAGE] = _message;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _message << "\" for key \""
                                                             << RESPONSE_PAYLOAD_KEY_MESSAGE << "\"");
    }
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (const nlohmann::json::exception& e)
  {
    ACS_THROW(DirectDLResolverResolveUUIDResponsePayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
