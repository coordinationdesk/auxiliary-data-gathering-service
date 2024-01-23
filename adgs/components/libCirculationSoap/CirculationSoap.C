// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA, Inc.


	$Prod:  A.C.S. libCirculationSoap $
*/


#include <CirculationSoap.h>
#include <CirculationMode.h>
#include <LocalTransferMode.h>
#include <CirculationDBService.h>
#include <CirculationCallbackSingleton.h>

#include <TmpNameStrategyTable.h>
#include <File.h>
#include <Network.h>
#include <rsResourceSet.h>
#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>
#include <Application.h>
#include <NameIdAssociator.h>


#pragma GCC diagnostic push // PRQA S 1040 4
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#include <CirculationH.h> // PRQA S 1013 3 
#include <Circulation.nsmap>
#include <CirculationPDS_USCOREOrderingBindingProxy.h>
#include <CirculationPDS_USCOREOrderingBindingService.h>

#pragma GCC diagnostic pop

#include <Filterables.h>
#include <RemoteSiteFactory.h>
#include <RemoteSiteUtility.h>
#include <RemoteSite.h>
#include <SecurityZone.h>
#include <Status.h>
#include <RegExp.h>
#include <lCSoapV.h>
#include <dbConnectionPool.h>

#include <MCFilterables.h>
#include <MediaCirculationMngFunctions.h>


_ACS_BEGIN_NAMESPACE(Circulation)

/* Base class methods
*/

int PDS_USCOREOrderingBindingService::CirculateDynamic( // PRQA S 2017, 4020
                            pds__CirculateDynamicRequestType *  in, 
                            pds__CirculateResponseType*  out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int PDS_USCOREOrderingBindingService::CirculateMultiDynamic( // PRQA S 2017, 4020
                            pds__CirculateMultipleDynamicRequestType *  in, 
                            pds__CirculateMultipleResponseType*  out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}


int PDS_USCOREOrderingBindingService::CirculateExplicit(// PRQA S 2017, 4020
                             pds__CirculateExplicitRequestType *  in, 
                             pds__CirculateResponseType*  out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);

}

int PDS_USCOREOrderingBindingService::GetStatus( // PRQA S 2017, 4020
                     pds__StatusRequestType*    in,	
                     pds__StatusResponseType*   out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int PDS_USCOREOrderingBindingService::IncreaseCirculationPriority(  // PRQA S 2017, 4020
                 pds__IncreaseCirculationPriorityRequestType*    in,	
                 pds__IncreaseCirculationPriorityResponseType*   out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int PDS_USCOREOrderingBindingService::CirculationRecordOccurrence( // PRQA S 2017, 4020
                 pds__CirculationOccurrenceRequestType*    in,	
                 pds__CirculationOccurrenceResponseType*   out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}


int PDS_USCOREOrderingBindingService::GetMultipleStatus( // PRQA S 2017, 4020
                     pds__MultipleStatusRequestType*    in,	
                     pds__MultipleStatusResponseType*   out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int PDS_USCOREOrderingBindingService::GetMultipleDetailedStatus( // PRQA S 2017, 4020
                     pds__MultipleStatusRequestType*    in,	
                     pds__MultipleDetailedStatusResponseType*   out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}


int PDS_USCOREOrderingBindingService::ResetCirculationRetry( // PRQA S 2017, 4020
    				pds__ResetRetryRequestType*  in,
    				pds__CirculateResponseType* out)
{

	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int PDS_USCOREOrderingBindingService::ResetCirculationStatus(  // PRQA S 2017, 4020
    				pds__MultipleStatusRequestType*  in,
                    pds__MultipleStatusResponseType* out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int PDS_USCOREOrderingBindingService::ResetMultipleCirculationStatus(// PRQA S 2017, 4020
    				pds__MultipleStatusRequestType*  in,
                    pds__MultipleResponseType* out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}

int PDS_USCOREOrderingBindingService::ReInsertCirculation( // PRQA S 2017, 4020
    				pds__StatusRequestType*  in,
                    pds__CirculateResponseType* out)
{
	return soap_receiverfault("NOT IMPLEMENTED", NULL);
}


_ACS_END_NAMESPACE


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;
using namespace ::Circulation;
using namespace acs::circulation;
using namespace acs::mc;

namespace {
    lCSoapV version ; // versioning 
	
	class ClientFinder {
    public:
        explicit ClientFinder(const string & client ) : client_(client) {} // PRQA S 2528
        
        ClientFinder(const ClientFinder &r) : client_(r.client_) {} // PRQA S 2528
        ~ClientFinder() throw() {}
		bool operator()(const pair<string, dbPersistent::IdType> &p) const 
        {
            bool found = false;
			if( p.first == client_ ) {
				found = true;
			}
			return found;
        }
    private:
        ClientFinder() ; // not implemented
        ClientFinder& operator=(const ClientFinder &) ; // not implemented
    
        const string & client_ ;
    } ;
	

	int http_get(struct soap *soap) // PRQA S 4020
	{
    	/*
    	soap_response(soap, SOAP_HTML); // HTTP response header with text/html
    	soap_send(soap, "<HTML>My Web server is operational.</HTML>");
    	soap_end_send(soap);
    	return SOAP_OK;
    	*/  
    	string path(soap->path);
    	string ending("wsdl");
    	string end_xsd("xsd");

    	if (path.length() < ending.length())
    	{
        	soap_response(soap, SOAP_HTML); // HTTP response header with text/html
        	soap_send(soap, "<HTML><h3>Web server is operational.</h3>  Call path ?wsdl to get info.</HTML>");
        	soap_end_send(soap);
        	return SOAP_OK;
    	}

    	FILE *fd = 0 ;  

    	if (  path.compare (path.length() - ending.length(), ending.length(), ending) == 0) // PRQA S 3084
    	{ 
        	fd = fopen((string(base::defaultConfPath()) + "/pdsCirculation.wsdl").c_str(), "rb"); // open WSDL file to copy // PRQA S 3081
    	} 
    	else if (  path.compare (path.length() - end_xsd.length(), end_xsd.length(), end_xsd) == 0) {  // PRQA S 3084
        	fd = fopen((string(base::defaultConfPath()) + "/pdsCirculation.xsd").c_str(), "rb"); // open XSD file to copy // PRQA S 3081
    	}
    	else { 
        	soap_response(soap, SOAP_HTML); // HTTP response header with text/html
        	soap_send(soap, "<HTML><h3>Web server is operational.</h3>  Call path ?wsdl to get info.</HTML>");
        	soap_end_send(soap);
        	return SOAP_OK;
    	}

	   if (!fd) {
    	  return 404; // return HTTP not found error // PRQA S 4400
	   }
	   soap->http_content = "text/xml"; // HTTP header with text/xml content
	   soap_response(soap, SOAP_FILE);
	   for (;;)
	   {
    	  size_t r = fread(soap->tmpbuf, 1, sizeof(soap->tmpbuf), fd);
    	  if (0 == r) {
        	 break;
    	  }
    	  if (0 != soap_send_raw(soap, soap->tmpbuf, r)) {
        	 break; // can't send, but little we can do about that
    	  }
	   }
	   fclose(fd);
	   soap_end_send(soap);
	   return SOAP_OK; 

	} 


}

ostream &operator<<(ostream &s, const pds__Outcome & v)
{
	s << "pds__Outcome: Id=" << v.Id << " modification date=\"" << v.ModificationDate << "\" status=" << int(v.Status);
	return s ; // PRQA S 4028
}

exostream &operator<<(exostream &s, const pds__Outcome & v) 
{
	s << "pds__Outcome: Id=" << v.Id << " modification date=\"" << v.ModificationDate << "\" status=" << int(v.Status);
	return s ; // PRQA S 4028
}


const string CirculationOrderingService::_defaultTempnameMode  = "PREFIX" ;
const string CirculationOrderingService::_defaultTempnameValue = "." ;


ACS_CLASS_DEFINE_DEBUG_LEVEL (CirculationOrderingService);

CirculationOrderingService::CirculationOrderingService (dbConnectionPool & p, StopController &sp) :
	PDS_USCOREOrderingBindingService(),
    _hostIp(),
    _localUser(), 
    _localPwd(), 
    _localProtocol(), 
    _localPort(0), 
    _host(),
    _hostLevel(0), 
    _hostId(0), 
    _priority(0), 
    _removeSource(true), 
    _maxDbRetry(_defaultMaxDbRetries),
    _maxSoapRetry(_defaultMaxSoapRetries),
    _connPool(p), // PRQA S 2528
	_tempnameMode(_defaultTempnameMode),
	_tempnameValue(_defaultTempnameValue),
	_multipleRequestsResetRetry(false),
    _sendTimeout(60), // PRQA S 4400 3
    _recvTimeout(60),
    _acceptTimeout(5),
	_clientLUT(),
	_agentNodeId(),
	_dynamicCircLocalTransferMode(conf::CirculationConfigurerSingleton::instance()->getLocalTransferMode()),
	_explicitCircLocalTransferMode(conf::CirculationConfigurerSingleton::instance()->getLocalTransferMode()),
	_hostLevelSet(false),
	_stopController(sp)
{}

CirculationOrderingService::~CirculationOrderingService() throw() 
{
	
}

void CirculationOrderingService::loadConf()
{
	rsResourceSet &rset = ConfigurationSingleton::instance()->get();
   	rsPushNSpace cServ(rset,"CirculationService");
	try
	{
 		rset.getValue("DbRetries", _maxDbRetry);	
	}
	catch (exception &e)
	{
		// if no DbRetries configured -> set default value 3
		_maxDbRetry = _defaultMaxDbRetries ;
		ACS_LOG_INFO("DbRetries not found in configuration, set to default: " << _maxDbRetry );
	}
    
	try
	{
 		rset.getValue("SoapRetries", _maxSoapRetry);	
	}
	catch (exception &e)
	{
		// if no SoapRetries configured -> set default value 3
		_maxSoapRetry = _defaultMaxSoapRetries ;
        ACS_LOG_INFO( "SoapRetries not found in configuration, set to default: " << _maxSoapRetry ) ;
	}

    rset.getValue ("SoapSendTimeout", _sendTimeout, false);
    rset.getValue ("SoapReceiveTimeout", _recvTimeout, false);
    rset.getValue ("SoapAcceptTimeout", _acceptTimeout, false);

	rset.getValue("HostName", _host);		
	rset.getValue("Port", _localPort);		
	rset.getValue("User", _localUser);		
	rset.getValue("Password", _localPwd);		
	rset.getValue("Protocol", _localProtocol);
  	rset.getValue("RemoveSource", _removeSource);
 	rset.getValue("Priority", _priority);
#warning "Take the configuration from FtpCurl"
	try {
		rset.getValue("TempnameMode", _tempnameMode);
		rset.getValue("TempnameValue",_tempnameValue);
	}
	catch(rsResourceSet::NotFoundException &e)
	{}
	
	try {
		rset.getValue("MultipleRequestsResetRetry", _multipleRequestsResetRetry);
	}
	catch(rsResourceSet::NotFoundException &e)
	{}
	
	
	_clientLUT.clear();
	try {
		size_t itemsNum = rset.getArray("SoapClientLUT.Pair", _clientLUT); // PRQA S 4412
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "<" << itemsNum << "> Client LUT elements read") ;
	}
	catch(rsResourceSet::NotFoundException &e)
	{}
	
	try {
		rset.getValue("AgentNodeId", _agentNodeId);
	}
	catch(rsResourceSet::NotFoundException &e)
	{}
	
	try {
		rset.getValue("DynamicCircLocalTransferMode", _dynamicCircLocalTransferMode);
		conf::ImplicitCirculationConfigurer::checkLocalTransferModeString(_dynamicCircLocalTransferMode);
	}
	catch(rsResourceSet::NotFoundException &)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"DynamicCircLocalTransferMode not found in configuration, set to default: " << _dynamicCircLocalTransferMode );
	}

	try {
		rset.getValue("ExplicitCircLocalTransferMode", _explicitCircLocalTransferMode);
		conf::ImplicitCirculationConfigurer::checkLocalTransferModeString(_explicitCircLocalTransferMode);
	}
	catch(rsResourceSet::NotFoundException &)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"ExplicitCircLocalTransferMode not found in configuration, set to default: " << _explicitCircLocalTransferMode );
	}

	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Local User = " << _localUser) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Local Password = " << _localPwd) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Local Protocol = " << _localProtocol) ;
 	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Remove source = " << boolalpha << _removeSource) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Priority = " << _priority) ;
 	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Port = " << _localPort) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"TempnameMode = " << _tempnameMode) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"TempnameValue = " << _tempnameValue) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"MultipleRequestsResetRetry = " << boolalpha << _multipleRequestsResetRetry) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Soap Send timeout = " << _sendTimeout) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Soap Receive timeout = " << _recvTimeout) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Soap Accept timeout = " << _acceptTimeout) ;
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		size_t n = _clientLUT.size();
		for( size_t i = 0; i < n; i++) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Client LUT element #" << i << " clientId = " << _clientLUT[i].first << " hostId = " <<_clientLUT[i].second );
		}
	ACS_CLASS_END_DEBUG
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Agent Node Id = " << _agentNodeId) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Dynamic Circulations local transfer mode = " << _dynamicCircLocalTransferMode) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Explicit Circulations local transfer mode = " << _explicitCircLocalTransferMode) ;

	checkIpAddress(_host, _hostIp, _hostId);
  
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  "Host Ip = " << _hostIp << " Host Id = " << _hostId );
    ACS_COND_THROW( _hostIp.empty(), CriticalException( "error retrieving host ipaddress!") ) ; // PRQA S 3081
    ACS_COND_THROW( _host == _hostIp , CriticalException( "Configured hostname cannot be an IP address: \"" + _host + '\"') ) ; // PRQA S 3081
    ACS_COND_THROW( 0 == _hostId, LocalhostNotFoundException( '\"' + _host + "\" host not found in t_knownhosts table!") ) ; // PRQA S 3081
	
	if( not _hostLevelSet ) {
		_hostLevel = getHostSecZoneInfo(_hostIp, _connPool.getKey()).second ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Host Level=" << _hostLevel );
		_hostLevelSet = true;
	}


}

vector<CirculationOrderingService::relayinfo> CirculationOrderingService::getRelaysForZone(dbPersistent::IdType secZoneId)
{
    //search a relay
	//select h.ip_address, h.host_name, r.host_id from t_securityzonerelays r, t_knownhosts h where r.host_id = h.id and r.seczone_id = secZoneId ;
	const string &dbKey = _connPool.getKey() ;
    SQLString queryString(dbKey, SQLString::SELECT) ;
    queryString.addTableList("t_knownhosts") ;
    queryString.addSelectList("t_knownhosts.ip_address"); //it could be null
    queryString.addSelectList("t_knownhosts.host_name"); 
    queryString.addSelectList("t_securityzonerelays.id");
	{//COLUMN: id host_id seczone_id
        dbQueryParameters pp(dbKey) ;
	    pp.addParameter(dbParam(dbKey, "t_securityzonerelays","host_id" , dbParam::EQUAL_TO , "t_knownhosts.id" )) ;
		pp.addParameter(dbParam(dbKey, "t_securityzonerelays", "seczone_id", dbParam::EQUAL_TO, dbConv(dbKey, secZoneId))); // PRQA S 3050, 3081
	    queryString.addJoinList(SQLDialect::INNER,"t_securityzonerelays",pp) ;
	}
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query is: " << queryString.getSQL()) ;
    dbSet set ;
	performQueryForSet(queryString, set, "DC SEL RelayHost") ;
    vector<relayinfo> relays ;
    size_t setsize = set.size() ;
	for (size_t p = 0; p < setsize; ++p) 
	{
	    dbRow row(dbKey);
        set.getRow(p + 1, row); // PRQA S 3084
        // get the relay ipaddress and rHostId
        bool rIpAddressNull = false ;
        string rIpAddress ;
        row.column(0, rIpAddress, &rIpAddressNull);
        string RelayHostName ;
        row.column(1, RelayHostName);
        dbPersistent::IdType RelayId = 0;
        row.column(2, RelayId); // PRQA S 4400

        if (rIpAddressNull) {
            rIpAddress = getIpAddressByName(RelayHostName);
        }
        if(rIpAddress.empty()) { //cannot resolve relay ipaddress
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Cannot resolve ipAddress for hostname " << RelayHostName << " record ignored") ; 
            continue; //try next
        }
        
        relays.push_back(relayinfo(rIpAddress, RelayHostName, RelayId)) ;
    }
    return relays ;
}

pair<string, string> CirculationOrderingService::getRelayInfo(dbPersistent::IdType relayid)  
{
	// if relayid found send a status request to relay
	// find ipaddress
	const string &dbKey = _connPool.getKey() ;
	SQLString newQuery(dbKey, SQLString::SELECT) ;
    	newQuery.addTableList("t_knownhosts") ;         //COLUMN: id ip_address host_name
        newQuery.addSelectList("ip_address");
        newQuery.addSelectList("host_name");
	{//COLUMN: id host_id seczone_id
        dbQueryParameters pp(dbKey) ;
	    pp.addParameter(dbParam(dbKey, "t_securityzonerelays","host_id" , dbParam::EQUAL_TO , "t_knownhosts.id" )) ;
	    newQuery.addJoinList(SQLDialect::INNER,"t_securityzonerelays",pp) ;
	}
    {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_securityzonerelays","id", dbParam::EQUAL_TO, dbConv(dbKey, relayid))); // PRQA S 3050, 3081
		newQuery.addANDWhereParameter(pp);
    }
    dbSet set ;
	performQueryForSet(newQuery, set, "DC SEL RelayIp");
 	
	if(set.empty()) 
	{
        ostringstream os ;
        os << "error retrieving host infos for relay_id: " << relayid ;
		ACS_THROW(dbQuery::QueryExecuteException(os.str())); // PRQA S 3081
	}
    dbRow row(dbKey) ;
    set.getRow(1, row);

    // get relay ipaddress
	string rIpAddress;
    string rHostName;
    bool isNull = false ;
    row.column(0, rIpAddress, &isNull);
    row.column(1, rHostName);
    
    if (isNull) {
        rIpAddress = getIpAddressByName(rHostName);
    }
    
    return make_pair(rHostName, rIpAddress) ;
}


