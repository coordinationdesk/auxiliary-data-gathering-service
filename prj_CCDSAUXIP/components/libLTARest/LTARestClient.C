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

#include <LTARestClient.h>

#include <FtpCurl.h>
#include <CirculationCurl.h>
#include <RemoteSiteFactory.h>
#include <StringUtils.h>
#include <dbConnectionPool.h>

// Specific AuthHandler
#include <BaseAuthHandler.h>
#include <OAuth2Handler.h>

// Specific wrappers forward declared
#include <ODataQueryProductsResponsePayload.h>
#include <ODataOrderRequestPayload.h>
#include <ODataOrderResponsePayload.h>
#include <ODataBulkCreateRequestPayload.h>
#include <ODataBulkCreateResponsePayload.h>
#include <ODataBatchOrderResponsePayload.h>
#include <ODataBatchTriggeringRequestPayload.h>
#include <ProductOrderResponsePayload.h>
#include <ODataSubscriptionRequestPayload.h>
#include <ODataSubscriptionResponsePayload.h>

_ACS_BEGIN_NAMESPACE(acs)

/*********************
 * AttributesWrapper *
 *********************/

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTARestClient::LTADLRemoteLTA::AttributesWrapper);

const std::string LTARestClient::LTADLRemoteLTA::AttributesWrapper::KEY_AUTHENTICATIONTYPE = "authentication_type";
const std::string LTARestClient::LTADLRemoteLTA::AttributesWrapper::KEY_TOKENURL = "token_url";
const std::string LTARestClient::LTADLRemoteLTA::AttributesWrapper::KEY_CLIENTID = "client_id";
const std::string LTARestClient::LTADLRemoteLTA::AttributesWrapper::KEY_CLIENTSECRET = "client_secret";

const std::map<LTARestClient::LTADLRemoteLTA::AttributesWrapper::AuthenticationTypes, std::string>
  LTARestClient::LTADLRemoteLTA::AttributesWrapper::AUTHENTICATIONTYPESNAMES = {
    { BASIC, "BASIC" },
    { OAUTH2, "OAUTH2" }
  };

const std::map<std::string, LTARestClient::LTADLRemoteLTA::AttributesWrapper::AuthenticationTypes>
  LTARestClient::LTADLRemoteLTA::AttributesWrapper::AUTHENTICATIONTYPESVALUES = {
    { "BASIC", BASIC },
    { "OAUTH2", OAUTH2 }
  };

LTARestClient::LTADLRemoteLTA::AttributesWrapper::AttributesWrapper() :
  _authenticationType(INVALID),
  _tokenURLSet(false),
  _tokenURL(),
  _clientIDSet(false),
  _clientID(),
  _clientSecretSet(false),
  _clientSecret()
{
}

LTARestClient::LTADLRemoteLTA::AttributesWrapper::AttributesWrapper(const std::string& jsStr) :
  _authenticationType(INVALID),
  _tokenURLSet(false),
  _tokenURL(),
  _clientIDSet(false),
  _clientID(),
  _clientSecretSet(false),
  _clientSecret()
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  fromJSON(jsStr);
}

LTARestClient::LTADLRemoteLTA::AttributesWrapper::AttributesWrapper(
  const LTARestClient::LTADLRemoteLTA::AttributesWrapper& other) :
  _authenticationType(other._authenticationType),
  _tokenURLSet(other._tokenURLSet),
  _tokenURL(other._tokenURL),
  _clientIDSet(other._clientIDSet),
  _clientID(other._clientID),
  _clientSecretSet(other._clientSecretSet),
  _clientSecret(other._clientSecret)
{
}

LTARestClient::LTADLRemoteLTA::AttributesWrapper::~AttributesWrapper() ACS_NOEXCEPT
{
}

LTARestClient::LTADLRemoteLTA::AttributesWrapper& LTARestClient::LTADLRemoteLTA::AttributesWrapper::operator=(
  const LTARestClient::LTADLRemoteLTA::AttributesWrapper& other)
{
  if (this != &other)
  {
    _authenticationType = other._authenticationType;
    _tokenURLSet = other._tokenURLSet;
    _tokenURL = other._tokenURL;
    _clientIDSet = other._clientIDSet;
    _clientID = other._clientID;
    _clientSecretSet = other._clientSecretSet;
    _clientSecret = other._clientSecret;
  }
  return *this;
}

