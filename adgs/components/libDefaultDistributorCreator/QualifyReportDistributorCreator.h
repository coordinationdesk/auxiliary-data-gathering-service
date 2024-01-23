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
	
	Revision 1.3  2012/12/21 13:30:34  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.2  2009/03/24 09:57:46  crivig
	modified in order to handle new fields in t_distributionitems for carts
	
	Revision 1.1  2008/02/01 15:01:15  crivig
	imported new source for algo QualifyReport
	
*/

#ifndef _QualifyReportDistributorCreator_H_
#define _QualifyReportDistributorCreator_H_ 

#include <DistributorCreator.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool ;

class QualifyReportDistributorCreator: public DistributorCreator // PRQA S 2109
{		
	public:
		explicit QualifyReportDistributorCreator(const std::string &algo);
		virtual ~QualifyReportDistributorCreator() throw() ;
		virtual void createDistribution(dbConnectionPool &, ConfigurationSingleton::StopPredicate & ) const ;

	private:
		// declared but not implemented 
		QualifyReportDistributorCreator(const QualifyReportDistributorCreator&);
		QualifyReportDistributorCreator & operator=(const QualifyReportDistributorCreator&);
		QualifyReportDistributorCreator();
	private:
		double _agelimit;
		ACS_CLASS_DECLARE_DEBUG_LEVEL(QualifyReportDistributorCreator);
};

_ACS_END_NAMESPACE

#endif /* _QualifyReportDistributorCreator_H_ */
