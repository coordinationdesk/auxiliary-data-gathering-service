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

#include <OAuth2Handler.h>

#include <RemoteSiteFactory.h>
#include <RemoteSiteUtility.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(OAuth2Handler);

OAuth2Handler::OAuth2Handler(const std::string& userName, const std::string& password,
                             const std::string& tokenProviderURI, const bool clientIDSet, const std::string& clientID,
                             const bool clientSecretSet, const std::string& clientSecret, const bool decryptPassword,
                             const bool decryptSecret, const StopController& stopController) :
  GenericAuthHandler(OAuth2),
  _userName(userName),
  _password(password),
  _tokenProviderURI(tokenProviderURI),
  _tokenProviderAuthType(OAuth2ClientCredentialsTokenProvider::TokenProviderAuthTypes::None),
  _clientID(clientID),
  _clientSecret(clientSecret),
  _decryptPassword(decryptPassword),
  _decryptSecret(decryptSecret),
  _stopController(stopController)
{
  if (clientIDSet)
  {
    if (clientSecretSet)
    {
      _tokenProviderAuthType = OAuth2ClientCredentialsTokenProvider::TokenProviderAuthTypes::UserPassword;
    }
    else
    {
      _tokenProviderAuthType = OAuth2ClientCredentialsTokenProvider::TokenProviderAuthTypes::User;
    }
  }
  else
  {
    _tokenProviderAuthType = OAuth2ClientCredentialsTokenProvider::TokenProviderAuthTypes::None;
  }
}

OAuth2Handler::~OAuth2Handler() ACS_NOEXCEPT {}

OAuth2Handler::OAuth2Handler(const OAuth2Handler& c) :
  GenericAuthHandler(BaseAuth),
  _userName(c._userName),
  _password(c._password),
  _tokenProviderURI(c._tokenProviderURI),
  _tokenProviderAuthType(c._tokenProviderAuthType),
  _clientID(c._clientID),
  _clientSecret(c._clientSecret),
  _decryptPassword(c._decryptPassword),
  _decryptSecret(c._decryptSecret),
  _stopController(c._stopController)
{
}

std::unique_ptr<GenericAuthHandler> OAuth2Handler::clone() const
{
  return std::make_unique<OAuth2Handler>(*this);
}

curl::RemoteSite OAuth2Handler::initAuthenticatedEndpoint(const std::string& uri, curl::Curl::CurlHttpOpt* opt) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

  if (nullptr != opt)
  {
    std::ostringstream osUserPassword;
    osUserPassword << _userName << ":";

    if (_decryptPassword) {
      osUserPassword << curl::RemoteSiteUtility::decrypt(_password);
    }
    else {
      osUserPassword << _password;
    }

    opt->setUri(uri);
    opt->setUserPassword(osUserPassword.str());
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "URI set to \"" << uri << "\"");

  curl::RemoteSite site = curl::RemoteSiteFactory::makeSite(uri);
  OAuth2ClientCredentialsTokenProvider client(_tokenProviderURI, _tokenProviderAuthType, _clientID, _clientSecret,
                                                    _decryptSecret, _stopController);
  std::ostringstream osToken;
  osToken << "Bearer: " << client.getAccessToken(_userName, _password, _decryptPassword);
  site.addCustomHeader(osToken.str());

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Authentication token set to \"******\"");

  return site;
}

_ACS_END_NAMESPACE