void LTARestClient::LTADLRemoteLTA::AttributesWrapper::fromJSON(const std::string& jsStr)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  try
  {
    const nlohmann::json jsObj = nlohmann::json::parse(jsStr);

    if (jsObj.find(KEY_AUTHENTICATIONTYPE) == jsObj.end() || jsObj[KEY_AUTHENTICATIONTYPE].is_null())
    {
      ACS_THROW(LTARestClient::LTADLRemoteLTA::AttributesWrapper::AttributesWrapperException()
                << "Cannot find key \"" << KEY_AUTHENTICATIONTYPE
                << "\" into json response payload or the associated value is null. Key is mandatory");
    }
    std::map<std::string, AuthenticationTypes>::const_iterator it =
      AUTHENTICATIONTYPESVALUES.find(StringUtils::uppercase(jsObj[KEY_AUTHENTICATIONTYPE]));
    if (it != AUTHENTICATIONTYPESVALUES.end())
    {
      _authenticationType = it->second;
    }
    else
    {
      _authenticationType = INVALID;
      ACS_THROW(LTARestClient::LTADLRemoteLTA::AttributesWrapper::AttributesWrapperException()
                << "Invalid value for \"" << KEY_AUTHENTICATIONTYPE << "\" key into json response payload");
    }

    if (OAUTH2 == _authenticationType)
    {
      if (jsObj.find(KEY_TOKENURL) == jsObj.end() || jsObj[KEY_TOKENURL].is_null())
      {
        _tokenURLSet = false;

        ACS_THROW(LTARestClient::LTADLRemoteLTA::AttributesWrapper::AttributesWrapperException()
                  << "Cannot find key \"" << KEY_TOKENURL
                  << "\" into json response payload or the associated value is null. Key is mandatory");
      }
      _tokenURL = jsObj[KEY_TOKENURL];
      _tokenURLSet = true;
      if (_tokenURL.empty())
      {
        _tokenURLSet = false;

        ACS_THROW(LTARestClient::LTADLRemoteLTA::AttributesWrapper::AttributesWrapperException()
                  << "Empty value for key \"" << KEY_TOKENURL << "\" into json response payload. Value is mandatory");
      }
    }
    else
    {
      if (jsObj.find(KEY_TOKENURL) != jsObj.end() && !jsObj[KEY_TOKENURL].is_null())
      {
        _tokenURL = jsObj[KEY_TOKENURL];
        _tokenURLSet = true;
      }
      else
      {
        _tokenURL = "";
        _tokenURLSet = false;
      }
    }

    if (jsObj.find(KEY_CLIENTID) != jsObj.end() && !jsObj[KEY_CLIENTID].is_null())
    {
      _clientID = jsObj[KEY_CLIENTID];
      _clientIDSet = true;
    }
    else
    {
      _clientID = "";
      _clientIDSet = false;
    }

    if (jsObj.find(KEY_CLIENTSECRET) != jsObj.end() && !jsObj[KEY_CLIENTSECRET].is_null())
    {
      _clientSecret = jsObj[KEY_CLIENTSECRET];
      _clientSecretSet = true;
    }
    else
    {
      _clientSecret = "";
      _clientSecretSet = false;
    }
  }
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(LTARestClient::LTADLRemoteLTA::AttributesWrapper::AttributesWrapperException(e, "Error reading JSON."));
  }
}

std::string LTARestClient::LTADLRemoteLTA::AttributesWrapper::toJSON() const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string jsStr;
  try
  {
    nlohmann::json jsObj;

    if (INVALID != _authenticationType)
    {
      std::string AuthenticationTypesName = AUTHENTICATIONTYPESNAMES.at(_authenticationType);
      jsObj[KEY_AUTHENTICATIONTYPE] = AuthenticationTypesName;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << AuthenticationTypesName << "\" for \""
                                                             << KEY_AUTHENTICATIONTYPE << "\" key");
    }
    else
    {
      ACS_THROW(LTARestClient::LTADLRemoteLTA::AttributesWrapper::AttributesWrapperException()
                << "Invalid value for \"" << KEY_AUTHENTICATIONTYPE << "\" key");
    }

    if (_tokenURLSet)
    {
      jsObj[KEY_TOKENURL] = _tokenURL;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _tokenURL << "\" for \"" << KEY_TOKENURL << "\" key");
    }
    else if (OAUTH2 == _authenticationType)
    {
      ACS_THROW(LTARestClient::LTADLRemoteLTA::AttributesWrapper::AttributesWrapperException()
                << "Invalid value for \"" << KEY_TOKENURL << "\" key");
    }

    if (_clientIDSet)
    {
      jsObj[KEY_CLIENTID] = _clientID;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _clientID << "\" for \"" << KEY_CLIENTID << "\" key");
    }

    if (_clientSecretSet)
    {
      jsObj[KEY_CLIENTSECRET] = _clientSecret;
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting value \"" << _clientSecret << "\" for \"" << KEY_CLIENTSECRET
                                                             << "\" key");
    }

    jsStr = jsObj.dump();
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Request data transformed data into the following json string: \"\n"
                                          << jsStr << "\"");
  }
  // LCOV_EXCL_START
  // Defensive coding: the data validity is guaranteed by the object
  catch (nlohmann::json::exception& e)
  {
    ACS_THROW(LTARestClient::LTADLRemoteLTA::AttributesWrapper::AttributesWrapperException(e, "Error creating JSON."));
  }
  // LCOV_EXCL_STOP
  return jsStr;
}

std::string LTARestClient::LTADLRemoteLTA::AttributesWrapper::getTokenURL(bool* set) const
{
  if (nullptr != set)
  {
    *set = _tokenURLSet;
  }
  else if (!_tokenURLSet)
  {
    ACS_THROW(AttributesWrapperException() << "Key " << KEY_TOKENURL << " not set");
  }
  return _tokenURL;
}

