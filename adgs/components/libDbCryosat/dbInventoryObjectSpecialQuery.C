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
	Revision 6.3  2017/04/07 13:27:25  marfav
	addLinkedFileTypesToWait operation const
	
	Revision 6.2  2017/04/07 10:36:57  marfav
	APF-412
	Added support to multiple waiting filetypes
	
	Revision 6.1  2017/02/02 16:07:36  marfav
	CSGACQ-110
	Explicit declaration of expiration dates in case of waiting order insertion
	
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.1  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:58  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:26  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.10  2012/03/07 17:30:46  marpas
	new special query: dbInventoryOrderTriggeringFilesQuery added
	
	Revision 2.9  2012/02/14 12:52:48  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.8  2010/03/23 17:29:04  marfav
	Added expiration date computation from available inventory data
	
	Revision 2.7  2010/03/12 17:26:31  marfav
	Added Delayed Inventory Objects Special Queries
	
	Revision 2.6  2009/11/25 19:32:23  marpas
	optional filter parameters stored in private data section.
	protected method to retrieve them added.
	
	Revision 2.5  2009/11/25 17:43:35  marpas
	dbInventoryObjectSpecialQuery::setOptionalFilter virtual method implemented
	
	Revision 2.4  2009/09/29 14:05:16  marpas
	dbResources is no longer used
	
	Revision 2.3  2009/02/04 15:48:23  marfav
	Addedd doesOverlap method
	
	Revision 2.2  2008/12/17 14:41:29  paoscu
	Overloaded setParametersCLOB
	
	Revision 2.1  2006/10/24 16:01:59  marfav
	Addedd parameter file string attribute
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2005/03/08 10:17:35  marpas
	getTriggerType method interface fixed
	
	Revision 1.8  2005/03/04 23:11:06  marpas
	getTriggerType method added to special queries interface
	By default it returns always true, it has been, hence, reimplemented for some cases
	
	Revision 1.7  2004/07/14 09:36:32  paoscu
	documentation and some fixes
	
	Revision 1.6  2004/04/28 13:08:40  paoscu
	Incremental query support.
	
	Revision 1.5  2004/04/13 16:52:56  paoscu
	Copy constructor and operator= added
	clone() method added.
	
	Revision 1.4  2003/07/25 15:17:53  paoscu
	potential memroy leaks removed
	order by management improved
	
	Revision 1.3  2003/05/06 11:15:31  paoscu
	std namespace policy changed.
	
	Revision 1.2  2002/12/17 17:45:57  paoscu
	A few changes in protected methods.
	
	Revision 1.1  2002/12/13 18:36:24  paoscu
	Imported
	
		

*/

#include<dbInventoryObjectSpecialQuery.h>
#include<dbConv.h>

using namespace acs;
using namespace std;


dbInventoryObjectSpecialQuery::dbInventoryObjectSpecialQuery(const string &dbkey) :
    dbInventoryObjectQuery(dbkey),
	_t0(true) ,
	_t1(true) ,
	_dt0(0.0) ,
	_dt1(0.0) ,
	_parameterCLOB() ,
	_optFilterXml() ,
	_optFilterRoot() ,	
	_delayTime (0)
	
{}



dbInventoryObjectSpecialQuery::dbInventoryObjectSpecialQuery(dbInventoryObjectSpecialQuery const & r ) :
    dbInventoryObjectQuery(r),
    _t0(r._t0) ,
	_t1(r._t1) ,
	_dt0(r._dt0) ,
	_dt1(r._dt1) ,
	_parameterCLOB(r._parameterCLOB) ,
	_optFilterXml(r._optFilterXml) ,
	_optFilterRoot(r._optFilterRoot) ,	
	_delayTime (r._delayTime)
    

{
}


dbInventoryObjectSpecialQuery &dbInventoryObjectSpecialQuery::operator=(const dbInventoryObjectSpecialQuery & other )
{
	if(this != &other)
	{
		Lock lock1(*this) ;
		Lock lock2(other) ;
		
		dbInventoryObjectQuery::operator=(other) ;
		
		_t0 = other._t0;
		_t1 = other._t1;
		_dt0 = other._dt0;
		_dt1 = other._dt1;
		_parameterCLOB = other._parameterCLOB;
		_delayTime = other._delayTime;
	}
	return *this;
}


