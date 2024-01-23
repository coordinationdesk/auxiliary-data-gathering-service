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

#ifndef _LTARestClient_h_
#define _LTARestClient_h_

#include <acs_c++config.hpp>

#include <StopController.h>
#include <RemoteSite.h>
#include <dbSmartPersistent.h>

#include <GenericAuthHandler.h>

_ACS_BEGIN_NAMESPACE(acs)

// Forward declarations
class ODataQueryProductsResponsePayload;
class ODataOrderRequestPayload;
class ODataOrderResponsePayload;
class ODataBulkCreateRequestPayload;
class ODataBulkCreateResponsePayload;
class ODataBatchOrderResponsePayload;
class ODataBatchTriggeringRequestPayload;
class ProductOrderResponsePayload;
class ODataSubscriptionRequestPayload;
class ODataSubscriptionResponsePayload;

class dbConnectionPool;

/** This object implements the RESTful interfaces with the Remote LTA */
class LTARestClient
{
public:
  class LTADLRemoteLTA : public dbSmartPersistent
  {
  public:
    class AttributesWrapper
    {
    public:
      exDECLARE_EXCEPTION(AttributesWrapperException, exException);
      static const std::string KEY_AUTHENTICATIONTYPE;
      static const std::string KEY_TOKENURL;
      static const std::string KEY_CLIENTID;
      static const std::string KEY_CLIENTSECRET;

      enum AuthenticationTypes
      {
        BASIC,
        OAUTH2,
        INVALID
      };
      static const std::map<AuthenticationTypes, std::string> AUTHENTICATIONTYPESNAMES;
      static const std::map<std::string, AuthenticationTypes> AUTHENTICATIONTYPESVALUES;

    public:
      /** Ctors/Dtors */
      AttributesWrapper();
      explicit AttributesWrapper(const std::string& jsstr);
      AttributesWrapper(const AttributesWrapper& c);
      virtual ~AttributesWrapper() ACS_NOEXCEPT;

      /** Operators overload */
      AttributesWrapper& operator=(const AttributesWrapper& c);

      /** Returns the serialized json representation of this object */
      std::string toJSON() const;

      /** Initialize this object from the json string in input */
      void fromJSON(const std::string& jsstr);

      inline AuthenticationTypes getAuthenticationType() const
      {
        return _authenticationType;
      }

      std::string getTokenURL(bool* set = nullptr) const;
      std::string getClientID(bool* set = nullptr) const;
      std::string getClientSecret(bool* set = nullptr) const;

      inline void setAuthenticationType(const AuthenticationTypes newVal)
      {
        _authenticationType = newVal;
      }

      inline void setTokenURL(const std::string& newVal)
      {
        _tokenURL = newVal;
        _tokenURLSet = true;
      }

      inline void setClientID(const std::string& newVal)
      {
        _clientID = newVal;
        _clientIDSet = true;
      }

      inline void setClientSecret(const std::string& newVal)
      {
        _clientSecret = newVal;
        _clientSecretSet = true;
      }

    private:
      AuthenticationTypes _authenticationType;
      bool _tokenURLSet;
      std::string _tokenURL;
      bool _clientIDSet;
      std::string _clientID;
      bool _clientSecretSet;
      std::string _clientSecret;

      ACS_CLASS_DECLARE_DEBUG_LEVEL(AttributesWrapper);
    };

  public:
    static std::string table()
    {
      return "t_ltadl_remote_lta";
    }

  public:
    /** Defaulted special functions */
    virtual ~LTADLRemoteLTA() noexcept = default;
    LTADLRemoteLTA(const LTADLRemoteLTA&) = default;
    LTADLRemoteLTA& operator=(const LTADLRemoteLTA&) = default;
    LTADLRemoteLTA(LTADLRemoteLTA&&) = default;
    LTADLRemoteLTA& operator=(LTADLRemoteLTA&&) = default;

    /** Deleted special functions */
    LTADLRemoteLTA() = delete;

    /** Constructor */
    explicit LTADLRemoteLTA(dbConnection&);

    /** load by unique keys */
    bool loadByName(const std::string&, dbConnection&);

    /** remove by unique keys */
    bool removeByName(const std::string&, dbConnection&);

