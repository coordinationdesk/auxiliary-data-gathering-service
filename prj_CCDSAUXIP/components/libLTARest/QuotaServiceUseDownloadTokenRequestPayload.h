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

#ifndef _QuotaServiceUseDownloadTokenRequestPayload_h_
#define _QuotaServiceUseDownloadTokenRequestPayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

#include <QuotaServicePayloadsExceptions.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful request payload sent to LTAQuotaService */
class QuotaServiceUseDownloadTokenRequestPayload
{
public:
  exDECLARE_EXCEPTION(QuotaServiceUseDownloadTokenRequestPayloadException,
                      QuotaServicePayloadsExceptions::RequestPayloadException);
  static const std::string REQUEST_PAYLOAD_KEY_USERID;
  static const std::string REQUEST_PAYLOAD_KEY_TOKENUUID;
  static const std::string REQUEST_PAYLOAD_KEY_DATASIZE;

public:
  /** Ctors/Dtors */
  QuotaServiceUseDownloadTokenRequestPayload();
  explicit QuotaServiceUseDownloadTokenRequestPayload(const std::string& jsstr);
  explicit QuotaServiceUseDownloadTokenRequestPayload(const nlohmann::json& json);
  QuotaServiceUseDownloadTokenRequestPayload(const QuotaServiceUseDownloadTokenRequestPayload& c);
  virtual ~QuotaServiceUseDownloadTokenRequestPayload() ACS_NOEXCEPT;

  /** Operators overload */
  QuotaServiceUseDownloadTokenRequestPayload& operator=(const QuotaServiceUseDownloadTokenRequestPayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline dbPersistent::IdType getUserID() const
  {
    Lock l(_mutex);
    return _userID;
  }

  inline std::string getTokenUUID() const
  {
    Lock l(_mutex);
    return _tokenUUID;
  }

  inline size_t getDataSize() const
  {
    Lock l(_mutex);
    return _dataSize;
  }

  inline void setUserID(dbPersistent::IdType newVal)
  {
    Lock l(_mutex);
    _userID = newVal;
  }

  inline void setTokenUUID(const std::string& newVal)
  {
    Lock l(_mutex);
    _tokenUUID = newVal;
  }

  inline void setDataSize(size_t newVal)
  {
    Lock l(_mutex);
    _dataSize = newVal;
  }

private:
  Mutex _mutex;

  dbPersistent::IdType _userID;
  std::string _tokenUUID;
  size_t _dataSize;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(QuotaServiceUseDownloadTokenRequestPayload);
};

_ACS_END_NAMESPACE

#endif /* _QuotaServiceUseDownloadTokenRequestPayload_h_ */
