// PRQA S 1050 EOF
/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libCirculation$

	$Id$

	$Author$

	$Log$
	Revision 7.1  2017/10/19 16:18:03  marpas
	getting rid of EntityBean
	
	Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
	Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
	
	Revision 1.1  2016/08/31 08:52:19  damdec
	Class added.
	
        
*/

#ifndef _AsyncCallbackNotifier_H_
#define _AsyncCallbackNotifier_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Thread.h>

_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Thread that notify Circulation Status changes 
**/

class AsyncCallbackNotifier : public Thread  { // PRQA S 2109
public:

	/** DS_CirculationCallback Exceptions */
	exDECLARE_EXCEPTION(exAsyncCallbackNotifierException, exException); // PRQA S 2131, 2502

	AsyncCallbackNotifier();
	virtual ~AsyncCallbackNotifier() throw();

    /**
     * Consume the queue pf pending circulations (and notify the callback).
     * <ul>
     * <li> terminats all when a TERM or SIGIN signal arrives.
     * </ul>
     */
    virtual void run();
//    void halt_(); 
	void stopImmediate();
	void stopGracefully();	

	void waitForExit() const;
	
private:  
	AsyncCallbackNotifier(const AsyncCallbackNotifier & ); // not implemented 
	AsyncCallbackNotifier &operator=(const AsyncCallbackNotifier &); // not implemented 

#ifdef ACS_TEST
public:
#else
private:
#endif
	void consumeQueue();
	void loadConf();
	void stopProcessQueue();
		
#ifdef ACS_TEST
public:
#else
private:
#endif
	int _sendTimeout; // soap has int
	int _acceptTimeout; // soap has int
	int _recvTimeout; // soap has int
	unsigned int _maxSoapRetry;
	
	bool _errorFound;
	bool _stopRequested;

    static const int _defaultSendTimeout = 60 ;
    static const int _defaultAcceptTimeout = 60 ;
    static const int _defaultRecvTimeout = 60 ;
    static const int _defaultMaxSoapRetries = 3 ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(AsyncCallbackNotifier)

};

_ACS_END_NAMESPACE

#endif //_AsyncCallbackNotifier_H_