    /** Setters */
    void setBaseURL(const std::string&, dbConnection&);
    void unsetBaseURL(dbConnection&);
    void setUsername(const std::string&, dbConnection&);
    void unsetUsername(dbConnection&);
    void setPassword(const std::string&, dbConnection&);
    void unsetPassword(dbConnection&);
    void setName(const std::string&, dbConnection&);
    void unsetName(dbConnection&);
    void setAttributes(const std::string&, dbConnection&);
    void setAttributesWrapper(const AttributesWrapper&, dbConnection&);
    void unsetAttributes(dbConnection&);
    void setAllowOrdersCreation(const bool, dbConnection&);
    void unsetAllowOrdersCreation(dbConnection&);

    /** Getters */
    std::string getBaseURL(dbConnection&, bool* set = nullptr) const;
    std::string getUsername(dbConnection&, bool* set = nullptr) const;
    std::string getPassword(dbConnection&, bool* set = nullptr) const;
    std::string getName(dbConnection&, bool* set = nullptr) const;
    std::string getAttributes(dbConnection&, bool* set = nullptr) const;
    AttributesWrapper getAttributesWrapper(dbConnection&, bool* set = nullptr) const;
    bool getAllowOrdersCreation(dbConnection&, bool* set = nullptr) const;

  private:
    ACS_CLASS_DECLARE_DEBUG_LEVEL(LTADLRemoteLTA);
  };

public:
  exDECLARE_EXCEPTION(LTARestClientException, exException);

  enum httpMethods
  {
    Post,
    Get,
    Put,
    Delete,
    Patch
  };

  static std::unique_ptr<GenericAuthHandler> createAuth(dbConnection& conn, const LTADLRemoteLTA& lta,
                                                        const StopController& stopController = AppStopController());

  static const std::string sendRequest(const httpMethods httpMethod, const std::string& uri,
                                       const GenericAuthHandler* const auth = nullptr,
                                       const std::string* const contentType = nullptr,
                                       const std::string* const payload = nullptr, long* const responseCode = nullptr,
                                       const size_t* connectionTimeout = nullptr,
                                       const bool* adjustResponseTimeout = nullptr);

  static const std::string sendRequest(const httpMethods httpMethod, dbConnectionPool& pool,
                                       const dbPersistent::IdType ltaID, const std::string& endpoint,
                                       const std::string* const contentType = nullptr,
                                       const std::string* const payload = nullptr, long* const responseCode = nullptr,
                                       const size_t* connectionTimeout = nullptr,
                                       const bool* adjustResponseTimeout = nullptr);

  static void sendCirculationRequest(const curl::RemoteSite& destination, const std::string& uri,
                                     const GenericAuthHandler* const auth = nullptr,
                                     long* const sourceResponseCode = nullptr,
                                     long* const destinationResponseCode = nullptr,
                                     const StopController& stopController = AppStopController(),
                                     const bool removeTarget = false);

  static void sendCirculationRequest(const curl::RemoteSite& destination, dbConnectionPool& pool,
                                     const dbPersistent::IdType ltaID,
                                     const std::string& endpoint, long* const sourceResponseCode = nullptr,
                                     long* const destinationResponseCode = nullptr,
                                     const StopController& stopController = AppStopController(),
                                     const bool removeTarget = false);

private:
  static std::string composeURI(const std::string& baseURL, const std::string& endpoint);

public:
  /** Ctors/Dtors */
  // Get data from t_ltadl_remote_lta DB table
  LTARestClient(dbConnectionPool& pool, const dbPersistent::IdType ltaID,
                const StopController& stopController = AppStopController());
  LTARestClient(dbConnectionPool& pool, const LTADLRemoteLTA& lta,
                const StopController& stopController = AppStopController());
  LTARestClient(dbConnectionPool& pool, const std::string& ltaName,
                const StopController& stopController = AppStopController());
  // Set fixed data (it may be inconsistent with DB data)
  LTARestClient(const std::string& baseURL, const GenericAuthHandler& auth, dbPersistent::IdType ltaID = 0,
                const std::string& ltaName = "CustomLTA", const bool allowOrdersCreation = true,
                const StopController& stopController = AppStopController());
  // Copy ctor
  LTARestClient(const LTARestClient& c);
  virtual ~LTARestClient() ACS_NOEXCEPT;

