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

#ifndef _ODataQueryProductsRequestPayload_h_
#define _ODataQueryProductsRequestPayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful request payload sent to LTA Service in OData format */
class ODataOrderRequestPayload
{
public:
  exDECLARE_EXCEPTION(ODataOrderRequestPayloadException, exException);
  static const std::string REQUEST_PAYLOAD_KEY_NOTIFICATIONENDPOINT;
  static const std::string REQUEST_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME;
  static const std::string REQUEST_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD;
  static const std::string REQUEST_PAYLOAD_KEY_PRIORITY;

public:
  /** Ctors/Dtors */
  ODataOrderRequestPayload();
  explicit ODataOrderRequestPayload(const std::string& jsstr);
  explicit ODataOrderRequestPayload(const nlohmann::json& json);
  ODataOrderRequestPayload(const ODataOrderRequestPayload& c);
  virtual ~ODataOrderRequestPayload() ACS_NOEXCEPT;

  /** Operators overload */
  ODataOrderRequestPayload& operator=(const ODataOrderRequestPayload& c);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& jsstr);

  inline std::string getNotificationEndpoint(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr != set)
    {
      *set = _notificationEndpointSet;
      return _notificationEndpoint;
    }
    else
    {
      if (_notificationEndpointSet)
      {
        return _notificationEndpoint;
      }
      else
      {
        ACS_THROW(ODataOrderRequestPayloadException("NotificationEndpoint not set"));
      }
    }
  }

  inline std::string getNotificationEpUsername(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr != set)
    {
      *set = _notificationEpUsernameSet;
      return _notificationEpUsername;
    }
    else
    {
      if (_notificationEpUsernameSet)
      {
        return _notificationEpUsername;
      }
      else
      {
        ACS_THROW(ODataOrderRequestPayloadException("NotificationEpUsername not set"));
      }
    }
  }

  inline std::string getNotificationEpPassword(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr != set)
    {
      *set = _notificationEpPasswordSet;
      return _notificationEpPassword;
    }
    else
    {
      if (_notificationEpPasswordSet)
      {
        return _notificationEpPassword;
      }
      else
      {
        ACS_THROW(ODataOrderRequestPayloadException("NotificationEpPassword not set"));
      }
    }
  }

  inline int64_t getPriority(bool* set = nullptr) const
  {
    Lock l(_mutex);
    if (nullptr != set)
    {
      *set = _prioritySet;
      return _priority;
    }
    else
    {
      if (_prioritySet)
      {
        return _priority;
      }
      else
      {
        ACS_THROW(ODataOrderRequestPayloadException("Priority not set"));
      }
    }
  }

  inline void setNotificationEndpoint(const std::string& newVal)
  {
    Lock l(_mutex);
    _notificationEndpoint = newVal;
    _notificationEndpointSet = true;
  }

  inline void setNotificationEpUsername(const std::string& newVal)
  {
    Lock l(_mutex);
    _notificationEpUsername = newVal;
    _notificationEpUsernameSet = true;
  }

  inline void setNotificationEpPassword(const std::string& newVal)
  {
    Lock l(_mutex);
    _notificationEpPassword = newVal;
    _notificationEpPasswordSet = true;
  }

  inline void setPriority(const int64_t newVal)
  {
    Lock l(_mutex);
    _priority = newVal;
    _prioritySet = true;
  }

private:
  Mutex _mutex;

  bool _notificationEndpointSet;
  std::string _notificationEndpoint;
  bool _notificationEpUsernameSet;
  std::string _notificationEpUsername;
  bool _notificationEpPasswordSet;
  std::string _notificationEpPassword;
  bool _prioritySet;
  int64_t _priority;
  //TODO: Aggiungere gli altri campi (ad chiedere ICD completo)

  ACS_CLASS_DECLARE_DEBUG_LEVEL(ODataOrderRequestPayload);
};

_ACS_END_NAMESPACE

#endif /* _ODataOrderRequestPayload_h_ */
