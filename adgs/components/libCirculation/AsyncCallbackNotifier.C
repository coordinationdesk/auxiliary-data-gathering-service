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

	$Prod: A.C.S. libCirculation$

	$Id$

	$Author$

	$Log$
	Revision 7.3  2018/06/14 14:42:24  damdec
	PDSEV-24: implementation finalized.
	
	Revision 7.2  2017/10/19 16:18:03  marpas
	getting rid of EntityBean
	
	Revision 7.1  2017/03/03 09:30:23  marfav
	S3MPM-874
	Aligned to libException Rev_5_93 new Condition Interface
	
	Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
	Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
	
	Revision 1.1  2016/08/31 08:52:19  damdec
	Class added.
	
        
*/ 

#include <AsyncCallbackNotifier.h>
#include <CirculationCallbackSingleton.h>
#include <CirculationCallbackSoap.h>

#include <Filterables.h>
#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor" // PRQA S 1040 3
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wold-style-cast"


#include "CirculationCallbackPDS_USCOREDCBindingProxy.h"

#pragma GCC diagnostic pop

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(AsyncCallbackNotifier)

//Mutex AsyncCallbackNotifier::_mutex;


AsyncCallbackNotifier::AsyncCallbackNotifier() :	
		Thread(""),
		_sendTimeout(_defaultSendTimeout),
		_acceptTimeout(_defaultAcceptTimeout),
		_recvTimeout(_defaultRecvTimeout),
		_maxSoapRetry(0),
		_errorFound(false),
		_stopRequested(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

AsyncCallbackNotifier::~AsyncCallbackNotifier() throw(){
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

void AsyncCallbackNotifier::run() // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	loadConf();

	while( ! this->requestedCancel() ) {

		{
			//--lock the condition
			ThreadSafe::Lock lock( CirculationCallbackSingleton::instance()->getStandbyCondition() ); // PRQA S 3050

			//You can rest if there's nothing to do -- while (!(boolean_predicate)) {
			while ( ( CirculationCallbackSingleton::instance()->empty() ) && ! this->requestedCancel() ) 
			{
				try {
					//Sleep and wake me up on _standbyCondition notification or on timer elapses.
					//release mutex and block on condition
					if ( false == CirculationCallbackSingleton::instance()->getStandbyCondition().timedwait( 10000 ) ) { //-- wait on condition // PRQA S 4400
						// Time check expired. Checking status...
					}
					//The mutex is locked by me here (as per timedwait implementation)

				}  catch (exception& ex) {
					ACS_LOG_WARNING("Got exception in the wake-up condition. Try on next loop. Exception:" << ex.what() );
				}

				//The mutex is locked by me here (as per pthread_cond_wait ans timedwait)
				//Notification received OR No notification received for a while. Check the status
			}

		}
		//-- do protected work. The mutex is locked by me here
		//Something to do at last...

		try
		{
			//Consume the PendingCirculation queue
			this->consumeQueue( );
		}
		catch (std::exception const & ex)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Callback pending circulation queue cannot be consumed!" << ex.what());
		}

		//-- unlock the condition automatically
	}

	if( requestedCancel()) {
		ACS_LOG_INFO("Stop request received.");
	}

}
void AsyncCallbackNotifier::consumeQueue() // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// callback call
	// create new Thread and start it.
	while( !CirculationCallbackSingleton::instance()->empty() && ! this->requestedCancel()) // some item in the queue
	{
		CirculationInfo item;
		// consume queue
		if (CirculationCallbackSingleton::instance()->getCallbackEventsFromQueue(item))
		{	
			// Perform SOAP call to the service that performs callback Reception

			::CirculationCallback::PDS_USCOREDCBindingProxy proxy;
			proxy.soap_endpoint = item.url.c_str();
			proxy.send_timeout = _sendTimeout ; // PRQA S 4400 3
			proxy.accept_timeout = _acceptTimeout ;
			proxy.recv_timeout = _recvTimeout ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "SOAP endpoint: \"" << proxy.soap_endpoint << "\"");

			::CirculationCallback::pds__CallbackRequestType callback;
			//fill request
			ostringstream os;
			os << item.extId;
			string extString = os.str();
			callback.ExternalID = extString;
			callback.ModificationDate = item.timestamp.getSqlString();
			callback.Status = item.status_id;
			callback.Hostname = &(item.hostname);
			ostringstream osC;
			osC << item.circId;
			string circId = osC.str();
			callback.CirculationId = &circId;

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Get from Pending Circulations the following info: status_id = " << callback.Status  << " updatetime = "<< callback.ModificationDate<< " external_id = " << callback.ExternalID << " circulation id = "
			<< *callback.CirculationId << " Hostname = " << *callback.Hostname);

			::CirculationCallback::pds__CallbackResponseType response;

			bool soapServerContacted = false;
			for (unsigned int j = 0 ; j < _maxSoapRetry && ! this->requestedCancel(); j++)
			{
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "retry =  " << j );

				int soap_error_code = proxy.StatusUpdateCallback(&callback , &response);
				if ( soap_error_code == SOAP_OK) 
				{
					soapServerContacted = true;
					break;
				}
				else {

					ostringstream error ;
					error << "Error while contacting server: " << proxy.soap_endpoint << " for callback reception request ";
					//soap_stream_fault(proxy.soap, error) ;
					ACS_LOG_WARNING(error.str());

					//--lock the condition
					ThreadSafe::Lock lock( CirculationCallbackSingleton::instance()->getStandbyCondition() ); // PRQA S 3050
					CirculationCallbackSingleton::instance()->getStandbyCondition().timedwait( 1000 ); // 1 second delay between attempts
				}
			}

			if (!soapServerContacted)
			{
				ostringstream os;
				os << "Cannot definitely connect to Soap service \""  << proxy.soap_endpoint << "\" to update status for \"" << item.extId << "\" id cross table";
				ACS_LOG_WARNING(os.str()); // or ACS_LOG_INFO???
				_errorFound = true;
			}
		}

	}
}
void AsyncCallbackNotifier::loadConf()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	rsResourceSet &rset = ConfigurationSingleton::instance()->get();
   	rsPushNSpace cServ(rset,"CirculationService");

    rset.getValue ("SoapSendTimeout", _sendTimeout, false);
    rset.getValue ("SoapReceiveTimeout", _recvTimeout, false);
    rset.getValue ("SoapAcceptTimeout", _acceptTimeout, false);
	
	try
	{
 		rset.getValue("SoapRetries", _maxSoapRetry);
        ACS_COND_THROW(	_maxSoapRetry < 0, exIllegalValueException("SoapRetries must be > 0")) ; // PRQA S 3081
	}
	catch (exception &e)
	{
		// if no SoapRetries configured -> set default value 
		_maxSoapRetry = _defaultMaxSoapRetries ;
        ACS_LOG_INFO( "SoapRetries not found in configuration, set to default: " << _maxSoapRetry ) ;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Soap Send timeout = " << _sendTimeout) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Soap Receive timeout = " << _recvTimeout) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Soap Accept timeout = " << _acceptTimeout) ;
}