std::string LTARestClient::LTADLRemoteLTA::AttributesWrapper::getClientID(bool* set) const
{
  if (nullptr != set)
  {
    *set = _clientIDSet;
  }
  else if (!_clientIDSet)
  {
    ACS_THROW(AttributesWrapperException() << "Key " << KEY_CLIENTID << " not set");
  }
  return _clientID;
}

std::string LTARestClient::LTADLRemoteLTA::AttributesWrapper::getClientSecret(bool* set) const
{
  if (nullptr != set)
  {
    *set = _clientSecretSet;
  }
  else if (!_clientSecretSet)
  {
    ACS_THROW(AttributesWrapperException() << "Key " << KEY_CLIENTSECRET << " not set");
  }
  return _clientSecret;
}

/******************
 * LTADLRemoteLTA *
 ******************/

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTARestClient::LTADLRemoteLTA);

/*
 * Constructor
 */
LTARestClient::LTADLRemoteLTA::LTADLRemoteLTA(dbConnection& conn) :
  dbSmartPersistent(table(), conn)
{
}

bool LTARestClient::LTADLRemoteLTA::loadByName(const std::string& name, dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  return loadByUniqueKey<std::string>(conn, "name", name);
}

bool LTARestClient::LTADLRemoteLTA::removeByName(const std::string& name, dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  return removeByUniqueKey<std::string>(conn, "name", name);
}

/*
 * Setters
 */
void LTARestClient::LTADLRemoteLTA::setBaseURL(const std::string& val, dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  set<std::string>("lta_base_url", val, conn);
}

void LTARestClient::LTADLRemoteLTA::unsetBaseURL(dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  unset("lta_base_url", conn);
}

void LTARestClient::LTADLRemoteLTA::setUsername(const std::string& val, dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  set<std::string>("username", val, conn);
}

void LTARestClient::LTADLRemoteLTA::unsetUsername(dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  unset("username", conn);
}

void LTARestClient::LTADLRemoteLTA::setPassword(const std::string& val, dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  set<std::string>("passwd", val, conn);
}

void LTARestClient::LTADLRemoteLTA::unsetPassword(dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  unset("passwd", conn);
}

void LTARestClient::LTADLRemoteLTA::setName(const std::string& val, dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  set<std::string>("name", val, conn);
}

void LTARestClient::LTADLRemoteLTA::unsetName(dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  unset("name", conn);
}

void LTARestClient::LTADLRemoteLTA::setAttributes(const std::string& val, dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  set<std::string>("attributes", val, conn);
}

void LTARestClient::LTADLRemoteLTA::setAttributesWrapper(const AttributesWrapper& wrapper, dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  setAttributes(wrapper.toJSON(), conn);
}

void LTARestClient::LTADLRemoteLTA::unsetAttributes(dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  unset("attributes", conn);
}

void LTARestClient::LTADLRemoteLTA::setAllowOrdersCreation(const bool val, dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  set<bool>("allow_orders_creation", val, conn);
}

void LTARestClient::LTADLRemoteLTA::unsetAllowOrdersCreation(dbConnection& conn)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  unset("allow_orders_creation", conn);
}

std::string LTARestClient::LTADLRemoteLTA::getBaseURL(dbConnection& conn, bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string val;
  bool isNull = false;
  get<std::string>("lta_base_url", val, isNull, conn);
  if (nullptr != set)
  {
    *set = !isNull;
  }
  else
  {
    ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException()
                             << "Cannot return any value for null field \"lta_base_url\" in table " << table());
  }

  return val;
}

std::string LTARestClient::LTADLRemoteLTA::getUsername(dbConnection& conn, bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string val = "";
  bool isNull = false;
  get<std::string>("username", val, isNull, conn);
  if (nullptr != set)
  {
    *set = !isNull;
  }
  else
  {
    ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException()
                             << "Cannot return any value for null field \"username\" in table " << table());
  }

  return val;
}

std::string LTARestClient::LTADLRemoteLTA::getPassword(dbConnection& conn, bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string val = "";
  bool isNull = false;
  get<std::string>("passwd", val, isNull, conn);
  if (nullptr != set)
  {
    *set = !isNull;
  }
  else
  {
    ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException()
                             << "Cannot return any value for null field \"passwd\" in table " << table());
  }

  return val;
}

std::string LTARestClient::LTADLRemoteLTA::getName(dbConnection& conn, bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string val;
  bool isNull = false;
  get<std::string>("name", val, isNull, conn);
  if (nullptr != set)
  {
    *set = !isNull;
  }
  else
  {
    ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException()
                             << "Cannot return any value for null field \"name\" in table " << table());
  }

  return val;
}

std::string LTARestClient::LTADLRemoteLTA::getAttributes(dbConnection& conn, bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string val;
  bool isNull = false;
  get<std::string>("attributes", val, isNull, conn);
  if (nullptr != set)
  {
    *set = !isNull;
  }
  else
  {
    ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException()
                             << "Cannot return any value for null field \"attributes\" in table " << table());
  }

  return val;
}

LTARestClient::LTADLRemoteLTA::AttributesWrapper LTARestClient::LTADLRemoteLTA::getAttributesWrapper(dbConnection& conn,
                                                                                                     bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  std::string jsonStr = getAttributes(conn, set);
  if (*set)
  {
    return AttributesWrapper(jsonStr);
  }
  else
  {
    return AttributesWrapper();
  }
}

