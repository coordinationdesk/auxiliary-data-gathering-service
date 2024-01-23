// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Scheduler Default Log Plugin library $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2013/10/18 11:19:51  marpas
	adopting libException 5.16 interfaces
	
	Revision 1.2  2004/12/03 17:54:43  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/09/14 16:36:52  marfav
	Import libScDefaultLogGroup
	

*/



#include <ScDefaultLogGroupFactory.h>
#include <ScLogHandlerFactoryChain.h>
#include <ScLogHandlerGroup.h>
#include <ScDefaultLogHandlerGroup.h>

#include <scdlgV.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace {

    const char* _cosmoScLogGroupConfigKey = "Scheduler.PluginsRegistrationSettings.ScDefaultLogGroupFactory";

    scdlgV version;
}


// ********************************************************************************
// ***************** SCHEDULER DEFAULT LOG GROUP FACTORY **************************
// ********************************************************************************

ACS_CLASS_DEFINE_DEBUG_LEVEL(ScDefaultLogGroupFactory);

ScDefaultLogGroupFactory::ScDefaultLogGroupFactory () :
		StringKeyFactory <ScLogHandlerGroup, scOrder> (_cosmoScLogGroupConfigKey, &ScDefaultLogGroupFactory::buildScDefaultLogGroup)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

ScLogHandlerGroup*
ScDefaultLogGroupFactory::buildScDefaultLogGroup (const scOrder& theWorkOrder)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	return new ScDefaultLogHandlerGroup (theWorkOrder);
}

// ********************************************************************************
// ***************** SCHEDULER DEFAULT LOG GROUP FACTORY **************************
// ********************************************************************************




// *************************************************************
// ***************** EMBEDDED FACTORY **************************
// *************************************************************

ScDefaultLogGroupFactory EmbeddedScDefaultLogGroupFactory;

// *************************************************************
// ***************** EMBEDDED FACTORY **************************
// *************************************************************


_ACS_END_NAMESPACE
