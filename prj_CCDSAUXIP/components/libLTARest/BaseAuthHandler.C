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

#include <BaseAuthHandler.h>

#include <RemoteSiteFactory.h>
#include <RemoteSiteUtility.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(BaseAuthHandler);

BaseAuthHandler::BaseAuthHandler(const std::string& userName, const std::string& password, bool decryptPassword) :
  GenericAuthHandler(BaseAuth),
  _userName(userName),
  _password(password),
  _decryptPassword(decryptPassword)
{
}

BaseAuthHandler::~BaseAuthHandler() ACS_NOEXCEPT {}

BaseAuthHandler::BaseAuthHandler(const BaseAuthHandler& c) :
  GenericAuthHandler(BaseAuth),
  _userName(c._userName),
  _password(c._password),
  _decryptPassword(c._decryptPassword)
{
}

BaseAuthHandler& BaseAuthHandler::operator=(const BaseAuthHandler& c)
{
  if (this != &c)
  {
    _userName = c._userName;
    _password = c._password;
    _decryptPassword = c._decryptPassword;
  }
  return *this;
}

std::unique_ptr<GenericAuthHandler> BaseAuthHandler::clone() const
{
  return std::make_unique<BaseAuthHandler>(*this);
}

curl::RemoteSite BaseAuthHandler::initAuthenticatedEndpoint(const std::string& uri, curl::Curl::CurlHttpOpt* opt) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

  if (nullptr != opt)
  {
    std::ostringstream osUserPassword;
    osUserPassword << _userName << ":";
    if (_decryptPassword)
    {
      osUserPassword << curl::RemoteSiteUtility::decrypt(_password);
    }
    else
    {
      osUserPassword << _password;
    }

    opt->setUri(uri);
    opt->setUserPassword(osUserPassword.str());
  }

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "URI set to \"" << uri << "\"");
  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Authentication set to \"" << _userName << ":******\"");

  return curl::RemoteSiteFactory::makeSite(uri, _userName, _password, _decryptPassword);
}

_ACS_END_NAMESPACE
