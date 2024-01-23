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

#ifndef _ODataBulkCreateResponsePayload_h_
#define _ODataBulkCreateResponsePayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>
#include <map>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTA Service in OData format */
class ODataBulkCreateResponsePayload
{
public:
  /** This object implements the batches_details list. */
  class BulkCreateValueList
  {
  public:
    /** This object implements a batches_detail entry */
    class BulkCreateValueElement
    {
    public:
      exDECLARE_EXCEPTION(BulkCreateValueElementException, exException);
      static const std::string RESPONSE_PAYLOAD_KEY_ID;
      static const std::string RESPONSE_PAYLOAD_KEY_STATUS;

      enum Status
      {
        CREATED,
        INPROGRESS,
        COMPLETED,
        FAILED,
        CANCELLED,
        INVALIDSTATUS
      };
      static const std::map<Status, std::string> STATUSTOSTRING;
      static const std::map<std::string, Status> STRINGTOSTATUS;

    public:
      /** Ctors/Dtors */
      BulkCreateValueElement();
      explicit BulkCreateValueElement(const std::string& jsstr);
      explicit BulkCreateValueElement(const nlohmann::json& json);
      BulkCreateValueElement(const BulkCreateValueElement&);
      virtual ~BulkCreateValueElement() ACS_NOEXCEPT;

      /** Operators overload */
      BulkCreateValueElement& operator=(const BulkCreateValueElement&);

      inline std::string getID() const
      {
        Lock l(_mutex);
        return _id;
      }

      inline Status getStatus() const
      {
        Lock l(_mutex);
        return _status;
      }

      inline void setID(const std::string& newVal)
      {
        Lock l(_mutex);
        _id = newVal;
      }

      inline void setStatus(const Status newVal)
      {
        Lock l(_mutex);
        _status = newVal;
      }

      /** Returns the serialized json representation of this object */
      std::string toJSON() const;
      nlohmann::json toJSONObject() const;

      /** Initialize this object from the json string in input */
      void fromJSONObject(const nlohmann::json& json);
      void fromJSON(const std::string& json);

    private:
      Mutex _mutex;
      std::string _id;
      Status _status;
      //TODO: Aggiungere gli altri campi di "@odata.context": "$metadata#OData.CSC.Bulk""

      ACS_CLASS_DECLARE_DEBUG_LEVEL(BulkCreateValueElement);
    };

  public:
    exDECLARE_EXCEPTION(BulkCreateValueListException, exException);

  public:
    /** Ctors / Dtors */
    BulkCreateValueList();
    explicit BulkCreateValueList(const std::string& jsstr);
    explicit BulkCreateValueList(const nlohmann::json& js);
    BulkCreateValueList(const BulkCreateValueList&);
    virtual ~BulkCreateValueList() ACS_NOEXCEPT;

    /** Access operators overload */
    const BulkCreateValueElement& operator[](size_t idx) const;
    BulkCreateValueElement& operator[](size_t idx);

    /** Returns the value vector. */
    inline const std::vector<BulkCreateValueElement>& getList() const
    {
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
      Lock l(_mutex);
      return _value;
    }

    /** Set a new value vector. */
    inline void setList(const std::vector<BulkCreateValueElement>& newVal)
    {
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
      Lock l(_mutex);
      _value = newVal;
    }

    /** Inserts the payload in the list */
    void add(const BulkCreateValueElement& payload);
    void add(const BulkCreateValueList& payload);
    void add(const std::vector<BulkCreateValueElement>& payload);

    /** In-place sort function. Sorts the payloads with respect to their batch_id */
    void sort();
    /** Out-of-place sort function. Sorts the payloads with respect to their batch_id */
    void sort(std::vector<BulkCreateValueElement>&) const;

    /** Clears the payload list */
    void clear();

    /** Returns the size of the list */
    size_t size() const;

    /** Returns the serialized json representation of this object */
    std::string toJSON() const;
    /** Returns the nlohmann object associated to this list */
    nlohmann::json toJSONObject() const;

    /** Initialize this object from the nlohmann json object in input */
    void fromJSONObject(const nlohmann::json& json);
    /** Initialize this object from the json string in input */
    void fromJSON(const std::string& json);

  private:
    Mutex _mutex;
    std::vector<BulkCreateValueElement> _value;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(BulkCreateValueList);
  };

public:
  exDECLARE_EXCEPTION(ODataBulkCreateResponsePayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_ODATACONTEXT;
  static const std::string RESPONSE_PAYLOAD_KEY_VALUE;

  static const std::string RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT;

public:
  /** Ctors/Dtors */
  ODataBulkCreateResponsePayload();
  explicit ODataBulkCreateResponsePayload(const std::string& jsstr);
  explicit ODataBulkCreateResponsePayload(const nlohmann::json& json);
  ODataBulkCreateResponsePayload(const ODataBulkCreateResponsePayload&);
  virtual ~ODataBulkCreateResponsePayload() ACS_NOEXCEPT;

  /** Operators overload */
  ODataBulkCreateResponsePayload& operator=(const ODataBulkCreateResponsePayload&);

  inline std::string getODataContext() const
  {
    Lock l(_mutex);
    return _oDataContext;
  }

  inline BulkCreateValueList getValue() const
  {
    Lock l(_mutex);
    return _value;
  }

  const BulkCreateValueList& value = _value;

  inline void setODataContext(const std::string& newVal)
  {
    Lock l(_mutex);
    ACS_LOG_WARNING("Value of \"" << RESPONSE_PAYLOAD_KEY_ODATACONTEXT << "\" tag changed to \"" << newVal << "\"");
    _oDataContext = newVal;
  }

  inline void setValue(const BulkCreateValueList& newVal)
  {
    Lock l(_mutex);
    _value = newVal;
  }

  /** Returns the serialized json representation of this object */
  std::string toJSON() const;
  nlohmann::json toJSONObject() const;

  /** Initialize this object from the json string in input */
  void fromJSONObject(const nlohmann::json& json);
  void fromJSON(const std::string& json);

private:
  Mutex _mutex;
  std::string _oDataContext;
  BulkCreateValueList _value;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(ODataBulkCreateResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _ODataBulkCreateResponsePayload_h_ */