void CirculationOrderingService::processStatusRequest(pds__StatusRequestType*    in,	pds__StatusResponseType*   out) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// check for mandatory fields
	if(in->RequestID == "")
	{
        ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException("Empty requestid")) ; // PRQA S 3081
		out->ErrorCode = pds__ErrorCodeEnum__1;
		return;
	}
    
	// reload configuration params
	try {
		loadConf();
	}
	catch(LocalhostNotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Host not found: " << e.what()) ; 
	}
	
    dbPersistent::IdType localCirculationId = 0 ;
    istringstream is(in->RequestID) ;
    is >> localCirculationId ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Request Id: " << localCirculationId) ; 
    const string & dbKey = _connPool.getKey() ;
	SQLString queryString(dbKey, SQLString::SELECT) ;
    	queryString.addTableList("t_circulationsqueue") ;
        queryString.addSelectList("status_id");
        queryString.addSelectList("relay_host_id");
        queryString.addSelectList("relay_activity_id");
    {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","id", dbParam::EQUAL_TO, dbConv(dbKey, localCirculationId))); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
    }
	dbSet set;
	performQueryForSet(queryString, set, "DC SEL Request");
	size_t num = set.size();		
	if(num != 1) {
        ACS_THROW(dbQuery::QueryExecuteException("error retrieving status for request = " + in->RequestID)); // PRQA S 3081
    }
    dbRow row(dbKey);
    set.getRow(1, row);

    // get circulation mode id
	short status_id = 0 ;
    row.column(0, status_id);

	dbPersistent::IdType relayid = 0 ; 
    bool relayidNull = false ;
    row.column(1, relayid, &relayidNull);

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relayidNull: " << boolalpha << relayidNull << " relayid: " << relayid) ; 
    dbPersistent::IdType activityid = 0 ;
    bool activityidNull = false ;
    row.column(2, activityid, &activityidNull); // PRQA S 4400
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "activityidNull: " << boolalpha << activityidNull << " activityid: " << activityid) ; 
    
	string reqStatus = Status::instance(dbKey)->name(status_id) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Request status: " << reqStatus) ; 
    if ( Status::error == reqStatus || Status::completed == reqStatus || relayidNull || (relayid == 0) ) { // no relay (circulated from this host) or final status
		out->ErrorCode = pds__ErrorCodeEnum__0;
		out->Status = reqStatus;
		if( Status::error == out->Status ) {
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning pds__ErrorCodeEnum__99 for error status") ; 
			out->ErrorCode = pds__ErrorCodeEnum__99;
		}
		return;
	}
    
    // here if a relay was circulating it
    pair<string, string> relayInfo ;
    string rIpAddress ;
    try {
        relayInfo = getRelayInfo(relayid) ;
        string rHostName = relayInfo.first ;
	    rIpAddress = relayInfo.second ;
        //should never happen..
        ACS_COND_THROW(rIpAddress.empty(),CriticalException("Cannot resolve relay ip address for hostname: " + rHostName ) ) ; // PRQA S 3081
    }
    catch(exception &ex) {
        ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException(ex,"Cannot report status for requestid: " + in->RequestID )) ; // PRQA S 3081
		out->ErrorCode = pds__ErrorCodeEnum__2; //NO RELAYS FOUND
		return;       
    }
      

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to ask status to relay: \"" << rIpAddress << "\" for request: " << activityid);
	// fill the new Status request
	PDS_USCOREOrderingBindingProxy proxy;
	//unsigned long port=11111;
	pds__StatusRequestType newRequest;
    ostringstream osaid ; 
    osaid << activityid ;
	newRequest.RequestID = osaid.str() ;

	// fill the proxy endpoint
	string endpointString;
	{
		ostringstream endpointStream ;
		endpointStream << "http://" << rIpAddress << ":" << _localPort ;
		endpointString = endpointStream.str();
	}
	proxy.soap_endpoint = endpointString.c_str();
    proxy.send_timeout = _sendTimeout ; // PRQA S 3000 3
    proxy.accept_timeout = _acceptTimeout ;
    proxy.recv_timeout = _recvTimeout ;

	//  generate a getStatus request 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to contact relay: \"" << proxy.soap_endpoint << "\"");
	for (unsigned int i = 0 ; i< _maxSoapRetry; i++)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "retry =  " << i );
		pds__StatusResponseType newResponse;
        int soap_error_code = proxy.GetStatus(&newRequest , &newResponse) ;
		if ( soap_error_code == SOAP_OK) 
		{
	        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relay contacted: \"" << proxy.soap_endpoint << "\"");
#warning MP should update local status !!!
			out->Status = newResponse.Status;
			out->ErrorCode = newResponse.ErrorCode;
			try{
            	updateCirculationStatus(localCirculationId, out->Status) ; 
			}
			catch(exception &e)
			{
				ACS_LOG_NOTIFY_EX(e) ;
				ACS_LOG_ERROR("Cannot update status of t_circulationsqueue with id=" << localCirculationId << " to " << out->Status );
			}
		    if( Status::error == out->Status ) {
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning pds__ErrorCodeEnum__99 for error status") ; 
			    out->ErrorCode = pds__ErrorCodeEnum__99;
		    }
			return;
		}
		ostringstream error ;
		error << "Error while contacting relay: \"" << proxy.soap_endpoint << "\" - "; 
		proxy.soap_stream_fault(error) ;
		ACS_LOG_WARNING(error.str());

		Timer::delay(1000); // PRQA S 4400
	}
	//  it is impossible to contact  relay set errorCode and return
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning pds__ErrorCodeEnum__3: no relay connection") ; 
	out->ErrorCode = pds__ErrorCodeEnum__3;
}

void CirculationOrderingService::resetCirculationStatus(pds__MultipleStatusRequestType*    in, pds__MultipleStatusResponseType* out) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// check for mandatory fields
	if(in->RequestIDs.empty())
	{
        ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException("Empty requestid")) ; // PRQA S 3081
		return;
	}
    
	// reload configuration params
	try {
		loadConf();
	}
	catch(LocalhostNotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Host not found: " << e.what()) ; 
	}
	
    vector<dbPersistent::IdType> circulationIds ;
// 	dbConnectionWrapper pw(_connPool);
// 	dbConnection &conn = pw ; // PRQA S 3050
// 	dbTemporaryTable ttable(conn) ; 
// 	string ttableName = ttable.createTable(in->RequestIDs) ; // create and populate temp.table

    const string & dbKey = _connPool.getKey() ;
	SQLString queryString(dbKey, SQLString::SELECT) ;
    	queryString.addTableList("t_circulationsqueue") ;
        queryString.addSelectList("id");
        queryString.addSelectList("status_id");
        queryString.addSelectList("relay_host_id");
        queryString.addSelectList("relay_activity_id");
		queryString.addSelectList("modificationdate");
        queryString.addOrderByList("relay_host_id", SQLString::ASC );
    {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","id", dbParam::IN , in->RequestIDs)); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
    }
	dbSet set;
	performQueryForSet(queryString, set, "DC SEL Request");
	size_t num = set.size();		
	if(num != in->RequestIDs.size()) {
    	ostringstream os;
        os << num << " records retrieved instead of " << in->RequestIDs.size() << ". They should match!" << endl;
        ACS_THROW(dbQuery::QueryExecuteException(os.str())); // PRQA S 3081
        //out->ErrorCode = pds__ErrorCodeEnum__99;
        //return;
    }
    
    dbRow row(dbKey);
    
    map<dbPersistent::IdType, vector<pair<dbPersistent::IdType, pds__RecordInfo> > > relIdVsCircIds; // relayid --> vector <activity_id, (circid, modificationDate)>
	while( set.getNextRow(row) ) {

		unsigned short col = 0;
		
		dbPersistent::IdType circid = 0 ; 
    	row.column(col++, circid);
		
    	// get circulation status id
		short status_id = 0 ;
    	row.column(col++, status_id);
		
		dbPersistent::IdType relayid = 0 ; 
    	bool relayidNull = false ;
    	row.column(col++, relayid, &relayidNull);
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relayidNull: " << boolalpha << relayidNull << " relayid: " << relayid) ; 
		
    	dbPersistent::IdType activityid = 0 ;
    	bool activityidNull = false ;
    	row.column(col++, activityid, &activityidNull); // PRQA S 4400
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "activityidNull: " << boolalpha << activityidNull << " activityid: " << activityid) ; 
    
    	// get modification date
		string modificationDate;
    	row.column(col++, modificationDate);
        
		string reqStatus = Status::instance(dbKey)->name(status_id) ;
    	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id " << circid << " status=" << reqStatus) ; 
		pds__RecordInfo *recordInfo = new pds__RecordInfo;
		// save circulation info
		ostringstream os;
		os << circid;
		recordInfo->Id = os.str();
		recordInfo->ModificationDate = modificationDate;
    	if ( relayidNull || (relayid == 0) ) { // no relay (circulated from this host) 
			
			if( Status::error == reqStatus) {
            	try
                {
		     	    updateCirculationStatus(circid, Status::eligible);
                    // successful status updating...
                    out->CompletedIDs.push_back(recordInfo);
                }
                catch (exException & e)
                {
                	// no status updating performed...
                    out->ErrorIDs.push_back(recordInfo);
                }
			}
            else // status not in error (not expected)
            {
            	// no status updating performed...
                out->ErrorIDs.push_back(recordInfo);
            }
		}
		else	// if a relay was circulating it...
        {
             
                // save info into a map, for each relay the pair <activity id, (circid, modificationDate)>
                relIdVsCircIds[relayid].push_back(make_pair(activityid, *recordInfo)); 
		}
	
	} // while( set.getNextRow(row) )

	// handle relays
    if (relIdVsCircIds.size() != 0)
    {    
    	map<dbPersistent::IdType, vector < pair < dbPersistent::IdType, pds__RecordInfo> > >::iterator map_it = relIdVsCircIds.begin();
        // loop on relays
        while (map_it != relIdVsCircIds.end())
        {
	    	map<dbPersistent::IdType, pds__RecordInfo > actIdVsCircId; // activity_id --> (circid, modificationDate)
    
        	dbPersistent::IdType relayid = map_it->first;
    	    pair<string, string> relayInfo ;
    	    string rIpAddress ;
    	    try {
        	    relayInfo = getRelayInfo(relayid) ;
        	    string rHostName = relayInfo.first ;
	    	    rIpAddress = relayInfo.second ;
        	    //should never happen..
        	    ACS_COND_THROW(rIpAddress.empty(),CriticalException("Cannot resolve relay ip address for hostname: " + rHostName ) ) ; // PRQA S 3081
    	    }
    	    catch(exception &ex) {
        	    ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException(ex,"Cannot update status for requests on relay : " + rIpAddress)) ; // PRQA S 3081
			    continue;
    	    }

		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to update status to relay: \"" << rIpAddress );
		    // fill the new Status request
		    PDS_USCOREOrderingBindingProxy proxy;
		    pds__MultipleStatusRequestType newRequest;
        	for (uint i = 0; i < map_it->second.size(); ++i)
            {
                ostringstream osaid ; 
    	        osaid <<  map_it->second[i].first;
		        newRequest.RequestIDs.push_back(osaid.str()) ;
            	actIdVsCircId[map_it->second[i].first] = map_it->second[i].second;

            }
    	    
		    // fill the proxy endpoint
		    string endpointString;
		    {
			    ostringstream endpointStream ;
			    endpointStream << "http://" << rIpAddress << ":" << _localPort ;
			    endpointString = endpointStream.str();
		    }
		    proxy.soap_endpoint = endpointString.c_str();
            proxy.send_timeout = _sendTimeout ; // PRQA S 3000 3
            proxy.accept_timeout = _acceptTimeout ;
            proxy.recv_timeout = _recvTimeout ;

		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to contact relay: \"" << proxy.soap_endpoint << "\"");
			
            uint count(0);
            bool found = false;
            pds__MultipleStatusResponseType relayResponse;
		    while ((count< _maxSoapRetry) && !found)
		    {
			    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "retry =  " << count );
					int soap_error_code = proxy.ResetCirculationStatus(&newRequest, &relayResponse);
					if ( soap_error_code == SOAP_OK) 
					{
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relay contacted: \"" << proxy.soap_endpoint << "\"");
                        found = true;
                    }
                    else
                    {
                    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Error contacting " << proxy.soap_endpoint);
                        count++;
					    Timer::delay(1000); // PRQA S 4400
                    }

		    }
            // set global response
            size_t ei_size =  relayResponse.ErrorIDs.size() ; 
   		    for (uint i = 0; i < ei_size; ++i) // loop on error IDs
            {
                // save activityId ( = circulation id on relay)
                dbPersistent::IdType actId = atoi((relayResponse.ErrorIDs[i])->Id.c_str());
                if (actIdVsCircId.find(actId) != actIdVsCircId.end())
                {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "circulation queue id " <<  actId << " on relay \"" << proxy.soap_endpoint << "\" not updated");
					pds__RecordInfo *circInfo = new pds__RecordInfo;
			       	*circInfo = actIdVsCircId[actId];
                    // no status updating performed...
                    out->ErrorIDs.push_back(circInfo);
                }
            }
            
            size_t ci_size = relayResponse.CompletedIDs.size() ;
   		    for (uint i = 0; i < ci_size; ++i) // loop on successfull IDs
            {

                // save activityId ( = circulation id on relay)
                dbPersistent::IdType actId = atoi((relayResponse.CompletedIDs[i])->Id.c_str());
                if (actIdVsCircId.find(actId) != actIdVsCircId.end())
                {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "circulation queue id " <<  actId << " on relay \"" << proxy.soap_endpoint << "\" updated to ELIGIBLE");
					pds__RecordInfo *circInfo = new pds__RecordInfo;
			        *circInfo = actIdVsCircId[actId];
					try {
                    	// update status of circulator
                    	updateCirculationStatus(atoi(circInfo->Id.c_str()), Status::eligible) ;
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "circulation queue id " <<  circInfo->Id << " on local host updated to ELIGIBLE");
                    	// successful status updating...
                    	out->CompletedIDs.push_back(circInfo);
					}
					catch( exception &e)
					{
						ACS_LOG_NOTIFY_EX(e);
						ACS_LOG_ERROR("Error trying to updated circulation queue id " <<  circInfo->Id << " on local host.");
					}
                }
            }
		    //  it is impossible to contact relay
		    if (!found) 
		    {
			    ACS_LOG_WARNING("Error contacting " << proxy.soap_endpoint);
                ACS_LOG_INFO("Cannot update status of the following circulation ids:	");
                for (uint i = 0 ; i <map_it->second.size(); ++i )
                {
                	ACS_LOG_INFO(map_it->second[i].second.Id << " - ");
                }
                //out->ErrorCode = pds__ErrorCodeEnum__3;
		    }
        	++map_it;
        }
    }   
    // if all updating are successfully, error code is OK
    if (out->CompletedIDs.size() == in->RequestIDs.size())
    {
    	out->ErrorCode = pds__ErrorCodeEnum__0;
    }
    else	//...some updating in error
    {
    	out->ErrorCode = pds__ErrorCodeEnum__99;
    }
}

void CirculationOrderingService::resetMultipleCirculationStatus(pds__MultipleStatusRequestType*    in, pds__MultipleResponseType* out) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// check for mandatory fields
	if(in->RequestIDs.empty())
	{
        ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException("Empty requestid")) ; // PRQA S 3081
		return;
	}
    
	// reload configuration params
	try {
		loadConf();
	}
	catch(LocalhostNotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Host not found: " << e.what()) ; 
	}
	
    vector<dbPersistent::IdType> circulationIds ;
	size_t reqSize = in->RequestIDs.size();
	for( size_t i = 0; i < reqSize; i++ ) {
    	istringstream is(in->RequestIDs[i]) ;
		dbPersistent::IdType circ = 0 ;
    	is >> circ ;
		circulationIds.push_back(circ);
	}
	dbConnectionWrapper pw(_connPool);
	dbConnection &conn = pw ; // PRQA S 3050
	dbTemporaryTable ttable(conn) ; 
	string ttableName = ttable.createTable(circulationIds) ; // create and populate temp.table

    const string & dbKey = _connPool.getKey() ;
	SQLString queryString(dbKey, SQLString::SELECT) ;
    	queryString.addTableList("t_circulationsqueue", "t_c") ;
        queryString.addSelectList("t_c.id");
        queryString.addSelectList("t_c.status_id");
        queryString.addSelectList("t_c.relay_host_id");
        queryString.addSelectList("t_c.relay_activity_id");
		queryString.addSelectList("t_c.modificationdate");
		queryString.addSelectList("tt.e");		// temporary table element
        
        queryString.addOrderByList("relay_host_id", SQLString::ASC );
	// Add Join
	{
        dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "t_c", "id", dbParam::EQUAL_TO , "tt.e" )) ;
	    queryString.addJoinList(SQLDialect::RIGHT, ttableName+" tt", pp) ;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following query\n" << queryString.getSQL() );
	dbQuery queryStatus(conn, queryString, "DC SEL Request");
	
	dbSet set ;
	queryStatus.execForSet(set) ;
    
    dbRow row(dbKey);
    
    map<dbPersistent::IdType, vector<pair<dbPersistent::IdType, pds__Outcome> > > relIdVsCircIds; // relayid --> vector <activity_id, (circid, modificationDate)>
	while( set.getNextRow(row) ) {

		unsigned short col = 0;
		
    	bool idNull = false ;
		dbPersistent::IdType circid = 0 ; 
    	row.column(col++, circid, &idNull);
   		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "circId: " << circid) ; 
		
    	// get circulation status id
        bool isSet(false);
		short status_id = 0 ;
    	row.columnSet(col++, status_id, isSet);
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "status_id: " << boolalpha << isSet << " : " << status_id) ; 
		
		dbPersistent::IdType relayid = 0 ; 
    	bool relayidNull = false ;
    	row.column(col++, relayid, &relayidNull);
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relayidNull: " << boolalpha << relayidNull << " relayid: " << relayid) ; 
		
    	dbPersistent::IdType activityid = 0 ;
    	bool activityidNull = false ;
    	row.column(col++, activityid, &activityidNull); // PRQA S 4400
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "activityidNull: " << boolalpha << activityidNull << " activityid: " << activityid) ; 
    
    	// get modification date
    	bool dateNull = false ;
		string modificationDate;
    	row.column(col++, modificationDate, &dateNull);
        
		dbPersistent::IdType reqId = 0 ; // request circulation id
    	row.column(col++, reqId);
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "required circulation id: " << reqId) ; 
        pds__Outcome * recordInfo = new pds__Outcome;
        
        if (!isSet) // circulation record not found
        {
    	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id " << reqId << " not found in circulation queue.") ; 
		    // save circulation info
		    ostringstream os;
		    os << reqId;
		    recordInfo->Id = os.str();
		    recordInfo->Status = pds__ErrorCodeEnum__8;	// record not found error code returned
            out->List_USCOREof_USCOREOutcomes.push_back(recordInfo);
        }
        else
        {
		    string reqStatus = Status::instance(dbKey)->name(status_id) ;
    	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id " << circid << " status= " << reqStatus) ; 
		    pds__Outcome *recInfo = new pds__Outcome;
		    // save circulation info
		    ostringstream os;
		    os << circid;
		    recInfo->Id = os.str();
		    recInfo->ModificationDate = modificationDate;
    	    if ( relayidNull || (relayid == 0) ) { // no relay (circulated from this host) 

			    if( Status::error == reqStatus) {
            	    try
                    {
		     	        updateCirculationStatus(circid, Status::eligible);
                        // successful status updating...
                        recInfo->Status = pds__ErrorCodeEnum__0;
                        out->List_USCOREof_USCOREOutcomes.push_back(recInfo);
    	    			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id " << circid << " status updated in circulation queue.") ; 
                    }
                    catch (exException & e)
                    {
                        recInfo->Status = pds__ErrorCodeEnum__9; // error updating db
                	    // no status updating performed...
                        out->List_USCOREof_USCOREOutcomes.push_back(recInfo);
    	    			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id " << circid << " status not updated in circulation queue.") ; 
                        
                    }
			    }
                else // status not in error (not expected)
                {
                    recInfo->Status = pds__ErrorCodeEnum__99;
            	    // no status updating performed...
                    out->List_USCOREof_USCOREOutcomes.push_back(recInfo);
    	    		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id " << circid << " status not updated in circulation queue.") ; 
                }
		    }
		    else	// if a relay was circulating it...
            {
                    // save info into a map, for each relay the pair <activity id, (circid, modificationDate)>
                    relIdVsCircIds[relayid].push_back(make_pair(activityid, *recInfo)); 
		    }
        }
	
	} // while( set.getNextRow(row) )

	// handle relays
    if (relIdVsCircIds.size() != 0)
    {    
    	map<dbPersistent::IdType, vector < pair < dbPersistent::IdType, pds__Outcome> > >::iterator map_it = relIdVsCircIds.begin();
        // loop on relays
        while (map_it != relIdVsCircIds.end())
        {
	    	map<dbPersistent::IdType, pds__Outcome > actIdVsCircId; // activity_id --> (circid, modificationDate, Status)
    
        	dbPersistent::IdType relayid = map_it->first;
    	    pair<string, string> relayInfo ;
    	    string rIpAddress ;
    	    try {
        	    relayInfo = getRelayInfo(relayid) ;
        	    string rHostName = relayInfo.first ;
	    	    rIpAddress = relayInfo.second ;
        	    //should never happen..
        	    ACS_COND_THROW(rIpAddress.empty(),CriticalException("Cannot resolve relay ip address for hostname: " + rHostName ) ) ; // PRQA S 3081
    	    }
    	    catch(exception &ex) {
        	    ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException(ex,"Cannot update status for requests on relay : " + rIpAddress)) ; // PRQA S 3081
			    continue;
    	    }

		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to update status to relay: \"" << rIpAddress << "\" corresponding to relay id " << relayid);
		    // fill the new Status request
		    PDS_USCOREOrderingBindingProxy proxy;
		    pds__MultipleStatusRequestType newRequest;
        	for (uint i = 0; i < map_it->second.size(); ++i)
            {
                ostringstream osaid ; 
    	        osaid <<  map_it->second[i].first;
		        newRequest.RequestIDs.push_back(osaid.str()) ;
            	actIdVsCircId[map_it->second[i].first] = map_it->second[i].second;
				ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Element #" << i << ": activity_id=" << map_it->second[i].first << ' ' << map_it->second[i].second); 

            }
    	    
		    // fill the proxy endpoint
		    string endpointString;
		    {
			    ostringstream endpointStream ;
			    endpointStream << "http://" << rIpAddress << ":" << _localPort ;
			    endpointString = endpointStream.str();
		    }
		    proxy.soap_endpoint = endpointString.c_str();
            proxy.send_timeout = _sendTimeout ; // PRQA S 3000 3
            proxy.accept_timeout = _acceptTimeout ;
            proxy.recv_timeout = _recvTimeout ;

		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to contact relay: \"" << proxy.soap_endpoint << "\"");
			
            uint count(0);
            bool found = false;
            pds__MultipleResponseType relayResponse;
		    while ((count< _maxSoapRetry) && !found)
		    {
			    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "retry =  " << count );
                    int soap_error_code = proxy.ResetMultipleCirculationStatus(&newRequest, &relayResponse);
					if ( soap_error_code == SOAP_OK) 
					{
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relay contacted: \"" << proxy.soap_endpoint << "\"");
                        found = true;
                    }
                    else
                    {
                    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Error contacting " << proxy.soap_endpoint);
                        count++;
					    Timer::delay(1000); // PRQA S 4400
                    }

		    }
            // set global response
            size_t l_size = relayResponse.List_USCOREof_USCOREOutcomes.size() ;
   		    for (uint i = 0; i < l_size ; ++i) // loop on IDs
            {
                // save activityId ( = circulation id on relay)
                dbPersistent::IdType actId = atoll((relayResponse.List_USCOREof_USCOREOutcomes[i])->Id.c_str());
				ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Element #" << i << ": activity_id=" << actId );

                if (actIdVsCircId.find(actId) != actIdVsCircId.end())
                {
					pds__Outcome *circInfo = new pds__Outcome;
			        *circInfo = actIdVsCircId[actId];
					ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "activity_id=" << actId << " found in map; " << *circInfo  );
                    circInfo->Status = relayResponse.List_USCOREof_USCOREOutcomes[i]->Status;
                    if ((relayResponse.List_USCOREof_USCOREOutcomes[i])->Status == pds__ErrorCodeEnum__0)
                    {
					    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Relay \"" << proxy.soap_endpoint << "\": request successfully updated to " << Status::eligible);
                        try
                        {
                            // update status of circulator
                            updateCirculationStatus(atoi(circInfo->Id.c_str()), Status::eligible) ;
							ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id " << circInfo->Id << " status succesfully updated in circulation queue.") ; 
                        }
                        catch (exException & e)
                        {
                            circInfo->Status = pds__ErrorCodeEnum__9; // error updating db
    	    				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id " << circInfo->Id << " status not updated in circulation queue.") ; 
                        }
                    }
                    // NOTE: if relay record is not found
                    else if (relayResponse.List_USCOREof_USCOREOutcomes[i]->Status == pds__ErrorCodeEnum__8)
                    {
                 		ACS_LOG_INFO("Relay " << proxy.soap_endpoint << "\": request " << actId << " not found. Circulation record with id " << circInfo->Id.c_str() << " will be removed for db." );
                        dbSmartPersistent theDsItem("t_circulationsqueue", conn) ;
						theDsItem.setId(atoi(circInfo->Id.c_str()));
                        theDsItem.remove(conn);
                    }
                    else { 
						ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id " << circInfo->Id << " status not updated in circulation queue.") ; 
					} 
                    
                    
                    out->List_USCOREof_USCOREOutcomes.push_back(circInfo);
					ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, *circInfo << " just put in List_USCOREof_USCOREOutcomes vector of output struct");
                }
            }
		    //  it is impossible to contact relay
		    if (!found) 
		    {
			    ACS_LOG_WARNING("Error contacting " << proxy.soap_endpoint);
                ACS_LOG_INFO("Cannot update status of the following circulation ids:	");
                for (uint i = 0 ; i <map_it->second.size(); ++i )
                {
 	              	ACS_LOG_INFO(map_it->second[i].second.Id << " - ");
				    pds__Outcome *circInfo = new pds__Outcome;
                    circInfo->Id = map_it->second[i].second.Id;
                    circInfo->Status = pds__ErrorCodeEnum__3;
                    
 					out->List_USCOREof_USCOREOutcomes.push_back(circInfo);
                }
		    }
        	++map_it;
        }
    } 
	
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		size_t vSize = out->List_USCOREof_USCOREOutcomes.size();
		for( size_t i = 0;  i < vSize;  ++i ) {
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "List_USCOREof_USCOREOutcomes[" << i << "]: pointer address=" << (out->List_USCOREof_USCOREOutcomes[i]) << "; " << *(out->List_USCOREof_USCOREOutcomes[i]) );
		}
    ACS_CLASS_END_DEBUG
}