  /** Requests **/
  // Ask if have a Product
  ODataQueryProductsResponsePayload queryProduct(const std::string& request, long* const responseCode = nullptr) const;
  // Request a Product
  ODataOrderResponsePayload orderRequest(const std::string& productID, const ODataOrderRequestPayload& request,
                                         long* const responseCode = nullptr) const;
  // Create a new Bulk
  ODataBulkCreateResponsePayload bulkCreate(const ODataBulkCreateRequestPayload& request,
                                            long* const responseCode = nullptr) const;
  // Get the Batches in a Bulk
  ODataBatchOrderResponsePayload batchOrder(const std::string& bulkID, long* const responseCode = nullptr) const;
  // Cancel a Bulk order
  void cancelBulkOrder(const std::string& bulkID, long* const responseCode = nullptr) const;
  // Trigger a Batch
  void triggerBatch(const std::string& batchUUID, const ODataBatchTriggeringRequestPayload& request,
                    long* const responseCode = nullptr) const;
  // Get the status of a Batch
  ODataBatchOrderResponsePayload batchStatus(const std::string& batchUUID, long* const responseCode = nullptr) const;
  // Get the Products in a Batch
  ODataQueryProductsResponsePayload batchProducts(const std::string& batchUUID,
                                                  long* const responseCode = nullptr) const;
  // Circulate a Product Metadata
  void circulateProductMetadata(const curl::RemoteSite& destination, const std::string& productUUID,
                                long* const sourceResponseCode = nullptr,
                                long* const destinationResponseCode = nullptr, const bool removeTarget=false) const;
  // Circulate a Product
  void circulateProduct(const curl::RemoteSite& destination, const std::string& productUUID,
                        long* const sourceResponseCode = nullptr, long* const destinationResponseCode = nullptr,
                        const bool removeTarget = false) const;
  // Get the status of a Order
  ProductOrderResponsePayload orderStatus(const std::string& orderUUID,
                                          long* const responseCode = nullptr) const;
  // Get the status of a Order in a Batch
  ODataQueryProductsResponsePayload batchOrdersStatus(const std::string& batchUUID, const std::string& orderUUID,
                                                      long* const responseCode = nullptr) const;
  // Create a new Subscription
  ODataSubscriptionResponsePayload subscription(const ODataSubscriptionRequestPayload& request,
                                                long* const responseCode = nullptr) const;
  // Get a Subscription status
  ODataSubscriptionResponsePayload subscriptionStatus(const std::string& subscriptionUUID,
                                                      long* const responseCode = nullptr) const;
  // Cancel a subscription
  void cancelSubscription(const std::string& subscriptionUUID, long* const responseCode = nullptr) const;

  /** Getters **/
  std::string getBaseURL() const
  {
    return _baseURL;
  }

  std::unique_ptr<GenericAuthHandler> getAuth() const
  {
    return _auth->clone();
  }

  dbPersistent::IdType getLTAID() const
  {
    return _ltaID;
  }

  std::string getLTAName() const
  {
    return _ltaName;
  }

  bool getLTAAllowOrdersCreation() const
  {
    return _ltaAllowOrdersCreation;
  }

  size_t getConnectionTimeout_ms(bool* set = nullptr, bool* adjustResponseTimeout = nullptr) const;
  // Set the connection timeout for the HTTP Curl requests. Not used for Circulation Requests
  void setConnectionTimeout_ms(const size_t ms, const bool adjustResponseTimeout = true);
  void unsetConnectionTimeout_ms();

private:
  /** Deleted special functions */
  LTARestClient() = delete;

  void saveDataFromDB(dbConnection& conn, const LTADLRemoteLTA& lta);
  const std::string sendSpecificRequest(const httpMethods httpMethod, const std::string& uri,
                                        const GenericAuthHandler* const auth = nullptr,
                                        const std::string* const contentType = nullptr,
                                        const std::string* const payload = nullptr, long* const responseCode = nullptr)
    const;

  std::string _baseURL;
  std::unique_ptr<GenericAuthHandler> _auth;
  dbPersistent::IdType _ltaID;
  std::string _ltaName;
  bool _ltaAllowOrdersCreation;
  const StopController& _stopController;

  bool _connectionTimeoutSet;
  size_t _connectionTimeout;
  bool _adjustResponseTimeout;

  ACS_CLASS_DECLARE_DEBUG_LEVEL(LTARestClient);
};

_ACS_END_NAMESPACE

#endif /* _LTARestClient_h_ */
