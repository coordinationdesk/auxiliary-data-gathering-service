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

#ifndef _ODataSubscriptionResponsePayload_h_
#define _ODataSubscriptionResponsePayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>
#include <map>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>
#include <LTACommon.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTA Service in OData format */
class ODataSubscriptionResponsePayload
{
public:
  exDECLARE_EXCEPTION(ODataSubscriptionResponsePayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_ODATACONTEXT;
  static const std::string RESPONSE_PAYLOAD_KEY_ID;
  static const std::string RESPONSE_PAYLOAD_KEY_STATUS;
  static const std::string RESPONSE_PAYLOAD_KEY_FILTERPARAM;
  static const std::string RESPONSE_PAYLOAD_KEY_STAGEORDER;
  static const std::string RESPONSE_PAYLOAD_KEY_NOTIFICATIONENDPOINT;
  static const std::string RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPUSERNAME;
  static const std::string RESPONSE_PAYLOAD_KEY_NOTIFICATIONEPPASSWORD;
  static const std::string RESPONSE_PAYLOAD_KEY_PRIORITY;

  static const std::string RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT;

  enum SubscriptionStatus
  {
    RUNNING,
    PAUSED,
    CANCELLED,
    INVALID_STATUS
  };

  static const std::map<SubscriptionStatus, std::string> SUBSCRIPTIONSTATUS_TO_STRING;
  static const std::map<std::string, SubscriptionStatus> STRING_TO_SUBSCRIPTIONSTATUS;

public:
  /** Ctors/Dtors */
  ODataSubscriptionResponsePayload();
  explicit ODataSubscriptionResponsePayload(const std::string& jsstr);
  explicit ODataSubscriptionResponsePayload(const nlohmann::json& json);
  ODataSubscriptionResponsePayload(const ODataSubscriptionResponsePayload&);
  virtual ~ODataSubscriptionResponsePayload() ACS_NOEXCEPT;

  /** Operators overload */
  ODataSubscriptionResponsePayload& operator=(const ODataSubscriptionResponsePayload&);

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& json);

  inline std::string getODataContext() const
  {
    Lock l(_mutex);
    return _oDataContext;
  }

  inline std::string getID() const
  {
    Lock l(_mutex);
    return _id;
  }

  inline SubscriptionStatus getStatus() const
  {
    Lock l(_mutex);
    return _status;
  }

  inline std::string getFilterParam() const
  {
    Lock l(_mutex);
    return _filterParam;
  }

  inline bool getStageOrder() const
  {
    Lock l(_mutex);
    return _stageOrder;
  }

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
        ACS_THROW(ODataSubscriptionResponsePayloadException("NotificationEndpoint not set"));
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
        ACS_THROW(ODataSubscriptionResponsePayloadException("NotificationEpUsername not set"));
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
        ACS_THROW(ODataSubscriptionResponsePayloadException("NotificationEpPassword not set"));
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
        ACS_THROW(ODataSubscriptionResponsePayloadException("Priority not set"));
      }
    }
  }

  inline void setODataContext(const std::string& newVal)
  {
    Lock l(_mutex);
    ACS_LOG_WARNING("Value of \"" << RESPONSE_PAYLOAD_KEY_ODATACONTEXT << "\" tag changed to \"" << newVal << "\"");
    _oDataContext = newVal;
  }

  inline void setID(const std::string& newVal)
  {
    Lock l(_mutex);
    _id = newVal;
  }

  inline void setStatus(const SubscriptionStatus newVal)
  {
    Lock l(_mutex);
    _status = newVal;
  }

  inline void setFilterParam(const std::string& newVal)
  {
    Lock l(_mutex);
    _filterParam = newVal;
  }

  inline void setStageOrder(const bool& newVal)
  {
    Lock l(_mutex);
    _stageOrder = newVal;
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

  std::string _oDataContext;
  std::string _id;
  SubscriptionStatus _status;
  std::string _filterParam;
  bool _stageOrder;
  bool _notificationEndpointSet;
  std::string _notificationEndpoint;
  bool _notificationEpUsernameSet;
  std::string _notificationEpUsername;
  bool _notificationEpPasswordSet;
  std::string _notificationEpPassword;
  bool _prioritySet;
  int64_t _priority;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(ODataSubscriptionResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _ODataSubscriptionResponsePayload_h_ */
