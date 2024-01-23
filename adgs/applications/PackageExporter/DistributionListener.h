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
	
	PackageExporter
*/

#ifndef _DistributionListener_H_
#define _DistributionListener_H_


#include <MQTTDbBroadcastListenerBase.h>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet;

///////////////////////////////////////////////////
///////////////////////////////////////////////////
//
// DistributionListener
//
///////////////////////////////////////////////////
///////////////////////////////////////////////////

/**
 *\brief Specialized class to receive the Db broadcast messages related to the PackageExporter
 *
*/
class DistributionListener : // PRQA S 2109, 2153
	public MQTTDbBroadcastListenerBase
{
public:
	explicit DistributionListener(dbConnectionPool& cp);
	virtual ~DistributionListener() ACS_NOEXCEPT ;

protected:
	virtual void init();
		
private:
	
	DistributionListener(const DistributionListener &) = delete; 
	DistributionListener &operator=(const DistributionListener &) = delete ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(DistributionListener)

} ;

_ACS_END_NAMESPACE

#endif // _DistributionListener_H_