bool LTARestClient::LTADLRemoteLTA::getAllowOrdersCreation(dbConnection& conn, bool* set) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
  bool val = true;
  bool isNull = false;
  get<bool>("allow_orders_creation", val, isNull, conn);
  if (nullptr != set)
  {
    *set = !isNull;
  }
  else
  {
    ACS_COND_THROW(isNull, dbSmartPersistent::NullValueException()
                             << "Cannot return any value for null field \"allow_orders_creation\" in table " << table());
  }

  return val;
}

/******************
 * LTARestClient  *
 ******************/

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTARestClient);

/** Public Static Methods **/

std::unique_ptr<GenericAuthHandler> LTARestClient::createAuth(dbConnection& conn, const LTADLRemoteLTA& lta,
                                                              const StopController& stopController)
{
  std::unique_ptr<GenericAuthHandler> auth;

  bool attributesSet = false;
  LTADLRemoteLTA::AttributesWrapper attributes = lta.getAttributesWrapper(conn, &attributesSet);
  std::string userName = lta.getUsername(conn);
  std::string password = lta.getPassword(conn);

  if (attributesSet)
  {
    switch (attributes.getAuthenticationType())
    {
      case LTADLRemoteLTA::AttributesWrapper::BASIC: {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Basic auth set");

        auth = std::make_unique<BaseAuthHandler>(userName, password);

        break;
      }
      case LTADLRemoteLTA::AttributesWrapper::OAUTH2: {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "OAuth2 auth set");

        std::string tokenURL = attributes.getTokenURL();
        bool clientIDSet = false;
        std::string clientID = attributes.getClientID(&clientIDSet);
        bool clientSecretSet = false;
        std::string clientSecret = attributes.getClientSecret(&clientSecretSet);

        auth = std::make_unique<OAuth2Handler>(userName, password, tokenURL, clientIDSet, clientID, clientSecretSet,
                                               clientSecret, true, true, stopController);

        break;
      }
      // LCOV_EXCL_START
      // Defensive coding: the Auth Type should never be Invalid
      default: {
        ACS_THROW(LTARestClientException("Unknow authentication type"));
      }
        // LCOV_EXCL_STOP
    }
  }
  else
  {
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Attributs not set, going to use Basic auth");

    auth = std::make_unique<BaseAuthHandler>(userName, password);
  }

  return auth;
}

const std::string LTARestClient::sendRequest(const httpMethods httpMethod, const std::string& uri,
                                             const GenericAuthHandler* const auth, const std::string* const contentType,
                                             const std::string* const payload, long* const responseCode,
                                             const size_t* connectionTimeout, const bool* adjustResponseTimeout)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

  // Instantiate curl
  curl::BandPriority noPrio;
  FtpCurl curl(noPrio);
  curl::Curl::CurlHttpOpt opt;

  std::string response = "";

  try
  {
    // URI and Authentication
    {
      curl::RemoteSite site;
      if (nullptr != auth)
      {
        site = auth->initAuthenticatedEndpoint(uri, &opt);
      }
      else
      {
        site = curl::RemoteSiteFactory::makeSite(uri);
        opt.setUri(uri);
      }
      curl.setSite(site);
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Uri set to \"" << uri << "\"");
    }

    // Content Type
    if (nullptr != contentType)
    {
      opt.setContentType(*contentType);
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Content Type set to \"" << *contentType << "\"");
    }

    // Payload
    if (nullptr != connectionTimeout)
    {
      if (nullptr != adjustResponseTimeout)
      {
        opt.setTimeouts(*connectionTimeout, *adjustResponseTimeout);
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Connection Timeout set to " << *connectionTimeout
                                                                         << " with Adjust Response Timeout set to "
                                                                         << std::boolalpha << *adjustResponseTimeout);
      }
      else
      {
        opt.setTimeouts(*connectionTimeout);
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Connection Timeout set to " << *connectionTimeout
                                                                         << " with Adjust Response Timeout not set");
      }
    }

    // Connection Timeout
    if (nullptr != payload)
    {
      opt.setMessage(*payload);
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Payload set to \"" << *payload << "\"");
    }

    // Send request
    switch (httpMethod)
    {
      case Post: {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Sending POST request to \"" << curl.getSite().str() << "\"");

        response = curl.httpPost(opt);

        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Response to POST request \"" << curl.getSite().str() << "\" is: \""
                                                                          << response << "\"");

        break;
      }
      case Get: {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Sending GET request to \"" << curl.getSite().str() << "\"");

        response = curl.httpGet(opt);

        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Response to GET request \"" << curl.getSite().str() << "\" is: \""
                                                                         << response << "\"");

        break;
      }
      case Put: {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Sending PUT request to \"" << curl.getSite().str() << "\"");

        response = curl.httpPut(opt);

        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Response to PUT request \"" << curl.getSite().str() << "\" is: \""
                                                                         << response << "\"");

        break;
      }
      case Delete: {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Sending DELETE request to \"" << curl.getSite().str() << "\"");

        response = curl.httpDel(opt);

        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Response to DELETE request \"" << curl.getSite().str() << "\" is: \""
                                                                            << response << "\"");

        break;
      }
      case Patch: {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Sending PATCH request to \"" << curl.getSite().str() << "\"");

        response = curl.httpPatch(opt);

        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Response to PATCH request \"" << curl.getSite().str() << "\" is: \""
                                                                           << response << "\"");

        break;
      }
    }

    // Save the response code
    if (nullptr != responseCode)
    {
      *responseCode = curl.getHttpResponseCode();
      ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Response code set to: " << *responseCode);
    }
  }
  catch (const std::exception& e)
  {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, e);

    if (nullptr != responseCode)
    {
      *responseCode = curl.getHttpResponseCode();
      ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Response code set to: " << *responseCode);
    }

    throw;
  }

  return response;
}

