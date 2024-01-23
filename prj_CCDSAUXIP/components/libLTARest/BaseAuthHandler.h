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

#ifndef _BaseAuthHandler_h_
#define _BaseAuthHandler_h_

#include <acs_c++config.hpp>

#include <GenericAuthHandler.h>
#include <FtpCurl.h>
#include <RemoteSite.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the Basic (user name/password) Authentication method Handler */
class BaseAuthHandler : public GenericAuthHandler
{
public:
  /** Ctors/Dtors */
  BaseAuthHandler(const std::string& userName, const std::string& password, bool decryptPassword = true);
  virtual ~BaseAuthHandler() ACS_NOEXCEPT;
  BaseAuthHandler(const BaseAuthHandler& c);

  /** Operators overload */
  BaseAuthHandler& operator=(const BaseAuthHandler& c);

  /** Clone method **/
  virtual std::unique_ptr<GenericAuthHandler> clone() const override;

  /** Set Authentication Parameters into CurlHttpOpt and RemoteSite for FtpCurl request **/
  virtual curl::RemoteSite initAuthenticatedEndpoint(const std::string& uri, curl::Curl::CurlHttpOpt* opt = nullptr)
    const;

private:
  /** Deleted special functions */
  BaseAuthHandler() = delete;

  std::string _userName;
  std::string _password;
  bool _decryptPassword;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(BaseAuthHandler);
};

_ACS_END_NAMESPACE

#endif /* _BaseAuthHandler_h_ */