dbInventoryObjectSpecialQuery::~dbInventoryObjectSpecialQuery() throw() {}

DateTime const &dbInventoryObjectSpecialQuery::t0() const
{
	return _t0;
}


DateTime const &dbInventoryObjectSpecialQuery::t1() const
{
	return _t1;
}


double dbInventoryObjectSpecialQuery::dt0() const
{
	return _dt0;
}

double dbInventoryObjectSpecialQuery::dt1() const
{
	return _dt1;
}

string dbInventoryObjectSpecialQuery::getTriggerType() const 
{
	return "FILETYPEMATCH" ;
}


void dbInventoryObjectSpecialQuery::t0( DateTime const &t)
{
	_t0=t;
}


void dbInventoryObjectSpecialQuery::t1( DateTime const &t)
{
	_t1=t;
}


void dbInventoryObjectSpecialQuery::dt0(double t)
{
	_dt0=t;
}


void dbInventoryObjectSpecialQuery::dt1(double t)
{
	_dt1=t;
}


string dbInventoryObjectSpecialQuery::startTimeString() const
{
	return string(db::toSQL(getKey(), startTime() , dbConv::YearToFraction)) ;
}


string dbInventoryObjectSpecialQuery::endTimeString() const
{
	return string(db::toSQL(getKey(), endTime() , dbConv::YearToFraction)) ;
}


DateTime dbInventoryObjectSpecialQuery::startTime() const
{
	DateTime res=_t0 ;
	res-=DateTime::JD50( _dt0 / 86400.0);
	return res;
}


DateTime dbInventoryObjectSpecialQuery::endTime() const
{
	DateTime res=_t1 ;
	res+=DateTime::JD50( _dt1 / 86400.0);
	return res;
}


string dbInventoryObjectSpecialQuery::getValidityStartField() const
{
	return "t_inventory.validitystart" ; 
}

string dbInventoryObjectSpecialQuery::getValidityStopField() const
{
	return "t_inventory.validitystop" ; 
}

string dbInventoryObjectSpecialQuery::getOrderByField()	const
{
	return "t_inventory.inventorydate" ; 
}

void dbInventoryObjectSpecialQuery::setParametersCLOB ( istream& theStream )
{
	_parameterCLOB = "";

	size_t BUFSIZE=1024;
	char buffer[BUFSIZE+1];
	size_t realRead;

	while (!theStream.eof())
	{
		theStream.read (buffer, BUFSIZE);
		realRead = theStream.gcount();

		buffer[realRead] = '\0';
		_parameterCLOB.append (buffer);
	}
}


void dbInventoryObjectSpecialQuery::setParametersCLOB ( std::string const & s)
{
	_parameterCLOB = s ;
}


const string &dbInventoryObjectSpecialQuery::getParametersCLOB () const
{
	return _parameterCLOB;
}

bool dbInventoryObjectSpecialQuery::doesOverlap () const
{
	// This is the default value that should be used for any special query
	return true;
}

void dbInventoryObjectSpecialQuery::setOptionalFilter( const string &xml, const string &root)
{
	_optFilterXml = xml ;
	_optFilterRoot = root ;	

}

const string &dbInventoryObjectSpecialQuery::getOptFilterXml() const 
{
	return _optFilterXml ;
}

const string &dbInventoryObjectSpecialQuery::getOptFilterRoot() const 
{
	return _optFilterRoot ;
}

long long dbInventoryObjectSpecialQuery::getDelayTime () const
{
	return _delayTime;
}

void dbInventoryObjectSpecialQuery::setDelayTime (long long d)
{
	_delayTime = d;
}

bool dbInventoryObjectSpecialQuery::hasExpirationDate () const
{
	return false;
}

DateTime dbInventoryObjectSpecialQuery::getComputedExpirationDate () const
{
	return DateTime();
}

void dbInventoryObjectSpecialQuery::addLinkedFileTypesToWait (std::vector <std::string>& ftArray) const
{
	// Does nothing by default
}

