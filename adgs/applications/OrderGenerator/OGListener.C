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
	Revision 2.4  2013/02/13 18:23:25  marpas
	Connection Pool concept adopted
	application flow completely rewritten
	some methods moved into libOrderCreator (new class OrderExecutor)
	coding best practices applied
	qa rules
	
	Revision 2.3  2009/06/26 10:28:03  marpas
	no message from some class dtors
	
	Revision 2.2  2006/06/23 12:28:20  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.1  2006/06/21 13:01:04  ivafam
	Method compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.0  2006/02/28 15:49:11  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/07/11 12:18:21  marpas
	new broadcast for t_temporaryorders
	
	Revision 1.4  2005/02/14 20:30:18  marpas
	headers fixed
	
	Revision 1.3  2003/06/04 19:33:53  marpas
	V1 test.
	
	Revision 1.2  2003/03/13 17:32:01  marpas
	some improvements
	
	Revision 1.1  2003/02/18 16:57:23  marpas
	test in progress
	
	

*/
 
#include <OGListener.h>
#include <StringUtils.h>


_ACS_BEGIN_NAMESPACE(acs)
ACS_CLASS_DEFINE_DEBUG_LEVEL(OGListener)

OGListener::OGListener(dbConnectionPool& cp) : MQTTDbBroadcastListenerBase(cp)
{
}
OGListener::~OGListener() ACS_NOEXCEPT {};
void OGListener::init()
{
	std::vector<MQTTDbEventType> dbEvents;
	// t_inventory events
	dbEvents.push_back(MQTTDbEventType("t_inventory", "INSERT"));
	// t_temporaryorders events
	dbEvents.push_back(MQTTDbEventType("t_temporaryorders", "INSERT"));

	setDbEventInterests(dbEvents);
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Subscription for Db broadcast events performed.");
}

_ACS_END_NAMESPACE
