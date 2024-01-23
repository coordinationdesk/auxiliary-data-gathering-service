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

#ifndef _OAuth2Handler_h_
#define _OAuth2Handler_h_

#include <acs_c++config.hpp>

#include <GenericAuthHandler.h>
#include <FtpCurl.h>
#include <RemoteSite.h>
#include <OAuth2ClientCredentialsTokenProvider.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the Basic (user name/password) Authentication method Handler */
class OAuth2Handler : public GenericAuthHandler
{
public:
  /** Ctors/Dtors */
  OAuth2Handler(const std::string& userName, const std::string& password, const std::string& tokenProviderURI,
                const bool clientIDSet, const std::string& clientID, const bool clientSecretSet,
                const std::string& clientSecret, const bool decryptPassword = true, const bool decryptSecret = true,
                const StopController& stopController = AppStopController());
  virtual ~OAuth2Handler() ACS_NOEXCEPT;
  OAuth2Handler(const OAuth2Handler& c);

  /** Clone method **/
  virtual std::unique_ptr<GenericAuthHandler> clone() const override;

  /** Set Authentication Parameters into CurlHttpOpt and RemoteSite for FtpCurl request **/
  virtual curl::RemoteSite initAuthenticatedEndpoint(const std::string& uri, curl::Curl::CurlHttpOpt* opt = nullptr)
    const;

private:
  /** Deleted special functions */
  OAuth2Handler() = delete;

  std::string _userName;
  std::string _password;
  std::string _tokenProviderURI;
  OAuth2ClientCredentialsTokenProvider::TokenProviderAuthTypes _tokenProviderAuthType;
  std::string _clientID;
  std::string _clientSecret;
  bool _decryptPassword;
  bool _decryptSecret;
  const StopController& _stopController;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(OAuth2Handler);
};

_ACS_END_NAMESPACE

#endif /* _OAuth2Handler_h_ */
