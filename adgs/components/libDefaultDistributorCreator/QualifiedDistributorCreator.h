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

	$Prod: $

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
	
	Revision 1.5  2012/12/21 13:30:34  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.4  2009/03/24 09:57:46  crivig
	modified in order to handle new fields in t_distributionitems for carts
	
	Revision 1.3  2007/03/16 16:27:15  crivig
	coding + testing
	
	Revision 1.2  2007/03/15 15:08:03  crivig
	coding and compiling
	
	Revision 1.1.1.1  2007/03/14 17:15:00  crivig
	Imported new source
	
*/

#ifndef _QualifiedDistributorCreator_H_
#define _QualifiedDistributorCreator_H_ 

#include <DistributorCreator.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool ;

class QualifiedDistributorCreator: public DistributorCreator // PRQA S 2109
{		
	public:
		explicit QualifiedDistributorCreator(const std::string &algo);
		virtual ~QualifiedDistributorCreator() throw() ;
		virtual void createDistribution(dbConnectionPool &, ConfigurationSingleton::StopPredicate & ) const ;

	private:
		// declared but not implemented 
		QualifiedDistributorCreator(const QualifiedDistributorCreator&);
		QualifiedDistributorCreator & operator=(const QualifiedDistributorCreator&);
		QualifiedDistributorCreator();
	private:
		double _agelimit;
		ACS_CLASS_DECLARE_DEBUG_LEVEL(QualifiedDistributorCreator);
};

_ACS_END_NAMESPACE


#endif /* _QualifiedDistributorCreator_H_ */
