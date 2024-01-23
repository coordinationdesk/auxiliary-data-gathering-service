// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
	
	Revision 5.4  2016/04/19 13:53:47  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:57  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:25  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.4  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.3  2011/05/13 14:54:29  chicas
	changed processing tag
	
	Revision 2.2  2011/05/02 13:45:39  chicas
	work in progress
	
	Revision 2.1  2011/04/29 13:35:01  chicas
	work in progress for GeoValIntersect special query
	
	
	
*/

#ifndef _dbInventoryGeoValIntersectQuery_H_
#define _dbInventoryGeoValIntersectQuery_H_

#include <dbInventoryValCoverOrIntersectQuery.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbConnection;

class dbInventoryGeoValIntersectQuery: // PRQA S 2109, 2153
	public dbInventoryValCoverOrIntersectQuery
{
	friend class dbCryosatFactory ; // PRQA S 2107
public:

	dbInventoryGeoValIntersectQuery(dbInventoryGeoValIntersectQuery const &) ;
	dbInventoryGeoValIntersectQuery&operator=(dbInventoryGeoValIntersectQuery const &) ;
	virtual ~dbInventoryGeoValIntersectQuery() throw() ;
	static  std::string staticClassName() ;

	virtual unsigned long doQuery(dbConnection &, bool incremental=false, bool enabled = true)  ; 

protected:
	explicit dbInventoryGeoValIntersectQuery(const std::string &dbkey) ;
private:
    dbInventoryGeoValIntersectQuery() ; // not implemented 
	bool setTimeFromLatLonCentre(dbConnection &c, const rsResourceSet & );
	bool setTimeFromPolygon(dbConnection &c, const rsResourceSet &);

	static void getProcessingOptionArray(const rsResourceSet &, const std::string &, std::vector<float> &, bool raiseExc = true) ;
	static std::string getProcessingOption(const rsResourceSet &, const std::string &, bool raiseExc=true) ;
	static bool getProcessingOption(const rsResourceSet &, const std::string &, double &, bool raiseExc=true) ;
	static bool hasProcessingOption(const rsResourceSet &, const std::string &) ;
private:	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbInventoryGeoValIntersectQuery)
};


_ACS_END_NAMESPACE

#endif // _dbInventoryGeoValIntersectQuery_H

