/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Default PreConditionChecker Library $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2015/06/22 12:43:02  damdec
	Compilation warnings removed.
	
	Revision 1.3  2012/02/10 17:31:22  marpas
	refactoring in progress
	
	Revision 1.2  2008/11/07 15:41:27  marfav
	Dead code removed
	
	Revision 1.1.1.1  2008/08/04 10:10:39  marfav
	Importing libTlDefaultPCC
	
	

*/

#include <DefaultTl_PCC.h>


#include <TaskTable.h>
#include <StringKey.h>
#include <AlternativeCheckerFactoryChain.h>
#include <Alternative_PCC.h>
#include<dbConnection.h>

#include <functional>
#include <algorithm>


using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(DefaultTl_PCC)

DefaultTl_PCC::DefaultTl_PCC (StringKey factoryKey) :
	ThinLayer_PCC (factoryKey)
{
	// CTOR
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	// This implementation of the PCC will halt on any error 
	// (Default implementation)
	stopOnFailure(true);
}

DefaultTl_PCC::~DefaultTl_PCC () throw() 
{
	// DTOR
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}
_ACS_END_NAMESPACE


