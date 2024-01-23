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

	$Id$

	$Author$

	$Log$
	Revision 2.0  2016/08/05 10:35:51  marfav
	Creating new major release due to change in the database schema
	
	Revision 1.46  2016/08/02 10:25:11  lucio.pulvirenti
	S1PDGS-31765: LocalTransferMode optional tag content managed in both dynamic and explicit requests. If tag not valorized, optional values from configuration
	taken for explicit and dynamic circ.
	
	Revision 1.45  2016/05/09 15:03:41  francesco.avanzi
	S2PDGS-1435: binding address now added. if no binding address found in configuration, host will be used
	
	Revision 1.44  2016/05/05 15:56:22  francesco.avanzi
	work in progress for s2pgs-25
	
	Revision 1.43  2016/03/26 21:32:13  marpas
	using new db I/F
	
	Revision 1.42  2016/02/04 13:18:49  lucio.pulvirenti
	S2PDGS-1396: MissingDbRecordsException added.
	
	Revision 1.41  2015/12/17 16:25:38  damdec
	Work in progress.
	
	Revision 1.40  2015/10/08 15:35:27  lucio.pulvirenti
	APF-324: if ClientID valorized for either explicit or dynamic request, passed to insertCirculationQueueRecord modified IF.
	Optional Client LUT and AgentNodeId loaded from conf. getAgentNodeId method implemented to return host_id given client_id string (it can be empty).
	
	Revision 1.39  2015/09/01 10:27:45  damdec
	S2PDGS-1247: record added in t_knownhosts for dynamic circulation when not already configured.
	
	Revision 1.38  2015/06/30 13:32:00  lucio.pulvirenti
	S3PDGS-2442: Bug fixed in explicit circulation request processing: priority overwritten by a default value (set to 0), although taken
	 from Configuration as a mandatory parameter, before setting by input (if occurring).
	
	Revision 1.37  2015/03/12 09:08:27  marfav
	S2PDGS-1122 added support to configurable soap timeouts
	
	Revision 1.36  2014/10/03 14:11:18  lucio.pulvirenti
	S2PDGS-869: if localhost not found in t_knownhosts table, set application status to DEGRADED and perform neither explicit nor dynamic circulations.
	If hostname in configuration is an IP address, launch exception.
	
	Revision 1.35  2014/10/02 16:36:54  lucio.pulvirenti
	S2PDGS-869 work in progress.
	
	Revision 1.34  2014/09/22 12:21:54  lucio.pulvirenti
	S1PDGS-31497: In insertCirculationQueueRecord method updateCirculationsStatus new method is invoked to update (if needed) status of all requests with given fromUrl and toUrl.
	duplicatedRequestExists new method developed to discover if a duplicated request wrt toUrl and fromUrl exists.
	
	Revision 1.33  2014/09/18 16:01:08  lucio.pulvirenti
	S1PDGS-31497: work in progress.
	
	Revision 1.32  2014/05/27 14:36:23  lucio.pulvirenti
	S2PDGS-671: new structures and methods defined to get status of a set of circulation queue records.
	
	Revision 1.31  2014/05/21 14:59:27  lucio.pulvirenti
	S2PDGS-671: circulationRecordOccurrence new method implemented to query Soap Server occurrence of a given circulation queue record.
	
	Revision 1.30  2014/04/16 12:37:31  lucio.pulvirenti
	S2PDGS-671: increaseCirculationPriority method added.
	
	Revision 1.29  2014/04/15 15:55:09  lucio.pulvirenti
	Work in progress.
	
	Revision 1.28  2014/01/14 13:37:03  lucio.pulvirenti
	S1PSDGS-31357: NoSecurityZoneFoundException launched by getHostSecZoneInfo method if no security zone found for input IP address.
	If such an exception is caught, NO RELAYS FOUND (2) error is returned when processing both dynamic and explicit circulation request.
	Warnings added when returning error codes with no exception.
	
	Revision 1.27  2013/12/16 16:53:17  lucio.pulvirenti
	qa rules applied.
	
	Revision 1.26  2013/09/09 15:41:00  lucio.pulvirenti
	CirculationParams::tempname_id now int.
	
	Revision 1.25  2013/07/23 17:11:00  marpas
	compilation warnings fixed
	
	Revision 1.24  2013/07/19 13:06:18  marpas
	attempt to rewrite locally relay status
	
	Revision 1.23  2013/07/18 12:30:31  marpas
	test improved
	
	Revision 1.22  2013/07/15 17:21:33  marpas
	work in progress
	
	Revision 1.21  2013/07/11 18:31:10  marpas
	work in progress
	
	Revision 1.20  2013/06/14 12:31:52  lucio.pulvirenti
	Parameters added to insertCirculationQueueRecord method. _tempnamemode and _tempnamevalue members added to IF to store
	values from configuration.
	
	Revision 1.19  2013/03/14 18:30:49  marpas
	xonnection pool concept enforced
	
	Revision 1.18  2012/02/22 14:55:24  chicas
	Added _hostId ; inserted inside t_circulationsqueue if found (host_id)
	
	Revision 1.17  2012/02/21 14:22:47  chicas
	Host ip searched using gethostbyname even if host name is no present inside t_knownhost (still JIRA S1PDGS-2931); t_knownhost.dest_host_id filled whenever possible (JIRA S3PDGS-586)
	
	Revision 1.16  2012/02/20 13:43:28  chicas
	Solved JIRA S1PDGS-2931 ; if t_knownhost.ip_address is NULL, use gethostbyname for resolve searched ip addres from host name
	
	Revision 1.15  2012/02/17 17:26:44  chicas
	Added getIpAddressByName; now t_knownhosts.ip_address column could be NULL and ip is resolved using system function gethostbyname;
	
	Revision 1.14  2012/02/09 16:29:21  marpas
	refactoring,
	SENT-12 implemented
	removing compiler warnings
	
	Revision 1.13  2011/11/14 13:27:16  crivig
	si torna indietro a pds
	
	Revision 1.12  2011/11/02 14:35:58  crivig
	hostname got from configuration; solve s1pdgs2343
	
	Revision 1.11  2011/08/29 10:10:28  crivig
	resolve Jira-1610
	
	Revision 1.10  2011/08/24 11:15:11  crivig
	solve bug JIRA 1782; alcso connectionpool passed
	
	Revision 1.9  2011/07/01 12:17:51  crivig
	bug fixed; more retries in soap request handled
	
	Revision 1.8  2011/06/30 11:45:17  crivig
	implicit thred moved to CirculationServive application
	
	Revision 1.7  2011/06/28 17:41:14  micmaz
	integrated the implicit circulator
	
	Revision 1.6  2011/06/28 11:57:06  crivig
	modified interface for response; set status to eligible when insert new record; handle both ip/hostname for remoteUrl interface in dynamic request
	
	Revision 1.5  2011/06/21 13:18:07  crivig
	testing
	
	Revision 1.4  2011/06/16 14:29:58  crivig
	testing..
	
	Revision 1.3  2011/06/15 08:49:11  crivig
	coding
	
	Revision 1.2  2011/06/10 13:53:31  crivig
	still coding
	
	Revision 1.1.1.1  2011/05/31 13:51:16  crivig
	imported new library
	
