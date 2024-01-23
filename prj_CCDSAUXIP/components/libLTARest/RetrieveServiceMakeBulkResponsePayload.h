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

#ifndef _RetrieveServiceMakeBulkResponsePayload_h_
#define _RetrieveServiceMakeBulkResponsePayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

#include <RetrieveServiceResponseErrorCodeEnum.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTARetrieveService */
class RetrieveServiceMakeBulkResponsePayload
{
public:
  exDECLARE_EXCEPTION(RetrieveServiceMakeBulkResponsePayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_BULKID;
  static const std::string RESPONSE_PAYLOAD_KEY_ERRORMSG;
  static const std::string RESPONSE_PAYLOAD_KEY_ERRORCODE;

public:
  /** Ctors/Dtors */
  RetrieveServiceMakeBulkResponsePayload();
  explicit RetrieveServiceMakeBulkResponsePayload(const std::string& jsstr);
  explicit RetrieveServiceMakeBulkResponsePayload(const nlohmann::json& json);
  RetrieveServiceMakeBulkResponsePayload(const RetrieveServiceMakeBulkResponsePayload& c);
  virtual ~RetrieveServiceMakeBulkResponsePayload() ACS_NOEXCEPT;

  /** Operators overload */
  RetrieveServiceMakeBulkResponsePayload& operator=(const RetrieveServiceMakeBulkResponsePayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline dbPersistent::IdType getBulkID() const
  {
    Lock l(_mutex);
    return _bulkID;
  }

  inline std::string getErrormsg() const
  {
    Lock l(_mutex);
    return _errormsg;
  }

  inline RetrieveServiceResponseErrorCode getErrorCode(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr == set)
    {
      ACS_COND_THROW(!_errorCodeSet, RetrieveServiceMakeBulkResponsePayloadException(
                                       RESPONSE_PAYLOAD_KEY_ERRORCODE + " not set"));
    }
    else
    {
      *set = _errorCodeSet;
    }

    return _errorCode;
  }

  inline void setBulkID(dbPersistent::IdType newVal)
  {
    Lock l(_mutex);
    _bulkID = newVal;
  }

  inline void setErrormsg(const std::string& newVal)
  {
    Lock l(_mutex);
    _errormsg = newVal;
  }

  inline void setErrorCode(const RetrieveServiceResponseErrorCode& newVal)
  {
    Lock l(_mutex);
    _errorCode = newVal;
    _errorCodeSet = true;
  }

  inline void unsetErrorCode()
  {
    Lock l(_mutex);
    _errorCodeSet = false;
  }

private:
  Mutex _mutex;

  dbPersistent::IdType _bulkID;
  std::string _errormsg;
  RetrieveServiceResponseErrorCode _errorCode;
  bool _errorCodeSet;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(RetrieveServiceMakeBulkResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _RetrieveServiceMakeBulkResponsePayload_h_ */
