// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DGFBroadcastListener Library $

	$Name$

	$Id$

	$Author$

	$Log$
	Revision 1.11  2014/06/24 11:48:52  marpas
	coding best practices applied
	
	Revision 1.10  2009/06/26 10:41:00  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.9  2005/05/31 08:11:26  marpas
	Thread synchronization improved.
	Documentation fixed
	
	Revision 1.8  2005/03/25 10:11:23  serfol
	modified the messages as exFilterable and not as ostringstream
	
	Revision 1.7  2004/10/14 10:36:24  serfol
	adding a flag to generate the artificial broadcast message
	
	Revision 1.6  2003/08/06 07:47:27  serfol
	improving the elapsed time timer. deleted a private attribute
	
	Revision 1.5  2003/08/05 14:32:00  serfol
	implemented the reload configuration
	
	Revision 1.4  2003/07/04 12:49:13  serfol
	Wished time is read in ctor setting a private variable
	
	Revision 1.3  2003/04/28 15:26:01  serfol
	GCC 3.2
	
	Revision 1.2  2003/01/09 10:01:37  serfol
	Modify on WS
	
	Revision 1.1.1.1  2002/12/18 17:35:38  luipir
	import libDgfListener start
	
	

*/

#ifndef _DGFBroadcastListener_H_
#define _DGFBroadcastListener_H_


#include <MQTTDbBroadcastListenerBase.h>
#include <Timer.h>
#include <vector>
#include <Thread.h>

_ACS_BEGIN_NAMESPACE(acs)

class rsResourceSet;

///////////////////////////////////////////////////
///////////////////////////////////////////////////
//
// DGFBroadcastListener
//
///////////////////////////////////////////////////
///////////////////////////////////////////////////

/**
 *\brief Specialized class to receive the Db broadcast messages related to the PackageOptimizer
 *
*/
class DGFBroadcastListener : // PRQA S 2109, 2153
	public MQTTDbBroadcastListenerBase
{
public:
	/*
	 *\param true/false to enable the wished messages.
	*/
	explicit DGFBroadcastListener(dbConnectionPool& cp);
	virtual ~DGFBroadcastListener() ACS_NOEXCEPT ;

protected:
	virtual void init();
		
private:
	
	DGFBroadcastListener(const DGFBroadcastListener &) = delete; 
	DGFBroadcastListener &operator=(const DGFBroadcastListener &) = delete ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(DGFBroadcastListener)

} ;

_ACS_END_NAMESPACE

#endif // _DGFBroadcastListener_H_
