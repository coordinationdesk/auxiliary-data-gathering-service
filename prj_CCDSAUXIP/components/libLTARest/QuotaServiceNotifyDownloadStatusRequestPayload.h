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

#ifndef _QuotaServiceNotifyDownloadStatusRequestPayload_h_
#define _QuotaServiceNotifyDownloadStatusRequestPayload_h_

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
class QuotaServiceNotifyDownloadStatusRequestPayload
{
public:
  exDECLARE_EXCEPTION(QuotaServiceNotifyDownloadStatusRequestPayloadException,
                      QuotaServicePayloadsExceptions::RequestPayloadException);
  static const std::string REQUEST_PAYLOAD_KEY_TOKENUUID;
  static const std::string REQUEST_PAYLOAD_KEY_FILENAME;
  static const std::string REQUEST_PAYLOAD_KEY_DATASIZE;
  static const std::string REQUEST_PAYLOAD_KEY_COMPLETED;

public:
  /** Ctors/Dtors */
  QuotaServiceNotifyDownloadStatusRequestPayload();
  explicit QuotaServiceNotifyDownloadStatusRequestPayload(const std::string& jsstr);
  explicit QuotaServiceNotifyDownloadStatusRequestPayload(const nlohmann::json& json);
  QuotaServiceNotifyDownloadStatusRequestPayload(const QuotaServiceNotifyDownloadStatusRequestPayload& c);
  virtual ~QuotaServiceNotifyDownloadStatusRequestPayload() ACS_NOEXCEPT;

  /** Operators overload */
  QuotaServiceNotifyDownloadStatusRequestPayload& operator=(const QuotaServiceNotifyDownloadStatusRequestPayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline std::string getTokenUUID() const
  {
    Lock l(_mutex);
    return _tokenUUID;
  }

  inline std::string getFileName() const
  {
    Lock l(_mutex);
    return _fileName;
  }

  inline size_t getDataSize(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr == set)
    {
      ACS_COND_THROW(!_dataSizeSet, QuotaServiceNotifyDownloadStatusRequestPayloadException(
                                      "Key \"" + REQUEST_PAYLOAD_KEY_DATASIZE + "\" not set."));
    }
    else
    {
      *set = _dataSizeSet;
    }
    return _dataSize;
  }

  inline bool getCompleted() const
  {
    Lock l(_mutex);
    return _completed;
  }

  inline void setTokenUUID(const std::string& newVal)
  {
    Lock l(_mutex);
    _tokenUUID = newVal;
  }

  inline void setFileName(const std::string& newVal)
  {
    Lock l(_mutex);
    _fileName = newVal;
  }

  inline void setDataSize(const size_t newVal)
  {
    Lock l(_mutex);
    _dataSize = newVal;
    _dataSizeSet = true;
  }

  inline void setCompleted(const bool newVal)
  {
    Lock l(_mutex);
    _completed = newVal;
  }

private:
  Mutex _mutex;

  std::string _tokenUUID;
  std::string _fileName;
  size_t _dataSize;
  bool _dataSizeSet;
  bool _completed;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(QuotaServiceNotifyDownloadStatusRequestPayload);
};

_ACS_END_NAMESPACE

#endif /* _QuotaServiceNotifyDownloadStatusRequestPayload_h_ */
