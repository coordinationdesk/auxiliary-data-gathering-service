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
	Revision 1.3  2016/08/31 08:38:05  damdec
	Compilation problems fixed
	
	Revision 1.2  2016/08/30 08:58:17  damdec
	Work in progress.
	
	Revision 1.1.1.1  2016/08/29 08:41:32  marfav
	Importing libCirculationCallback
	
        
*/

#ifndef _CircCallbackSoapServiceThread_H_
#define _CircCallbackSoapServiceThread_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Thread.h>
#include <dbConnectionPool.h>
#include <dbPersistent.h>

#pragma GCC diagnostic push  // PRQA S 1040 4
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor" 
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#include <CirculationCallbackPDS_USCOREDCBindingService.h>

// PRQA S 1040 2
#pragma GCC diagnostic pop

_ACS_BEGIN_NAMESPACE(CirculationCallback)
	// SOAP Messages pre declarations
	class pds__CallbackRequestType;
	class pds__CallbackResponseType;

_ACS_END_NAMESPACE

_ACS_BEGIN_NAMESPACE(acs)

using namespace CirculationCallback;

// Service class implementing SOAP requests
class CirculationCallbackSoapService: public CirculationCallback::PDS_USCOREDCBindingService {
public:
	CirculationCallbackSoapService();
	virtual ~CirculationCallbackSoapService() ACS_NOEXCEPT ;
	virtual int StatusUpdateCallback( pds__CallbackRequestType*  in, pds__CallbackResponseType* out);	// PRQA S 2017, 4020

private:
	/** Copy Constructor */
	CirculationCallbackSoapService(const CirculationCallbackSoapService&);
	/** Operator = */
	CirculationCallbackSoapService &operator=(const CirculationCallbackSoapService &);
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(CirculationCallbackSoapService) ;
};

/**
* \brief
* Generic interface for calling Circulation callback 
**/

class CircCallbackSoapServiceThread : public Thread { 
public:

	/** CircCallbackSoapServiceThread Exceptions */
	exDECLARE_EXCEPTION(exCirculationCallbackSoapException, exException); // CircCallbackSoapServiceThread Exception.							// PRQA S 2131, 2502
	exDECLARE_EXCEPTION(exCirculationCallbackSoapCriticalException, exCriticalException); // CircCallbackSoapServiceThread Critical Exception.// PRQA S 2131, 2502
	exDECLARE_EXCEPTION(LocalhostNotFoundException, exException) ; // Local host not found in t_knownhosts table						// PRQA S 2131, 2502
	exDECLARE_EXCEPTION(CriticalException, exException) ; // Critical Exception.					// PRQA S 2131, 2502
	exDECLARE_EXCEPTION(StopException, exException) ; // Tell caller that this thread cannot be run // PRQA S 2131, 2502
	exDECLARE_EXCEPTION(CirculationCallbackSoapThreadNotRunningException, exException) ; // Tell caller that this thread cannot be run // PRQA S 2131, 2502
	
	/** Default Class constructor */
	CircCallbackSoapServiceThread(const std::string & host);
	/** Destructor */
	virtual ~CircCallbackSoapServiceThread() throw();
	virtual void run();
	
	/**
	 * Waits for waitTimeoutMsec on binding address condition.
	 * Returns true if binding was performed within the requested timeout,
	 * false otherwise
	 */
	bool waitAddressBinding(unsigned long waitTimeoutMsec = 100);
	
	/**
	 * Returns the url at which the callback service is currently listening.
	 * The returned string is valid iff the service is currently binded on some address.
	 * The 'binded' flag is set to true if the service has been initialized and it is
	 * currently binded on the returned address.
	 */
	virtual std::string getCirculationCallbackServiceUrl(bool& binded, unsigned long waitBindingMsec = 100);
	
private:  
	CircCallbackSoapServiceThread(); // not implemented
	// copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	CircCallbackSoapServiceThread(const CircCallbackSoapServiceThread & );
	/** Operator = */
	CircCallbackSoapServiceThread &operator=(const CircCallbackSoapServiceThread &);

	void loadConf();
	
	void setBindingAddress(const std::string& addr);
	
	bool checkIpAddress(const std::string & ip, std::string & newIp/*, unsigned int & hostId, bool insHN = false*/) ;
    static std::string getIpAddressByName(const std::string &hostname) throw();
    static bool isSoapValidSocket(int) throw() ;

private:
	static ThreadSafe _allocationMutex ;
	dbConnectionPool & _connPool;
	
	Condition _bindingCondition;
	
   	std::string _hostIp;
	unsigned int _localPort;
	std::string _host;  //name
	
	unsigned short _maxDbRetry;
	unsigned int _maxSoapRetry;

    unsigned int _sendTimeout;
    unsigned int _recvTimeout;
    unsigned int _acceptTimeout;
	std::string _bindingAddress;
	bool _isBinded;
	
    static const int _defaultMaxDbRetries = 3 ;
    static const int _defaultMaxSoapRetries = 3 ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(CircCallbackSoapServiceThread)

// Needed for test purposes
   friend class CircCallbackSoapServiceThreadTest;

};

_ACS_END_NAMESPACE

#endif //_CircCallbackSoapServiceThread_H_

