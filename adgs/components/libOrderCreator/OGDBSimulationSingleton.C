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

	$Prod: A.C.S. Order Generator $

	$Id$

	$Author$

	$Log$
	Revision 2.2  2013/02/11 12:50:19  marpas
	coding best practices
	dbConnectionPool concept enforced
	qa rules
	new interfaces
	
	Revision 2.1  2007/04/18 13:34:11  marpas
	simulation can be forced
	
	Revision 2.0  2006/02/28 10:23:55  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/10/06 15:18:37  marfav
	Header tags fixed
	

*/


#include <OGDBSimulationSingleton.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

//
//
//	Singleton Implementation
//
//
ACS_CLASS_DEFINE_DEBUG_LEVEL(OGDBSimulationSingleton) 


OGDBSimulationSingleton::OGDBSimulationSingleton() :
    Singleton<OGDBSimulationSingleton >()
{
}

OGDBSimulationSingleton::~OGDBSimulationSingleton() throw() 
{
}

bool OGDBSimulationSingleton::simulation() const 
{
	return ACS_CLASS_GET_DEBUG() > 0 ;
}

void OGDBSimulationSingleton::simulation(bool simul)  
{
	ACS_CLASS_SET_DEBUG(simul ? 1 : 0) ; // PRQA S 3380
}


_ACS_END_NAMESPACE
