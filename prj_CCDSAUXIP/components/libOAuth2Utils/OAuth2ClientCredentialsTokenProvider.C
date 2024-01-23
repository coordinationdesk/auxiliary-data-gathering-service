/*

    Copyright 2020-2021, Exprivia SPA - DFDA-AS
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it


    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: OAuth2 Utils library $

 */

#include <OAuth2ClientCredentialsTokenProvider.h>

#include <FtpCurl.h>
#include <RemoteSiteUtility.h>

#include <AccessTokenPayload.h>
#include <libOAuth2UtV.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace
{
libOAuth2UtV vers;
}

/****************************************
 * OAuth2ClientCredentialsTokenProvider *
 ****************************************/

ACS_CLASS_DEFINE_DEBUG_LEVEL(OAuth2ClientCredentialsTokenProvider);

OAuth2ClientCredentialsTokenProvider::OAuth2ClientCredentialsTokenProvider(const std::string& tokenProviderURI,
                                                                           const TokenProviderAuthTypes type,
                                                                           const std::string& clientID,
                                                                           const std::string& clientSecret,
                                                                           const bool decryptSecret,
                                                                           const StopController& stopController) :
  _tokenProviderURI(tokenProviderURI),
  _type(type),
  _clientID(clientID),
  _clientSecret(clientSecret),
  _decryptSecret(decryptSecret),
  _stopController(stopController)
{
}

OAuth2ClientCredentialsTokenProvider::OAuth2ClientCredentialsTokenProvider(
  OAuth2ClientCredentialsTokenProvider const& c) :
  _tokenProviderURI(c._tokenProviderURI),
  _type(c._type),
  _clientID(c._clientID),
  _clientSecret(c._clientSecret),
  _decryptSecret(c._decryptSecret),
  _stopController(c._stopController)
{
}

OAuth2ClientCredentialsTokenProvider::~OAuth2ClientCredentialsTokenProvider() ACS_NOEXCEPT
{
}

std::string OAuth2ClientCredentialsTokenProvider::getAccessToken(const std::string& user, const std::string& password,
                                                                 const bool decryptPassword) const
{
  curl::BandPriority noPrio;
  FtpCurl curl(noPrio);
  curl::Curl::CurlHttpOpt opt;

  opt.setUri(_tokenProviderURI);

  if (_type == UserPassword)
  {
    std::ostringstream osUserPassword;
    osUserPassword << _clientID << ":";
    
    if (_decryptSecret)
    {
      osUserPassword << curl::RemoteSiteUtility::decrypt(_clientSecret);
    }
    else
    {
      osUserPassword << _clientSecret;
    }

    opt.setUserPassword(osUserPassword.str());
  }
  else if (_type == User)
  {
    opt.setUser(_clientID);
  }

  opt.setContentType("application/x-www-form-urlencoded");

  {
    std::ostringstream osMessage;
    osMessage << "grant_type=password&username=" << user << "&password=";

    if (decryptPassword)
    {
      osMessage << curl::RemoteSiteUtility::decrypt(password);
    }
    else
    {
      osMessage << password;
    }

    opt.setMessage(osMessage.str());
  }

  std::string responseStr;
  try
  {
    responseStr = curl.httpPost(opt, _stopController);
  }
  catch(std::exception& e)
  {
    std::ostringstream os;
    os << "Error in Token request to  TokenProvider. HTTP code: " << curl.getHttpResponseCode();
    ACS_THROW(OAuth2ClientCredentialsTokenProviderException(e, os.str()));
  }

  AccessTokenPayload response;
  try
  {
    response.fromJSON(responseStr);
  }
  catch(std::exception& e)
  {
    ACS_THROW(OAuth2ClientCredentialsTokenProviderException(e, "Error parsign TokenProvider response."));
  }

  AccessTokenPayload::TokenTypes tokenType = response.getTokenType();

  // LCOV_EXCL_START
  // Defensive coding: For now, there is only BEARER tokenType in response wrapper
  if (AccessTokenPayload::BEARER != tokenType)
  {
    std::ostringstream os;
    os << "Invalid TokenType:";
    if (AccessTokenPayload::INVALID != tokenType)
    {
      os << " \'" << AccessTokenPayload::TOKENTYPESNAMES.at(tokenType) << "\'";
    }
    os << " (" << static_cast<int>(tokenType) << ")";
    ACS_THROW(OAuth2ClientCredentialsTokenProviderException(os.str()));
  }
  // LCOV_EXCL_STOP

  return response.getAccessToken();
}

_ACS_END_NAMESPACE