const std::string LTARestClient::sendRequest(const httpMethods httpMethod, dbConnectionPool& pool,
                                             const dbPersistent::IdType ltaID, const std::string& endpoint,
                                             const std::string* const contentType, const std::string* const payload,
                                             long* const responseCode, const size_t* connectionTimeout,
                                             const bool* adjustResponseTimeout)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

  std::string uri;
  std::unique_ptr<GenericAuthHandler> auth;
  {
    // Get DB connection
    dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;

    // Get the LTA Remote data from DB
    LTADLRemoteLTA lta(conn);
    lta.load(conn, ltaID);

    // Get the URI
    uri = composeURI(lta.getBaseURL(conn), endpoint);

    // Get the Authentication Parameters
    auth = createAuth(conn, lta);
  }

  // Send the request
  return sendRequest(httpMethod, uri, auth.get(), contentType, payload, responseCode, connectionTimeout,
                     adjustResponseTimeout);
}

// LCOV_EXCL_START
// TODO: Circulation request cannot be tested
void LTARestClient::sendCirculationRequest(const curl::RemoteSite& destination, const std::string& uri,
                                           const GenericAuthHandler* const auth, long* const sourceResponseCode,
                                           long* const destinationResponseCode, const StopController& stopController,
                                           const bool removeTarget)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

  ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "URI set to \"" << uri << "\"");

  // URI + Authentication
  curl::RemoteSite source;
  if (nullptr != auth)
  {
    source = auth->initAuthenticatedEndpoint(uri);
  }
  else
  {
    source = curl::RemoteSiteFactory::makeSite(uri);
  }

  // Instantiate curl
  curl::CirculationCurl curl(source, destination, stopController);
  curl.setRemoveTargetBeforeTransfer(removeTarget);
  try
  {
    // Send request
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Sending Circulation request");
    curl();

    // Save the source response code
    if (nullptr != sourceResponseCode)
    {
      *sourceResponseCode = curl.getSourceHttpResponseCode();
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Source response code set to: " << *sourceResponseCode);
    }

    // Save the destination response code
    if (nullptr != destinationResponseCode)
    {
      *destinationResponseCode = curl.getTargetHttpResponseCode();
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destination response code set to: " << *destinationResponseCode);
    }
  }
  catch (const std::exception& e)
  {
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, e);

    // Save the source response code
    if (nullptr != sourceResponseCode)
    {
      *sourceResponseCode = curl.getSourceHttpResponseCode();
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Source response code set to: " << *sourceResponseCode);
    }

    // Save the destination response code
    if (nullptr != destinationResponseCode)
    {
      *destinationResponseCode = curl.getTargetHttpResponseCode();
      ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destination response code set to: " << *destinationResponseCode);
    }

    throw;
  }
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
// TODO: Circulation request cannot be tested
void LTARestClient::sendCirculationRequest(const curl::RemoteSite& destination, dbConnectionPool& pool,
                                           const dbPersistent::IdType ltaID, const std::string& endpoint,
                                           long* const sourceResponseCode, long* const destinationResponseCode,
                                           const StopController& stopController, const bool removeTarget)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_HIG_VERB);

  std::string uri;
  std::unique_ptr<GenericAuthHandler> auth;
  {
    // Get DB connection
    dbConnectionWrapper cw(pool);
    dbConnection& conn = cw;

    // Get the LTA Remote data from DB
    LTADLRemoteLTA lta(conn);
    lta.load(conn, ltaID);

    // Get the URI
    uri = composeURI(lta.getBaseURL(conn), endpoint);

    // Get the Authentication Parameters
    auth = createAuth(conn, lta, stopController);
  }

  // Send the request
  sendCirculationRequest(destination, uri, auth.get(), sourceResponseCode, destinationResponseCode, stopController,
                         removeTarget);
}
// LCOV_EXCL_STOP

/** Private Static Methods **/
std::string LTARestClient::composeURI(const std::string& baseURL, const std::string& endpoint)
{
  std::ostringstream os;

  os << baseURL;
  if (!StringUtils::endsWith(baseURL, "/"))
  {
    os << "/";
  }

  if (!endpoint.empty())
  {
    os << endpoint;
  }

  return os.str();
}

