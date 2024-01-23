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

#ifndef _RetrieveServiceGetBulkStatusRequestPayload_h_
#define _RetrieveServiceGetBulkStatusRequestPayload_h_

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
class RetrieveServiceGetBulkStatusRequestPayload
{
public:
  exDECLARE_EXCEPTION(RetrieveServiceGetBulkStatusRequestPayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_BULKID;

public:
  /** Ctors/Dtors */
  RetrieveServiceGetBulkStatusRequestPayload();
  explicit RetrieveServiceGetBulkStatusRequestPayload(const std::string& jsstr);
  explicit RetrieveServiceGetBulkStatusRequestPayload(const nlohmann::json& json);
  RetrieveServiceGetBulkStatusRequestPayload(const RetrieveServiceGetBulkStatusRequestPayload& c);
  virtual ~RetrieveServiceGetBulkStatusRequestPayload() ACS_NOEXCEPT;

  /** Operators overload */
  RetrieveServiceGetBulkStatusRequestPayload& operator=(const RetrieveServiceGetBulkStatusRequestPayload& c);

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

  inline void setBulkID(dbPersistent::IdType newVal)
  {
    Lock l(_mutex);
    _bulkID = newVal;
  }

private:
  Mutex _mutex;

  dbPersistent::IdType _bulkID;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(RetrieveServiceGetBulkStatusRequestPayload);
};

_ACS_END_NAMESPACE

#endif /* _RetrieveServiceGetBulkStatusRequestPayload_h_ */
