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

#ifndef _RetrieveServiceGetFileStatusRequestPayload_h_
#define _RetrieveServiceGetFileStatusRequestPayload_h_

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
class RetrieveServiceGetFileStatusRequestPayload
{
public:
  exDECLARE_EXCEPTION(RetrieveServiceGetFileStatusRequestPayloadException, exException);
  static const std::string REQUEST_PAYLOAD_KEY_FILEQUEUEID;

public:
  /** Ctors/Dtors */
  RetrieveServiceGetFileStatusRequestPayload();
  explicit RetrieveServiceGetFileStatusRequestPayload(const std::string& jsstr);
  explicit RetrieveServiceGetFileStatusRequestPayload(const nlohmann::json& json);
  RetrieveServiceGetFileStatusRequestPayload(const RetrieveServiceGetFileStatusRequestPayload& c);
  virtual ~RetrieveServiceGetFileStatusRequestPayload() ACS_NOEXCEPT;

  /** Operators overload */
  RetrieveServiceGetFileStatusRequestPayload& operator=(const RetrieveServiceGetFileStatusRequestPayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline dbPersistent::IdType getFilequeueID() const
  {
    Lock l(_mutex);
    return _filequeueID;
  }

  inline void setFilequeueID(dbPersistent::IdType newVal)
  {
    Lock l(_mutex);
    _filequeueID = newVal;
  }

private:
  Mutex _mutex;

  dbPersistent::IdType _filequeueID;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(RetrieveServiceGetFileStatusRequestPayload);
};

_ACS_END_NAMESPACE

#endif /* _RetrieveServiceGetFileStatusRequestPayload_h_ */
