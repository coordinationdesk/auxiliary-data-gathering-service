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

	$Prod: A.C.S. libCirculationCallbackSoap$

	$Id$

	$Author$

	$Log$
	Revision 1.3  2016/09/08 08:46:08  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 1.2  2016/08/30 08:58:17  damdec
	Work in progress.
	
	Revision 1.1.1.1  2016/08/29 08:41:32  marfav
	Importing libCirculationCallback
	
        
*/ 

#include <CirculationCallbackSoap.h>
#include <DC_CallbackSingleton.h>
#include "CirculationCallbackV.h"
#include <Filterables.h>
#include <dbQuery.h>
#include <Application.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <RegExp.h>
#include <dbSet.h>
#include <Status.h>
#include <Network.h>
#include <exException.h>
#include <RemoteSite.h>
#include <RemoteSiteFactory.h>

#pragma GCC diagnostic push  // PRQA S 1040 4
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor" 
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#include <CirculationCallbackH.h>// PRQA S 1013 3 
#include <CirculationCallback.nsmap>
#include <CirculationCallbackPDS_USCOREDCBindingProxy.h>
#include <CirculationCallbackPDS_USCOREDCBindingService.h>
#include <CirculationCallbackStub.h>  // PRQA S 1013 3 

// PRQA S 1040 2
#pragma GCC diagnostic pop

using namespace std;
using namespace ::CirculationCallback;
using namespace acs;
using namespace curl;

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(CircCallbackSoapServiceThread)

static CirculationCallbackV version;

ThreadSafe CircCallbackSoapServiceThread::_allocationMutex ;

CircCallbackSoapServiceThread::CircCallbackSoapServiceThread(const std::string & host) :
		Thread(""),
    	_connPool(*Application::instance()->getDefaultConnectionPool()), // PRQA S 2528
    	_bindingCondition(),
	_hostIp(""),
    	_localPort(11120), 
    	_host(host),
    	_maxDbRetry(_defaultMaxDbRetries),
    	_maxSoapRetry(_defaultMaxSoapRetries),
    	_sendTimeout(60),
    	_recvTimeout(60),
    	_acceptTimeout(5),
	_bindingAddress(),
	_isBinded(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
   	ThreadSafe::Lock lock(_allocationMutex) ;
}

CircCallbackSoapServiceThread::~CircCallbackSoapServiceThread() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);	// PRQA S 4631
   	ThreadSafe::Lock lock(_allocationMutex) ;
}

string CircCallbackSoapServiceThread::getIpAddressByName(const string &hostname) throw() // PRQA S 4020
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

bool CircCallbackSoapServiceThread::checkIpAddress(const string & ip, string & newIp/* , unsigned int & hostId, bool insHN */) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Input: \"" << ip << '\"');
//	hostId = 0;
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
		//performQueryForSet(queryString, set, "DC SEL HostIp" );
		dbConnectionWrapper pw(_connPool);

		dbConnection &conn = pw ; // PRQA S 3050
		dbQuery query(conn, queryString, "DC SEL HostIp") ;
		query.execForSetRetry(*this, 100, _maxDbRetry, set); // PRQA S 4400
		

        bool isNull = true;
        if (not set.empty() ) //found as known host!!
        {  
            dbRow row(dbKey);
            set.getRow(1, row);

            // get ipaddress
            row.column(0, newIp,&isNull);
//            row.column(1, hostId);
        }
