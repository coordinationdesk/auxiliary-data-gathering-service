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

#ifndef _AccessTokenPayload_H_
#define _AccessTokenPayload_H_

#include <acs_c++config.hpp>
#include <exMacros.h>
#include <Filterables.h>
#include <unistd.h>
#include <string>
#include <map>

#include <StopController.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This object wrap the Response of a Get Token Request.
 */
class AccessTokenPayload
{
public:
  exDECLARE_EXCEPTION(AccessTokenPayloadException, exException);
  static const std::string PAYLOAD_KEY_ACCESSTOKEN;
  static const std::string PAYLOAD_KEY_TOKENTYPE;
  static const std::string PAYLOAD_KEY_EXPIREIN;

  enum TokenTypes
  {
    BEARER,
    INVALID
  };
  static const std::map<TokenTypes, std::string> TOKENTYPESNAMES;
  static const std::map<std::string, TokenTypes> TOKENTYPESVALUES;

public:
  /** Ctors/Dtors */
  AccessTokenPayload();
  explicit AccessTokenPayload(const std::string& jsStr);
  AccessTokenPayload(const AccessTokenPayload& c);
  virtual ~AccessTokenPayload() ACS_NOEXCEPT;

  /** Operators overload */
  AccessTokenPayload& operator=(const AccessTokenPayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;

  /** Initialize this object from the json string in input */
  void fromJSON(const std::string& jsStr);

  inline std::string getAccessToken() const
  {
    return _accessToken;
  }

  inline TokenTypes getTokenType() const
  {
    return _tokenType;
  }

  inline long getExpireIn() const
  {
    return _expireIn;
  }

  inline void setAccessToken(const std::string& newVal)
  {
    _accessToken = newVal;
  }

  inline void setTokenType(const TokenTypes newVal)
  {
    _tokenType = newVal;
  }

  inline void setExpireIn(long newVal)
  {
    _expireIn = newVal;
  }

private:
  std::string _accessToken;
  TokenTypes _tokenType;
  long _expireIn;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(AccessTokenPayload);
};

_ACS_END_NAMESPACE

#endif /* _AccessTokenPayload_H_ */