void CirculationOrderingService::reInsertCirculation(pds__StatusRequestType* in, pds__CirculateResponseType* out) // PRQA S 4020
{
	// check for mandatory fields
	if(in->RequestID == "")
	{
        ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException("Empty requestid")) ; // PRQA S 3081
		return;
	}
    
	// reload configuration params
	try {
		loadConf();
	}
	catch(LocalhostNotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Host not found: " << e.what()) ; 
	}
	
    dbPersistent::IdType localCirculationId = 0 ;
    istringstream is(in->RequestID) ;
    is >> localCirculationId ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Request Id: " << localCirculationId) ; 
    const string & dbKey = _connPool.getKey() ;
	SQLString queryString(dbKey, SQLString::SELECT) ;
    	queryString.addTableList("t_circulationsqueue") ;
        queryString.addSelectList("fromUrl");
        queryString.addSelectList("toUrl");
    {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","id", dbParam::EQUAL_TO, dbConv(dbKey, localCirculationId))); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
    }
 	dbSet set;
 	performQueryForSet(queryString, set, "DC SEL Request");
    vector<dbRow> const& rows = set.set();
	size_t num = set.size();		
	if(num != 1) {
    	ostringstream os;
        os << num << " circulation queue records occur with id=" << localCirculationId << ". 1 expected." << endl;
        ACS_THROW(CriticalException(os.str())); // PRQA S 3081
    }
		
    string fromUrl;
    rows[0].column(0, fromUrl); 	// get fromUrl
	string toUrl;
    rows[0].column(1, toUrl); 		// get toUrl

	curl::RemoteSite site = curl::RemoteSiteFactory::makeSite(toUrl);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RemoteSite: \"" << site.str() << "\"" );

	string path = site.getPath();

	string protocol = site.getProtocol();
	string remoteUrl = protocol + "://" + site.getHost() + "/" + StringUtils::trimBegin(path, '/');
	string remoteUser = site.getUser();
	string remoteUserPassword = site.getPassword();
	if( StringUtils::equalsNoCase( acs::curl::RemoteSite::file, site.getProtocol() ) ) {
		remoteUser = _localUser;
		remoteUserPassword = _localPwd;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Protocol is file: destination url=\"" << remoteUrl << "\" user=\"" << remoteUser << "\" password=\"" << remoteUserPassword << '\"' );
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Complex url \"" << toUrl << "\" decoded as:" 
							<< "\n\tUrl=\"" << remoteUrl << "\"\n\tUser=\"" << remoteUser << "\"\n\tPasswd=\"" << remoteUserPassword << "\"");
	}

	::Circulation::pds__CirculateDynamicRequestType dynRequest;
	dynRequest.RemoteUser         = remoteUser;
	dynRequest.RemoteUserPassword = remoteUserPassword;
	dynRequest.RemoteUrl          = remoteUrl;
	dynRequest.LocalPath          = fromUrl ;

	processCirculateDynamicRequest(&dynRequest, out);
    
}

dbPersistent::IdType CirculationOrderingService::performQueryForStatus(const SQLString & queryString,  // PRQA S 4020
                                                            const string &statQuery, 
                                                            bool firstInsert,
                                                            bool noException)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "executing SQL command : " << queryString.getSQL()  );
    try {
	    dbConnectionWrapper pw(_connPool);
	    dbConnection &conn = pw ; // PRQA S 3050
	    dbQuery query(conn) ;
	    query.sqlString(queryString, statQuery) ;			
	    query.execForStatusRetry(_stopController, 100, _maxDbRetry) ; // PRQA S 4400

	    //return the circulationId inserted
	    if (firstInsert)
	    {
		    return conn.getLastSerial("t_circulationsqueue");
	    }

        return 0 ;
    }
    catch(exception &x) {
        if (noException) { ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, x) ; return 0 ; }
        else { throw ; }
    }
}


void CirculationOrderingService::updateCirculationStatus(dbPersistent::IdType id, const std::string &status, bool noException)
{
    const string & dbKey = _connPool.getKey() ;
	// update status to ELIGIBLE in t_distributionqueue
	SQLString sqlString(dbKey, SQLString::UPDATE ) ;
	sqlString.addTableList("t_circulationsqueue") ;
    int status_id = Status::instance(dbKey)->id(status) ;
	sqlString.addSetList( "status_id", dbConv(dbKey, status_id) ); // PRQA S 3050, 3081
	DateTime now;
	sqlString.addSetList( "modificationdate", db::toSQL(dbKey, now, dbConv::YearToFraction)); 
    {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","id", dbParam::EQUAL_TO, dbConv(dbKey, id))); // PRQA S 3050, 3081
		sqlString.addANDWhereParameter(pp);
    }
    { // this param is to avoid writing the same status twice.
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","status_id", dbParam::DIFFERENT_FROM, dbConv(dbKey, status_id))); // PRQA S 3050, 3081
		sqlString.addANDWhereParameter(pp);
    }

	performQueryForStatus(sqlString, "DC Eligible", false, noException); // is an update on existing
}

#ifdef USELESS_CODE
void CirculationOrderingService::updateCirculationStatus(const vector<dbPersistent::IdType> & id, const std::string &status, bool noException)
{
    const string & dbKey = _connPool.getKey() ;
	// update status to ELIGIBLE in t_distributionqueue
	SQLString sqlString(dbKey, SQLString::UPDATE ) ;
	sqlString.addTableList("t_circulationsqueue") ;
    int status_id = Status::instance(dbKey)->id(status) ;
	sqlString.addSetList( "status_id", dbConv(dbKey, status_id) ); // PRQA S 3050, 3081
    {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","id", dbParam::IN, id)); // PRQA S 3050, 3081
		sqlString.addANDWhereParameter(pp);
    }
    { // this param is to avoid writing the same status twice.
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","status_id", dbParam::DIFFERENT_FROM, dbConv(dbKey, status_id))); // PRQA S 3050, 3081
		sqlString.addANDWhereParameter(pp);
    }

	performQueryForStatus(sqlString, "DC Eligible", false, noException); // is an update on existing
}
#endif


void  CirculationOrderingService::performQueryForSet(const SQLString & queryString, dbSet & set,const string &statQuery)  
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "executing SQL command : " << queryString.getSQL()  );
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "statQuery: " << statQuery  );

	dbConnectionWrapper pw(_connPool);
	dbConnection &conn = pw ; // PRQA S 3050
	dbQuery query(conn) ;
	query.sqlString(queryString, statQuery) ;			
    query.execForSetRetry(_stopController, 100, _maxDbRetry, set); // PRQA S 4400
}

namespace {

    exostream &operator<<(exostream &os, pds__StatusEnum s) // PRQA S 2072, 4222
    {
        switch(s) {
        case pds__StatusEnum__ACCEPTED:
            {
                os << "ACCEPTED" ;
                break ;
            } 
        case pds__StatusEnum__REJECTED: 
            {
                os << "REJECTED" ;
                break ;
            } 
        default:
            {
                os << "invalid <pds__StatusEnum> " << int(s) ; // PRQA S 3081
                break ;
            }
        }
        return os ; // PRQA S 4028
    }

    string toString(pds__ErrorCodeEnum s) // PRQA S 2502, 4020
    {
        switch(s) {
	    case pds__ErrorCodeEnum__0: { return "0" ; }
	    case pds__ErrorCodeEnum__1: { return "1" ; }
	    case pds__ErrorCodeEnum__2: { return "2" ; }
	    case pds__ErrorCodeEnum__3: { return "3" ; }
	    case pds__ErrorCodeEnum__4: { return "4" ; }
	    case pds__ErrorCodeEnum__5: { return "5" ; }
	    case pds__ErrorCodeEnum__6: { return "6" ; }
	    case pds__ErrorCodeEnum__7: { return "7" ; }
	    case pds__ErrorCodeEnum__8: { return "8" ; }
	    case pds__ErrorCodeEnum__9: { return "9" ; }
	    case pds__ErrorCodeEnum__99: { return "99" ; }
        default:
            {
                ostringstream os ;
                os << "invalid <pds__ErrorCodeEnum> " << int(s) ; // PRQA S 3081
                return os.str() ;
            }
        }
    }

    exostream &operator<<(exostream &os, pds__ErrorCodeEnum e) // PRQA S 2072, 4222
    {
        os << toString(e) ; 
        return os ; // PRQA S 4028
    }

}

void CirculationOrderingService::processCirculateExplicitRequest(pds__CirculateExplicitRequestType *  in, pds__CirculateResponseType * out) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start" );
	// check mandatory fields	
	if ((in->LocalPath == "") || (in->DataConsumerID == "") )
	{
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Wrong request: in->LocalPath: \"" << in->LocalPath << "\" in->DataConsumerID: \"" << in->DataConsumerID << "\"");
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning pds__ErrorCodeEnum__1");
		out->ErrorCode = pds__ErrorCodeEnum__1;
		return;
	}

	unsigned int prevHostId = _hostId;
	// reload configuration params
	try {
	    loadConf();
    }
	catch(LocalhostNotFoundException &e)
	{
		if( _hostId != prevHostId ) {
			Application::setStatus(Application::Warning, "Localhost not found: " + string(e.what()) ); // PRQA S 3081
		}
		throw;
	}
	
	if( _hostId != prevHostId ) {
		Application::setStatus(Application::Ok); // PRQA S 3081
	}

    
	if(in->Priority) { _priority = atoi((*(in->Priority)).c_str()); } // PRQA S 3000
	
	string clientId;
	if(in->ClientID) { 
		clientId = *(in->ClientID); 
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ClientID from request=\"" << clientId << '\"');
	}
	
	// retrieve info from DataConsumerID
    const string & dbKey = _connPool.getKey() ;
	
	int localTransferModeId = 0;
	{	
		string tmpmode = _explicitCircLocalTransferMode;
		if( in->LocalTransferMode) { 
			tmpmode =  *(in->LocalTransferMode);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Local Transfer Mode from request=\"" << tmpmode << '\"');
		}
		localTransferModeId = LocalTransferMode::instance(dbKey)->id(tmpmode);
	}
	
	bool removeSource = _removeSource;
	if(in->RemoveSource) { 
		removeSource = *(in->RemoveSource);
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RemoveSource from SOAP IF: " << boolalpha << removeSource );
	}

	bool streamTypeSet = false;
	std::string streamType;
	if (in->StreamType) {
		dbConnectionWrapper cw(_connPool);
		// The streamer was set, but no field streamer_id found on t_circulationsqueue
		ACS_COND_THROW(!dbSchemaMetaDataProvider::hasField("t_circulationsqueue", "streamer_id", cw), 
			CriticalException("The StreamerId has been provided in the soap request, but the streamer_id attribute was not found on t_circulationsqueue."));
			
		streamTypeSet = true;
		streamType = *(in->StreamType);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "StreamType from request=\"" << streamType << "\"");
	}
	
	SQLString queryString(dbKey, SQLString::SELECT) ;

    queryString.addTableList("t_repositories") ;
    queryString.addTableList("t_knownhosts") ;
    queryString.addTableList("t_transferprotocols") ;
    queryString.addSelectList("t_knownhosts.ip_address");  //it could be NULL
    queryString.addSelectList("t_knownhosts.host_name"); 
    queryString.addSelectList("t_knownhosts.id");
    queryString.addSelectList("t_repositories.username");
    queryString.addSelectList("t_repositories.password");
    queryString.addSelectList("t_repositories.remotepath");
	queryString.addSelectList("protocol") ;
	queryString.addSelectList("t_repositories.tempname_value");
    queryString.addSelectList("t_repositories.tempname_id");
    {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_repositories", "host_id", dbParam::EQUAL_TO, "t_knownhosts.id"));
		queryString.addANDWhereParameter(pp);
    }
    {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_repositories", "transferprotocol_id", dbParam::EQUAL_TO, "t_transferprotocols.id"));
		queryString.addANDWhereParameter(pp);
    }
    {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_repositories", "symbolicname", dbParam::EQUAL_TO, dbConv(dbKey, in->DataConsumerID))); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
    }
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following query: " << queryString.getSQL() );
	dbSet set;
	performQueryForSet(queryString, set, "DC SEL Consumer");
	

    if (set.empty())  //no circulation user found
	{
		ACS_LOG_WARNING("no data consumer found for " << in->DataConsumerID  );
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning pds__ErrorCodeEnum__6");

		out->ErrorCode = pds__ErrorCodeEnum__6;
		out->Status = pds__StatusEnum__REJECTED;
		return;
	}
    
    // get all user info
    dbRow row(dbKey);
    set.getRow(1, row);

	unsigned int hostId = 0;
    string destIp ;
    bool ipNull = false ; 
    row.column(0, destIp, &ipNull);
    string userHostName;
    row.column(1, userHostName);
    row.column(2, hostId); // PRQA S 4400
    string username ;
    row.column(3, username); // PRQA S 4400
    string password ;
    row.column(4, password); // PRQA S 4400
    string path ;
    row.column(5, path); // PRQA S 4400
    string protocol ;
    row.column(6, protocol); // PRQA S 4400
	protocol = StringUtils::lowercase(protocol);
	
	string tempnamevalue;
	bool isNull = false ;
    row.column(7, tempnamevalue, &isNull); // PRQA S 4400
	int tempnamemodeId = 0;
	isNull = false ;
    row.column(8, tempnamemodeId, &isNull); // PRQA S 4400
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "tempnamevalue=\"" << tempnamevalue << "\", tempnamemode_id=" << tempnamemodeId);

    if (ipNull) {
        destIp = getIpAddressByName(userHostName);
    }

    //user is configured but cannot resolve ip -> exception
    ACS_COND_THROW(destIp.empty(),CriticalException("Cannot resolve user ip address") ) ; // PRQA S 3081

    // path = path + '/' + in->LocalPath;
	size_t pos = in->LocalPath.rfind('/');
	ACS_COND_THROW( pos == string::npos, CriticalException("Wrong input path format: cannot find '/' character") ) ; // PRQA S 3081
  	path = acs::curl::RemoteSiteUtility::composeUrl(path, in->LocalPath.substr(pos));

