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

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/02/07 17:52:27  marpas
	Factories and Creators interfaces changed to allow easier connection pool handling and stop predicates
	
	Revision 2.0  2013/01/21 18:02:23  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.3  2012/12/21 13:30:34  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.2  2007/03/15 15:08:03  crivig
	coding and compiling
	
	Revision 1.1.1.1  2007/03/14 17:15:00  crivig
	Imported new source
	
*/


#ifndef _QualifiedDistributorFactory_H_
#define _QualifiedDistributorFactory_H_ 

#include <DistributorCreatorFactory.h>

_ACS_BEGIN_NAMESPACE(acs)

// Base class for orders creator factory
class QualifiedDistributorFactory : public DistributorCreatorFactory // PRQA S 2109
{	

public:

	QualifiedDistributorFactory() ;
	virtual ~QualifiedDistributorFactory() throw() ;

	virtual DistributorCreator *newCreator(const std::string &algo) ;
	virtual void capabilities(std::vector<std::string> &) const ;
private:

	// declared but not implemented	
	QualifiedDistributorFactory(const QualifiedDistributorFactory &) ;
	QualifiedDistributorFactory &operator=(const QualifiedDistributorFactory &) ; 
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(QualifiedDistributorFactory) ;
} ;

_ACS_END_NAMESPACE

#endif /* _QualifiedDistributorFactory_H_ */

