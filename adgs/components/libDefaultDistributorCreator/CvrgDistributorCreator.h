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
	Revision 2.1  2013/02/07 17:52:26  marpas
	Factories and Creators interfaces changed to allow easier connection pool handling and stop predicates
	
	Revision 2.0  2013/01/21 18:02:22  marpas
	introducing dbConnectionPool use
	coding best practices
	filterable management improved
	qa rules
	
	Revision 1.5  2012/12/21 13:30:33  marpas
	if compiled withoud PDS_OLD_SCHEMA use the rule policy id to fill the distribution items.
	coding best practices
	qa rules
	
	Revision 1.4  2010/04/08 09:51:09  crivig
	check on filetype added; bug solved in intersect check
	
	Revision 1.3  2009/06/23 13:34:10  marpas
	if the inventory object has no geographic localization, it will be considered covering the whole world
	
	Revision 1.2  2009/06/22 14:42:30  crivig
	checkdataInfo added when performing intersect/inside methods
	
	Revision 1.1  2009/06/05 08:52:42  crivig
	added new distribution algo
	
*/

#ifndef _CvrgDistributorCreator_H_
#define _CvrgDistributorCreator_H_ 

#include <DistributorCreator.h>
#include <boost/smart_ptr.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool ;
class dbGeoObject;	
class CvrgDistributorCreator: public DistributorCreator // PRQA S 2109
{		
public:
	CvrgDistributorCreator(const std::string &algo, bool total );
	virtual ~CvrgDistributorCreator() throw() ;
	virtual void createDistribution(dbConnectionPool &, ConfigurationSingleton::StopPredicate & ) const ;
private:
	
	// declared but not implemented 
	CvrgDistributorCreator(const CvrgDistributorCreator&);
	CvrgDistributorCreator & operator=(const CvrgDistributorCreator&);
	CvrgDistributorCreator();
    
	bool intersectGeoObjects(const dbGeoObject *,
                             std::vector<boost::shared_ptr<dbGeoObject> > &,
                             std::vector<dbPersistent::IdType> & , 
                             int,
                             int,
                             bool,
                             const std::string &, 
                             dbConnection &) const;
                             
	void updateDistribution(TemporaryInfo &, std::vector<TemporaryInfo> &, dbConnection &) const;
	bool checkDataInfo(dbPersistent::IdType , int ,int ,bool ,const std::string &, dbConnection &) const;
	std::string getOption(rsResourceSet &, const std::string &) const;
private:
	double _agelimit;
	bool _totalCvrg;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(CvrgDistributorCreator);
};

_ACS_END_NAMESPACE

#endif /* _CvrgDistributorCreator_H_ */
