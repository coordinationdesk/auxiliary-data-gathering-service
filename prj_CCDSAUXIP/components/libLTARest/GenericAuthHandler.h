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

#ifndef _GenericAuthHandler_h_
#define _GenericAuthHandler_h_

#include <acs_c++config.hpp>

#include <FtpCurl.h>
#include <RemoteSite.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object define the common interfaces of all the the Authentications Handler objects */
class GenericAuthHandler
{
public:
  enum AuthType
  {
    BaseAuth,
    OAuth2
  };

public:
  /** Ctor/Dtor */
  GenericAuthHandler(AuthType type);
  virtual ~GenericAuthHandler() ACS_NOEXCEPT;

  /** Clone method **/
  virtual std::unique_ptr<GenericAuthHandler> clone() const = 0;

  /** Set Authentication Parameters into CurlHttpOpt and RemoteSite for FtpCurl request **/
  virtual curl::RemoteSite initAuthenticatedEndpoint(const std::string& uri, curl::Curl::CurlHttpOpt* opt = nullptr)
    const = 0;

  /** Get the Authentication type */
  inline const AuthType getAuthType() const
  {
    return _type;
  }

private:
  /** Deleted special functions */
  GenericAuthHandler() = delete;
  GenericAuthHandler(const GenericAuthHandler& c) = delete;

  /** delete Operators overload */
  GenericAuthHandler& operator=(const GenericAuthHandler& c) = delete;

  AuthType _type;
};

_ACS_END_NAMESPACE

#endif /* _GenericAuthHandler_h_ */