/** Ctors/Dtors **/
LTARestClient::LTARestClient(dbConnectionPool& pool, const dbPersistent::IdType ltaID,
                             const StopController& stopController) :
  _baseURL(""),
  _auth(nullptr),
  _ltaID(0),
  _ltaName(""),
  _ltaAllowOrdersCreation(true),
  _stopController(stopController),
  _connectionTimeoutSet(false),
  _connectionTimeout(0),
  _adjustResponseTimeout(true)
{
  // Get DB connection
  dbConnectionWrapper cw(pool);
  dbConnection& conn = cw;

  LTADLRemoteLTA lta(conn);
  lta.load(conn, ltaID);
  saveDataFromDB(conn, lta);
}

LTARestClient::LTARestClient(dbConnectionPool& pool, const LTADLRemoteLTA& lta, const StopController& stopController) :
  _baseURL(""),
  _auth(nullptr),
  _ltaID(0),
  _ltaName(""),
  _ltaAllowOrdersCreation(true),
  _stopController(stopController),
  _connectionTimeoutSet(false),
  _connectionTimeout(0),
  _adjustResponseTimeout(true)
{
  // Get DB connection
  dbConnectionWrapper cw(pool);
  dbConnection& conn = cw;

  saveDataFromDB(conn, lta);
}

LTARestClient::LTARestClient(dbConnectionPool& pool, const std::string& ltaName, const StopController& stopController) :
  _baseURL(""),
  _auth(nullptr),
  _ltaID(0),
  _ltaName(""),
  _ltaAllowOrdersCreation(true),
  _stopController(stopController),
  _connectionTimeoutSet(false),
  _connectionTimeout(0),
  _adjustResponseTimeout(true)
{
  // Get DB connection
  dbConnectionWrapper cw(pool);
  dbConnection& conn = cw;

  LTADLRemoteLTA lta(conn);
  lta.loadByName(ltaName, conn);
  saveDataFromDB(conn, lta);
}

LTARestClient::LTARestClient(const std::string& baseURL, const GenericAuthHandler& auth, dbPersistent::IdType ltaID,
                             const std::string& ltaName, const bool allowOrdersCreation,
                             const StopController& stopController) :
  _baseURL(baseURL),
  _auth(auth.clone()),
  _ltaID(ltaID),
  _ltaName(ltaName),
  _ltaAllowOrdersCreation(allowOrdersCreation),
  _stopController(stopController),
  _connectionTimeoutSet(false),
  _connectionTimeout(0),
  _adjustResponseTimeout(true)
{
}

LTARestClient::LTARestClient(const LTARestClient& c) :
  _baseURL(c.getBaseURL()),
  _auth(c.getAuth()),
  _ltaID(c.getLTAID()),
  _ltaName(c.getLTAName()),
  _ltaAllowOrdersCreation(c.getLTAAllowOrdersCreation()),
  _stopController(c._stopController),
  _connectionTimeoutSet(c._connectionTimeoutSet),
  _connectionTimeout(c._connectionTimeout),
  _adjustResponseTimeout(c._adjustResponseTimeout)
{
}

LTARestClient::~LTARestClient() ACS_NOEXCEPT {}

void LTARestClient::saveDataFromDB(dbConnection& conn, const LTADLRemoteLTA& lta)
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Get the Base URL
  _baseURL = lta.getBaseURL(conn);
  if (!StringUtils::endsWith(_baseURL, "/"))
  {
    _baseURL = _baseURL + "/";
  }

  // Get the Authentication Parameters
  _auth = createAuth(conn, lta, _stopController);

  // Get LTA ID
  _ltaID = lta.getId();

  // Get LTA Name
  _ltaName = lta.getName(conn);

  // Get LTA AllowOrdersCreation
  _ltaAllowOrdersCreation = lta.getAllowOrdersCreation(conn);
}

ODataQueryProductsResponsePayload LTARestClient::queryProduct(const std::string& productName,
                                                              long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  std::ostringstream os;
  os << _baseURL << "Products?$filter="
     << curl::Curl::encodeRFC398("Name eq '" + productName + "'");
  const std::string uri = os.str();

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Query Product' request to LTA '" << _ltaName << "' for product '"
                                                                                   << productName << "'");
  const std::string responsePayload = sendSpecificRequest(Get, uri, _auth.get(), nullptr, nullptr, responseCode);

  // Parse response
  ODataQueryProductsResponsePayload response(responsePayload);
  return response;
}

ODataOrderResponsePayload LTARestClient::orderRequest(const std::string& productID,
                                                      const ODataOrderRequestPayload& request,
                                                      long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  std::ostringstream os;
  os << _baseURL << "Products(" << productID << ")/OData.CSC.Order";
  const std::string uri = os.str();

  // Set payload
  const std::string contentType = "application/json";
  const std::string requestPayload = request.toJSON();
  const std::string* requestPayloadPtr = &requestPayload;
  if ("" == requestPayload)
  {
    requestPayloadPtr = nullptr;
  }

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Order Request' request to LTA '" << _ltaName << "' for product ID '"
                                                                                   << productID << "'");
  const std::string responsePayload = sendSpecificRequest(Post, uri, _auth.get(), &contentType, requestPayloadPtr,
                                                          responseCode);

  // Parse response
  ODataOrderResponsePayload response(responsePayload);
  return response;
}

