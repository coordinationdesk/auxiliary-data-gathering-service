// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

*/

#ifndef _PackagerOptBroadcastListener_H_
#define _PackagerOptBroadcastListener_H_


#include <MQTTDbBroadcastListenerBase.h>
#include <Timer.h>
#include <vector>
#include <Thread.h>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet;

///////////////////////////////////////////////////
///////////////////////////////////////////////////
//
// PackagerOptBroadcastListener
//
///////////////////////////////////////////////////
///////////////////////////////////////////////////

/**
 *\brief Specialized class to receive the Db broadcast messages related to the PackagerOptimizer
 *
*/
class PackagerOptBroadcastListener : // PRQA S 2109, 2153
	public MQTTDbBroadcastListenerBase
{
public:
	/*
	 *\param true/false to enable the wished messages.
	*/
	explicit PackagerOptBroadcastListener(dbConnectionPool& cp);
	virtual ~PackagerOptBroadcastListener() ACS_NOEXCEPT ;

protected:
	virtual void init();
		
private:
	
	PackagerOptBroadcastListener(const PackagerOptBroadcastListener &) = delete; 
	PackagerOptBroadcastListener &operator=(const PackagerOptBroadcastListener &) = delete ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackagerOptBroadcastListener)

} ;

_ACS_END_NAMESPACE

#endif // _PackagerOptBroadcastListener_H_
