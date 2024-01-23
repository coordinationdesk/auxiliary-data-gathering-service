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

#ifndef _QuotaServiceCheckOrderQuotaRequestPayload_h_
#define _QuotaServiceCheckOrderQuotaRequestPayload_h_

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
class QuotaServiceCheckOrderQuotaRequestPayload
{
public:
  exDECLARE_EXCEPTION(QuotaServiceCheckOrderQuotaRequestPayloadException,
                      QuotaServicePayloadsExceptions::RequestPayloadException);
  static const std::string REQUEST_PAYLOAD_KEY_USERID;

public:
  /** Ctors/Dtors */
  QuotaServiceCheckOrderQuotaRequestPayload();
  explicit QuotaServiceCheckOrderQuotaRequestPayload(const std::string& jsstr);
  explicit QuotaServiceCheckOrderQuotaRequestPayload(const nlohmann::json& json);
  QuotaServiceCheckOrderQuotaRequestPayload(const QuotaServiceCheckOrderQuotaRequestPayload& c);
  virtual ~QuotaServiceCheckOrderQuotaRequestPayload() ACS_NOEXCEPT;

  /** Operators overload */
  QuotaServiceCheckOrderQuotaRequestPayload& operator=(const QuotaServiceCheckOrderQuotaRequestPayload& c);

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

  inline void setUserID(dbPersistent::IdType newVal)
  {
    Lock l(_mutex);
    _userID = newVal;
  }

private:
  Mutex _mutex;

  dbPersistent::IdType _userID;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(QuotaServiceCheckOrderQuotaRequestPayload);
};

_ACS_END_NAMESPACE

#endif /* _QuotaServiceCheckOrderQuotaRequestPayload_h_ */