#warning MP to be continued
/*
	dbConnectionWrapper pw(_connPool);
	dbConnection &conn = pw ; // PRQA S 3050 
    // check if relay needed
    int secZoneId = circulation::getNeededSecurityZoneId(destIp, _hostLevel, conn) ;

    if (secZoneId > 0 ) { // relay needed
        
    }
*/

    circulation_params cirpar ;
	// make a complete toUrl
    //Michele Mazzei Originale: string toUrl = acs::curl::RemoteSiteFactory::toURL(protocol,username, password, destIp, path) ;
    //Michele Mazzei cambiato:
    cirpar.toUrl = acs::curl::RemoteSiteFactory::toURL(protocol,username, password, userHostName, path) ;
	cirpar.destHostIp = destIp ;
	cirpar.destHostId = hostId ;
    cirpar.fromUrl = in->LocalPath ;
    cirpar.tempnamevalue = tempnamevalue ;
    cirpar.tempnameId = tempnamemodeId ;
    cirpar.mode = CirculationMode::explicitMode ;
    cirpar.removeSource = removeSource ;
    cirpar.priority = _priority ;
	cirpar.localTransferModeId = localTransferModeId;
    cirpar.streamType = streamType ;
    cirpar.streamTypeSet = streamTypeSet ;

    dbPersistent::IdType circulationId = insertCirculationQueueRecord(cirpar, set, dbKey, clientId) ;
	if(circulationId > 0) //..ok
	{
	
        ostringstream cidos ; 
        cidos << circulationId ;
        out->RequestID = cidos.str() ;
		out->ErrorCode = pds__ErrorCodeEnum__0;
		out->Status = pds__StatusEnum__ACCEPTED;
	} 
	else  //need a relay
	{
		if(set.empty()) // if no relays found set error code and return
		{
			ACS_LOG_WARNING("Destination IP " << destIp << " belongs to an inaccessible security zone and no relay is found.");
        	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning pds__ErrorCodeEnum__2");

			out->ErrorCode = pds__ErrorCodeEnum__2; // NO RELAYS FOUND
			out->Status = pds__StatusEnum__REJECTED;
			return;
		}

		// make a complete fromUrl
		string fromUrl = acs::curl::RemoteSiteFactory::toURL(_localProtocol,_localUser, _localPwd, _host, in->LocalPath) ;

		// fill the new Circulate request
		string rIpAddress;
		string rHostName;
		dbPersistent::IdType rHostId = 0 ;
		//unsigned long port=11111;
		pds__CirculateExplicitRequestType newRequest;

		newRequest.DataConsumerID = in->DataConsumerID;
		newRequest.Priority = in->Priority;
		newRequest.LocalPath = fromUrl;
		newRequest.LocalTransferMode = in->LocalTransferMode;
		newRequest.RemoveSource = &removeSource;

		// loop on all relays found in remotehost security zone
		bool found = false;
        size_t setsize = set.size() ;
        SQLString insertString = getCirculationInsertNoRelay(cirpar, dbKey, in->LocalTransferMode) ;

	    for (size_t p = 0; p < setsize; ++p) 
		{
	       	dbRow row1(dbKey);
        	set.getRow(p + 1, row1); // PRQA S 3084
        	// get the relay ipaddress and rHostId
            bool rIpAddressNull = false ;
            row1.column(0, rIpAddress, &rIpAddressNull);
        	row1.column(1, rHostName);
        	row1.column(2, rHostId); // PRQA S 4400

            if (rIpAddressNull) {
                rIpAddress = getIpAddressByName(rHostName);
            }
            if(rIpAddress.empty()) { //cannot resolve relay ipaddress
                continue; //try next
            }

			// fill the proxy endpoint
		    PDS_USCOREOrderingBindingProxy proxy;
			string endpointString;
			{
				ostringstream endpointStream ;
				endpointStream << "http://" << rIpAddress << ":" << _localPort;
				endpointString = endpointStream.str();
			}
			proxy.soap_endpoint = endpointString.c_str();
            proxy.send_timeout = _sendTimeout ; // PRQA S 3000 3
            proxy.accept_timeout = _acceptTimeout ;
            proxy.recv_timeout = _recvTimeout ;

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "try to contact relay \"" << proxy.soap_endpoint << "\"" );
			//  generate a circulation EXPLICIT request 
            for (unsigned int i = 0 ; i< _maxSoapRetry; i++)
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "retry =  " << i );
				pds__CirculateResponseType newResponse;
                int soap_error_code = proxy.CirculateExplicit(&newRequest , &newResponse) ;
				if (soap_error_code == SOAP_OK) 
				{
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relay contacted: \"" << proxy.soap_endpoint << "\"");
					if( newResponse.Status == pds__StatusEnum__ACCEPTED ) {
						// if relay rhostId answer, exit loop
						found = true;
                    	insertString.addValuesList("relay_activity_id" , dbConv(dbKey, newResponse.RequestID)) ; // PRQA S 3050, 3081
					}
					else {
						ACS_LOG_WARNING("Error from contacted relay \"" << proxy.soap_endpoint << "\". Status: " << newResponse.Status << " - error code: " << newResponse.ErrorCode ) ;
					}
					break;
				}
				ostringstream error ;
				error << "Error while contacting relay = \"" << proxy.soap_endpoint << "\" - " ;
				proxy.soap_stream_fault(error) ;
				ACS_LOG_WARNING(error.str()) ;
				Timer::delay(1000); // PRQA S 4400
			}
			if (found) { break; }
			// if the relay is not responding continue to next relay

		}
		
		//  it is impossible to contact any relay 
		if (!found) 
		{
			ACS_LOG_WARNING("No relay can be contacted belonging to security zone of destination IP " << destIp << ".");
        	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning pds__ErrorCodeEnum__3");
			
			out->ErrorCode = pds__ErrorCodeEnum__3; // NO RELAYS CONTACTED
			out->Status = pds__StatusEnum__REJECTED;
			return;
		}

 		// insert new record in t_circulationsqueue 
        	insertString.addValuesList("relay_host_id" , dbConv(dbKey, rHostId)) ; // PRQA S 3050, 3081
		circulationId = performQueryForStatus(insertString, "DC INS Explicit", true ); // first insert of new record

		// APF-273
		// Issue the MC message for the status update
    	{
		    ACS_LOG_MC_INFO (
				mc_metric(Metric_IN) << mc_param("filename", tempnamevalue) << 
				mc_param("queueid", circulationId) << mc_param ("fromurl", fromUrl)
			);
		    
		    if (protocol == "media")
		    {
			    // S2PDGS-1247
			    ACS_LOG_OPEVENTALERTMSG(MediaCirculationMngFunctions::getMcAlertTag(), "", "Media circulation request (#id " << circulationId << ") received." ) ;
		    }
		}

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "New Explicit record inserted: " << circulationId);
		updateCirculationStatus(circulationId, Status::eligible);
        ostringstream cidos ; 
        cidos << circulationId ;
		out->RequestID = cidos.str() ;
		out->ErrorCode = pds__ErrorCodeEnum__0;
		out->Status = pds__StatusEnum__ACCEPTED;
 
	} //relay
	
	string fromUrl = in->LocalPath;
	try {
		fromUrl = acs::curl::RemoteSiteFactory::toURL(acs::curl::RemoteSiteFactory::makeSite(fromUrl, true), false);
	}
	catch( exception &)
	{
		ACS_LOG_WARNING("Wrong LocalPath format: \"" << fromUrl << '\"');
	}
	
	ACS_LOG_INFO ("Accepted a SOAP Explicit request (id# " << out->RequestID << ") FROM " << fromUrl << " TO Data Consumer ID [" << in->DataConsumerID << "]" );
}




bool CirculationOrderingService::checkIpAddress(const string & ip, string & newIp, unsigned int & hostId, bool insHN) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input: \"" << ip << '\"');
	hostId = 0;
	// check if ipaddress is correct
	string pattern = "\\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)" \
                     "\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)" \
                     "\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)" \
                     "\\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)" ;

	RegExp re(pattern) ;
	if (re.match(ip)) { // input is an IP address
		newIp = ip; 
	}
	else // try just in case was hostname instead of destIp
	{
		const string &dbKey = _connPool.getKey() ;
		//  search inside t_knownhosts
		SQLString queryString(dbKey, SQLString::SELECT) ;
    	queryString.addTableList("t_knownhosts") ;
        queryString.addSelectList("t_knownhosts.ip_address");
        queryString.addSelectList("t_knownhosts.id");
		{
			dbQueryParameters pp(dbKey);
			pp.addParameter(dbParam(dbKey, "t_knownhosts", "host_name", dbParam::EQUAL_TO, dbConv(dbKey, ip))); // PRQA S 3050, 3081
			queryString.addANDWhereParameter(pp);
		}
		dbSet set;
		performQueryForSet(queryString, set, "DC SEL HostIp" );

        bool isNull = true;
        if (not set.empty() ) //found as known host!!
        {  
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, '\"' << ip << "\" host_name found in t_knownhosts records");
            dbRow row(dbKey);
            set.getRow(1, row);

            // get ipaddress
            row.column(0, newIp,&isNull);
            row.column(1, hostId);
        }
	    else if (insHN)	// insert host_name in t_knownhosts
	    {	
		    // insert new record in t_knownhosts
		    SQLString q_str(dbKey, SQLString::INSERT) ;
            q_str.addTableList("t_knownhosts") ;
		    q_str.addValuesList("host_name", db::toSQL(dbKey, ip));
		    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following query: " << q_str.getSQL() );

        	    // execute query
		    dbConnectionWrapper pw(_connPool);
		    dbConnection &conn = pw ; // PRQA S 3050
		    dbQuery getQuery(conn, q_str,"Insert Hostname");
            getQuery.execForStatusRetry(_stopController, 100, _maxDbRetry); // PRQA S 4400
		    hostId = conn.getLastSerial("t_knownhosts");
	    }
        else { /* nothing */ }

        if (isNull || StringUtils::trim(newIp).empty() ) {  //not known host or null ip value or empty string
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, '\"' << ip << "\" ip_address is either NULL or empty string, going to get it");
            newIp = getIpAddressByName(ip);
        }
		else {
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, '\"' << ip << "\" ip_address=\"" << newIp << '\"');
		}
		
        if (newIp.empty()) { 
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"' << ip << "\" ip_address is empty");
            return false; 
        }

	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to return ip: \"" << newIp << "\", host id=" << hostId);
	return true;
}



void CirculationOrderingService::processCirculateDynamicRequest( // PRQA S 4020
    pds__CirculateDynamicRequestType *  in, 
    pds__CirculateResponseType * out)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Start" );
	// check mandatory fields	
	if ((in->LocalPath == "")|| (in->RemoteUrl== "") || (in->RemoteUser== "") )
	{
		out->ErrorCode = pds__ErrorCodeEnum__1;
		out->Status = pds__StatusEnum__REJECTED;
		return;
	}
	// convert remote url in compact format
	acs::curl::RemoteSite site = acs::curl::RemoteSiteFactory::makeSite(in->RemoteUrl,in->RemoteUser,in->RemoteUserPassword,false);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RemoteSite: \"" << site.str() << "\"" );
	if( in->RemoteUserPassword.empty() && not StringUtils::equalsNoCase( acs::curl::RemoteSite::sftp, site.getProtocol() ) ) {
		ACS_LOG_WARNING("Password is empty but protocol is " << site.getProtocol() << " (" << acs::curl::RemoteSite::sftp << " expected)");
		out->ErrorCode = pds__ErrorCodeEnum__1;
		out->Status = pds__StatusEnum__REJECTED;
		return;
	}
	
	const string &dbKey = _connPool.getKey() ;
	
    circulation_params cirpar ;
	{
		string tmp = acs::curl::RemoteSiteFactory::toURL(site);
		cirpar.toUrl = acs::curl::RemoteSiteUtility::composeUrl(File::getDirPath(tmp), File::getFileName(tmp));	
	}

	unsigned int prevHostId = _hostId;
	// reload configuration params
	try {
	    loadConf();
    }
	catch(LocalhostNotFoundException &e)
	{
		if( _hostId != prevHostId ) {
			Application::setStatus(Application::Warning, "Localhost not found: " + string(e.what()) ); // PRQA S 3081
		}
		throw;
	}
	
	if( _hostId != prevHostId ) {
		Application::setStatus(Application::Ok); // PRQA S 3081
	}
	
	if(in->Priority) { _priority = atoi((*(in->Priority)).c_str()); } // PRQA S 3000

	int tempnamemodeId=0;
	string localTempnameValue = _tempnameValue;
	string localTempnameMode = _tempnameMode;

	{
		tempnamemodeId = TmpNameStrategyTable::instance(dbKey)->id(localTempnameMode);
		if( in->TempnameMode && in->TempnameValue && not in->TempnameValue->empty() ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Tempnamemode from SOAP IF=\"" << *(in->TempnameMode) << "\"" );
			try {
				tempnamemodeId = TmpNameStrategyTable::instance(dbKey)->id(*(in->TempnameMode));
				localTempnameMode = *(in->TempnameMode);
				localTempnameValue = *(in->TempnameValue);
    		} catch (const std::exception & ex) {
				ACS_LOG_WARNING("Tempnamemode from SOAP IF=\"" << *(in->TempnameMode) << "\" not according to table t_circulationtempnames content." 
								<< " Taking from configuration Tempnamemode=\"" << _tempnameMode << "\", Tempnamevalue=\"" << _tempnameValue << "\"");
    		}
		}
	}
	
	bool removeSource = _removeSource;
	if(in->RemoveSource) { 
		removeSource = *(in->RemoveSource);
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RemoveSource from SOAP IF: " << boolalpha << removeSource );
	}
	
	bool streamTypeSet = false;
	std::string streamType;
	if (in->StreamType) {
		dbConnectionWrapper cw(_connPool);
		// The streamer was set, but no field streamer_id found on t_circulationsqueue
		ACS_COND_THROW(!dbSchemaMetaDataProvider::hasField("t_circulationsqueue", "streamer_id", cw), 
			CriticalException("The StreamerId has been provided in the soap request, but the streamer_id attribute was not found on t_circulationsqueue."));
		
		streamTypeSet = true;
		streamType = *(in->StreamType);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "StreamType from request=\"" << streamType << "\"");
	}

	string clientId;
	if(in->ClientID) { 
		clientId = *(in->ClientID); 
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ClientID from request=\"" << clientId << '\"');
	}
	
	string localTransferMode = _dynamicCircLocalTransferMode;
	if( in->LocalTransferMode) { 
		localTransferMode =  *(in->LocalTransferMode);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Local Transfer Mode from request=\"" << localTransferMode << '\"');
	}
	cirpar.localTransferModeId = LocalTransferMode::instance(dbKey)->id(localTransferMode);


	// check the ipaddress
	string hostIp; 
    unsigned int hostId = 0; //filled if host is known (t_knownhost)

	if (!checkIpAddress(site.getHost(), hostIp, hostId, true))
	{
		// if check fails -> exit in error
		ACS_LOG_WARNING("Cannot get IP address of \"" << site.getHost() << "\" host");
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning pds__ErrorCodeEnum__6");
		
		out->ErrorCode = pds__ErrorCodeEnum__6;
		out->Status = pds__StatusEnum__REJECTED;
		return;
	}
    
    cirpar.destHostIp = hostIp ;
    cirpar.destHostId = hostId ;
    cirpar.fromUrl = in->LocalPath ;
    cirpar.mode = CirculationMode::dynamicMode ;
    cirpar.tempnamevalue = localTempnameValue ;
    cirpar.tempnameId = tempnamemodeId ;
    cirpar.removeSource = removeSource ;
    cirpar.priority = _priority ;
    cirpar.streamType = streamType ;
    cirpar.streamTypeSet = streamTypeSet ;

    dbSet set;
    dbPersistent::IdType circulationId = insertCirculationQueueRecord(cirpar, set, dbKey, clientId) ;
	
	
	if(circulationId > 0)
	{
		dbConnectionWrapper pw(_connPool);
		dbConnection &conn = pw ; // PRQA S 3050
		if (in->CallbackUrl != NULL && dbSchemaMetaDataProvider::instance(dbKey)->existTable("t_circ_callbacks", conn)) // Db schema compatibility ensured
		{
			try
			{
				// insert in t_circ_callbacks info related to the circulation record just inserted
				SQLString queryString( dbKey, SQLString::INSERT ) ;
				queryString.addTableList ( "t_circ_callbacks" );

				queryString.addValuesList("circ_queue_id", db::toSQL(dbKey,circulationId) ) ;
				queryString.addValuesList("url", db::toSQL(dbKey,*(in->CallbackUrl)) ) ;
				queryString.addValuesList("external_id", db::toSQL(dbKey, *(in->ExternalID))) ;

				dbQuery query(conn, queryString, "INS t_circ_callbacks") ;	
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following query: " << queryString.getSQL() );
				query.execForStatus ();
			}
			catch(exception & e)
			{
				// if the item already exists...update it
				SQLString sqlString ( dbKey, SQLString::UPDATE ) ;
				sqlString.addTableList("t_circ_callbacks") ;
	    		sqlString.addSetList( "url", db::toSQL( dbKey, *(in->CallbackUrl) ) );
    			sqlString.addSetList( "external_id", db::toSQL( dbKey, *(in->ExternalID) ) );
				{
					dbQueryParameters pp(dbKey) ;
		       		pp.addParameter(dbParam(dbKey, "t_circ_callbacks", "circ_queue_id" , dbParam::EQUAL_TO , db::toSQL( dbKey, circulationId ) )) ;
					sqlString.addANDWhereParameter(pp) ;
				}
				dbQuery q(conn, sqlString, "Update t_circ_callbacks" ) ;
				try
				{
					q.execForStatus();
				}
				catch(exception & ex)
				{
					ACS_LOG_INFO("Error setting t_circ_callbacks. No record shall be inserted/updated for circulation id "<< circulationId);
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Exception got: " << ex.what());
				}
				
			}
		}

        ostringstream cidos ; 
        cidos << circulationId ;
        out->RequestID = cidos.str() ;
		out->ErrorCode = pds__ErrorCodeEnum__0;
		out->Status = pds__StatusEnum__ACCEPTED;
	} 
	else 
	{		
        // if no relays found set error code and return
		if(set.empty()) 
		{
			ACS_LOG_WARNING("Destination IP " << hostIp << " belongs to an inaccessible security zone and no relay is found.");
        	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning pds__ErrorCodeEnum__2");

			out->ErrorCode = pds__ErrorCodeEnum__2; // NO RELAYS FOUND

			out->Status = pds__StatusEnum__REJECTED;
			return;
		}
		// make a complete fromUrl
		string fromUrl = acs::curl::RemoteSiteFactory::toURL(_localProtocol,_localUser, _localPwd, _host,  in->LocalPath) ;

		// fill the new Circulate request
		PDS_USCOREOrderingBindingProxy proxy;
		//unsigned long port=11111;
		pds__CirculateDynamicRequestType newRequest;
		newRequest.RemoteUser = in->RemoteUser;
		newRequest.RemoteUserPassword = in->RemoteUserPassword;
		newRequest.RemoteUrl = in->RemoteUrl;
		newRequest.LocalPath = fromUrl;
		newRequest.Priority = in->Priority;
		newRequest.TempnameMode = in->TempnameMode;
		newRequest.TempnameValue = in->TempnameValue;
		newRequest.RemoveSource = in->RemoveSource;
		newRequest.LocalTransferMode = in->LocalTransferMode;
		newRequest.CallbackUrl = in->CallbackUrl;
		newRequest.ExternalID = in->ExternalID;
		

		// loop on all relays found in remotehost security zone
		string rIpAddress;
		string rHostName;
		dbPersistent::IdType relayId = 0 ;
		bool found = false;
        size_t setsize = set.size() ;
        SQLString insertString = getCirculationInsertNoRelay(cirpar, dbKey, in->LocalTransferMode) ;
		for (size_t p = 0; p < setsize; ++p) 
		{
	       		dbRow row(dbKey);
        		set.getRow(p + 1, row); // PRQA S 3084
        		// get the relay ipaddress and relayId
                bool isNull = false ;
                row.column(0, rIpAddress, &isNull);
                row.column(1, rHostName);
        		row.column(2, relayId); // PRQA S 4400

            if (isNull) {
                rIpAddress = getIpAddressByName(rHostName);
            }
            if(rIpAddress.empty()) { //cannot resolve relay ipaddress
                continue; //try next 
            }

			// fill the proxy endpoint
			string endpointString;
			{
				ostringstream endpointStream ;
				endpointStream << "http://" << rIpAddress << ":" << _localPort ;
				endpointString = endpointStream.str();		
			}
			proxy.soap_endpoint = endpointString.c_str();
            proxy.send_timeout = _sendTimeout ; // PRQA S 3000 3
            proxy.accept_timeout = _acceptTimeout ;
            proxy.recv_timeout = _recvTimeout ;

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "try to contact relay \"" << proxy.soap_endpoint << "\"");
			//  generate a circulation DYNAMIC request 
			for (unsigned int i = 0 ; i < _maxSoapRetry; i++)
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "retry = " << i  );
				pds__CirculateResponseType newResponse;
                int soap_error_code = proxy.CirculateDynamic(&newRequest , &newResponse) ;
				if ( soap_error_code == SOAP_OK) 
				{
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relay contacted: \"" << proxy.soap_endpoint << "\"");
					if( newResponse.Status == pds__StatusEnum__ACCEPTED ) {
						// if relay answer exit loop
						found = true;
        				insertString.addValuesList("relay_activity_id" , dbConv(dbKey, newResponse.RequestID)) ; // PRQA S 3050, 3081
					}
					else {
						ACS_LOG_WARNING("Error from contacted relay \"" << proxy.soap_endpoint << "\". Status: " << newResponse.Status << " - error code: " << newResponse.ErrorCode ) ;
					}
					
					break;
				}
				ostringstream error ;
				error << "Error while contacting relay = \"" << proxy.soap_endpoint << "\" - " ;
				proxy.soap_stream_fault(error) ;
				ACS_LOG_WARNING(error.str());
				Timer::delay(1000); // PRQA S 4400
			}
			if (found) { break; }
				
			// if the relay is not responding continue to next relay

		}
		
		//  it is impossible to contact any relay set error code and return
		if (!found) 
		{
			ACS_LOG_WARNING("No relay can be contacted belonging to security zone of destination IP " << hostIp << ".");
        	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning pds__ErrorCodeEnum__3");

			out->ErrorCode = pds__ErrorCodeEnum__3; // NO RELAYS CONTACTED
			out->Status = pds__StatusEnum__REJECTED;
			return;
		}
 		// insert new record in t_circulationsqueue 
        insertString.addValuesList("relay_host_id" , dbConv(dbKey, relayId)) ; // PRQA S 3050, 3081
		circulationId = performQueryForStatus(insertString, "DC INS Dynamic", true); // first insert of new record
	
		// APF-273
		// Issue the MC message for the status update
    	{
		    ACS_LOG_MC_INFO( 
				mc_metric(Metric_IN) << mc_param("filename", in->TempnameValue) << 
				mc_param("queueid", circulationId) << mc_param ("fromurl", fromUrl)
			) ;
		    
		    if (site.getProtocol() == "media")
		    {
			    // S2PDGS-1247
			    ACS_LOG_OPEVENTALERTMSG(MediaCirculationMngFunctions::getMcAlertTag(), "", "Media circulation request (#id " << circulationId << ") received." ) ;
		    }
		}

		
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "New Dynamic w/ Relay circulation inserted: " << circulationId);
		updateCirculationStatus(circulationId, Status::eligible);

		// insert in t_circ_callbacks info related to the circulation record just inserted and updated
		dbConnectionWrapper pw(_connPool);
		dbConnection &conn = pw ; // PRQA S 3050
		if (in->CallbackUrl != NULL && dbSchemaMetaDataProvider::instance(dbKey)->existTable("t_circ_callbacks", conn)) // Db schema compatibility ensured
		{
				
			try
			{
				// insert in t_circ_callbacks info related to the circulation record just inserted
				SQLString queryString( dbKey, SQLString::INSERT ) ;
				queryString.addTableList ( "t_circ_callbacks" );

				queryString.addValuesList("circ_queue_id", db::toSQL(dbKey,circulationId) ) ;
				queryString.addValuesList("url", db::toSQL(dbKey,*(in->CallbackUrl)) ) ;
				queryString.addValuesList("external_id", db::toSQL(dbKey, *(in->ExternalID))) ;

				dbQuery query(conn, queryString, "INS t_circ_callbacks") ;	
				query.execForStatus ();
			}
			catch(exception & e)
			{
				// if the item already exists...update it
				SQLString sqlString ( dbKey, SQLString::UPDATE ) ;
				sqlString.addTableList("t_circ_callbacks") ;
    			sqlString.addSetList( "url", db::toSQL( dbKey, *(in->CallbackUrl) ) );
	    		sqlString.addSetList( "external_id", db::toSQL( dbKey, *(in->ExternalID) ) );
				{
					dbQueryParameters pp(dbKey) ;
		    	   	pp.addParameter(dbParam(dbKey, "t_circ_callbacks", "circ_queue_id" , dbParam::EQUAL_TO , db::toSQL( dbKey, circulationId ) )) ;
					sqlString.addANDWhereParameter(pp) ;
				}
				dbQuery q(conn, sqlString, "Update t_circ_callbacks" ) ;
				try
				{
					q.execForStatus();
				}
				catch(exception & ex)
				{
					ACS_LOG_INFO("Error setting t_circ_callbacks. No record shall be inserted/updated for circulation id "<< circulationId);
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Exception got: " << ex.what());
				}
				
			}
		}

        ostringstream cidos ; 
        cidos << circulationId ;
        out->RequestID = cidos.str() ;
		out->ErrorCode = pds__ErrorCodeEnum__0;
		out->Status = pds__StatusEnum__ACCEPTED;
	}
	
	string localPath = in->LocalPath;
	try {
		localPath = acs::curl::RemoteSiteFactory::toURL(acs::curl::RemoteSiteFactory::makeSite(localPath, true), false);
	}
	catch( exception &)
	{
		ACS_LOG_WARNING("Wrong LocalPath format: \"" << localPath << '\"');
	}
	
	string remoteUrl = in->RemoteUrl;
	try {
		remoteUrl = acs::curl::RemoteSiteFactory::toURL(acs::curl::RemoteSiteFactory::makeSite(in->RemoteUrl,in->RemoteUser,in->RemoteUserPassword, true), false);
	}
	catch( exception &)
	{
		ACS_LOG_WARNING("Wrong RemoteUrl format: \"" << remoteUrl << '\"');
	}
	
	
	ACS_LOG_INFO ("Accepted a SOAP Dynamic request (id# " << out->RequestID << ") FROM " << localPath << " TO " << remoteUrl);
}
void CirculationOrderingService::processCirculateMultiDynamicRequest( // PRQA S 4020
    pds__CirculateMultipleDynamicRequestType *  in, 
    pds__CirculateMultipleResponseType * out)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

    out->CirculateResponse.clear();
    size_t cdr_size = in->CirculateDynamicRequest.size() ;
    for (uint k = 0; k < cdr_size; ++k)
    {
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing request number " << k );
        pds__CirculateDynamicRequestType *	singleReq = in->CirculateDynamicRequest[k];
        pds__CirculateResponseType*		singleRes = new pds__CirculateResponseType;
        try
        {
            ACS_LOG_INFO("processCirculateDynamicRequest for request..." << k );
            processCirculateDynamicRequest(singleReq, singleRes);
		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Response for request number " << k << " ErrorCode " << singleRes->ErrorCode << " Status " << singleRes->Status << " RequestID " << singleRes->RequestID);

	        out->CirculateResponse.push_back(singleRes);
        }
	    catch(dbQuery::QueryExecuteException &e)
	    {
		    ACS_LOG_NOTIFY_EX(e);
		    ACS_LOG_ERROR("Dynamic circulation request rejected: error contacting DB.") ;
            pds__CirculateResponseType * resp = new pds__CirculateResponseType;
		    resp->ErrorCode = pds__ErrorCodeEnum__4; //ERROR CONTACTING DB
		    resp->Status = pds__StatusEnum__REJECTED;
            out->CirculateResponse.push_back(resp);

	    }
	    catch(acs::curl::URLMalformedException &e)
	    {
		    ACS_LOG_NOTIFY_EX(e);
		    ACS_LOG_ERROR("Dynamic circulation request rejected: wrong url format.") ;
            pds__CirculateResponseType * resp = new pds__CirculateResponseType;
		    resp->ErrorCode = pds__ErrorCodeEnum__5; //WRONG URL FORMAT
		    resp->Status = pds__StatusEnum__REJECTED;
            out->CirculateResponse.push_back(resp);

	    }
	    catch(NoSecurityZoneFoundException &e)
	    {
		    ACS_LOG_ERROR("Dynamic circulation request rejected: " << e.what()) ;
            pds__CirculateResponseType * resp = new pds__CirculateResponseType;
		    resp->ErrorCode = pds__ErrorCodeEnum__2; // NO RELAYS FOUND
		    resp->Status = pds__StatusEnum__REJECTED;
            out->CirculateResponse.push_back(resp);
	    }
	    catch(LocalhostNotFoundException &e)
	    {
		    ACS_LOG_ERROR("Local host not found in t_knownhosts table") ;
            pds__CirculateResponseType * resp = new pds__CirculateResponseType;
		    resp->ErrorCode = pds__ErrorCodeEnum__6;
		    resp->Status = pds__StatusEnum__REJECTED;
            out->CirculateResponse.push_back(resp);
	    }
	    catch(exception &e)
	    {
		    ACS_LOG_NOTIFY_EX(e);
		    ACS_LOG_ERROR("Dynamic circulation request rejected.") ;
            pds__CirculateResponseType * resp = new pds__CirculateResponseType;
		    resp->ErrorCode = pds__ErrorCodeEnum__99;
		    resp->Status = pds__StatusEnum__REJECTED;
            out->CirculateResponse.push_back(resp);
	    }
        
    }
}