*/


#ifndef _CirculationSoap_H_
#define _CirculationSoap_H_


#include <acs_c++config.hpp>
#include <Thread.h>
#include <dbPersistent.h>

#pragma GCC diagnostic push // PRQA S 1040 4
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor" 
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wold-style-cast"

//Include SOAP auto generated classes

#include <CirculationPDS_USCOREOrderingBindingService.h>

#pragma GCC diagnostic pop


_ACS_BEGIN_NAMESPACE(Circulation)

	// SOAP Messages pre declarations
	class pds__CirculateDynamicRequestType;
	class pds__CirculateResponseType;
	class pds__CirculateMultipleDynamicRequestType;
	class pds__CirculateMultipleResponseType;
	class pds__CirculateExplicitRequestType;
	class pds__StatusRequestType;
	class pds__StatusResponseType;
	class pds__IncreaseCirculationPriorityRequestType;
	class pds__IncreaseCirculationPriorityResponseType;
	class pds__CirculationOccurrenceRequestType;
	class pds__CirculationOccurrenceResponseType;
	class pds__MultipleStatusRequestType;
	class pds__MultipleStatusResponseType;
	class pds__MultipleResponseType;
	class pds__ResetRetryRequestType;
    class pds__MultipleDetailedStatusResponseType;
	class pds__Outcome ;

