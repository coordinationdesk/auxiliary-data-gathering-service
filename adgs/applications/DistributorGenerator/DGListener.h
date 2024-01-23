// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod:  $

	$Name$

	$Id$

	$Author$

	$Log$
	Revision 1.2  2009/06/26 10:21:21  marpas
	no message from some class dtors
	
	Revision 1.1.1.1  2007/03/15 15:01:03  crivig
	Imported source
	
*/

#ifndef _DGListener_H_
#define _DGListener_H_


#include <acs_c++config.hpp>
#include <MQTTDbBroadcastListenerBase.h>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)


///////////////////////////////////////////////////
///////////////////////////////////////////////////
//
// DGListener
//
///////////////////////////////////////////////////
///////////////////////////////////////////////////

class DGListener :      // PRQA S 2153
	public MQTTDbBroadcastListenerBase
{
public:
	DGListener(dbConnectionPool& cp) ;
	virtual ~DGListener() ACS_NOEXCEPT ;
	
	// declared but not implemented
	DGListener(const DGListener &)  = delete;
	DGListener &operator=(const DGListener &)  = delete ;
protected:
	virtual void init();
private:
   
	ACS_CLASS_DECLARE_DEBUG_LEVEL (DGListener)
} ;

_ACS_END_NAMESPACE

#endif // _DGListener_H_
