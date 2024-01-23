// PRQA S 1050 EOF
/*

    Copyright 1995-2020, Exprivia SPA - DFDA-AS
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it


    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA

    SRV Notification Dispatcher

*/ 
#ifndef _SRVNotifier_H_
#define _SRVNotifier_H_ 

#include <acs_c++config.hpp>
#include <Thread.h>
#include <exException.h>
#include <iostream>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

//enum NotificationType {ProductDownloadReadiness, ProductAvailability, ProductDeletion, BatchProductDownloadReadiness};
class SRVNotifier  : public Thread {
public:
	exDECLARE_EXCEPTION(SRVNotifierException, exException) ; 
	exDECLARE_EXCEPTION(RecordNotUpdatedException, SRVNotifierException) ; 

	SRVNotifier();
	virtual ~SRVNotifier() throw();
	
	virtual void run() ;
private:    
	SRVNotifier(const SRVNotifier & ) ; // not implemented
	SRVNotifier& operator= (const SRVNotifier & ) ; // not implemented
    
	void loadConf() ;

#ifdef TEST_ENABLED
public:
#endif
	void cleanExpiredNotifications();
	void getPendingNotification();
	void deleteRow(const dbPersistent::IdType & id);
	void update_subscription(const std::string & uuid, const DateTime & date);

private:
	static const unsigned short _defaultMaxRetry = 5;
	static const unsigned short _defaultLimit = 50;
	static const unsigned short _defaultRetryTime = 3600; // sec
	static const unsigned int _defaultRetentionTime = 259200; // 3 days in seconds

	uint 	_maxRetry;
	size_t	_queryLimit;
	size_t 	_retryTime;
	size_t	_retentionTime;
	bool 	_authentication;
	bool 	_disableNotificationInfo;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SRVNotifier) ;
};

_ACS_END_NAMESPACE

#endif /* _SRVNotifier_H_ */