ODataBulkCreateResponsePayload LTARestClient::bulkCreate(const ODataBulkCreateRequestPayload& request,
                                                         long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  const std::string uri = _baseURL + "Bulks";

  // Set payload
  const std::string contentType = "application/json";
  const std::string requestPayload = request.toJSON();

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Bulk Create' request to LTA '" << _ltaName << "' with filter '"
                                                                                 << request.getFilter() << "'");
  const std::string responsePayload = sendSpecificRequest(Post, uri, _auth.get(), &contentType, &requestPayload,
                                                          responseCode);

  // Parse response
  ODataBulkCreateResponsePayload response(responsePayload);
  return response;
}

ODataBatchOrderResponsePayload LTARestClient::batchOrder(const std::string& bulkID, long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  std::ostringstream os;
  os << _baseURL << "Bulk(" << bulkID << ")/BatchOrders";
  const std::string uri = os.str();

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Batch Order' request to LTA '" << _ltaName
                                                                                 << "' for bulk order UUID '" << bulkID
                                                                                 << "'");
  const std::string responsePayload = sendSpecificRequest(Get, uri, _auth.get(), nullptr, nullptr, responseCode);

  // Parse response
  ODataBatchOrderResponsePayload response(responsePayload);
  return response;
}

void LTARestClient::cancelBulkOrder(const std::string& bulkID, long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  std::ostringstream os;
  os << _baseURL << "Bulks(" << bulkID << ")/OData.CSC.Cancel";
  const std::string uri = os.str();

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Cancel Order' request to LTA '" << _ltaName
                                                                                  << "' for bulk order UUID '" << bulkID
                                                                                  << "'");
  sendSpecificRequest(Get, uri, _auth.get(), nullptr, nullptr, responseCode);
}

void LTARestClient::triggerBatch(const std::string& batchUUID, const ODataBatchTriggeringRequestPayload& request,
                                 long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  std::ostringstream os;
  os << _baseURL << "BatchOrder(" << batchUUID << ")/OData.CSC.BatchOrder";
  const std::string uri = os.str();

  // Set payload
  const std::string contentType = "application/json";
  const std::string requestPayload = request.toJSON();
  const std::string* requestPayloadPtr = &requestPayload;
  if ("" == requestPayload)
  {
    requestPayloadPtr = nullptr;
  }

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Trigger Batch' request to LTA '" << _ltaName
                                                                                   << "' for batch order UUID '"
                                                                                   << batchUUID << "'");
  sendSpecificRequest(Post, uri, _auth.get(), &contentType, requestPayloadPtr, responseCode);
}

ODataBatchOrderResponsePayload LTARestClient::batchStatus(const std::string& batchUUID, long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  std::ostringstream os;
  os << _baseURL << "BatchOrder(" << batchUUID << ")";
  const std::string uri = os.str();

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Batch Order' request to LTA '" << _ltaName
                                                                                 << "' for batch order UUID '"
                                                                                 << batchUUID << "'");
  const std::string responsePayload = sendSpecificRequest(Get, uri, _auth.get(), nullptr, nullptr, responseCode);

  // Parse response
  ODataBatchOrderResponsePayload response(responsePayload);
  return response;
}

ODataQueryProductsResponsePayload LTARestClient::batchProducts(const std::string& batchUUID,
                                                               long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  std::ostringstream os;
  os << _baseURL << "BatchOrder(" << batchUUID << ")/Products";
  const std::string uri = os.str();

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Batch Products' request to LTA '" << _ltaName
                                                                                    << "' for batch order UUID '"
                                                                                    << batchUUID << "'");
  const std::string responsePayload = sendSpecificRequest(Get, uri, _auth.get(), nullptr, nullptr, responseCode);

  // Parse response
  ODataQueryProductsResponsePayload response(responsePayload);
  return response;
}

// LCOV_EXCL_START
// TODO: Circulation request cannot be tested
void LTARestClient::circulateProductMetadata(const curl::RemoteSite& destination, const std::string& productUUID,
                                             long* const sourceResponseCode, long* const destinationResponseCode,
                                             const bool removeTarget) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  std::ostringstream os;
  os << _baseURL << "Products(" << productUUID << ")";
  const std::string uri = os.str();

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,
                        "Sending a 'Circulate Products Metadata' request to LTA '" << _ltaName << "' for product UUID '"
                                                                                   << productUUID << "'");
  sendCirculationRequest(destination, uri, _auth.get(), sourceResponseCode, destinationResponseCode, _stopController,
                         removeTarget);
}
// LCOV_EXCL_STOP

// LCOV_EXCL_START
// TODO: Circulation request cannot be tested
void LTARestClient::circulateProduct(const curl::RemoteSite& destination, const std::string& productUUID,
                                     long* const sourceResponseCode, long* const destinationResponseCode,
                                     const bool removeTarget) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  //https://<service-root-uri>/odata/v1/Products(ProductId)/$value
  // Compose URI
  std::ostringstream os;
  os << _baseURL << "Products(" << productUUID << ")/$value";
  const std::string uri = os.str();

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Circulate Products' request to LTA '" << _ltaName
                                                                                        << "' for product UUID '"
                                                                                        << productUUID << "'");
  sendCirculationRequest(destination, uri, _auth.get(), sourceResponseCode, destinationResponseCode, _stopController,
                         removeTarget);
}
// LCOV_EXCL_STOP

