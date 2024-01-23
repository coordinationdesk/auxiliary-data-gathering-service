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

	$Prod: A.C.S. Order Creator library $

	$Id$

	$Author$

	$Log$
	Revision 2.5  2013/02/11 12:50:19  marpas
	coding best practices
	dbConnectionPool concept enforced
	qa rules
	new interfaces
	
	Revision 2.4  2012/03/07 13:09:24  marpas
	refactoring
	introducing XMLOrderParameters class and its unitary test
	
	Revision 2.3  2011/03/01 17:24:21  marpas
	GCC 4.4.x support
	
	Revision 2.2  2009/04/20 17:29:07  marpas
	debug improved
	
	Revision 2.1  2007/03/12 13:21:59  marpas
	singleton release now conditioned to the usual RELEASE_SINGLETON_AT_EXIT compilation variable.
	
	Revision 2.0  2006/02/28 10:23:56  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:09:06  marfav
	Import libOrderCreator

*/



#include <OrderCreatorFactoryChain.h>
#include <OrderCreatorFactory.h>
#include <Filterables.h>
#include <algorithm>

#if defined __linux__ 
  #if (__GNUC__ < 3) || (__GNUC__ == 4)
#include <algorithm>
  #endif
#endif

#include <cstdlib>

#include <ogenV.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace {
    ogenV version;
}

using std::string ;
using std::vector ;
using std::map ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(OrderCreatorFactoryChain)

OrderCreatorFactoryChain::OrderCreatorFactoryChain() :
    Singleton<OrderCreatorFactoryChain >(), 
    _factories()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
}

OrderCreatorFactoryChain::~OrderCreatorFactoryChain() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631  
}


void OrderCreatorFactoryChain::registerFactory(OrderCreatorFactory *f)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"registering " << f) ;
	_factories.push_back(f) ;
}

void OrderCreatorFactoryChain::unregisterFactory(OrderCreatorFactory *f)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"unregistering " << f) ;
	vector<OrderCreatorFactory *>::iterator tbd = std::find(_factories.begin(), _factories.end(), f) ;
	if (tbd != _factories.end()) {
		_factories.erase(tbd) ;
	}
}

OrderCreator *OrderCreatorFactoryChain::newCreator(const string &orderType, const string &algo, unsigned int satId, dbConnection &c)  // PRQA S 4020
{
	OrderCreator *oc = 0 ;
    size_t fnum = _factories.size() ;
	for (size_t i=0; i < fnum ; ++i) {
		if ((oc = _factories[i]->newCreator(orderType,algo,satId,c))) {
			return oc ;
        }
    }
	return 0 ;
}

string OrderCreatorFactoryChain::capabilities() const 
{
	map <string, string> cap ;
    size_t fnum = _factories.size() ;
	for (size_t i=0; i < fnum ; ++i) {
		_factories[i]->capabilities(cap) ;
    }
	string ret ;
	for (map <string, string>::iterator i = cap.begin(); i != cap.end(); ++i) { // PRQA S 4238
		ret += (*i).first + "\n" + (*i).second + "\n"  ;
	}

	return ret ;
}


_ACS_END_NAMESPACE
