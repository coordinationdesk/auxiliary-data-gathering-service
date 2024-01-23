// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia, SpA

	$Prod: A.C.S. OGListener Library $

	$Name$

	$Id$

	$Author$

	$Log$
	Revision 2.2  2013/02/13 18:23:25  marpas
	Connection Pool concept adopted
	application flow completely rewritten
	some methods moved into libOrderCreator (new class OrderExecutor)
	coding best practices applied
	qa rules
	
	Revision 2.1  2009/06/26 10:28:03  marpas
	no message from some class dtors
	
	Revision 2.0  2006/02/28 15:49:11  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/02/14 20:30:18  marpas
	headers fixed
	
	Revision 1.1  2003/02/18 16:57:23  marpas
	test in progress
	
	Revision 1.2  2003/01/09 10:01:37  serfol
	Modify on WS
	
	Revision 1.1.1.1  2002/12/18 17:35:38  luipir
	import libDgfListener start
	
	

*/

#ifndef _OGListener_H_
#define _OGListener_H_


#include <acs_c++config.hpp>
#include <MQTTDbBroadcastListenerBase.h>
#include <MQTTMessageHandler.h>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

///////////////////////////////////////////////////
///////////////////////////////////////////////////
//
// OGListener
//
///////////////////////////////////////////////////
///////////////////////////////////////////////////

class OGListener :      // PRQA S 2153
	public MQTTDbBroadcastListenerBase
{
public:
	OGListener(dbConnectionPool& cp);
	virtual ~OGListener() ACS_NOEXCEPT;
	
protected:
	virtual void init();
private:
	
	// declared but not implemented
	OGListener(const OGListener &) = delete;
	OGListener &operator=(const OGListener &) = delete ;
   
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL (OGListener)
} ;

_ACS_END_NAMESPACE

#endif // _OGListener_H_