void CirculationOrderingService::increaseCirculationPriority(pds__IncreaseCirculationPriorityRequestType*    in,	pds__IncreaseCirculationPriorityResponseType*   out) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB );
	
	out->RecordID = "0";
	out->RecordStatus = "";
	out->Priority = "0";
	out->EndDate = "";
	out->Circulating = false;
	
	if( in->RecordID ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input record Id=" << *(in->RecordID) );
	}
	if( in->RemoteUrl ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input remote url=\"" << *(in->RemoteUrl) << "\"");
	}
	if( in->RemoteUser ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input remote user=\"" << *(in->RemoteUser) << "\"");
	}
	if( in->RemoteUserPassword ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input remote password=\"" << *(in->RemoteUserPassword) << "\"");
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Priority plain order: " << boolalpha << in->PriorityPlainOrder);
	
	// check mandatory fields
	if( !in->RecordID && ( !in->RemoteUrl || !in->RemoteUser || !in->RemoteUserPassword ) ) {
		string errStr("CirculationOrderingService::increaseCirculationPriority: wrong input");
		ACS_LOG_ERROR(errStr);
		out->ErrorCode = pds__ErrorCodeEnum__1;
		out->Status = pds__StatusEnum__REJECTED;
		return;
	}
	
	// convert remote url in compact format
	string toUrl;
	if( !in->RecordID ) {
		
		if( *(in->RemoteUrl) == "" || *(in->RemoteUser) == "" || *(in->RemoteUserPassword) == "" ) {
			string errStr("CirculationOrderingService::increaseCirculationPriority: wrong input");
			ACS_LOG_ERROR(errStr);
			out->ErrorCode = pds__ErrorCodeEnum__1;
			out->Status = pds__StatusEnum__REJECTED;
			return;
		}
		acs::curl::RemoteSite site = acs::curl::RemoteSiteFactory::makeSite(*(in->RemoteUrl), *(in->RemoteUser), *(in->RemoteUserPassword) ,false);
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RemoteSite: \"" << site.str() << "\"" );
		
		string siteUrl = acs::curl::RemoteSiteFactory::toURL(site);
		toUrl = acs::curl::RemoteSiteUtility::composeUrl(File::getDirPath(siteUrl), File::getFileName(siteUrl));
   		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "toUrl: \"" << toUrl << "\"" );
	}
    
	// reload configuration params
	try {
		loadConf();
	}
	catch(LocalhostNotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Host not found: " << e.what()) ; 
	}
	const string &dbKey = _connPool.getKey() ;
	
	SQLString queryString(dbKey, SQLString::SELECT) ;
    queryString.addTableList("t_circulationsqueue") ;
    queryString.addSelectList("id");
    queryString.addSelectList("status_id");
    queryString.addSelectList("priority");
	queryString.addSelectList("enddate");
    queryString.addSelectList("relay_host_id");
    queryString.addSelectList("relay_activity_id");
	string queryLabel;
    if( in->RecordID ) {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","id", dbParam::EQUAL_TO, db::toSQL(dbKey, *(in->RecordID)) )); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
		queryLabel = "DC SEL id";
    }
	else {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","tourl", dbParam::EQUAL_TO, db::toSQL(dbKey, toUrl))); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
		queryString.addOrderByList( "modificationdate", SQLString::DESC );
		queryLabel = "DC SEL toUrl";
	}
	
	dbSet set;
	performQueryForSet(queryString, set, queryLabel);
	
	if( set.empty() ) {
		
		ostringstream msg;
		msg << "No t_circulationsqueue record found with ";
		if( in->RecordID ) {
			msg << "id=" << *(in->RecordID) ;
		}
		else {
			msg << "tourl=\"" << toUrl << "\"";
		}
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, msg.str());
 		out->ErrorCode = pds__ErrorCodeEnum__7; // no data found
		out->Status = pds__StatusEnum__REJECTED;
		
		return;
	}
	
    dbRow row(dbKey);
    set.getRow(1, row);

	uint16_t selId = 0;
	dbPersistent::IdType circId = 0;
    row.column(selId++, circId);
	{
		ostringstream os ; 
        os << circId ;
        out->RecordID = os.str() ;
	}
	
    // get circulation status
	short status_id = 0 ;
    row.column(selId++, status_id);

	int priority = 0;
    row.column(selId++, priority);
	{
		ostringstream os ; 
        os << priority ;
        out->Priority = os.str() ;
	}
	
	
	bool endDateNull = false;
	string endDate;
	row.column(selId++, endDate, &endDateNull);
	if( !endDateNull ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "end date: \"" << endDate << "\"") ; 
		out->EndDate = endDate;
	}
	
	dbPersistent::IdType relayid = 0 ; 
    bool relayidNull = false ;
    row.column(selId++, relayid, &relayidNull);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relayidNull: " << boolalpha << relayidNull << " relayid: " << relayid) ; 
	
    dbPersistent::IdType activityid = 0 ;
    bool activityidNull = false ;
    row.column(selId++, activityid, &activityidNull); // PRQA S 4400
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "activityidNull: " << boolalpha << activityidNull << " activityid: " << activityid) ; 
    
	string reqStatus = Status::instance(dbKey)->name(status_id) ;
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Circulation queue with id=" << circId << " has status " << reqStatus) ; 
	out->RecordStatus = reqStatus;
	if( Status::completed == reqStatus ) {
		
		out->ErrorCode = pds__ErrorCodeEnum__0;
		out->Status = pds__StatusEnum__ACCEPTED;
		
	}
	else if( Status::error == reqStatus || Status::aborted == reqStatus ) {
		
		out->ErrorCode = pds__ErrorCodeEnum__99;
		out->Status = pds__StatusEnum__REJECTED;
		ACS_LOG_WARNING("Cannot increase priority to t_circulationsqueue record with toUrl \"" << toUrl << "\" (id=" << out->RecordID << ") since it is in " << reqStatus << " status");
		
	}
	else {
		// update priority
		if( relayidNull || (relayid == 0) )  { // no relay: circulated from this host
			
			out->Circulating = true;

			try {
				SQLString sqlString(dbKey, SQLString::UPDATE ) ;
				sqlString.addTableList("t_circulationsqueue") ;
				
				int newPriority = 0 ;
				if( in->PriorityPlainOrder ) {
					newPriority = priority + 10; // PRQA S 4400
				}
				else {
					newPriority = priority - 10; // PRQA S 4400 
					if( newPriority < 1 ) {
						newPriority = 1;
					}
				}
				
				sqlString.addSetList( "priority", dbConv(dbKey, newPriority) ); // PRQA S 3050, 3081
    			{
					dbQueryParameters pp(dbKey);
					pp.addParameter(dbParam(dbKey, "t_circulationsqueue","id", dbParam::EQUAL_TO, dbConv(dbKey, circId))); // PRQA S 3050, 3081
					sqlString.addANDWhereParameter(pp);
    			}

				performQueryForStatus(sqlString, "DC Update priority", false); // is an update on existing
				{
					ostringstream os ; 
        			os << newPriority ;
        			out->Priority = os.str() ;
				}
				
 				out->ErrorCode = pds__ErrorCodeEnum__0;
				out->Status = pds__StatusEnum__ACCEPTED;

			}
			catch( exception &e )
			{
				ACS_LOG_NOTIFY_EX(e);
				ostringstream err;
				err << "CirculationOrderingService::increaseCirculationPriority: Cannot update priority of t_circulationsqueue record with id=" << circId  ;
				ACS_LOG_ERROR(err.str());
				out->ErrorCode = pds__ErrorCodeEnum__4;
				out->Status = pds__StatusEnum__REJECTED;

			}
			
		}
		else {
    		pair<string, string> relayInfo ;
    		string rIpAddress ;
    		try {
        		relayInfo = getRelayInfo(relayid) ;
        		string rHostName = relayInfo.first ;
	    		rIpAddress = relayInfo.second ;
        		//should never happen..
        		ACS_COND_THROW(rIpAddress.empty(),CriticalException("Cannot resolve relay ip address for hostname: " + rHostName ) ) ; // PRQA S 3081
    		}
    		catch(exception &ex) {
				ostringstream err;
				err << "Cannot update priority for circulation queue id=" << circId;
       		ACS_LOG_TRYCATCHNOTIFY_EX( CriticalException( ex, err.str() ) ) ; // PRQA S 3081
				out->ErrorCode = pds__ErrorCodeEnum__2; //NO RELAYS FOUND
#warning "Is correct to set status of current record to ERROR?"

				out->Status = pds__StatusEnum__REJECTED;
				return;       
    		}
			
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to ask status to relay: \"" << rIpAddress << "\" for request: " << activityid);
			// fill the new Status request
			PDS_USCOREOrderingBindingProxy proxy;
			pds__IncreaseCirculationPriorityRequestType newRequest;
    		ostringstream osaid ; 
    		osaid << activityid ;
			string recordId = osaid.str() ;
			newRequest.RecordID = &recordId;
			newRequest.PriorityPlainOrder = in->PriorityPlainOrder;

			// fill the proxy endpoint
			string endpointString;
			{
				ostringstream endpointStream ;
				endpointStream << "http://" << rIpAddress << ":" << _localPort ;
				endpointString = endpointStream.str();
			}
			proxy.soap_endpoint = endpointString.c_str();
            proxy.send_timeout = _sendTimeout ; // PRQA S 3000 3
            proxy.accept_timeout = _acceptTimeout ;
            proxy.recv_timeout = _recvTimeout ;

			//  generate a IncreaseCirculationPriority request 
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to contact relay: \"" << proxy.soap_endpoint << "\"");
			for (unsigned int i = 0 ; i< _maxSoapRetry; i++)
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "retry =  " << i );
				pds__IncreaseCirculationPriorityResponseType newResponse;
        		int soap_error_code = proxy.IncreaseCirculationPriority(&newRequest , &newResponse) ;
				if ( soap_error_code == SOAP_OK) 
				{
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relay contacted: \"" << proxy.soap_endpoint << "\"");
					
					out->EndDate = newResponse.EndDate; // this is the real end date
					out->RecordStatus = newResponse.RecordStatus;
					out->ErrorCode = newResponse.ErrorCode;
					out->Status = newResponse.Status;
					out->Circulating = newResponse.Circulating;
					
					if( (out->Status == pds__StatusEnum__ACCEPTED) && (newResponse.ErrorCode == pds__ErrorCodeEnum__0) ) {
						try{

							SQLString sqlString(dbKey, SQLString::UPDATE ) ;
							sqlString.addTableList("t_circulationsqueue") ;
    						int st_id = Status::instance(dbKey)->id(newResponse.RecordStatus) ;
							sqlString.addSetList( "status_id", dbConv(dbKey, st_id) ); // PRQA S 3050, 3081
							sqlString.addSetList( "priority", dbConv(dbKey, newResponse.Priority) ); // PRQA S 3050, 3081
    						{
								dbQueryParameters pp(dbKey);
								pp.addParameter(dbParam(dbKey, "t_circulationsqueue","id", dbParam::EQUAL_TO, dbConv(dbKey, circId))); // PRQA S 3050, 3081
								sqlString.addANDWhereParameter(pp);
    						}

							performQueryForStatus(sqlString, "DC Update Status and priority", false); // is an update on existing
							out->Priority = newResponse.Priority;


						}
						catch(exception &e)
						{
							ACS_LOG_NOTIFY_EX(e);
							ostringstream err;
							err << "CirculationOrderingService::increaseCirculationPriority: From relay \"" << proxy.soap_endpoint << "\" (t_circulationsqueue.id=" << activityid << ") cannot update local record with id=" << circId
								<< " to status=" << newResponse.RecordStatus << " and priority=" << newResponse.Priority;
							ACS_LOG_ERROR(err.str());
						}
					}
						
					return;
				}
				ostringstream error ;
				error << "Error while contacting relay: \"" << proxy.soap_endpoint << "\" - "; 
				proxy.soap_stream_fault( error) ;
				ACS_LOG_WARNING(error.str());

				Timer::delay(1000); // PRQA S 4400
			}
			
			//  it is impossible to contact  relay set errorCode and return
			string err("Returning pds__ErrorCodeEnum__3: no relay connection");
    		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, err) ; 
			out->ErrorCode = pds__ErrorCodeEnum__3;
			out->Status = pds__StatusEnum__REJECTED;
			
		}
		
		
	} //  record status NOT ERROR && NOT COMPLETED
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Returning record id " << out->RecordID) ;

	return;
	
}

