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

#ifndef _ODataQueryProductsResponsePayload_h_
#define _ODataQueryProductsResponsePayload_h_

#include <acs_c++config.hpp>
#include <unistd.h>
#include <string>

#include <ThreadSafe.h>
#include <json.hpp>
#include <Filterables.h>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTA Service in OData format */
class ODataQueryProductsResponsePayload
{
public:
  /** This object implements the batches_details list. */
  class QueryProductsValueList
  {
  public:
    /** This object implements a batches_detail entry */
    class QueryProductsValueElement
    {
    public:
      exDECLARE_EXCEPTION(QueryProductsValueElementException, exException);
      static const std::string RESPONSE_PAYLOAD_KEY_ID;
      static const std::string RESPONSE_PAYLOAD_KEY_NAME;
      static const std::string RESPONSE_PAYLOAD_KEY_ONLINE;

    public:
      /** Ctors/Dtors */
      QueryProductsValueElement();
      explicit QueryProductsValueElement(const std::string& jsstr);
      explicit QueryProductsValueElement(const nlohmann::json& json);
      QueryProductsValueElement(const QueryProductsValueElement& c);
      virtual ~QueryProductsValueElement() ACS_NOEXCEPT;

      /** Operators overload */
      QueryProductsValueElement& operator=(const QueryProductsValueElement& c);

      /** Returns the serialized json representation of this object */
      std::string toJSON() const;
      nlohmann::json toJSONObject() const;

      /** Initialize this object from the json string in input */
      void fromJSONObject(const nlohmann::json& json);
      void fromJSON(const std::string& jsstr);

      inline std::string getID() const
      {
        Lock l(_mutex);
        return _id;
      }

      inline std::string getName() const
      {
        Lock l(_mutex);
        return _name;
      }

      inline bool getOnline() const
      {
        Lock l(_mutex);
        return _online;
      }

      inline void setID(const std::string& newVal)
      {
        Lock l(_mutex);
        _id = newVal;
      }

      inline void setName(const std::string& newVal)
      {
        Lock l(_mutex);
        _name = newVal;
      }

      inline void setOnline(const bool newVal)
      {
        Lock l(_mutex);
        _online = newVal;
      }

    private:
      Mutex _mutex;

      std::string _id;
      std::string _name;
      bool _online;
      //TODO: Aggiungere gli altri campi di "@odata.context": "$metadata#Products/$entity"

      ACS_CLASS_DECLARE_DEBUG_LEVEL(QueryProductsValueElement);
    };

  public:
    exDECLARE_EXCEPTION(QueryProductsValueListException, exException);

  public:
    /** Ctors / Dtors */
    QueryProductsValueList();
    explicit QueryProductsValueList(const std::string& jsstr);
    explicit QueryProductsValueList(const nlohmann::json& js);
    QueryProductsValueList(const QueryProductsValueList&);
    virtual ~QueryProductsValueList() ACS_NOEXCEPT;

    /** Access operators overload */
    const QueryProductsValueElement& operator[](size_t idx) const;
    QueryProductsValueElement& operator[](size_t idx);

    /** Returns the value vector. */
    inline const std::vector<QueryProductsValueElement>& getList() const
    {
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
      Lock l(_mutex);
      return _value;
    }

    /** Set a new value vector. */
    inline void setList(const std::vector<QueryProductsValueElement>& newVal)
    {
      ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
      Lock l(_mutex);
      _value = newVal;
    }

    /** Inserts the payload in the list */
    void add(const QueryProductsValueElement& payload);
    void add(const QueryProductsValueList& payload);
    void add(const std::vector<QueryProductsValueElement>& payload);

    /** In-place sort function. Sorts the payloads with respect to their batch_id */
    void sort();
    /** Out-of-place sort function. Sorts the payloads with respect to their batch_id */
    void sort(std::vector<QueryProductsValueElement>&) const;

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
    std::vector<QueryProductsValueElement> _value;

    ACS_CLASS_DECLARE_DEBUG_LEVEL(QueryProductsValueList);
  };

public:
  exDECLARE_EXCEPTION(ODataQueryProductsResponsePayloadException, exException);
  static const std::string RESPONSE_PAYLOAD_KEY_ODATACONTEXT;
  static const std::string RESPONSE_PAYLOAD_KEY_VALUE;
  static const std::string RESPONSE_PAYLOAD_DEFAULT_ODATACONTEXT;

public:
  /** Ctors/Dtors */
  ODataQueryProductsResponsePayload();
  explicit ODataQueryProductsResponsePayload(const std::string& jsstr);
  explicit ODataQueryProductsResponsePayload(const nlohmann::json& json);
  ODataQueryProductsResponsePayload(const ODataQueryProductsResponsePayload&);
  virtual ~ODataQueryProductsResponsePayload() ACS_NOEXCEPT;

  /** Operators overload */
  ODataQueryProductsResponsePayload& operator=(const ODataQueryProductsResponsePayload&);

  inline std::string getODataContext() const
  {
    Lock l(_mutex);
    return _oDataContext;
  }

  inline QueryProductsValueList getValue() const
  {
    Lock l(_mutex);
    return _value;
  }

  const QueryProductsValueList& value = _value;

  inline void setODataContext(const std::string& newVal)
  {
    Lock l(_mutex);
    ACS_LOG_WARNING("Value of \"" << RESPONSE_PAYLOAD_KEY_ODATACONTEXT << "\" tag changed to \"" << newVal << "\"");
    _oDataContext = newVal;
  }

  inline void setValue(const QueryProductsValueList& newVal)
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
  QueryProductsValueList _value;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(ODataQueryProductsResponsePayload);
};

_ACS_END_NAMESPACE

#endif /* _ODataQueryProductsResponsePayload_h_ */