void AsyncCallbackNotifier::stopImmediate()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TERM all immediate.\n");
	setHaltLevel(StopController::IMMEDIATE);
	// get the condition and broadcast to allow immediate exit from the run
	{
		ThreadSafe::Lock lock( CirculationCallbackSingleton::instance()->getStandbyCondition() ); // PRQA S 3050
		CirculationCallbackSingleton::instance()->getStandbyCondition().broadcast();
	}

	stopProcessQueue();
	stop();
}
void AsyncCallbackNotifier::stopGracefully()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "TERM all gracefully.\n");
	setHaltLevel(StopController::LAZY);
	// get the condition and broadcast to allow immediate exit from the run
	{
		ThreadSafe::Lock lock( CirculationCallbackSingleton::instance()->getStandbyCondition() ); // PRQA S 3050
		CirculationCallbackSingleton::instance()->getStandbyCondition().broadcast();
	}
	stopProcessQueue();
}
void AsyncCallbackNotifier::stopProcessQueue()
{
	if (_errorFound)
	{
		_stopRequested = true;
	}
}

void AsyncCallbackNotifier::waitForExit () const
{
	ACS_CLASS_ANNOUNCE_DEBUG_TH(ACS_LOW_VERB);
	while ( Thread::running() )
	{
		Thread::waitExitOrTimeout (1000);
	}
}

_ACS_END_NAMESPACE