void CirculationOrderingService::circulationRecordOccurrence(pds__CirculationOccurrenceRequestType* in, pds__CirculationOccurrenceResponseType* out) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB );
	
	size_t statesNum = in->StatusId.size();
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "url key=\"" << in->UrlKey << "\", value=\"" << in->UrlValue << "\"") ;
		for(size_t i = 0; i < statesNum; i++ ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "status #" << i << ": " << in->StatusId[i]) ;
		}		
    ACS_CLASS_END_DEBUG
	
    
	// reload configuration params
	try {
		loadConf();
	}
	catch(LocalhostNotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Host not found: " << e.what()) ; 
	}
	const string &dbKey = _connPool.getKey() ;
	
	SQLString queryString(dbKey, SQLString::SELECT) ;
    queryString.addTableList("t_circulationsqueue") ;
    queryString.addSelectList("id");
	{
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue", in->UrlKey, dbParam::EQUAL_TO, db::toSQL(dbKey, in->UrlValue) ) ); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
	}
	
	vector<int> circStates;
    for(size_t i = 0; i < statesNum; i++ ) {
		istringstream iss(in->StatusId[i]);
		int statusId = 0 ;
		iss >> statusId;
		circStates.push_back(statusId);
	}
	
	if( !circStates.empty() ) {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","status_id" , dbParam::IN, circStates));
		queryString.addANDWhereParameter(pp);
    }
	
	dbSet set;
	performQueryForSet(queryString, set, "Circulation occurrence");
	
	out->ErrorCode = pds__ErrorCodeEnum__0;
	out->Status = pds__StatusEnum__ACCEPTED;
	if( set.empty() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "No t_circulationsqueue record found") ;
	}
	else {
	
		dbRow row(dbKey);
		while( set.getNextRow(row) ) {
			dbPersistent::IdType circId = 0 ;
			row.column(0, circId);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "t_circulationsqueue record with id=" << circId << " found") ;
			
			ostringstream os;
			os << circId;
			out->RecordID.push_back(os.str());
		}
	}
	
 	return;
	
}

void CirculationOrderingService::processMultipleStatusRequest(::Circulation::pds__MultipleStatusRequestType* in, ::Circulation::pds__MultipleStatusResponseType* out) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB );

	// check for mandatory fields
	if( in->RequestIDs.empty() )
	{
        ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException("Empty request ids")) ; // PRQA S 3081
		out->ErrorCode = pds__ErrorCodeEnum__1;
		return;
	}
    
	// reload configuration params
	try {
		loadConf();
	}
	catch(LocalhostNotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Host not found: " << e.what()) ; 
	}
    vector<dbPersistent::IdType> circulationIds ;
	size_t reqSize = in->RequestIDs.size();
	for( size_t i = 0; i < reqSize; i++ ) {
    	istringstream is(in->RequestIDs[i]) ;
		dbPersistent::IdType circ = 0 ;
    	is >> circ ;
		circulationIds.push_back(circ);
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Going to get status of the following circulation requests:");
	for(size_t i = 0; i < reqSize; i++ ) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id #" << i << ": " << circulationIds[i]) ;
	}		

	dbConnectionWrapper pw(_connPool);
	dbConnection &conn = pw ; // PRQA S 3050

	dbTemporaryTable ttable(conn) ; 
	string ttableName = ttable.createTable(circulationIds) ; // create and populate temp.table
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << ttableName << "\" table populated with " << reqSize << " records");
	
    const string & dbKey = _connPool.getKey() ;
	SQLString queryString(dbKey, SQLString::SELECT) ;
    	queryString.addTableList("t_circulationsqueue") ;
        queryString.addSelectList("id");
        queryString.addSelectList("status_id");
		queryString.addSelectList("modificationdate");
        queryString.addSelectList("relay_host_id");
        queryString.addSelectList("relay_activity_id");
    {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam( dbKey, "t_circulationsqueue","id", dbParam::IN_TABLE , ttableName) ); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
    }
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute SQL command : " << queryString.getSQL()  );

	dbSet set;
	dbQuery query(conn) ;
	query.sqlString(queryString, "DC Multiple SEL Request") ;			
    query.execForSetRetry(_stopController, 100, _maxDbRetry, set); // PRQA S 4400

	size_t num = set.size();		
	if(num != reqSize) {
		ostringstream exMsg;
		exMsg << num << " t_circulationsqueue records retrieved from db: " << reqSize << " expected";
		if( num < reqSize ) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, exMsg.str() );
        	ACS_THROW(MissingDbRecordsException(exMsg.str())); // PRQA S 3081
		}
		else {
		   	ACS_THROW(dbQuery::QueryExecuteException(exMsg.str())); // PRQA S 3081
		}
    }
	
	// explicitely remove temporary table
	ttable.removeTable();

	
	out->ErrorCode = pds__ErrorCodeEnum__0;
    dbRow row(dbKey);
	while( set.getNextRow(row) ) {

		unsigned short col = 0;
		
		dbPersistent::IdType circid = 0 ; 
    	row.column(col++, circid);
		
    	// get circulation status id
		short status_id = 0 ;
    	row.column(col++, status_id);
		
		string modificationDate;
		row.column(col++, modificationDate);

		dbPersistent::IdType relayid = 0 ; 
    	bool relayidNull = false ;
    	row.column(col++, relayid, &relayidNull);
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relayidNull: " << boolalpha << relayidNull << " relayid: " << relayid) ; 
		
    	dbPersistent::IdType activityid = 0 ;
    	bool activityidNull = false ;
    	row.column(col++, activityid, &activityidNull); // PRQA S 4400
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "activityidNull: " << boolalpha << activityidNull << " activityid: " << activityid) ; 
    
		string reqStatus = Status::instance(dbKey)->name(status_id) ;
    	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id " << circid << " status=" << reqStatus) ; 
    	if ( Status::error == reqStatus || Status::completed == reqStatus || Status::aborted == reqStatus || relayidNull || (relayid == 0) ) { // no relay (circulated from this host) or final status
			
			if( Status::error == reqStatus || Status::aborted == reqStatus || Status::completed == reqStatus ) {
				
				pds__RecordInfo *recordInfo = new pds__RecordInfo;
				
				ostringstream os;
				os << circid;
				recordInfo->Id = os.str();
				recordInfo->ModificationDate = modificationDate;
				
				if( Status::error == reqStatus || Status::aborted == reqStatus ) {
					out->ErrorIDs.push_back(recordInfo);
				}
				else { // if( Status::completed == reqStatus )
					out->CompletedIDs.push_back(recordInfo);
				}
			}
			
			continue;
		}

#warning "In next version optimize it by a querying records order by relayid: now it is inefficient!"
		
		// HERE if a relay was circulating it!!
    	pair<string, string> relayInfo ;
    	string rIpAddress ;
    	try {
        	relayInfo = getRelayInfo(relayid) ;
        	string rHostName = relayInfo.first ;
	    	rIpAddress = relayInfo.second ;
        	//should never happen..
        	ACS_COND_THROW(rIpAddress.empty(),CriticalException("Cannot resolve relay ip address for hostname: " + rHostName ) ) ; // PRQA S 3081
    	}
    	catch(exception &ex) {
        	ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException(ex,"Cannot report status for requestid: " + circid )) ; // PRQA S 3081
			continue;
    	}

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to ask status to relay: \"" << rIpAddress << "\" for request: " << activityid);
		// fill the new Status request
		PDS_USCOREOrderingBindingProxy proxy;
		//unsigned long port=11111;
		pds__StatusRequestType newRequest;
    	ostringstream osaid ; 
    	osaid << activityid ;
		newRequest.RequestID = osaid.str() ;

		// fill the proxy endpoint
		string endpointString;
		{
			ostringstream endpointStream ;
			endpointStream << "http://" << rIpAddress << ":" << _localPort ;
			endpointString = endpointStream.str();
		}
		proxy.soap_endpoint = endpointString.c_str();
        proxy.send_timeout = _sendTimeout ; // PRQA S 3000 3
        proxy.accept_timeout = _acceptTimeout ;
        proxy.recv_timeout = _recvTimeout ;

		//  generate a getStatus request 
		bool relayContacted = false;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to contact relay: \"" << proxy.soap_endpoint << "\"");
		for (unsigned int i = 0 ; i< _maxSoapRetry; i++)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "retry =  " << i );
			pds__StatusResponseType newResponse;
        	int soap_error_code = proxy.GetStatus(&newRequest , &newResponse) ;
			if ( soap_error_code == SOAP_OK) 
			{
	        	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relay contacted: \"" << proxy.soap_endpoint << "\"");
				relayContacted = true;
				try{
            		updateCirculationStatus(circid, newResponse.Status) ;
					reqStatus = newResponse.Status;
					ACS_LOG_INFO("Relay \"" << proxy.soap_endpoint << "\": request id " << circid << " successfully updated to " << newResponse.Status);
					DateTime modificationDate;
					
 					try
					{
						// fill the queue of the pending circulations
						CirculationCallbackSingleton::instance()->insertCallbackEventsInQueue (conn, circid, Status::instance(dbKey)->id(reqStatus), modificationDate);	
					}
					catch (std::exception const & ex)
					{
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Callback pending circulation queue cannot be updated!" << ex.what());
					}
					
				}
				catch(exception &e)
				{
					ACS_LOG_NOTIFY_EX(e);
					ACS_LOG_ERROR("Cannot update status of request id " << circid << " to " << newResponse.Status );
				}
				
				break;
			}
			ostringstream error ;
			error << "Error while contacting relay: \"" << proxy.soap_endpoint << "\" - "; 
			proxy.soap_stream_fault(error) ;
			ACS_LOG_WARNING(error.str());

			Timer::delay(1000); // PRQA S 4400
		}
		
		if( !relayContacted ) { //  it is impossible to contact  relay set errorCode and return
    		ACS_LOG_ERROR("No relay connection for request id " << circid << ": impossible to update status from " << reqStatus) ; 
			continue;
		}
		
		if( Status::error == reqStatus || Status::aborted == reqStatus || Status::completed == reqStatus ) {

			pds__RecordInfo *recordInfo = new pds__RecordInfo;
			
			ostringstream os;
			os << circid;
			recordInfo->Id = os.str();
			DateTime now;
			recordInfo->ModificationDate = now.getSqlString();

			if( Status::error == reqStatus || Status::aborted == reqStatus ) {
				out->ErrorIDs.push_back(recordInfo);
			}
			else { // if( Status::completed == reqStatus )
				out->CompletedIDs.push_back(recordInfo);
			}
		}
		
	
	} // while( set.getNextRow(row) )
  
}

void CirculationOrderingService::processMultipleStatusRequest(::Circulation::pds__MultipleStatusRequestType* in, ::Circulation::pds__MultipleDetailedStatusResponseType* out) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB );

	// check for mandatory fields
	if( in->RequestIDs.empty() )
	{
        ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException("Empty request ids")) ; // PRQA S 3081
		out->ErrorCode = pds__ErrorCodeEnum__1;
		return;
	}
    
	// reload configuration params
	try {
		loadConf();
	}
	catch(LocalhostNotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Host not found: " << e.what()) ; 
	}
    vector<dbPersistent::IdType> circulationIds ;
	size_t reqSize = in->RequestIDs.size();
	for( size_t i = 0; i < reqSize; i++ ) {
    	istringstream is(in->RequestIDs[i]) ;
		dbPersistent::IdType circ = 0 ;
    	is >> circ ;
		circulationIds.push_back(circ);
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Going to get status of the following circulation requests:");
	for(size_t i = 0; i < reqSize; i++ ) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id #" << i << ": " << circulationIds[i]) ;
	}		

	dbConnectionWrapper pw(_connPool);
	dbConnection &conn = pw ; // PRQA S 3050

	dbTemporaryTable ttable(conn) ; 
	string ttableName = ttable.createTable(circulationIds) ; // create and populate temp.table
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << ttableName << "\" table populated with " << reqSize << " records");
	
    const string & dbKey = _connPool.getKey() ;
// 	SQLString queryString(dbKey, SQLString::SELECT) ;
//     	queryString.addTableList("t_circulationsqueue") ;
//         queryString.addSelectList("id");
//         queryString.addSelectList("status_id");
// 		queryString.addSelectList("modificationdate");
//         queryString.addSelectList("relay_host_id");
//         queryString.addSelectList("relay_activity_id");
//     {
// 		dbQueryParameters pp(dbKey);
// 		pp.addParameter(dbParam( dbKey, "t_circulationsqueue","id", dbParam::IN_TABLE , ttableName) ); // PRQA S 3050, 3081
// 		queryString.addANDWhereParameter(pp);
//     }
// 	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to execute SQL command : " << queryString.getSQL()  );

	SQLString queryString(dbKey, SQLString::SELECT) ;
    	queryString.addTableList("t_circulationsqueue", "t_c") ;
		queryString.addSelectList("tt.e");		// temporary table element
        queryString.addSelectList("t_c.id");
        queryString.addSelectList("t_c.status_id");
		queryString.addSelectList("t_c.modificationdate");
        queryString.addSelectList("t_c.relay_host_id");
        queryString.addSelectList("t_c.relay_activity_id");
        
        queryString.addOrderByList("relay_host_id", SQLString::ASC );
	// Add Join
	{
        dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "t_c", "id", dbParam::EQUAL_TO , "tt.e" )) ;
	    queryString.addJoinList(SQLDialect::RIGHT, ttableName+" tt", pp) ;
	}

	dbSet set;
	dbQuery query(conn) ;
	query.sqlString(queryString, "DC Multiple SEL Request") ;			
    query.execForSetRetry(_stopController, 100, _maxDbRetry, set); // PRQA S 4400

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following query\n" << queryString.getSQL() );
	dbQuery queryStatus(conn, queryString, "DC SEL Request");
	size_t num = set.size();		
	if(num != reqSize) {
		ostringstream exMsg;
		exMsg << num << " t_circulationsqueue records retrieved from db: " << reqSize << " expected";
		if( num < reqSize ) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, exMsg.str() );
