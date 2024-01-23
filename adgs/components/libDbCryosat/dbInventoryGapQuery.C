/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
	
	Revision 5.4  2016/05/10 21:19:54  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.3  2016/04/11 13:28:13  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2016/03/22 17:14:53  marpas
	getting rid of obsolescent HMI calls
	
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
	
	Revision 2.4  2012/11/16 13:01:14  marpas
	realignement of MAIN STREAM
	Introducing ADCS1FixedHeaderFactory class
	
	Revision 2.2  2012/02/14 12:52:47  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2008/09/17 15:45:17  crivig
	porting to postgres8
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2006/01/04 16:05:17  enrcar
	Updated to DBEXPRESS
	
	Revision 1.2  2005/10/11 08:42:28  enrcar
	getParameters method used
	
	Revision 1.1  2005/06/17 16:37:49  marpas
	Inventory gaps concepts introduced
	
		

*/


#include <dbInventoryGapQuery.h>
#include <dbInventoryGap.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>
#include <sstream>


using namespace acs ;
using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryGapQuery)

string dbInventoryGapQuery::staticClassName()
{
	return "dbInventoryGapQuery" ;
}


// constructor
dbInventoryGapQuery::dbInventoryGapQuery(const string &dbkey) :
    dbGeneralQuery(dbkey)
{
}


dbInventoryGapQuery::dbInventoryGapQuery(dbInventoryGapQuery const &other):
    dbGeneralQuery(other.getKey()) 
{
	*this=other ;
}

dbInventoryGapQuery &dbInventoryGapQuery::operator=(dbInventoryGapQuery const &other)
{
	if(this != &other)
	{
		Lock lock1(*this) ;
		Lock lock2(other) ;
		
		dbGeneralQuery::operator=(other) ;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbInventoryGapQuery::clone() const
{
	return new dbInventoryGapQuery(*this) ;
}


dbInventoryGapQuery::~dbInventoryGapQuery() throw() {}


unsigned long dbInventoryGapQuery::doQuery(dbConnection &conn, bool incremental, bool enabled)
{
	if(incremental)
	{
		InvalidUse e("Incremental query not allowed.") ;
		ACS_THROW(e) ;
	}
	
	clear() ; // erase previous result set
	if(!enabled)
		return 0;

    SQLString queryString = _sqlString ;
    queryString.setType( SQLString::SELECT );
    if (maxItems() > 0)
		queryString.setLimit( maxItems() ) ;

    queryString.addSelectList( dbInventoryGap::getFields() );
    queryString.addTableList( dbInventoryGap::getTable() );

// where parameters and orderby have been copied from _sqlString above 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << queryString.getSQL() << "\"") ;

	dbQuery query(conn, queryString, "dbInventoryGapQuery::doQuery") ;

	dbRow row(conn.getKey()) ;
	dbSet set ;

	query.execForSet(set) ;

	for (size_t i=0; set.getNextRow(row); i++) 
	{
		dbInventoryGap *record ;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Added a new InventoryGap : ") ; 
        
		// record = new dbInventoryGap ;
		record=dynamic_cast<dbInventoryGap*>( dbPersistentFactoryChain::instance()->newObject("dbInventoryGap", conn.getKey())) ;
		try{
			record->loadRow(row,0) ;
		}
		catch(...){
			delete record;
			throw;
		}
		_objects.push_back(record) ;
	}

	return size() ;
}
dbInventoryGap *dbInventoryGapQuery::operator[](unsigned long i)
{
	return dynamic_cast<dbInventoryGap *>(item(i)) ;
}

const dbInventoryGap *dbInventoryGapQuery::operator[](unsigned long i) const
{
	return dynamic_cast<const dbInventoryGap *>(item(i)) ;
}

