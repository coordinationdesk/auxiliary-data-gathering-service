// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Rolling Archive $

*/


#include <KeepPerformer.h>
#include <RollingAction.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(KeepPerformer)

KeepPerformer::KeepPerformer() : ActionPerformer()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

KeepPerformer::~KeepPerformer() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void KeepPerformer::dowork(const RollingAction & actionToProcess, bool & errorOccurred, RollingReport & rollingReport, ConfigurationSingleton::StopPredicate & stop, dbConnection & conn) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//This action is NOP.
	
}

_ACS_END_NAMESPACE