//        	ACS_THROW(MissingDbRecordsException(exMsg.str())); // PRQA S 3081
		}
		else {
		   	ACS_THROW(dbQuery::QueryExecuteException(exMsg.str())); // PRQA S 3081
		}
    }
	
	// explicitely remove temporary table
	ttable.removeTable();

	
	out->ErrorCode = pds__ErrorCodeEnum__0;
    dbRow row(dbKey);
	while( set.getNextRow(row) ) {

		unsigned short col = 0;

		dbPersistent::IdType reqCircid = 0 ; // circulation id of the record searched
    	row.column(col++, reqCircid);
		
        bool isNull(false);
		dbPersistent::IdType circid = 0 ; 
    	row.column(col++, circid, &isNull);
		
    	if (isNull == true)
        {
    	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Circulation id " << reqCircid << " not found ") ; 
        	// circulation record not found
            pds__DetailedStatusResponseType *recordInfo = new pds__DetailedStatusResponseType;

			ostringstream os;
			os << reqCircid;
			recordInfo->Id = os.str();
			recordInfo->ModificationDate = "";
			recordInfo->Status = "";
			recordInfo->RecordFound = false;

            out->List_USCOREof_USCOREStatusResponseType.push_back(recordInfo);
        }
        else
        {
    	    // get circulation status id
		    short status_id = 0 ;
    	    row.column(col++, status_id);

		    string modificationDate;
		    row.column(col++, modificationDate);

		    dbPersistent::IdType relayid = 0 ; 
    	    bool relayidNull = false ;
    	    row.column(col++, relayid, &relayidNull);
    	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relayidNull: " << boolalpha << relayidNull << " relayid: " << relayid) ; 

    	    dbPersistent::IdType activityid = 0 ;
    	    bool activityidNull = false ;
    	    row.column(col++, activityid, &activityidNull); // PRQA S 4400
    	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "activityidNull: " << boolalpha << activityidNull << " activityid: " << activityid) ; 

		    string reqStatus = Status::instance(dbKey)->name(status_id) ;
    	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Request id " << circid << " status=" << reqStatus) ; 
    	    if ( relayidNull || (relayid == 0) ) { // no relay (circulated from this host) 

                pds__DetailedStatusResponseType *recordInfo = new pds__DetailedStatusResponseType;

			    ostringstream os;
			    os << circid;
			    recordInfo->Id = os.str();
			    recordInfo->ModificationDate = modificationDate;
			    recordInfo->Status = reqStatus;
				recordInfo->RecordFound = true;
                
                out->List_USCOREof_USCOREStatusResponseType.push_back(recordInfo);

			    continue;
		    }

		    // HERE if a relay was circulating it!!
    	    pair<string, string> relayInfo ;
    	    string rIpAddress ;
    	    try {
        	    relayInfo = getRelayInfo(relayid) ;
        	    string rHostName = relayInfo.first ;
	    	    rIpAddress = relayInfo.second ;
        	    //should never happen..
        	    ACS_COND_THROW(rIpAddress.empty(),CriticalException("Cannot resolve relay ip address for hostname: " + rHostName ) ) ; // PRQA S 3081
    	    }
    	    catch(exception &ex) {
        	    ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException(ex,"Cannot report status for requestid: " + circid )) ; // PRQA S 3081
			    continue;
    	    }

		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to ask status to relay: \"" << rIpAddress << "\" for request: " << activityid);
		    // fill the new Status request
		    PDS_USCOREOrderingBindingProxy proxy;
		    pds__MultipleStatusRequestType newRequest;
    	    ostringstream osaid ; 
    	    osaid << activityid ;
		    newRequest.RequestIDs.push_back(osaid.str()) ;

		    // fill the proxy endpoint
		    string endpointString;
		    {
			    ostringstream endpointStream ;
			    endpointStream << "http://" << rIpAddress << ":" << _localPort ;
			    endpointString = endpointStream.str();
		    }
		    proxy.soap_endpoint = endpointString.c_str();
            proxy.send_timeout = _sendTimeout ; // PRQA S 3000 3
            proxy.accept_timeout = _acceptTimeout ;
            proxy.recv_timeout = _recvTimeout ;

		    //  generate a getMultipleDetailedStatus request for relay
		    bool relayContacted = false;
		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to contact relay: \"" << proxy.soap_endpoint << "\"");
		    for (unsigned int i = 0 ; i< _maxSoapRetry; i++)
		    {
			    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "retry =  " << i );
			    pds__MultipleDetailedStatusResponseType newResponse;
        	    int soap_error_code = proxy.GetMultipleDetailedStatus(&newRequest , &newResponse) ;
			    if ( soap_error_code == SOAP_OK) 
			    {
	        	    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relay contacted: \"" << proxy.soap_endpoint << "\"");
				    relayContacted = true;
				    try{
            		    updateCirculationStatus(circid, newResponse.List_USCOREof_USCOREStatusResponseType[0]->Status) ;
                        pds__DetailedStatusResponseType *recordInfo = new pds__DetailedStatusResponseType;

			            ostringstream os;
			            os << circid;
			            recordInfo->Id = os.str();
			            recordInfo->ModificationDate = newResponse.List_USCOREof_USCOREStatusResponseType[0]->ModificationDate;
			            recordInfo->Status = newResponse.List_USCOREof_USCOREStatusResponseType[0]->Status;
                        recordInfo->RecordFound = true;
                        out->List_USCOREof_USCOREStatusResponseType.push_back(recordInfo);
					    ACS_LOG_INFO("Relay \"" << proxy.soap_endpoint << "\": request id " << circid << " successfully updated to " << newResponse.List_USCOREof_USCOREStatusResponseType[0]->Status);
				    }
				    catch(exception &e)
				    {
					    ACS_LOG_NOTIFY_EX(e);
					    ACS_LOG_ERROR("Cannot update status of request id " << circid << " to " << newResponse.List_USCOREof_USCOREStatusResponseType[0]->Status );
				    }

				    break;
			    }
                else
                {
			        ostringstream error ;
			        error << "Error while contacting relay: \"" << proxy.soap_endpoint << "\" - "; 
			        proxy.soap_stream_fault(error) ;
			        ACS_LOG_WARNING(error.str());

				    Timer::delay(1000); // PRQA S 4400
                }
		    }

		    if( !relayContacted ) { //  it is impossible to contact  relay 
    		    ACS_LOG_ERROR("No relay connection for request id " << circid << ": impossible to update status from " << reqStatus) ; 
			    continue;
		    }
		}
			
	} // while( set.getNextRow(row) )
      
}
void CirculationOrderingService::resetCirculationRetry(::Circulation::pds__ResetRetryRequestType* in, ::Circulation::pds__CirculateResponseType* out) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB );
	
	// check for mandatory fields
	if(in->RequestID.empty())
	{
        ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException("Empty request id")) ; // PRQA S 3081
		out->ErrorCode = pds__ErrorCodeEnum__1;
		out->Status = pds__StatusEnum__REJECTED;
		return;
	}
	
	// reload configuration params
	try {
		loadConf();
	}
	catch(LocalhostNotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Host not found: " << e.what()) ; 
	}
    dbPersistent::IdType localCirculationId = 0 ;
	{
    	istringstream is(in->RequestID) ;
    	is >> localCirculationId ;
	}
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Request Id: " << localCirculationId) ; 
	
    const string & dbKey = _connPool.getKey() ;
	SQLString queryString(dbKey, SQLString::SELECT) ;
    queryString.addTableList("t_circulationsqueue") ;
    queryString.addSelectList("relay_host_id");
    queryString.addSelectList("relay_activity_id");
    {
		dbQueryParameters pp(dbKey);
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","id", dbParam::EQUAL_TO, dbConv(dbKey, localCirculationId))); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
    }
	dbSet set;
	performQueryForSet(queryString, set, "Circulation Request");
	size_t num = set.size();		
	if(num != 1) {
		ostringstream exMsg;
		exMsg << num << " circulation queue records occur with id=" << localCirculationId << ": 1 expected";
        ACS_THROW(dbQuery::QueryExecuteException( exMsg.str() )); // PRQA S 3081
    }
	
    dbRow row(dbKey);
    set.getNextRow(row);
	
	unsigned short selId = 0;
	
	dbPersistent::IdType relayid = 0 ; 
    bool relayidNull = false ;
    row.column(selId++, relayid, &relayidNull);
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relayidNull: " << boolalpha << relayidNull << " relayid: " << relayid) ; 
	
    dbPersistent::IdType activityid = 0 ;
    bool activityidNull = false ;
    row.column(selId++, activityid, &activityidNull); // PRQA S 4400
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "activityidNull: " << boolalpha << activityidNull << " activityid: " << activityid) ; 
    
	if( relayidNull || (relayid == 0) ) { // no relay: circulated from this host
						
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No relay: going to update retries and nextretrydate of t_circulationsqueue record with id=" << localCirculationId) ;
		SQLString sqlString(dbKey, SQLString::UPDATE ) ;
		sqlString.addTableList("t_circulationsqueue") ;
		sqlString.addSetList( "retries", dbConv(dbKey, 0) ); // PRQA S 3050, 3081
	    dbConnectionWrapper pw(_connPool);
	    dbConnection &conn = pw ; // PRQA S 3050
		DateTime now;
		sqlString.addSetList( "nextretrydate", dbConv(dbKey, now, dbColumnTypeHelper::columnToConvDateTimeMode (  // PRQA S 3050
					dbSchemaMetaDataProvider::instance(dbKey)->getDateTimeMode("t_circulationsqueue" ,"nextretrydate", conn) ) ) );
    	{
			dbQueryParameters pp(dbKey);
			pp.addParameter(dbParam(dbKey, "t_circulationsqueue","id", dbParam::EQUAL_TO, dbConv(dbKey, localCirculationId))); // PRQA S 3050, 3081
			sqlString.addANDWhereParameter(pp);
    	}

		performQueryForStatus(sqlString, "Reset circulation queue retry", false); // is an update on existing
		out->ErrorCode = pds__ErrorCodeEnum__0;
		out->Status = pds__StatusEnum__ACCEPTED;
		
	}
	else {
    // here if a relay was circulating it

    	pair<string, string> relayInfo ;
    	string rIpAddress ;
    	try {
        	relayInfo = getRelayInfo(relayid) ;
        	string rHostName = relayInfo.first ;
	    	rIpAddress = relayInfo.second ;
        	//should never happen..
        	ACS_COND_THROW(rIpAddress.empty(),CriticalException("Cannot resolve relay ip address for hostname: " + rHostName ) ) ; // PRQA S 3081
    	}
    	catch(exception &ex) {
        	ACS_LOG_TRYCATCHNOTIFY_EX(CriticalException(ex,"Cannot update reset for request id: " + in->RequestID )) ; // PRQA S 3081
			out->ErrorCode = pds__ErrorCodeEnum__2; //NO RELAYS FOUND
			out->Status = pds__StatusEnum__REJECTED;
			return;       
    	}

		if( activityidNull || (0 == activityid) ) {
			ostringstream exMsg;
			exMsg << "Request id " << in->RequestID << " is circulated by relay \"" << rIpAddress << "\", but activity id is not properly set";
        	ACS_THROW(CriticalException( exMsg.str() ) ) ; // PRQA S 3081
		}

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to contact relay: \"" << rIpAddress << "\" to update reset for request id=" << activityid);
		// fill the new Status request
		pds__ResetRetryRequestType newRequest;
		{
    		ostringstream osaid ; 
    		osaid << activityid ;
			newRequest.RequestID = osaid.str() ;
		}

		// fill the proxy endpoint
		string endpointString;
		{
			ostringstream endpointStream ;
			endpointStream << "http://" << rIpAddress << ":" << _localPort ;
			endpointString = endpointStream.str();
		}
		PDS_USCOREOrderingBindingProxy proxy;
		proxy.soap_endpoint = endpointString.c_str();
        proxy.send_timeout = _sendTimeout ; // PRQA S 3000 3
        proxy.accept_timeout = _acceptTimeout ;
        proxy.recv_timeout = _recvTimeout ;
    
		//  generate a resetCirculationRetry request 
		bool contacted = false;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Try to contact relay: \"" << proxy.soap_endpoint << "\"");
		for (unsigned int i = 0 ; i< _maxSoapRetry; i++)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "retry =  " << i );
			pds__CirculateResponseType newResponse;
        	int soap_error_code = proxy.ResetCirculationRetry(&newRequest , &newResponse) ;
			if ( soap_error_code == SOAP_OK) 
			{
	        	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "relay contacted: \"" << proxy.soap_endpoint << "\"");
				out->Status = newResponse.Status;
				out->ErrorCode = newResponse.ErrorCode;
				contacted = true;
				break;
			}
			ostringstream error ;
			error << "Error while contacting relay: \"" << proxy.soap_endpoint << "\" - "; 
			proxy.soap_stream_fault(error) ;
			ACS_LOG_WARNING(error.str());

			Timer::delay(1000); // PRQA S 4400
		}

		if( not contacted ) {
			//  it is impossible to contact  relay set errorCode and return
    		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning pds__ErrorCodeEnum__3: no relay connection") ; 
			out->ErrorCode = pds__ErrorCodeEnum__3;
			out->Status = pds__StatusEnum__REJECTED;
		}

	} // circulated by relay

}


SQLString CirculationOrderingService::getCirculationInsertNoRelay(const circulation_params &p, const string &key, bool insertLocalTransferMode) const  // PRQA S 4214
{
/*
        std::string toUrl ;
        std::string destHostIp ;
        unsigned int destHostId ;
        std::string fromUrl ;
        std::string mode ; 
	    std::string tempnamevalue ;
	    int tempnameId ;
        bool removeSource  ;
	    unsigned int priority ;
*/
    SQLString insertString(key, SQLString::INSERT) ;
    
	// prepare the INSERT SQLString
    insertString.addTableList("t_circulationsqueue") ;
    insertString.addValuesList("fromUrl" , dbConv(key, p.fromUrl)) ; // PRQA S 3050, 3081
    insertString.addValuesList("toUrl" , dbConv(key, p.toUrl)) ; // PRQA S 3050, 3081
    insertString.addValuesList("removesource" , dbConv(key, p.removeSource)) ; // PRQA S 3050, 3081
    insertString.addValuesList("priority" , dbConv(key, p.priority)) ; // PRQA S 3050, 3081
	int circulation_mode_id = CirculationMode::instance(key)->id(p.mode, false);
    insertString.addValuesList("circulationmode_id" , dbConv(key,circulation_mode_id)) ; // PRQA S 3050, 3081
    insertString.addValuesList("host_id" , dbConv(key, _hostId)) ; // PRQA S 3050, 3081
    if (0 != p.destHostId) {
        insertString.addValuesList("dest_host_id" , dbConv(key, p.destHostId)) ; // PRQA S 3050, 3081
    }
	if( not p.tempnamevalue.empty() ) {
		insertString.addValuesList("tempnamevalue" , dbConv(key, p.tempnamevalue)) ; // PRQA S 3050, 3081
	}
	if( 0 != p.tempnameId ) {
		insertString.addValuesList("tempname_id" , dbConv(key, p.tempnameId)) ; // PRQA S 3050, 3081
	}
    if( insertLocalTransferMode ) {
		insertString.addValuesList("localtransfermode_id" , dbConv(key, p.localTransferModeId)) ; // PRQA S 3050, 3081
	}
    return insertString ;
}


dbPersistent::IdType CirculationOrderingService::insertCirculationQueueRecord( // PRQA S 4020
    const circulation_params &p, 
    dbSet & set, 
    const string &dbkey,
	const string & client)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB );


// Invocation of this method will be removed when a class thread devoted to update circulation managed by relays will be implemented
	try {
		// update status of all circulation records with given fromUrl and toUrl
		updateCirculationsStatus( p.fromUrl, p.toUrl, dbkey);
	}
	catch(exception & e)
	{
		string fromUrl = p.fromUrl;
		try {
			fromUrl = acs::curl::RemoteSiteFactory::toURL(acs::curl::RemoteSiteFactory::makeSite(fromUrl, true), false);
		}
		catch( exception &)
		{
			ACS_LOG_WARNING("Wrong LocalPath format: \"" << fromUrl << '\"');
		}
		string toUrl = p.toUrl;
		try {
			toUrl = acs::curl::RemoteSiteFactory::toURL(acs::curl::RemoteSiteFactory::makeSite(toUrl, true), false);
		}
		catch( exception &)
		{
			ACS_LOG_WARNING("Wrong RemoteUrl format: \"" << toUrl << '\"');
		}
							
		ACS_LOG_WARNING("Error trying to get updated status of circulation queue records with fromurl=\"" << fromUrl << "\" and tourl=\"" << toUrl << "\": " << e.what() );
	}
	
	// find if a request identical to the current one already exist
	dbPersistent::IdType circulationId = duplicatedRequestExists( p, dbkey ) ;
	if( circulationId != 0 ) {
		return circulationId;
	}

	// get remote host security level 
	bool isInSecZone = false;
	
	pair<dbPersistent::IdType, int> seczoneinfo =  getHostSecZoneInfo(p.destHostIp, dbkey);
    
    dbPersistent::IdType secZoneId = seczoneinfo.first ;
    int level = seczoneinfo.second ;
    
    //Source Host Security Level + 1 >= Dest Host Security Level
    // 1 internal LAN; increase for new security level 
    //+1 for allowing contact between LAN and DMZ
	if (_hostLevel + 1 >= level) { isInSecZone = true; } //ok. They can comunicate! 

    SQLString insertString(dbkey, SQLString::INSERT) ;
    
	// prepare the INSERT SQLString
    insertString.addTableList("t_circulationsqueue") ;
    insertString.addValuesList("fromUrl" , dbConv(dbkey, p.fromUrl)) ; // PRQA S 3050, 3081
    insertString.addValuesList("toUrl" , dbConv(dbkey, p.toUrl)) ; // PRQA S 3050, 3081
    insertString.addValuesList("removesource" , dbConv(dbkey, p.removeSource)) ; // PRQA S 3050, 3081
    insertString.addValuesList("priority" , dbConv(dbkey, p.priority)) ; // PRQA S 3050, 3081
	// retrieve circulation mode id
	int circulation_mode_id = CirculationMode::instance(dbkey)->id(p.mode, false);
    insertString.addValuesList("circulationmode_id" , dbConv(dbkey, circulation_mode_id)) ; // PRQA S 3050, 3081
    insertString.addValuesList("host_id" , dbConv(dbkey, getAgentNodeId(client))) ; // PRQA S 3050, 3081
    if (0 != p.destHostId) {
        insertString.addValuesList("dest_host_id" , dbConv(dbkey, p.destHostId)) ; // PRQA S 3050, 3081
	}
    if( not p.tempnamevalue.empty() ) {
		insertString.addValuesList("tempnamevalue" , dbConv(dbkey, p.tempnamevalue)) ; // PRQA S 3050, 3081
	}
	if( 0 != p.tempnameId ) {
		insertString.addValuesList("tempname_id" , dbConv(dbkey, p.tempnameId)) ; // PRQA S 3050, 3081
	}
	if (p.streamTypeSet) {
		NameIdAssociator<dbPersistent::IdType> nia("t_streamers", "id", "streamtype", dbkey);
		nia.load();
		dbPersistent::IdType streamerId = nia.id(p.streamType);
		insertString.addValuesList("streamer_id" , dbConv(dbkey, streamerId)) ; // PRQA S 3050, 3081
	}
	insertString.addValuesList( "localtransfermode_id" , dbConv( dbkey, p.localTransferModeId ) ) ; // PRQA S 3050, 3081
	

	if (isInSecZone) //same security zone
	{
		// if remote host is in the same security zone, insert a new record in t_circulationsqueue
		circulationId = performQueryForStatus(insertString, "DC INS " + p.mode, true); // firs insert of new record // PRQA S 4412
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "New " << p.mode << " circulation inserted: " << circulationId);
    	updateCirculationStatus(circulationId, Status::eligible);

		// APF-273
		// Issue the MC message for the status update
    	{

		    ACS_LOG_MC_INFO(
				mc_metric(Metric_IN) << mc_param("filename", p.tempnamevalue) <<
				mc_param("queueid", circulationId) << mc_param ("fromurl", p.fromUrl)
			) ;
		    
		    
		    acs::curl::RemoteSite toUrl = acs::curl::RemoteSiteFactory::makeSite(p.toUrl, true);
		    if (toUrl.getProtocol() == "media")
		    {
			    // S2PDGS-1247
			    ACS_LOG_OPEVENTALERTMSG(MediaCirculationMngFunctions::getMcAlertTag(), "", "Media circulation request (#id " << circulationId << ") received." ) ;
		    }
		}


		return circulationId ;
	}
	else // different security zone; need a relay, if any
	{
        //search a relay
		//select h.ip_address, h.host_name, r.host_id from t_securityzonerelays r, t_knownhosts h where r.host_id = h.id and r.seczone_id = secZoneId ;
		SQLString queryString(dbkey, SQLString::SELECT) ;
    	queryString.addTableList("t_securityzonerelays") ;
    	queryString.addTableList("t_knownhosts") ;
        queryString.addSelectList("t_knownhosts.ip_address"); //it could be null
        queryString.addSelectList("t_knownhosts.host_name"); 
        queryString.addSelectList("t_securityzonerelays.id");
        {
		    dbQueryParameters pp(dbkey);
		    pp.addParameter(dbParam(dbkey, "t_securityzonerelays", "host_id", dbParam::EQUAL_TO, "t_knownhosts.id"));
		    queryString.addANDWhereParameter(pp);
        }

        {
		    dbQueryParameters pp(dbkey);
		    pp.addParameter(dbParam(dbkey, "t_securityzonerelays", "seczone_id", dbParam::EQUAL_TO, dbConv(dbkey, secZoneId))); // PRQA S 3050, 3081
		    queryString.addANDWhereParameter(pp);
        }
		performQueryForSet(queryString, set, "DC SEL RelayHost") ;
	}

	return 0 ;
}


dbPersistent::IdType CirculationOrderingService::duplicatedRequestExists( 
    const circulation_params &p, 
    const string &dbKey)
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB );

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to search occurrence of t_circulationsqueue records in not final status with fromUrl=\"" << p.fromUrl << "\" and toUrl=\"" << p.toUrl << '\"') ;
	
	string fromUrl = p.fromUrl;
	try {
		fromUrl = acs::curl::RemoteSiteFactory::toURL(acs::curl::RemoteSiteFactory::makeSite(fromUrl, true), false);
	}
	catch( exception &)
	{
		ACS_LOG_WARNING("Wrong fromUrl format: \"" << fromUrl << '\"');
	}

	string toUrl = p.toUrl;
	try {
		toUrl = acs::curl::RemoteSiteFactory::toURL(acs::curl::RemoteSiteFactory::makeSite(toUrl, true), false);
	}
	catch( exception &)
	{
		ACS_LOG_WARNING("Wrong toUrl format: \"" << toUrl << '\"');
	}

    SQLString queryString(dbKey, SQLString::SELECT) ;
    queryString.addTableList("t_circulationsqueue") ;
    queryString.addSelectList("id"); 
	
	{
        dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","fromurl", dbParam::EQUAL_TO, db::toSQL(dbKey, p.fromUrl))); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
	}
	
	{
        dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","tourl", dbParam::EQUAL_TO, db::toSQL(dbKey, p.toUrl))); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
	}
	
	{
        dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","circulationmode_id", dbParam::EQUAL_TO, db::toSQL(dbKey, CirculationMode::instance(dbKey)->id(p.mode, false)))); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
	}
	
	{
	// Query records with not final status avoids scan because an index occurs on status_id (instead of querying records not COMPLETED AND not ERROR AND....)	
 		CirculationDBService manager;
		vector<int> notFinalStatus = manager.getNotEndStati(dbKey);
    	notFinalStatus.push_back(Status::instance(dbKey)->id(Status::mediaWait));
    	notFinalStatus.push_back(Status::instance(dbKey)->id(Status::mediaAcknowledge));

        dbQueryParameters pp(dbKey) ;
	    pp.addParameter(dbParam(dbKey, "t_circulationsqueue","status_id" , dbParam::IN , notFinalStatus )) ;
		queryString.addANDWhereParameter(pp);
	}
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Query is: " << queryString.getSQL()) ;
    
	dbSet set ;
	performQueryForSet(queryString, set, "Duplicated request") ;
    size_t setsize = set.size() ;
	dbPersistent::IdType circulationId = 0;
	switch(setsize) {
        case 0:
            {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "No duplicated t_circulationsqueue record found in not final status") ;
                break ;
            } 
        case 1: 
            {
				dbRow row(dbKey);
				set.getNextRow(row); // PRQA S 3084

				unsigned short i = 0;
				row.column(i++, circulationId);
				ACS_LOG_INFO(p.mode << " SOAP request FROM " << fromUrl << " TO " << toUrl << " (id# " << circulationId << ") already exists in not final status: no new request will be inserted");
				
				if( _multipleRequestsResetRetry ) {
					try {
						::Circulation::pds__ResetRetryRequestType resetRetryRequest;
						ostringstream os;
						os << circulationId;
						resetRetryRequest.RequestID = os.str();
						
						::Circulation::pds__CirculateResponseType resetRetryResponse;
						resetCirculationRetry(&resetRetryRequest, &resetRetryResponse);
						if( resetRetryResponse.Status == pds__StatusEnum__REJECTED ) {
						// rejected via SOAP
							ACS_LOG_WARNING("Request of reset of retry of circulation queue record with id=" << circulationId << " has been REJECTED with the error code " << resetRetryResponse.ErrorCode);
						}
						else {
							ACS_LOG_INFO("Retry fields of circulation queue record with id=" << circulationId << " have been successfully reset");
						}
					}
					catch(exception &e)
					{
						ACS_LOG_NOTIFY_EX(e);
						ACS_LOG_WARNING("Cannot reset retry of circulation queue record with id=" << circulationId);
					}
				}
                 
				break ;
            } 
        default: // > 1
            {
				// it should not happen
                ostringstream exMsg;
				exMsg 	<< setsize << " t_circulationsqueue records exist with fromurl=\"" << fromUrl << "\", tourl=\"" << toUrl << "\" and in not final status: <= 1 expected";
			    ACS_THROW(Exception(exMsg.str() ) ) ; // PRQA S 3081
                break ;
            }
    }
	
	return circulationId;

}