_ACS_END_NAMESPACE



_ACS_BEGIN_NAMESPACE(acs)
    
class dbConnection ;
class dbConnectionPool;
class SQLString;
class dbSet;

class CirculationOrderingService : public Circulation::PDS_USCOREOrderingBindingService
{ 

public:

	/** SoapThread Exceptions */
	exDECLARE_EXCEPTION(Exception, exException) ; // Base CirculationOrderingService Exception. // PRQA S 2131, 2502 7
	exDECLARE_EXCEPTION(ClientException, exException) ; // Base CirculationOrderingService Exception. // PRQA S 2153 6 
	exDECLARE_EXCEPTION(CriticalException, Exception) ; // Critical Exception.
	exDECLARE_EXCEPTION(NoSecurityZoneFoundException, Exception) ; // Tell caller that this thread cannot be run 
	exDECLARE_EXCEPTION(LocalhostNotFoundException, Exception) ; // Local host not found in t_knownhosts table
	exDECLARE_EXCEPTION(MissingDbRecordsException, Exception) ; // From query on db some records result as missing
	
	CirculationOrderingService (dbConnectionPool &, StopController &);

	/** Destructor */
	virtual ~CirculationOrderingService() throw() ;
	
	virtual int CirculateDynamic( Circulation::pds__CirculateDynamicRequestType * , Circulation::pds__CirculateResponseType* );
	virtual int CirculateMultiDynamic( Circulation::pds__CirculateMultipleDynamicRequestType * , Circulation::pds__CirculateMultipleResponseType* );
	virtual int CirculateExplicit( Circulation::pds__CirculateExplicitRequestType * ,Circulation::pds__CirculateResponseType* );	
	virtual int GetStatus( Circulation::pds__StatusRequestType* ,Circulation::pds__StatusResponseType* )	;
	virtual int IncreaseCirculationPriority( Circulation::pds__IncreaseCirculationPriorityRequestType*  ,Circulation::pds__IncreaseCirculationPriorityResponseType*);
	virtual int CirculationRecordOccurrence( Circulation::pds__CirculationOccurrenceRequestType* ,Circulation::pds__CirculationOccurrenceResponseType* );
	virtual int GetMultipleStatus(  Circulation::pds__MultipleStatusRequestType*    , Circulation::pds__MultipleStatusResponseType*   );
	virtual int GetMultipleDetailedStatus( Circulation::pds__MultipleStatusRequestType* , Circulation::pds__MultipleDetailedStatusResponseType* );
	virtual int ResetCirculationRetry( Circulation::pds__ResetRetryRequestType*  , Circulation::pds__CirculateResponseType* );
	virtual int ResetCirculationStatus( Circulation::pds__MultipleStatusRequestType*  ,Circulation::pds__MultipleStatusResponseType* );
	virtual int ResetMultipleCirculationStatus( Circulation::pds__MultipleStatusRequestType* ,Circulation::pds__MultipleResponseType* );
	virtual int ReInsertCirculation( Circulation::pds__StatusRequestType* , Circulation::pds__CirculateResponseType* out);
	

	void processCirculateDynamicRequest(Circulation::pds__CirculateDynamicRequestType* , Circulation::pds__CirculateResponseType* );
	void processCirculateMultiDynamicRequest(Circulation::pds__CirculateMultipleDynamicRequestType* , Circulation::pds__CirculateMultipleResponseType* );
	void processCirculateExplicitRequest(Circulation::pds__CirculateExplicitRequestType* , Circulation::pds__CirculateResponseType* );
	void processStatusRequest(Circulation::pds__StatusRequestType*   ,	Circulation::pds__StatusResponseType* )	;
	void increaseCirculationPriority(Circulation::pds__IncreaseCirculationPriorityRequestType*  ,Circulation::pds__IncreaseCirculationPriorityResponseType* );
	void circulationRecordOccurrence(Circulation::pds__CirculationOccurrenceRequestType* , Circulation::pds__CirculationOccurrenceResponseType* );
	void processMultipleStatusRequest(Circulation::pds__MultipleStatusRequestType* , Circulation::pds__MultipleDetailedStatusResponseType* );
	void processMultipleStatusRequest(Circulation::pds__MultipleStatusRequestType* , Circulation::pds__MultipleStatusResponseType* );
	void resetCirculationRetry(Circulation::pds__ResetRetryRequestType* , Circulation::pds__CirculateResponseType* );
	void resetCirculationStatus(Circulation::pds__MultipleStatusRequestType*, Circulation::pds__MultipleStatusResponseType* );
	void resetMultipleCirculationStatus(Circulation::pds__MultipleStatusRequestType*, Circulation::pds__MultipleResponseType* );
	void reInsertCirculation(Circulation::pds__StatusRequestType* , Circulation::pds__CirculateResponseType* ) ;
	void loadConf();
	
