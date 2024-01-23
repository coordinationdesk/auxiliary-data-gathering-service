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

	$Prod: A.C.S. Default Order Creation Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/02/07 17:52:27  marpas
	Factories and Creators interfaces changed to allow easier connection pool handling and stop predicates
	
	Revision 2.0  2013/01/21 18:02:22  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.4  2012/12/21 13:30:33  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.3  2007/03/16 16:27:15  crivig
	coding + testing
	
	Revision 1.2  2007/03/15 15:08:03  crivig
	coding and compiling
	
	Revision 1.1.1.1  2007/03/14 17:15:00  crivig
	Imported new source
	
	Revision 2.0  2006/02/28 10:24:18  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:28:45  marfav
	Import libDefaultDistributorCreator
	
	
*/


#ifndef _PreselectionDistributorFactory_H_
#define _PreselectionDistributorFactory_H_ 

#include <DistributorCreatorFactory.h>

_ACS_BEGIN_NAMESPACE(acs)
// Base class for orders creator factory
class PreselectionDistributorFactory : public DistributorCreatorFactory // PRQA S 2109
{	

public:

	PreselectionDistributorFactory() ;
	virtual ~PreselectionDistributorFactory() throw() ;

	virtual DistributorCreator *newCreator(const std::string &algo) ;
	virtual void capabilities(std::vector<std::string> &) const ;
private:

	// declared but not implemented	
	PreselectionDistributorFactory(const PreselectionDistributorFactory &) ;
	PreselectionDistributorFactory &operator=(const PreselectionDistributorFactory &) ; 
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PreselectionDistributorFactory);
} ;

_ACS_END_NAMESPACE

#endif /* _PreselectionDistributorFactory_H_ */