void CirculationOrderingService::updateCirculationsStatus( const string & fromUrl, const string & toUrl, const string & dbKey)
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB );

    SQLString queryString(dbKey, SQLString::SELECT) ;
    queryString.addTableList("t_circulationsqueue") ;
    queryString.addSelectList("id"); 
	
	{
        dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","fromurl", dbParam::EQUAL_TO, db::toSQL(dbKey, fromUrl))); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
	}
	
	{
        dbQueryParameters pp(dbKey) ;
		pp.addParameter(dbParam(dbKey, "t_circulationsqueue","tourl", dbParam::EQUAL_TO, db::toSQL(dbKey, toUrl))); // PRQA S 3050, 3081
		queryString.addANDWhereParameter(pp);
	}
	
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Query is: " << queryString.getSQL()) ;
    
	dbSet set ;
	performQueryForSet(queryString, set, "Requests with given fromUrl and toUrl") ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, set.size() << " t_circulationsqueue records found with fromurl=\"" << fromUrl << "\" and tourl=\"" << toUrl << '\"'); 
	if( set.size() != 0 ) {

		::Circulation::pds__MultipleStatusRequestType req;	
		dbRow row(dbKey);
		while( set.getNextRow(row) ) {
			dbPersistent::IdType cId = 0;
			row.column(0, cId);
			ostringstream os;
			os << cId;
			req.RequestIDs.push_back( os.str() );
		}

    	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to get status of " << req.RequestIDs.size() << " circulation records") ;
		::Circulation::pds__MultipleStatusResponseType resp;

		processMultipleStatusRequest(&req, &resp);
		if ( resp.ErrorCode != ::Circulation::pds__ErrorCodeEnum__0 ) {
			ACS_LOG_WARNING("Error code " <<  static_cast<unsigned int>(resp.ErrorCode) << " returned trying to get status of " <<  req.RequestIDs.size() << " circulation queue records"); // PRQA S 3081
		}
	}

	
}

ACS_CLASS_DEFINE_DEBUG_LEVEL (CirculationSoap);

//ctor
CirculationSoap::CirculationSoap():
    Thread(),
    _localPort(0), 
    _host(),
    _connPool(*Application::instance()->getDefaultConnectionPool()) // PRQA S 2528
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Constructor called." );
}



pair<dbPersistent::IdType, int> CirculationOrderingService::getHostSecZoneInfo(const string & ip_address, const string &dbKey) // PRQA S 4020
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"ip_address: " << ip_address) ;
	//select id, ip_cidr, level  from t_securityzones ;
	SQLString queryString(dbKey, SQLString::SELECT) ;
    	queryString.addTableList("t_securityzones") ;
        queryString.addSelectList("id");
        queryString.addSelectList("ip_cidr");
        queryString.addSelectList("level");
	dbSet set;
	performQueryForSet(queryString, set, "DC SEL SecZone");

	if (set.empty()) {
        ACS_THROW(NoSecurityZoneFoundException("t_securityzones table is empty!")); // PRQA S 3081
    }
	// loop on all security zones
    size_t setsize = set.size() ;
	for (size_t p = 0; p < setsize; ++p) 
	{
        dbRow row(dbKey);
        set.getRow(p + 1, row); // PRQA S 3084

	    dbPersistent::IdType id = 0 ;
        int level = 0 ;
	    string ip_cidr;
        // get id, ip_cidr, level
        row.column(0, id);
        row.column(1, ip_cidr);
        row.column(2, level); // PRQA S 4400
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Trying ip_address: " << ip_address << " on ip_cidr " << ip_cidr) ;
		try {
            if (StringUtils::ipBelongsToSubnet(ip_cidr, ip_address))
		    {
			    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "secZoneId = " <<  id << " secLevel ="  << level);
			    return make_pair(id, level) ;
		    }
        }
        catch(exception &x) {
            ACS_LOG_NOTIFY_EX(x) ;
        }
	}
	
	// if here t_securityzones is empty
	ACS_THROW(NoSecurityZoneFoundException("No security zone found for host = "+ip_address)); // PRQA S 3081
}

string CirculationOrderingService::getIpAddressByName(const string &hostname) throw() // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, hostname);
    try {
#ifdef ACS_USE_IPV6
#warning Using IPV6 address family
        const vector <string> &ips = net::Network::getIPAddresses(hostname, AF_INET6) ;
#else
#warning Using IPV4 address family
        const vector <string> &ips = net::Network::getIPAddresses(hostname, AF_INET) ;
#endif
        if (not ips.empty()) {
            return ips[0] ;
        }
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    return "";    
        
}


dbPersistent::IdType CirculationOrderingService::getAgentNodeId( const string & client )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
	
	dbPersistent::IdType hostId = _hostId;
	bool found = false;
	if( not client.empty() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Searching in configuration LUT hostId associated to \"" << client << "\" client");
		ClientFinder cf(client);
		vector< pair<string, dbPersistent::IdType> >::const_iterator it = find_if(_clientLUT.begin(), _clientLUT.end(), cf);
		if( it != _clientLUT.end() ) {
			hostId = it->second;
			found = true;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Host id associated to \"" << client << "\" client is " << hostId);
		}
	}
	
	if( not found && (0 != _agentNodeId) ) {
		hostId = _agentNodeId;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Host id valorized with configured AgentNodeId value: " << hostId );
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Returning host id " << hostId );
	return hostId;
	
}


CirculationSoap::~CirculationSoap() throw() 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB); // PRQA S 4631
}


void CirculationSoap::run() // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Started");

	const string &dbKey = _connPool.getKey() ;
    
	CirculationOrderingService soap(_connPool, *this);
    try {
	    // load configuration
	    soap.loadConf();
    }
	catch(CirculationOrderingService::LocalhostNotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, e.what());
		Application::setStatus(Application::Warning, "Localhost not found: " + string(e.what()) ); // PRQA S 3081
	}
    catch(exception &e)
    {             
        //any exception here should became a StopException in order to stop main thread
		ACS_THROW(StopException(e, "Startup configuration error" )) ; // PRQA S 3081
    }


    try { // just to check if exixts
        Status::instance(dbKey)->id(Status::eligible) ;
    }
    catch( exception &x) {
        ACS_THROW(StopException("error retrieving id for status " + Status::eligible));// PRQA S 3081
    }

	_host = soap.getHost() ;
	_localPort = soap.getLocalPort();

    
    int m = 0; // master and slave sockets
    string bindingAddress = _host; //if value not found in configuration, will use this default
    try {
        soap_init(&soap);
        soap_set_namespaces(&soap, Circulation_namespaces);
        soap.fget = http_get;

        soap.send_timeout = soap.getSendTimeout() ; 
        soap.accept_timeout = soap.getAcceptTimeout() ;
        soap.recv_timeout = soap.getRecvTimeout() ;

        //to enable server-side port reuse and local port sharing 
        //( this socket option explicitly allows a process to bind to a port which remains in TIME_WAIT)
        soap.bind_flags |= SO_REUSEADDR; // PRQA S 3003

// MP attempt to improve soap connection queue length and behaviour in reusing sockets
	    soap.accept_flags |= SO_LINGER; // PRQA S 3003
	    soap.linger_time = 5 ; // sec // PRQA S 4400 2
	    int backlog = 1024 ;
// MP
        rsResourceSet &rset = ConfigurationSingleton::instance()->get();
        rsPushNSpace cServ(rset,"CirculationService");
        
        if( rset.hasKey("BindingAddress") )
        {
            rset.getValue("BindingAddress", bindingAddress) ;
        }
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "BindingAddress: " << bindingAddress ) ;

        m = soap_bind(&soap, bindingAddress.c_str(), _localPort, backlog); // PRQA S 3000, 4400

        if(m < 0)
        {
		    ostringstream error ;
		    error << "Error while binding to host " << bindingAddress << " and port " << _localPort << " : " ;
		    soap_stream_fault(&soap, error) ;
            ACS_LOG_WARNING(error.str()) ;
            if(bindingAddress != _host)
            {
                ACS_LOG_WARNING("soap_bind failed on Binding Address: " << bindingAddress)
                ACS_LOG_INFO("Going to try soap_bind on host: " << _host )
                m = soap_bind(&soap, _host.c_str(), _localPort, backlog)  ; // PRQA S 3000, 4400
                bindingAddress = _host ; //this is due to the subsequent debug print.
            }
        }

    } catch (std::exception const& ex) {
		ACS_LOG_NOTIFY_EX(ex);
        ACS_LOG_ERROR("Socket open error " << _host << " and port " << _localPort );
        throw;
    }
	if (m < 0)
	{
		ostringstream error ;
		error << "Error while binding to host " << _host << " and port " << _localPort << " : " ;
		soap_stream_fault(&soap, error) ;
		ACS_THROW(CriticalException(error.str() ) ) ; // PRQA S 3081
	}

	ACS_LOG_INFO("Socket opened host " << bindingAddress 
		<< " and port " << _localPort << " : master socket = " << m );

	while(!requestedCancel() )
	{
		SOAP_SOCKET acceptResult = SOAP_EOF;
		while( (acceptResult=soap_accept(&soap)) == SOAP_EOF  ) // PRQA S 4244
		{
			if(requestedCancel() )
			{
   				break ;
			}
		}

        if (not requestedCancel()) {
		    if (not isSoapValidSocket( acceptResult ) )
		    {
			    ostringstream error ;
			    error << "Error while acceping connection : " ;
			    soap_stream_fault(&soap, error) ;
			    ACS_THROW(CriticalException(error.str() ) ) ; // PRQA S 3081
		    }
		    
            if (soap.serve() != SOAP_OK) // process RPC request
		    {
			    ostringstream error ;
			    error << "Error while processing request : " ;
			    soap_stream_fault(&soap, error) ;
			    ACS_LOG_WARNING(error.str());			
		    }
        }
		soap_destroy(&soap); // clean up class instances
		soap_end(&soap); // clean up everything and close socket
		
	}
    
	ACS_LOG_INFO("Closing soap service");
   	soap_done(&soap); // close master socket and detach environment
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "about to end thread.") ;
	
}

// the following pragma has been added to avoid - warning: use of old-style cast - SIG_IGN
// PRQA S 1040 2
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"

bool CirculationSoap::isSoapValidSocket(int ss) throw() { return soap_valid_socket( ss ) ; } // PRQA S 3080, 3081

// reactivate -Wold-style-cast
// PRQA S 1040 2
#pragma GCC diagnostic pop


unsigned int CirculationSoap::getPort() const throw() // PRQA S 4120
{
	return _localPort;
}
		   


const string &CirculationSoap::getHost() const throw() // PRQA S 4120
{
	return _host;
}

template<>
void rsResourceSet::getValue(const std::string &res, pair<string, dbPersistent::IdType> &v) const 
{
    Lock lock(_mutex) ;
	getValue(res+".clientId.value")->get(v.first) ;
	string hostId;
	getValue(res+".hostId.value")->get(hostId) ;
	
	istringstream iss(hostId);
	iss >> v.second;
	
} 


/* Error code
0 SUCCESSFUL
1 NULL MANDATORY FIELDS
2 NO RELAYS FOUND
3 NO RELAYS CONTACTED
4 ERROR CONTACTING DB
5 WRONG URL FORMAT
6 WRONG IP ADDRESS/HOSTNAME
7 NO DATA FOUND
8 MISSING DATA FROM DB
9 ERROR UPDATING DB
99 UNDEFINED
*/


int CirculationOrderingService::CirculateDynamic( // PRQA S 2017, 4020
                            pds__CirculateDynamicRequestType *  in, 
                            pds__CirculateResponseType*  out)
{
	try {
		processCirculateDynamicRequest( in , out ) ;
	}
	catch(dbQuery::QueryExecuteException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR("Dynamic circulation request rejected: error contacting DB.") ;
		out->ErrorCode = pds__ErrorCodeEnum__4; //ERROR CONTACTING DB
		out->Status = pds__StatusEnum__REJECTED;
		return SOAP_OK ;

	}
	catch(acs::curl::URLMalformedException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR("Dynamic circulation request rejected: wrong url format.") ;
		out->ErrorCode = pds__ErrorCodeEnum__5; //WRONG URL FORMAT
		out->Status = pds__StatusEnum__REJECTED;
		return SOAP_OK ;

	}
	catch(NoSecurityZoneFoundException &e)
	{
		ACS_LOG_ERROR("Dynamic circulation request rejected: " << e.what()) ;
		out->ErrorCode = pds__ErrorCodeEnum__2; // NO RELAYS FOUND
		out->Status = pds__StatusEnum__REJECTED;
		return SOAP_OK ;
	}
	catch(LocalhostNotFoundException &e)
	{
		ACS_LOG_ERROR("Local host not found in t_knownhosts table") ;
		out->ErrorCode = pds__ErrorCodeEnum__6;
		out->Status = pds__StatusEnum__REJECTED;
	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR("Dynamic circulation request rejected.") ;
		out->ErrorCode = pds__ErrorCodeEnum__99;
		out->Status = pds__StatusEnum__REJECTED;
		return SOAP_OK ;
	}
	return SOAP_OK ;

}
int CirculationOrderingService::CirculateMultiDynamic( // PRQA S 2017, 4020
                            pds__CirculateMultipleDynamicRequestType *  in, 
                            pds__CirculateMultipleResponseType*  out)
{
    processCirculateMultiDynamicRequest( in , out ) ;
	return SOAP_OK ;
}


int CirculationOrderingService::CirculateExplicit(// PRQA S 2017, 4020
                             pds__CirculateExplicitRequestType *  in, 
                             pds__CirculateResponseType*  out)
{
	try{
		processCirculateExplicitRequest( in , out ) ;
	}
	catch(dbQuery::QueryExecuteException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR("Explicit circulation request rejected: error contacting DB.") ;
		out->ErrorCode = pds__ErrorCodeEnum__4;
		out->Status = pds__StatusEnum__REJECTED;
		return SOAP_OK;

	}
	catch(acs::curl::URLMalformedException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR("Explicit circulation request rejected: wrong url format.") ;
		out->ErrorCode = pds__ErrorCodeEnum__5;
		out->Status = pds__StatusEnum__REJECTED;
		return SOAP_OK ;

	}
	catch( NoSecurityZoneFoundException &e)
	{
		ACS_LOG_ERROR("Explicit circulation request rejected: " << e.what()) ;
		out->ErrorCode = pds__ErrorCodeEnum__2; // NO RELAYS FOUND
		out->Status = pds__StatusEnum__REJECTED;
		return SOAP_OK ;
	}
	catch( LocalhostNotFoundException &e)
	{
		ACS_LOG_ERROR("Local host not found in t_knownhosts table") ;
		out->ErrorCode = pds__ErrorCodeEnum__6;
		out->Status = pds__StatusEnum__REJECTED;
	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR("Explicit circulation request rejected.") ;
		out->ErrorCode = pds__ErrorCodeEnum__99;
		out->Status = pds__StatusEnum__REJECTED;
		return SOAP_OK ;
	}
	return SOAP_OK ;

}

int CirculationOrderingService::GetStatus( // PRQA S 2017, 4020
                     pds__StatusRequestType*    in,	
                     pds__StatusResponseType*   out)
{
	try{
		processStatusRequest( in , out ) ;
	}
	catch(dbQuery::QueryExecuteException &e)
	{
		out->ErrorCode = pds__ErrorCodeEnum__4;
		return SOAP_OK;

	}
	catch(exception &e)
	{
		out->ErrorCode = pds__ErrorCodeEnum__99;
		return SOAP_OK;
	}
	return SOAP_OK ;
}

int CirculationOrderingService::IncreaseCirculationPriority(  // PRQA S 2017, 4020
                 pds__IncreaseCirculationPriorityRequestType*    in,	
                 pds__IncreaseCirculationPriorityResponseType*   out)
{
	try{
		increaseCirculationPriority( in , out ) ;
	}
	catch(dbQuery::QueryExecuteException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR("Increase circulation priority request rejected: error contacting DB.") ;
		out->ErrorCode = pds__ErrorCodeEnum__4; //ERROR CONTACTING DB
		out->Status = pds__StatusEnum__REJECTED;
	}
	catch(acs::curl::URLMalformedException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR("Increase circulation priority request rejected: wrong url format.") ;
		out->ErrorCode = pds__ErrorCodeEnum__5; //WRONG URL FORMAT
		out->Status = pds__StatusEnum__REJECTED;

	}
	catch( NoSecurityZoneFoundException &e)
	{
		ACS_LOG_ERROR("Increase circulation priority request rejected: " << e.what()) ;
		out->ErrorCode = pds__ErrorCodeEnum__2; // NO RELAYS FOUND
		out->Status = pds__StatusEnum__REJECTED;
	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR("Increase circulation priority request rejected.") ;
		out->ErrorCode = pds__ErrorCodeEnum__99;
		out->Status = pds__StatusEnum__REJECTED;
	}
	return SOAP_OK ;
}

int CirculationOrderingService::CirculationRecordOccurrence( // PRQA S 2017, 4020
                 pds__CirculationOccurrenceRequestType*    in,	
                 pds__CirculationOccurrenceResponseType*   out)
{
	try{
		circulationRecordOccurrence( in , out ) ;
	}
	catch(dbQuery::QueryExecuteException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR("Circulations occurrence request rejected: error contacting DB.") ;
		out->ErrorCode = pds__ErrorCodeEnum__4; //ERROR CONTACTING DB
		out->Status = pds__StatusEnum__REJECTED;
	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		ACS_LOG_ERROR("Circulations occurrence request rejected.") ;
		out->ErrorCode = pds__ErrorCodeEnum__99;
		out->Status = pds__StatusEnum__REJECTED;
	}
	return SOAP_OK ;
}


int CirculationOrderingService::GetMultipleStatus( // PRQA S 2017, 4020
                     pds__MultipleStatusRequestType*    in,	
                     pds__MultipleStatusResponseType*   out)
{
	try{
		processMultipleStatusRequest( in , out ) ;
	}
	catch( MissingDbRecordsException &e)
	{
		ACS_LOG_WARNING("Cannot return status of all desired circulation queue records since some of them are missing");
		out->ErrorCode = pds__ErrorCodeEnum__8;
		return SOAP_OK;
	}
	catch(dbQuery::QueryExecuteException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		out->ErrorCode = pds__ErrorCodeEnum__4;
		return SOAP_OK;

	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		out->ErrorCode = pds__ErrorCodeEnum__99;
		return SOAP_OK;
	}
	return SOAP_OK ;
}

int CirculationOrderingService::GetMultipleDetailedStatus( // PRQA S 2017, 4020
                     pds__MultipleStatusRequestType*    in,	
                     pds__MultipleDetailedStatusResponseType*   out)
{
	try{
		processMultipleStatusRequest( in , out ) ;
	}
	catch(dbQuery::QueryExecuteException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		out->ErrorCode = pds__ErrorCodeEnum__4;
		return SOAP_OK;

	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		out->ErrorCode = pds__ErrorCodeEnum__99;
		return SOAP_OK;
	}
	return SOAP_OK ;
}


int CirculationOrderingService::ResetCirculationRetry( // PRQA S 2017, 4020
    				pds__ResetRetryRequestType*  in,
    				pds__CirculateResponseType* out)
{

	try{
		resetCirculationRetry( in , out ) ;
	}
	catch(dbQuery::QueryExecuteException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		out->ErrorCode = pds__ErrorCodeEnum__4;
		out->Status = pds__StatusEnum__REJECTED;
	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		out->ErrorCode = pds__ErrorCodeEnum__99;
		out->Status = pds__StatusEnum__REJECTED;
	}
		
	return SOAP_OK ;
}

int CirculationOrderingService::ResetCirculationStatus(  // PRQA S 2017, 4020
    				pds__MultipleStatusRequestType*  in,
                    pds__MultipleStatusResponseType* out)
{

	try{
		resetCirculationStatus( in, out ) ;
	}
	catch(dbQuery::QueryExecuteException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
	}
		
	return SOAP_OK ;
}

int CirculationOrderingService::ResetMultipleCirculationStatus(// PRQA S 2017, 4020
    				pds__MultipleStatusRequestType*  in,
                    pds__MultipleResponseType* out)
{

	try{
		resetMultipleCirculationStatus( in, out ) ;
	}
	catch(dbQuery::QueryExecuteException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
	}
		
	return SOAP_OK ;
}

int CirculationOrderingService::ReInsertCirculation( // PRQA S 2017, 4020
    				pds__StatusRequestType*  in,
                    pds__CirculateResponseType* out)
{

	try{
		reInsertCirculation( in, out ) ;
	}
	catch(dbQuery::QueryExecuteException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		out->ErrorCode = pds__ErrorCodeEnum__4;
		out->Status = pds__StatusEnum__REJECTED;
	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		out->ErrorCode = pds__ErrorCodeEnum__99;
		out->Status = pds__StatusEnum__REJECTED;
	}
		
	return SOAP_OK ;
}





_ACS_END_NAMESPACE
