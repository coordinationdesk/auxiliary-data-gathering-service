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

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.2  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.1  2016/04/14 10:49:51  marpas
	implemented as singleton
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2013/10/31 17:07:17  marpas
	UniqueNameFormatter concept added
	getReportUniqueNameFormatter method added
	this allow a report to be generated using a unique name.
	
	Revision 4.0  2013/09/30 12:30:55  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:23  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.9  2013/07/02 12:03:19  marpas
	qa warnings and coding best practices
	
	Revision 2.8  2013/02/25 10:08:59  marpas
	useless predicate removed
	
	Revision 2.7  2012/03/08 15:21:46  marpas
	refactoring
	
	Revision 2.6  2012/02/14 12:52:46  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.5  2009/04/08 10:08:19  marpas
	system concept introduced in order to have a writer for a system with no satellite
	
	Revision 2.4  2006/06/23 10:17:52  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.3  2006/06/20 16:41:22  ivafam
	Method compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.2  2006/04/12 15:40:13  paoscu
	Added dbFixedHeaderFactoryChain::_mutex definition.
	
	Revision 2.1  2006/04/12 15:33:09  paoscu
	Tread safety added.
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/09/08 17:46:06  paoscu
	dbFixedHeaderFactoryChain bug fixed. Now ignoreCase for SatelliteName is implemented
	CryosatFixedHeaderFactory minor bug fixed when returning satelliteId
	
	Revision 1.1  2004/05/03 14:54:56  paoscu
	New header writers .
	
		

*/

#include <dbFixedHeaderFactoryChain.h>
#include <NoneFixedHeaderFactory.h>
#include <StringUtils.h>
#include <Filterables.h>
#include <algorithm>

using namespace acs;
using namespace std;


//**************************** dbFixedHeaderFactory *********************************************
dbFixedHeaderFactory::~dbFixedHeaderFactory() throw()
{
    try {
	    dbFixedHeaderFactoryChain::instance()->unregisterFactory(*this) ; // PRQA S 4631
    }
    catch(exception &x ) {
        ACS_LOG_NOTIFY_EX(x) ; // PRQA S 4631
    }
}



const string &dbFixedHeaderFactory::name() const // PRQA S 4120
{
	return _name ;
}

const string &dbFixedHeaderFactory::system() const // PRQA S 4120
{
	return _system ;
}

dbFixedHeaderFactory::UniqueNameFormatter &dbFixedHeaderFactory::getReportUniqueNameFormatter() const 
{ return repunamefmt_ ; }


dbFixedHeaderFactory::dbFixedHeaderFactory(const string &name, const string &system) :
	 _name (name),
	 _system(system),
      repunamefmt_()
{
	dbFixedHeaderFactoryChain::instance()->registerFactory(*this) ;
}



//**************************** dbFixedHeaderFactoryChain *********************************************

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbFixedHeaderFactoryChain) 

size_t dbFixedHeaderFactoryChain::getFactoryNames(vector<string> &names) 
{
	Lock l_(_M_mutex());

	names.clear() ;
	map<string, const dbFixedHeaderFactory *>::iterator i ;
	for (i=_nameFactories.begin(); i != _nameFactories.end(); ++i) { 
		names.push_back(i->second->name()) ;
    }
	return names.size() ;
}

size_t dbFixedHeaderFactoryChain::getFactorySystems(vector<string> &systems) 
{
	Lock l_(_M_mutex());

	systems.clear() ;
	map<string, const dbFixedHeaderFactory *>::iterator i ;
	for (i=_systemFactories.begin(); i != _systemFactories.end(); ++i) {
		systems.push_back(i->second->system()) ;
    }
	return systems.size() ;
}

void dbFixedHeaderFactoryChain::registerFactory(const dbFixedHeaderFactory &f) 
{
	Lock l_(_M_mutex());

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "name=\"" << f.name() <<"\"") ;
	_nameFactories[f.name()] = &f ;
	_systemFactories[f.system()] = &f ;
}

void dbFixedHeaderFactoryChain::unregisterFactory(const dbFixedHeaderFactory &f) 
{
	Lock l_(_M_mutex());

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "name=\"" << f.name() <<"\"") ;
	map<string, const dbFixedHeaderFactory *>::iterator nametbd=_nameFactories.find(f.name()) ;
	map<string, const dbFixedHeaderFactory *>::iterator systemtbd=_systemFactories.find(f.system()) ;
	if (nametbd != _nameFactories.end()) {
		_nameFactories.erase(nametbd) ;
    }
	if (systemtbd != _systemFactories.end()) {
		_systemFactories.erase(systemtbd) ;
    }
}



const dbFixedHeaderFactory *dbFixedHeaderFactoryChain::getFactoryByName(const string &n, bool ignoreCase) // PRQA S 4020
{
	Lock l_(_M_mutex());

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "name=\"" << n <<"\"" // PRQA S 3380
			   <<" case: " << (ignoreCase ? "ignored":"preserved")) ;

	map<string, const dbFixedHeaderFactory *>::iterator tbf ;
	if (!ignoreCase) {
		tbf = _nameFactories.find(n) ;
	}
	else {
		for (tbf=_nameFactories.begin(); tbf!=_nameFactories.end(); tbf++) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checking " << tbf->first) ;
			if (StringUtils::equalsNoCase(tbf->first, n)) {
				break ;
            }
		}
	}
	if (tbf != _nameFactories.end()) {
		return tbf->second ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "no factory found ") ;
	return 0 ;
}


const dbFixedHeaderFactory *dbFixedHeaderFactoryChain::getFactoryBySystem(const string &n, bool ignoreCase) // PRQA S 4020
{
	Lock l_(_M_mutex());

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "name=\"" << n <<"\"" // PRQA S 3380
			   <<" case: " << (ignoreCase ? "ignored":"preserved")) ;

	map<string, const dbFixedHeaderFactory *>::iterator tbf ;
	if (!ignoreCase) {
		tbf = _systemFactories.find(n) ;
	}
	else {
		for (tbf=_systemFactories.begin(); tbf!=_systemFactories.end(); tbf++) {
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checking " << tbf->first) ;
			if (StringUtils::equalsNoCase(tbf->first, n)) {
				break ;
            }
		}
	}
	if (tbf != _systemFactories.end()) { 
		return tbf->second ;
    }
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "no factory found ") ;
	return 0 ;
}


// DO NOT MOVE THE DECLARATION FROM HERE
namespace {
    NoneFixedHeaderFactory _NoneFixedHeaderFactory ;
}


