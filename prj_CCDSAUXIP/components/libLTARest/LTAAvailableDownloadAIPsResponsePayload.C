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

#include <LTAAvailableDownloadAIPsResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAAvailableDownloadAIPsResponsePayload);

namespace
{
constexpr char RESPONSE_PAYLOAD_KEY_AIP_ID[] = "download_aip_id";
constexpr char RESPONSE_PAYLOAD_KEY_MESSAGE[] = "message";
}  // namespace

LTAAvailableDownloadAIPsResponsePayload::LTAAvailableDownloadAIPsResponsePayload(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsstr);
}

LTAAvailableDownloadAIPsResponsePayload::LTAAvailableDownloadAIPsResponsePayload(const nlohmann::json& js)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSONObject(js);
}

void LTAAvailableDownloadAIPsResponsePayload::fromJSON(const std::string& jsstr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js = nlohmann::json::parse(jsstr);
  }
  catch (const nlohmann::json::exception& e)
  {
    ACS_THROW(LTAAvailableDownloadAIPsResponsePayloadException(e, "Error reading JSON."));
  }
  fromJSONObject(js);
}

void LTAAvailableDownloadAIPsResponsePayload::fromJSONObject(const nlohmann::json& json)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  try
  {
    ACS_COND_THROW(json.find(RESPONSE_PAYLOAD_KEY_AIP_ID) == json.end() || json[RESPONSE_PAYLOAD_KEY_AIP_ID].is_null(),
                   LTAAvailableDownloadAIPsResponsePayloadException()
                     << "Cannot find key \"" << RESPONSE_PAYLOAD_KEY_AIP_ID
                     << "\" into json response payload or the associated value is null. Key is mandatory.");

    _aipIDs = json[RESPONSE_PAYLOAD_KEY_AIP_ID].get<std::vector<dbPersistent::IdType>>();
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
    std::ostringstream os;
    if (_aipIDs.size() > 1)
    {
      auto last = std::prev(_aipIDs.end());
      std::copy(_aipIDs.begin(), last, std::ostream_iterator<dbPersistent::IdType>(os, ", "));
      os << *last;
    }
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Extracted value \"[" << os.str() << "]\" for key \""
                                                              << RESPONSE_PAYLOAD_KEY_AIP_ID << "\"");
    ACS_CLASS_END_DEBUG

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
    ACS_THROW(LTAAvailableDownloadAIPsResponsePayloadException(e, "Error reading JSON Object."));
  }
}

std::string LTAAvailableDownloadAIPsResponsePayload::toJSON() const
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
    ACS_THROW(LTAAvailableDownloadAIPsResponsePayloadException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return str;
}

nlohmann::json LTAAvailableDownloadAIPsResponsePayload::toJSONObject() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  nlohmann::json js;
  try
  {
    js[RESPONSE_PAYLOAD_KEY_AIP_ID] = _aipIDs;

    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
    std::ostringstream os;
    if (_aipIDs.size() > 1)
    {
      auto last = std::prev(_aipIDs.end());
      std::copy(_aipIDs.begin(), last, std::ostream_iterator<dbPersistent::IdType>(os, ", "));
      os << *last;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"[" << os.str() << "]\" for key \""
                                                            << RESPONSE_PAYLOAD_KEY_AIP_ID << "\"");
    ACS_CLASS_END_DEBUG

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
    ACS_THROW(LTAAvailableDownloadAIPsResponsePayloadException(e, "Error creating JSON Object."));
  }
  // LCOV_EXCL_STOP
  return js;
}

_ACS_END_NAMESPACE