// 	else if (insHN)	// insert host_name in t_knownhosts
// 	{	
// 		// insert new record in t_knownhosts
// 		SQLString queryString(dbKey, SQLString::INSERT) ;
//     		queryString.addTableList("t_knownhosts") ;
// 		queryString.addValuesList("host_name", db::toSQL(dbKey, ip));
// 		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following query: " << queryString.getSQL() );
// 		
//         	// execute query
// 		dbConnectionWrapper pw(_connPool);
// 		dbConnection &conn = pw ; // PRQA S 3050
// 		dbQuery getQuery(conn, queryString,"Insert Hostname");
//         	getQuery.execForStatusRetry(*this, 100, _maxDbRetry);
// 		hostId = conn.getLastSerial("t_knownhosts");
// 	}
 
        if (isNull) {  //not known host or null ip value
            newIp = getIpAddressByName(ip);
        }
        if (newIp.empty()) { 
            return false; 
        }

	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Going to return ip: \"" << newIp/* << "\", host id=" << hostId*/);
	return true;
}

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
        fd = fopen((string(base::defaultConfPath()) + "/circulationCallback.wsdl").c_str(), "rb"); // open WSDL file to copy // PRQA S 3081
    } 
    else if (  path.compare (path.length() - end_xsd.length(), end_xsd.length(), end_xsd) == 0) {  // PRQA S 3084
        fd = fopen((string(base::defaultConfPath()) + "/circulationCallback.xsd").c_str(), "rb"); // open XSD file to copy // PRQA S 3081
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
void CircCallbackSoapServiceThread::loadConf()
{
	rsResourceSet &rset = ConfigurationSingleton::instance()->get();
   	rsPushNSpace cServ(rset,"CirculationCallbackService");
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

	rset.getValue("HostName", _host, false);		
	rset.getValue("Port", _localPort, false);		
	
 	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Port = " << _localPort) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Soap Send timeout = " << _sendTimeout) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Soap Receive timeout = " << _recvTimeout) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Soap Accept timeout = " << _acceptTimeout) ;

	checkIpAddress(_host, _hostIp/* , _hostId */);
  
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,  "Host Ip = " << _hostIp /* << " Host Id = " << _hostId */ );
    ACS_COND_THROW( _hostIp.empty(), CriticalException( "error retrieving host ipaddress!") ) ; // PRQA S 3081
    ACS_COND_THROW( _host == _hostIp , CriticalException( "Configured hostname cannot be an IP address: \"" + _host + '\"') ) ; // PRQA S 3081

}

void CircCallbackSoapServiceThread::setBindingAddress(const std::string& addr) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	Lock l(mutex());
	_bindingAddress = addr;
	_isBinded = true;
}

void CircCallbackSoapServiceThread::run() // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Started");

    try {
	    // load configuration
	    loadConf();
    }
	catch(LocalhostNotFoundException &e)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, e.what());
		Application::setStatus(Application::Warning, "Localhost not found: " + string(e.what()) ); // PRQA S 3081
	}
    catch(exception &e)
    {             
        //any exception here should became a StopException in order to stop main thread
		ACS_THROW(StopException(e, "Startup configuration error" )) ; // PRQA S 3081
    }

//    struct soap soap;
    CirculationCallbackSoapService soap;
    int m = 0; // master and slave sockets
    std::string bindingAddress = _host ; //if value not found in configuration, will use this default
    try {
        soap_init(&soap);
        soap_set_namespaces(&soap, CirculationCallback_namespaces);
        soap.fget = http_get;

        soap.send_timeout = _sendTimeout ; // PRQA S 4400 3
        soap.accept_timeout = _acceptTimeout ;
        soap.recv_timeout = _recvTimeout ;

        //to enable server-side port reuse and local port sharing 
        //( this socket option explicitly allows a process to bind to a port which remains in TIME_WAIT)
        soap.bind_flags |= SO_REUSEADDR; // PRQA S 3003

// MP attempt to improve soap connection queue length and behaviour in reusing sockets
	    soap.accept_flags |= SO_LINGER;
	    soap.linger_time = 5 ; // sec
	    int backlog = 1024 ;
// MP
        rsResourceSet &rset = ConfigurationSingleton::instance()->get();
        rsPushNSpace cServ(rset,"CirculationCallbackService");
        
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
            if(_bindingAddress != _host)
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
	try {
		_bindingCondition.broadcast();
	} catch (std::exception& e) {
		ACS_LOG_NOTIFY_EX(e);
		// Threads waiting on the condition will timeout
	}
        throw;
    }
	if (m < 0)
	{
		ostringstream error ;
		error << "Error while binding to host " << _host << " and port " << _localPort << " : " ;
		soap_stream_fault(&soap, error) ;
		ACS_THROW(CriticalException(error.str() ) ) ; // PRQA S 3081
	}

	ACS_LOG_INFO("Circulation Callback Socket opened host " << bindingAddress 
		<< " and port " << _localPort << " : master socket = " << m );
	
	setBindingAddress(bindingAddress);
	try {
		_bindingCondition.broadcast();
	} catch (std::exception& e) {
		ACS_LOG_NOTIFY_EX(e);
		// Threads waiting on the condition will timeout
	}
	
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
		try
		{
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
		}
		catch (std::exception const& ex) {
			ACS_LOG_NOTIFY_EX(ex);
	        ACS_LOG_ERROR("Error processing soap request."  );
    	    throw;
    	}

		soap_destroy(&soap); // clean up class instances
		soap_end(&soap); // clean up everything and close socket	
	}
    
	ACS_LOG_INFO("Closing soap service");
   	soap_done(&soap); // close master socket and detach environment
	{
		Lock l (mutex());
		_isBinded = false;
		_bindingAddress = "";
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "about to end thread.") ;
	
}
// the following pragma has been added to avoid - warning: use of old-style cast - SIG_IGN
// PRQA S 1040 2 
#pragma GCC diagnostic ignored "-Wold-style-cast"