	unsigned int getSendTimeout() const throw() { return _sendTimeout; }
	unsigned int getRecvTimeout() const throw() { return _recvTimeout; }
	unsigned int getAcceptTimeout() const throw() { return _acceptTimeout; }
	unsigned int getLocalPort() const throw() { return _localPort; } 
	std::string getHost() const throw() { return _host; } 
	
	
	
protected:

	/** Default Class constructor */
	CirculationOrderingService() ;

private:
	/** Copy Constructor */
	CirculationOrderingService(const CirculationOrderingService & );
	/** Operator = */
	CirculationOrderingService &operator=(const CirculationOrderingService &);
#ifndef CirculationSoap_TEST
private:
#else
public:
#endif
    class circulation_params { // PRQA S 2109
    public:
        circulation_params() : 
            toUrl(),
            destHostIp(),
            destHostId(),
            fromUrl(),
            mode(), 
            tempnamevalue(), 
            tempnameId(),
            removeSource(),
            priority(),
			localTransferModeId(),
			streamType(),
			streamTypeSet(false)
        {} 
        
        circulation_params(const circulation_params &r) : 
            toUrl(r.toUrl),
            destHostIp(r.destHostIp),
            destHostId(r.destHostId),
            fromUrl(r.fromUrl),
            mode(r.mode), 
            tempnamevalue(r.tempnamevalue), 
            tempnameId(r.tempnameId),
            removeSource(r.removeSource),
            priority(r.priority),
			localTransferModeId(r.localTransferModeId),
			streamType(r.streamType),
			streamTypeSet(r.streamTypeSet)
        {} 
	
        ~circulation_params() throw() {}
        
    public:
        std::string toUrl ; // PRQA S 2100 9
        std::string destHostIp ;
        unsigned int destHostId ;
        std::string fromUrl ;
        std::string mode ; 
	    std::string tempnamevalue ;
	    int tempnameId ;
        bool removeSource  ;
	    unsigned int priority ;
		int localTransferModeId;
		std::string streamType;
		bool streamTypeSet;

    private:
        circulation_params &operator= (const circulation_params &r) ;
    } ;

    class relayinfo {
    public:
        relayinfo() : ip_address(), host_name(), relay_id(0) {}
        
        relayinfo(const std::string &ip, const std::string &name, dbPersistent::IdType id) :
            ip_address(ip),
            host_name(name), 
            relay_id(id)
        {}
        relayinfo(const relayinfo &r) :
            ip_address(r.ip_address),
            host_name(r.host_name), 
            relay_id(r.relay_id)
        {}
        relayinfo &operator= (const relayinfo &r) 
        {
            if (this != &r) {
                ip_address = r.ip_address ;
                host_name = r.host_name ;
                relay_id = r.relay_id ;
            }
            return *this ;
        }
        ~relayinfo() throw() {}
        
        std::string ip_address ; // PRQA S 2100 3
        std::string host_name ; 
        dbPersistent::IdType relay_id ;
    } ;

	std::vector<relayinfo> getRelaysForZone(dbPersistent::IdType secZoneId);

