/*

    Copyright 2020-2021, Exprivia SPA - DFDA-AS
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it


    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. libFtpCurl Library $

 */

#include <AccessTokenPayload.h>
#include <json.hpp>
#include <StringUtils.h>

_ACS_BEGIN_NAMESPACE(acs)

/**********************
 * AccessTokenPayload *
 **********************/

ACS_CLASS_DEFINE_DEBUG_LEVEL(AccessTokenPayload);

const std::string AccessTokenPayload::PAYLOAD_KEY_ACCESSTOKEN = "access_token";
const std::string AccessTokenPayload::PAYLOAD_KEY_TOKENTYPE = "token_type";
const std::string AccessTokenPayload::PAYLOAD_KEY_EXPIREIN = "expires_in";

const std::map<AccessTokenPayload::TokenTypes, std::string>
  AccessTokenPayload::TOKENTYPESNAMES = {
    { BEARER, "BEARER" }
  };

const std::map<std::string, AccessTokenPayload::TokenTypes>
  AccessTokenPayload::TOKENTYPESVALUES = {
    { "BEARER", BEARER }
  };

AccessTokenPayload::AccessTokenPayload() :
  _accessToken(),
  _tokenType(INVALID),
  _expireIn()
{
}

AccessTokenPayload::AccessTokenPayload(const std::string& jsStr) :
  _accessToken(),
  _tokenType(INVALID),
  _expireIn()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsStr);
}

AccessTokenPayload::AccessTokenPayload(
  const AccessTokenPayload& c) :
  _accessToken(c._accessToken),
  _tokenType(c._tokenType),
  _expireIn(c._expireIn)
{
}

AccessTokenPayload::~AccessTokenPayload() ACS_NOEXCEPT
{
}

AccessTokenPayload&
AccessTokenPayload::operator=(
  const AccessTokenPayload& c)
{
  if (this != &c)
  {
    _accessToken = c._accessToken;
    _tokenType = c._tokenType;
    _expireIn = c._expireIn;
  }
  return *this;
}

void AccessTokenPayload::fromJSON(const std::string& jsStr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  try
  {
    const nlohmann::json jsObj = nlohmann::json::parse(jsStr);

    if (jsObj.find(PAYLOAD_KEY_ACCESSTOKEN) == jsObj.end() || jsObj[PAYLOAD_KEY_ACCESSTOKEN].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << PAYLOAD_KEY_ACCESSTOKEN 
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(AccessTokenPayload::AccessTokenPayloadException(os.str()));
    }
    _accessToken = jsObj[PAYLOAD_KEY_ACCESSTOKEN];
    if (_accessToken.empty())
    {
      std::ostringstream os;
      os << "Empty value for key \"" << PAYLOAD_KEY_ACCESSTOKEN << "\" into json response payload. Value is mandatory";
      ACS_THROW(AccessTokenPayload::AccessTokenPayloadException(os.str()));
    }

    if (jsObj.find(PAYLOAD_KEY_TOKENTYPE) == jsObj.end() || jsObj[PAYLOAD_KEY_TOKENTYPE].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << PAYLOAD_KEY_TOKENTYPE
         << "\" into json response payload or the associated value is null. Key is mandatory";
      ACS_THROW(AccessTokenPayload::AccessTokenPayloadException(os.str()));
    }
    std::map<std::string, TokenTypes>::const_iterator it =
      TOKENTYPESVALUES.find(StringUtils::uppercase(jsObj[PAYLOAD_KEY_TOKENTYPE]));
    if (it != TOKENTYPESVALUES.end())
    {
      _tokenType = it->second;
    }
    else
    {
      _tokenType = INVALID;
      std::ostringstream os;
      os << "Invalid value for \"" << PAYLOAD_KEY_TOKENTYPE << "\" key into json response payload";
      ACS_THROW(AccessTokenPayload::AccessTokenPayloadException(os.str()));
    }

    if (jsObj.find(PAYLOAD_KEY_EXPIREIN) == jsObj.end() || jsObj[PAYLOAD_KEY_EXPIREIN].is_null())
    {
      std::ostringstream os;
      os << "Cannot find key \"" << PAYLOAD_KEY_EXPIREIN
         << "\" into AccessTokenPayload json response payload or the associated value is null.";
      ACS_LOG_WARNING(os.str());
    }
    else
    {
      _expireIn = jsObj[PAYLOAD_KEY_EXPIREIN];
    }
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(AccessTokenPayload::AccessTokenPayloadException(e.what()));
  }
}

std::string AccessTokenPayload::toJSON() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string jsStr;
  try
  {
    nlohmann::json jsObj;

    if (_accessToken.empty())
    {
      std::ostringstream os;
      os << "Empty value for \"" << PAYLOAD_KEY_ACCESSTOKEN << "\" key";
      ACS_THROW(AccessTokenPayload::AccessTokenPayloadException(os.str()));
    }
    else
    {
      jsObj[PAYLOAD_KEY_ACCESSTOKEN] = _accessToken;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _accessToken << "\" for \"" << PAYLOAD_KEY_ACCESSTOKEN
                                                             << "\" key");
    }

    if (_tokenType != INVALID)
    {
      std::string TokenTypesName = TOKENTYPESNAMES.at(_tokenType);
      jsObj[PAYLOAD_KEY_TOKENTYPE] = TokenTypesName;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << TokenTypesName << "\" for \""
                                                             << PAYLOAD_KEY_TOKENTYPE << "\" key");
    }
    else
    {
      std::ostringstream os;
      os << "Invalid value for \"" << PAYLOAD_KEY_TOKENTYPE << "\" key";
      ACS_THROW(AccessTokenPayload::AccessTokenPayloadException(os.str()));
    }

    jsObj[PAYLOAD_KEY_EXPIREIN] = _expireIn;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _expireIn << "\" for \""
                                                           << PAYLOAD_KEY_EXPIREIN << "\" key");

    jsStr = jsObj.dump();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Request data transformed data into the following json string: \"\n"
                                          << jsStr << "\"");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(AccessTokenPayload::AccessTokenPayloadException(e.what()));
  }
  // LCOV_EXCL_STOP
  return jsStr;
}

_ACS_END_NAMESPACE
