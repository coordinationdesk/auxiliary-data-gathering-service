// PRQA S 1050 EOF
/*

	Copyright 1995-2003, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Abstract Factory Library $

	$Id$

	$Author$

	$Log$
	Revision 3.0  2016/11/21 16:02:25  marfav
	Major release
	
	Revision 2.2  2016/11/21 16:02:12  marfav
	S3MPM-847
	Adding interfaces to dump factory chain builders registrations
	
	Revision 2.1  2013/12/16 19:26:29  marpas
	coding best practice applied
	
	Revision 2.0  2006/02/28 10:08:02  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:42:50  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:28:30  marfav
	Import libAbstractFactoryChain
	

*/

#include <afcV.h>
#include <AbstractFactoryChain.h>
#include <exStackTrace.h>

using namespace acs;
using namespace std;

namespace {
 afcV version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(AbstractFactoryChainDumpableInterface)

FactoryChainSingletonHelper::FactoryChainSingletonHelper() : 
	Singleton <FactoryChainSingletonHelper > (),
	_singletonsList(),
	_myMutex()
{
	// Empty implementation
}

void FactoryChainSingletonHelper::addFactoryChainPtr (AbstractFactoryChainDumpableInterface * theptr)
{
	ThreadSafe::Lock lock (_myMutex);
	if ( find (_singletonsList.begin(), _singletonsList.end(), theptr) == _singletonsList.end() )
	{
		_singletonsList.push_back (theptr);
	}
}

vector <AbstractFactoryChainDumpableInterface*> FactoryChainSingletonHelper::getFactoryList() const
{
	ThreadSafe::Lock lock (_myMutex);
	return _singletonsList;
}

void FactoryChainSingletonHelper::dumpCapabilities() const
{
	ThreadSafe::Lock lock (_myMutex);
	for (size_t idx=0; idx<_singletonsList.size(); ++idx)
	{
		ACS_LOG_INFO ("Dumping capabilities of factory [" << exStackTrace::demangle(typeid ( (*_singletonsList.at(idx)) ).name()) << "]");
		_singletonsList.at(idx)->dumpCapabilities();
	}
}



