// PRQA S 5600, 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA Common functions $


*/


#include <LTADBSimulationSingleton.h>


_ACS_BEGIN_NESTED_NAMESPACE (acs, lta)

using namespace std;

//
//
//	Singleton Implementation
//
//
ACS_CLASS_DEFINE_DEBUG_LEVEL(LTADBSimulationSingleton) 


LTADBSimulationSingleton::LTADBSimulationSingleton() :
    Singleton<LTADBSimulationSingleton >()
{
}

LTADBSimulationSingleton::~LTADBSimulationSingleton() throw() 
{
}

bool LTADBSimulationSingleton::simulation() const 
{
	return ACS_CLASS_GET_DEBUG() > 0 ;
}

void LTADBSimulationSingleton::simulation(bool simul)  
{
	ACS_CLASS_SET_DEBUG(simul ? 1 : 0) ; // PRQA S 3380
}


_ACS_END_NESTED_NAMESPACE
