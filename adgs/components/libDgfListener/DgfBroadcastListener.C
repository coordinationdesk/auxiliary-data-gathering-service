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
	Revision 1.23  2016/04/14 09:57:35  marpas
	Thread does not inheriths any longer from ThreadSafe
	
	Revision 1.22  2014/06/24 11:48:52  marpas
	coding best practices applied
	
	Revision 1.21  2012/02/13 17:04:55  marpas
	refactoring in progress
	
	Revision 1.20  2009/06/26 10:41:00  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.19  2006/06/23 09:20:25  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 1.18  2006/06/20 17:07:41  ivafam
	Method comapreNocase replaced with StringUtils::equalsNoCase
	
	Revision 1.17  2005/05/31 08:11:26  marpas
	Thread synchronization improved.
	Documentation fixed
	
	Revision 1.16  2005/03/25 10:11:23  serfol
	modified the messages as exFilterable and not as ostringstream
	
	Revision 1.15  2004/10/20 13:00:54  serfol
	adding exFwarning and exFError
	
	Revision 1.14  2004/10/14 10:36:24  serfol
	adding a flag to generate the artificial broadcast message
	
	Revision 1.13  2004/03/19 17:46:19  serfol
	
	n debug last message having wish...
	
	Revision 1.12  2003/10/15 15:12:50  serfol
	deleting the <wished message>
	
	Revision 1.11  2003/08/28 17:19:37  serfol
	bug in the custom action. restarted internal timer
	
	Revision 1.10  2003/08/06 07:47:27  serfol
	improving the elapsed time timer. deleted a private attribute
	
	Revision 1.9  2003/08/05 14:32:00  serfol
	implemented the reload configuration
	
	Revision 1.8  2003/07/04 12:49:13  serfol
	Wished time is read in ctor setting a private variable
	
	Revision 1.7  2003/06/06 09:55:36  serfol
	ostringstream
	
	Revision 1.6  2003/04/28 15:26:01  serfol
	GCC 3.2
	
	Revision 1.4  2003/02/20 18:17:39  marpas
	StringUtils::equalsNoCase fixed for empty strings
	
	Revision 1.3  2003/01/31 09:09:35  serfol
	filter messages is now case sensitive
	
	Revision 1.2  2003/01/09 10:01:37  serfol
	Modify on WS
	
	Revision 1.1.1.1  2002/12/18 17:35:38  luipir
	import libDgfListener start
	
	

*/
 
#include <DgfListenerV.h>
#include <DgfBroadcastListener.h>
#include <rsResourceSet.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h>
#include <StringUtils.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
namespace {
    DgfListenerV version;
}
ACS_CLASS_DEFINE_DEBUG_LEVEL(DGFBroadcastListener);


DGFBroadcastListener::DGFBroadcastListener(dbConnectionPool& cp):
    MQTTDbBroadcastListenerBase(cp)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)  ;

	// if true a SIGUSR2 is emitted when a "wished" message comes from the DB
	sendSignal(false);
}

DGFBroadcastListener::~DGFBroadcastListener() ACS_NOEXCEPT 
{
}

void DGFBroadcastListener::init()
{
	std::vector<MQTTDbEventType> dbEvents;
	// T_DistributionItems events
	MQTTDbEventType et1("t_distributionitems", "INSERT");
	et1.addEvent("UPDATE");
	dbEvents.push_back(et1);

	setDbEventInterests(dbEvents);
	ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "Subscription for Db broadcast events performed.");
}

_ACS_END_NAMESPACE