ProductOrderResponsePayload LTARestClient::orderStatus(const std::string& orderUUID, long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  std::ostringstream os;
  os << _baseURL << "Orders(" << orderUUID << ")";
  const std::string uri = os.str();

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Order Status' request to LTA '" << _ltaName << "' for order UUID '"
                                                                                  << orderUUID << "'");
  const std::string responsePayload = sendSpecificRequest(Get, uri, _auth.get(), nullptr, nullptr, responseCode);

  // Parse response
  ProductOrderResponsePayload response(responsePayload);
  return response;
}

ODataQueryProductsResponsePayload LTARestClient::batchOrdersStatus(const std::string& batchUUID,
                                                                   const std::string& orderUUID,
                                                                   long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  std::ostringstream os;
  // NB: Non dovrebbe esserci "Products?$filter=" all'interno della parte codificata, ma gli attuali test funzionano con
  // l'uri composto in questo modo
  os << _baseURL << "BatchOrder(" << batchUUID << ")/"
     << curl::Curl::encodeRFC398("Products?$filter=Id eq " + orderUUID);
  const std::string uri = os.str();

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Batch Order Status' request to LTA '" << _ltaName
                                                                                        << "' for batch order UUID '"
                                                                                        << batchUUID << "'");
  const std::string responsePayload = sendSpecificRequest(Get, uri, _auth.get(), nullptr, nullptr, responseCode);

  // Parse response
  ODataQueryProductsResponsePayload response(responsePayload);
  return response;
}

ODataSubscriptionResponsePayload LTARestClient::subscription(const ODataSubscriptionRequestPayload& request,
                                                             long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  const std::string uri = _baseURL + "Subscriptions";

  // Set payload
  const std::string contentType = "application/json";
  const std::string requestPayload = request.toJSON();

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Subscription' request to LTA '" << _ltaName << " with FilterParam '"
                                                                                  << request.getFilterParam() << "'");
  const std::string responsePayload = sendSpecificRequest(Post, uri, _auth.get(), &contentType, &requestPayload,
                                                          responseCode);

  // Parse response
  ODataSubscriptionResponsePayload response(responsePayload);
  return response;
}

ODataSubscriptionResponsePayload LTARestClient::subscriptionStatus(const std::string& subscriptionUUID,
                                                                   long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  std::ostringstream os;
  os << _baseURL << "Subscriptions(" << subscriptionUUID << ")";
  const std::string uri = os.str();

  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Subscription Status' request to LTA '" << _ltaName
                                                                                         << "' for subscription UUID '"
                                                                                         << subscriptionUUID << "'");
  const std::string responsePayload = sendSpecificRequest(Get, uri, _auth.get(), nullptr, nullptr, responseCode);

  // Parse response
  ODataSubscriptionResponsePayload response(responsePayload);
  return response;
}

void LTARestClient::cancelSubscription(const std::string& subscriptionUUID, long* const responseCode) const
{
  ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

  // Compose URI
  std::ostringstream os;
  os << _baseURL << "Subscriptions(" << subscriptionUUID << ")/OData.CSC.Cancel";
  const std::string uri = os.str();
  // Set payload
  const std::string contentType = "application/json";
  const std::string requestPayload = "";
  // Send the request
  ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Sending a 'Cancel Subscription' request to LTA '" << _ltaName
                                                                                         << "' for subscription UUID '"
                                                                                         << subscriptionUUID << "'");
  sendSpecificRequest(Post, uri, _auth.get(), &contentType, &requestPayload, responseCode);
}

size_t LTARestClient::getConnectionTimeout_ms(bool* set, bool* adjustResponseTimeout) const
{
  if (nullptr == set)
  {
    ACS_COND_THROW(!_connectionTimeoutSet, LTARestClientException("Connection Timeout not set"));
  }
  else
  {
    *set = _connectionTimeoutSet;
  }

  if (nullptr != adjustResponseTimeout)
  {
    *adjustResponseTimeout = _adjustResponseTimeout;
  }

  return _connectionTimeout;
}

void LTARestClient::setConnectionTimeout_ms(const size_t ms, const bool adjustResponseTimeout)
{
  _connectionTimeoutSet = true;
  _connectionTimeout = ms;
  _adjustResponseTimeout = adjustResponseTimeout;
}

// Reset to default values
void LTARestClient::unsetConnectionTimeout_ms()
{
  _connectionTimeoutSet = false;
  _connectionTimeout = 0;
  _adjustResponseTimeout = true;
}

const std::string LTARestClient::sendSpecificRequest(const httpMethods httpMethod, const std::string& uri,
                                                     const GenericAuthHandler* const auth,
                                                     const std::string* const contentType,
                                                     const std::string* const payload, long* const responseCode) const
{
  const size_t* connectionTimeoutPtr = nullptr;
  const bool* adjustResponseTimeoutPtr = nullptr;
  if (_connectionTimeoutSet)
  {
    connectionTimeoutPtr = &_connectionTimeout;
    adjustResponseTimeoutPtr = &_adjustResponseTimeout;
  }

  return sendRequest(httpMethod, uri, auth, contentType, payload, responseCode, connectionTimeoutPtr,
                     adjustResponseTimeoutPtr);
}

_ACS_END_NAMESPACE
