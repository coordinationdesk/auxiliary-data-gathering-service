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
	Revision 2.3  2014/02/05 15:39:15  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after been removed by mistake
	
	Revision 2.2  2013/03/12 17:55:22  marpas
	dbConnectionPool is mandatory in file actions
	
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
	
	Revision 1.2  2008/12/11 13:13:11  marpas
	Added Venus Redistribution
	
	Revision 1.1  2007/04/02 14:46:39  crivig
	*** empty log message ***
		
*/

#ifndef _RedistributionDistributorCreator_H_
#define _RedistributionDistributorCreator_H_ 

#include <DistributorCreator.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool ;
class dbInventoryObject ;

class rsResourceSet ;

class RedistributionDistributorCreator: public DistributorCreator // PRQA S 2109
{		
	public:
		explicit RedistributionDistributorCreator(const std::string &algo);
		virtual ~RedistributionDistributorCreator() throw() ;
		virtual void createDistribution(dbConnectionPool &, ConfigurationSingleton::StopPredicate & ) const ;

	private:
		// declared but not implemented 
        RedistributionDistributorCreator() ;
		RedistributionDistributorCreator(const RedistributionDistributorCreator&);
		RedistributionDistributorCreator & operator=(const RedistributionDistributorCreator&);

		void emitDistribution(const rsResourceSet &resSetDBlock, const TemporaryInfo& tmpInfo, dbConnection &) const ;
		void emitVenusDistribution(dbConnectionPool &, rsResourceSet &resSetDBlock, const TemporaryInfo& tmpInfo, dbConnection &) const ;
		void downloadAndEmitDistribution(const TemporaryInfo& , dbInventoryObject *, dbConnection &, dbConnectionPool &) const ;

	private:
		std::string _tmpDownloadPath;
		ACS_CLASS_DECLARE_DEBUG_LEVEL(RedistributionDistributorCreator);
};

_ACS_END_NAMESPACE

#endif /* _RedistributionDistributorCreator_H_ */
