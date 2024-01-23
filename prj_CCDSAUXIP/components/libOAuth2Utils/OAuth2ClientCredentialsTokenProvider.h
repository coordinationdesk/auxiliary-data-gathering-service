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

#ifndef _OAuth2ClientCredentialsTokenProvider_H_
#define _OAuth2ClientCredentialsTokenProvider_H_

#include <acs_c++config.hpp>
#include <exMacros.h>
#include <Filterables.h>
#include <unistd.h>
#include <string>
#include <map>

#include <StopController.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * Implements a client for OAuth2 authentication.
 */
class OAuth2ClientCredentialsTokenProvider
{
public:
  exDECLARE_EXCEPTION(OAuth2ClientCredentialsTokenProviderException, exException);

  enum TokenProviderAuthTypes
  {
    None,
    User,
    UserPassword
  };

public:
  /**
    * Ctors & dtor.
    */
  OAuth2ClientCredentialsTokenProvider(const std::string& tokenProviderURI, const TokenProviderAuthTypes type = None,
                                       const std::string& clientID = "", const std::string& clientSecret = "",
                                       const bool decryptSecret = true,
                                       const StopController& stopController = AppStopController());
  OAuth2ClientCredentialsTokenProvider(OAuth2ClientCredentialsTokenProvider const& c);
  ~OAuth2ClientCredentialsTokenProvider() ACS_NOEXCEPT;
  
  std::string getAccessToken(const std::string& user, const std::string& password,
                             const bool decryptPassword = true) const;

private:
  std::string _tokenProviderURI;
  TokenProviderAuthTypes _type;
  std::string _clientID;
  std::string _clientSecret;
  bool _decryptSecret;
  const StopController& _stopController;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(OAuth2ClientCredentialsTokenProvider);
};

_ACS_END_NAMESPACE

#endif /* _OAuth2ClientCredentialsTokenProvider_H_ */
