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

#ifndef _RetrieveServiceGetBatchDetailsRequestPayload_h_
#define _RetrieveServiceGetBatchDetailsRequestPayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <map>
#include <string>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful request payload sent to LTARetrieveService */
class RetrieveServiceGetBatchDetailsRequestPayload
{
public:
  exDECLARE_EXCEPTION(RetrieveServiceGetBatchDetailsRequestPayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_BATCHID;

public:
  /** Ctors/Dtors */
  RetrieveServiceGetBatchDetailsRequestPayload();
  explicit RetrieveServiceGetBatchDetailsRequestPayload(const std::string& jsstr);
  explicit RetrieveServiceGetBatchDetailsRequestPayload(const nlohmann::json& json);
  RetrieveServiceGetBatchDetailsRequestPayload(const RetrieveServiceGetBatchDetailsRequestPayload& c);
  virtual ~RetrieveServiceGetBatchDetailsRequestPayload() ACS_NOEXCEPT;

  /** Operators overload */
  RetrieveServiceGetBatchDetailsRequestPayload& operator=(const RetrieveServiceGetBatchDetailsRequestPayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline dbPersistent::IdType getBatchID() const
  {
    Lock l(_mutex);
    return _batchID;
  }

  inline void setBatchID(dbPersistent::IdType newVal)
  {
    Lock l(_mutex);
    _batchID = newVal;
  }

private:
  Mutex _mutex;

  dbPersistent::IdType _batchID;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(RetrieveServiceGetBatchDetailsRequestPayload);
};

_ACS_END_NAMESPACE

#endif /* _RetrieveServiceGetBatchDetailsRequestPayload_h_ */
