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
	Revision 6.1  2017/02/02 16:07:36  marfav
	CSGACQ-110
	Explicit declaration of expiration dates in case of waiting order insertion
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.2  2016/04/19 15:23:45  marpas
	coding best practices application in progress
	
	Revision 5.1  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
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
	
	Revision 3.0  2013/09/19 14:57:24  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.6  2013/04/09 08:07:16  marpas
	NameIdAssociator refactoring
	new Statistics namespace
	
	Revision 2.5  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.4  2010/03/23 17:36:28  marfav
	Added DELAY trigger type
	
	Revision 2.3  2010/03/23 17:29:04  marfav
	Added expiration date computation from available inventory data
	
	Revision 2.2  2010/03/12 18:04:12  marfav
	dbConv YearToFraction fixed
	
	Revision 2.1  2010/03/12 17:26:31  marfav
	Added Delayed Inventory Objects Special Queries
	
*/

#include <dbInventoryDelayedValCoverOrIntersectQuery.h>
#include <dbInventoryObject.h>

#include <XMLIstringstream.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


dbInventoryDelayedValCoverOrIntersectQuery::dbInventoryDelayedValCoverOrIntersectQuery(
        bool latest, 
        bool intersect, const string &dbKey) : 
    dbInventoryValCoverOrIntersectQuery(latest , intersect, dbKey),
    _somethingFound (false),
    _oldestDate()	

{

}


dbInventoryDelayedValCoverOrIntersectQuery::dbInventoryDelayedValCoverOrIntersectQuery(dbInventoryDelayedValCoverOrIntersectQuery const &rhs) :
    dbInventoryValCoverOrIntersectQuery(rhs),
    _somethingFound (rhs._somethingFound),
    _oldestDate(rhs._oldestDate)	
   
{
	// empty
}

dbInventoryDelayedValCoverOrIntersectQuery &dbInventoryDelayedValCoverOrIntersectQuery::operator=(dbInventoryDelayedValCoverOrIntersectQuery const &r)
{
	if (this != &r)
	{
		_somethingFound = r._somethingFound;
		_oldestDate = r._oldestDate;
		dbInventoryValCoverOrIntersectQuery:: operator= (r );
	}
	return *this ;
}

dbInventoryDelayedValCoverOrIntersectQuery::~dbInventoryDelayedValCoverOrIntersectQuery() throw()
{
}


unsigned long dbInventoryDelayedValCoverOrIntersectQuery::doQuery(dbConnection &c, bool incremental, bool enabled)
{

	// the query will report all the files 
	// having inventory date older than the set delay

	// XML Parameter parsing
	// Expected Structure is
	// <RootTag> 
	//     <DelayTime> VAL </DelayTime>
	// </RootTag>
	
	try
	{
		XMLIstringstream is (getOptFilterXml());
		rsResourceSet rs;
		is >> rs;
		long long timeDelay = 0 ;
		rs.getValue ("DelayTime", timeDelay);
		setDelayTime (timeDelay);
	}
	catch (exception& ex)
	{
		ACS_THROW(exIllegalValueException (ex, "Unable to retrieve DelayTime setting in Optional Filter section")); // PRQA S 3081
	}
	
	_somethingFound = (0 != dbInventoryValCoverOrIntersectQuery::doQuery( c, incremental, enabled )) ;
	
	// At the moment the return value is false
	unsigned long retValue = 0 ;
	
	if (_somethingFound)
	{
		// Here i have a list of potential candidate
		// to satisfy the delayed query
		// get the oldest one (driving the expiration time computation)
		_oldestDate = (this->operator[](0))->getGenerationTime();
		unsigned int i=0;
		while (++i < this->size()) // PRQA S 4244
		{
			if (_oldestDate > (this->operator[](i))->getGenerationTime())
			{
				_oldestDate = (this->operator[](i))->getGenerationTime();
			}
		}
		
		// If we have no files at all there is no need to perform the second query
		DateTime expirTime; // Now
		expirTime -= double(getDelayTime())/86400.0; // PRQA S 3081, 4400

		dbQueryParameters par(c.getKey()) ;
		par.addParameter( dbParam(c.getKey(), "t_inventory","generationtime" , dbParam::MINOR_OR_EQUAL_OF , db::toSQL(c.getKey(), expirTime, dbConv::YearToFraction)) ) ;
		addParameter(par) ; // remember I'm a query, so I can have parameters  
	
		retValue = dbInventoryValCoverOrIntersectQuery::doQuery( c, incremental, enabled ) ;
		
	}

	// Return the value of the second query (if performed)
	return retValue;
}

bool dbInventoryDelayedValCoverOrIntersectQuery::hasExpirationDate () const
{
	// This query has actually an expiration date
	return true;
}

DateTime dbInventoryDelayedValCoverOrIntersectQuery::getComputedExpirationDate () const
{
	// This is "now"
	DateTime expirationDate; 
	if (_somethingFound)
	{
		// The expiration time is the one of the oldest found data
		// plus the delay amount to wait for
		expirationDate = _oldestDate;
		expirationDate += double(getDelayTime())/86400.0; // PRQA S 3081, 4400
	}
	else
	{
		// No object in inventory 
		// Have to wait the full amount of time
		expirationDate += double(getDelayTime())/ 86400.0; // PRQA S 3081, 4400
	}
	return expirationDate;
}

string dbInventoryDelayedValCoverOrIntersectQuery::getTriggerType() const
{
	return "DELAY";
}

_ACS_END_NAMESPACE