bool CircCallbackSoapServiceThread::isSoapValidSocket(int ss) throw() { return soap_valid_socket( ss ) ; } // PRQA S 3080, 3081

// reactivate -Wold-style-cast
// PRQA S 1040 2
#pragma GCC diagnostic warning "-Wold-style-cast"
// _ACS_END_NAMESPACE
// 

bool CircCallbackSoapServiceThread::waitAddressBinding(unsigned long waitTimeoutMsec) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	bool notTimedout = true;
	if (running()) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Waiting address binding for " << waitTimeoutMsec << " msec.");
		Lock lock(_bindingCondition);
		notTimedout = _bindingCondition.timedwait(waitTimeoutMsec);
	} else {
		ACS_THROW(CirculationCallbackSoapThreadNotRunningException("Thread must be running to wait on address binding."));
	}

	return notTimedout;
}

std::string CircCallbackSoapServiceThread::getCirculationCallbackServiceUrl(bool& binded, unsigned long waitBindingMsec) {	
	std::string url;
	bool notTimedout = true;
	try {
		notTimedout = waitAddressBinding(waitBindingMsec);
	
		if (notTimedout) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Binding notification received.");
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Binding notification timedout. Checking binding address availability");
		}
	} catch (CirculationCallbackSoapThreadNotRunningException& e) {
		ACS_LOG_NOTIFY_EX(e);
	}

	Lock l(mutex());
	if (running() && _isBinded) {
		// https currently not supported by the library
		RemoteSite site("http", _bindingAddress, _localPort);
		url = RemoteSiteFactory::toURL(site);	
		binded = true;
	}
	return url;
}


CirculationCallbackSoapService::CirculationCallbackSoapService()
{}

CirculationCallbackSoapService::CirculationCallbackSoapService(const CirculationCallbackSoapService&)
{}

CirculationCallbackSoapService::~CirculationCallbackSoapService() ACS_NOEXCEPT
{}

int CirculationCallbackSoapService::StatusUpdateCallback( pds__CallbackRequestType*  in,	// PRQA S 2017, 4020
                    									pds__CallbackResponseType* out)
{

	try{
		CirculationCallbackInterface* p = DC_CallbackSingleton::instance()->getCallbackManager(); 
		if (p !=0 ) 
		{
			p->manageCallbackRequest(in, out) ;
		}
		else
		{
			ACS_LOG_WARNING("No Callback Manager instance found");
			out->ErrorCode = pds__ErrorCodeEnum__1;
		}
	}
	catch(dbQuery::QueryExecuteException &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		out->ErrorCode = pds__ErrorCodeEnum__1;
	}
	catch(exception &e)
	{
		ACS_LOG_NOTIFY_EX(e);
		out->ErrorCode = pds__ErrorCodeEnum__99;
	}
		
	return SOAP_OK ;
}

_ACS_END_NAMESPACE

_ACS_BEGIN_NAMESPACE(CirculationCallback)

// Implement base class empty methods
int PDS_USCOREDCBindingService::StatusUpdateCallback( pds__CallbackRequestType*  in, pds__CallbackResponseType* out)
{
	ACS_THROW (exNotImplemented("Base function not implemented"));
}


_ACS_END_NAMESPACE