    // first is the id of the security zone,
    // second is the sec zone level
	std::pair<dbPersistent::IdType, int> getHostSecZoneInfo(const std::string &ip_address, const std::string &dbKey);
	void performQueryForSet(const SQLString & , dbSet &, const std::string &statQuery ) ;
	void updateCirculationStatus(dbPersistent::IdType, const std::string &, bool noException = false);
	dbPersistent::IdType performQueryForStatus(const SQLString &, const std::string &statQuery, bool firstInsert, bool noException = false);
	bool checkIpAddress( const std::string & , std::string &, unsigned int & , bool insHN= false); 
	dbPersistent::IdType insertCirculationQueueRecord(const circulation_params &, dbSet &, const std::string & dbkey, const std::string & client);
    /* return empty string if not found*/ 
    static std::string getIpAddressByName(const std::string &hostname) throw();
    SQLString getCirculationInsertNoRelay(const circulation_params &, const std::string &, bool insertLocalTransferMode ) const ;
    // first hostname, second ip
    std::pair<std::string, std::string> getRelayInfo(dbPersistent::IdType relayid)  ;
	// Check if a circulation request identical to the current one already exists
	dbPersistent::IdType duplicatedRequestExists( const circulation_params &, const std::string & ) ;
	// update status of circulations with given toUrl and fromUrl possibly managed by relay.
	void updateCirculationsStatus( const std::string & fromUrl, const std::string & toUrl, const std::string & dbKey);
	
	dbPersistent::IdType getAgentNodeId( const std::string & client );
	

#ifndef CirculationSoap_TEST
private:
#else
public:
#endif
   	std::string _hostIp;
   	std::string _localUser;
   	std::string _localPwd;
   	std::string _localProtocol;
	unsigned int _localPort;
	std::string _host;  //name
   	int _hostLevel; //security level
   	unsigned int _hostId;    //id inside t_knownhost (if any)
	unsigned int _priority;
    bool _removeSource ;
	unsigned short _maxDbRetry;
	unsigned int _maxSoapRetry;
	dbConnectionPool & _connPool;
	std::string _tempnameMode;
	std::string _tempnameValue;
	bool _multipleRequestsResetRetry ;
	
    unsigned int _sendTimeout;
    unsigned int _recvTimeout;
    unsigned int _acceptTimeout;
	
	std::vector< std::pair<std::string, dbPersistent::IdType> > _clientLUT;
	dbPersistent::IdType _agentNodeId;
	
	std::string _dynamicCircLocalTransferMode;
	std::string _explicitCircLocalTransferMode;

	bool _hostLevelSet;
	StopController &_stopController;

	static const int _defaultMaxDbRetries = 3 ;
    static const int _defaultMaxSoapRetries = 3 ;
	static const std::string _defaultTempnameMode;
	static const std::string _defaultTempnameValue;
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(CirculationOrderingService) ;
     
};

/**
 * \brief
 * SOAP Server 
 **/

class CirculationSoap :  // PRQA S 2109, 2153
    public Thread
{ 
		
public:

	/** SoapThread Exceptions */
	exDECLARE_EXCEPTION(Exception, exException) ; // Base CirculationSoap Exception. // PRQA S 2131, 2502 7
	exDECLARE_EXCEPTION(CriticalException, Exception) ; // Critical Exception.
	exDECLARE_EXCEPTION(StopException, Exception) ; // Tell caller that this thread cannot be run 
	 

	/** Default Class constructor */
	CirculationSoap() ;
	/** Destructor */
	virtual ~CirculationSoap() throw() ;

	virtual void run() ;

	unsigned int getPort() const throw() ;
	const std::string &getHost() const throw() ;

private:
	/** Copy Constructor */
	CirculationSoap(const CirculationSoap & );
	/** Operator = */
	CirculationSoap &operator=(const CirculationSoap &);

#ifndef CirculationSoap_TEST
private:
#else
public:
#endif
    static bool isSoapValidSocket(int) throw() ;


#ifndef CirculationSoap_TEST
private:
#else
public:
#endif
	unsigned int _localPort;
	std::string _host;  //name
	dbConnectionPool & _connPool;
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(CirculationSoap) ;
     
};

std::ostream &operator<<(std::ostream &, const Circulation::pds__Outcome &) ;
acs::exostream &operator<<(acs::exostream &, const Circulation::pds__Outcome  &) ;

_ACS_END_NAMESPACE


#endif //_CirculationSoap_H_




