// PRQA S 1050 EOF
/*

	Copyright 2012-2021, Advanced Computer Systems , Inc.
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
	
	Revision 5.5  2016/07/04 14:56:11  damdec
	CSGACQ-82: SQLString used to execute queries.
	
	Revision 5.4  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.3  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2016/03/22 17:14:53  marpas
	getting rid of obsolescent HMI calls
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:59  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:27  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.3  2013/04/12 12:14:38  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.2  2013/02/22 11:58:55  marpas
	using StringUtils::findNoCase
	
	Revision 2.1  2012/03/07 17:30:46  marpas
	new special query: dbInventoryOrderTriggeringFilesQuery added
	
*/

#include <dbInventoryOrderTriggeringFilesQuery.h>
#include <dbInventoryObject.h>

#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>

#include <XMLIstringstream.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbInventoryOrderTriggeringFilesQuery)

struct TriggeringFile { // PRQA S 2173, 2175
    TriggeringFile() : name(), version() {};
    TriggeringFile(const std::string & n, const string & i) : name(n), version(i) {};
    
    std::string name; // PRQA S 2100 2
    std::string version;
};

template<> inline
void rsResourceSet::getValue<TriggeringFile>(const std::string &res,  TriggeringFile&v) const // template specialization // PRQA S 2133, 2134
{
    Lock lock(_mutex) ;
    std::string a("");
    std::string c("");
    if (res != "")
    {
        getValue(res+".FileName")->get(a) ;
        getValue(res+".FileVersion")->get(c) ;
    }
    else
    {
        getValue("FileName")->get(a) ;
        getValue("FileVersion")->get(c) ;
    }
        
   v = TriggeringFile(a,c);     
}



string dbInventoryOrderTriggeringFilesQuery::staticClassName()
{
	return "dbInventoryOrderTriggeringFilesQuery" ;
}

dbInventoryOrderTriggeringFilesQuery::dbInventoryOrderTriggeringFilesQuery(const string &dbkey) :
    dbInventoryObjectSpecialQuery(dbkey)
{
}


dbInventoryOrderTriggeringFilesQuery::dbInventoryOrderTriggeringFilesQuery(dbInventoryOrderTriggeringFilesQuery const &r) :
	dbInventoryObjectSpecialQuery(r)
{
	// empty
}

dbInventoryOrderTriggeringFilesQuery &dbInventoryOrderTriggeringFilesQuery::operator=(dbInventoryOrderTriggeringFilesQuery const &r)
{
	if(this != &r)
	{
		Lock lock1(*this) ; // PRQA S 3050 2
		Lock lock2(r) ;
		
		dbInventoryObjectSpecialQuery::operator=(r) ;
	}
	
	return *this ;
}


dbGeneralQueryIF * dbInventoryOrderTriggeringFilesQuery::clone() const
{
	return new dbInventoryOrderTriggeringFilesQuery(*this) ;
}


dbInventoryOrderTriggeringFilesQuery::~dbInventoryOrderTriggeringFilesQuery() throw() {}


unsigned long dbInventoryOrderTriggeringFilesQuery::doQuery(dbConnection &c, bool incremental, bool enabled) // PRQA S 4020
{
	if(!incremental) {
		clear() ; // erase previous result set
    }

	if(!enabled) {
		return 0;
    }

    ACS_COND_THROW(getParametersCLOB ().empty(), exIllegalValueException("Empty parameter clob")) ; // PRQA S 3081
	XMLIstringstream clobStream (  getParametersCLOB ()  ) ;
	rsResourceSet clobParameters;
	clobStream >> clobParameters ;

	SQLString queryString = _sqlString ;
	queryString.setType(SQLString::SELECT);
	queryString.addTableList(dbInventoryObject::getTable());	
	deque<string> fields = dbInventoryObject::getFieldArray(c);
	for (uint i = 0; i < fields.size(); ++i)
	{
    	queryString.addSelectList( fields[i] );
	}
    size_t num_tf = 0;

	try {
		num_tf = clobParameters.getArraySize("List_of_TriggeringFiles.TriggeringFile") ;
		ACS_COND_THROW(num_tf == 0, exIllegalValueException("No triggering files in clob")) ; // PRQA S 3081
		vector<TriggeringFile> files;
		
		clobParameters.getArray("List_of_TriggeringFiles.TriggeringFile", files);
		const string &dbKey = c.getKey() ;
		
		dbQueryParameters mp(dbKey);
		for (size_t i=0; i < num_tf; i++) {
	    	dbQueryParameters pp(dbKey);
	    	pp.addParameter( dbParam(dbKey, dbInventoryObject::getTable(), "filename", dbParam::EQUAL_TO, db::toSQL(c.getKey(),files.at(i).name)) ); 
	    	pp.addParameter( dbParam(dbKey, dbInventoryObject::getTable(), "fileversion", dbParam::EQUAL_TO, db::toSQL(c.getKey(),files.at(i).version)) ); 		
			pp.setParamOperator( dbQueryParameters::AND ) ;
			mp.addQueryParameter(pp);
			mp.setParamOperator( dbQueryParameters::OR ) ;
		}
		queryString.addANDWhereParameter(mp);
	}
	catch(exception &x) {
		ACS_THROW(exIllegalValueException(x,"Troubles with triggering files in order parameters clob")) ; // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query string is : \"" << queryString.getSQL() << "\"") ;

	dbQuery query(c, queryString, "dbInventoryOrderTriggeringFilesQuery::doQuery") ;

	dbRow row(c.getKey()) ;
	dbSet set ;

	query.execForSet(set) ;
    size_t rows = set.size() ;

	vector<dbInventoryObject *> newObjects ;
	for (size_t i=0; set.getNextRow(row); i++) // PRQA S 4238
	{
		ACS_SMARTPTR<dbInventoryObject> record(dynamic_cast<dbInventoryObject*>( dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", c.getKey()))) ; // PRQA S 3081
		record->loadRow(c, row,0) ;

		if(!incremental)
		{
			_objects.push_back(record.release()) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Added a new InventoryObject") ;
		}
	    else
		{
 	        // Check if the new record has already been loaded by a previous query
			// in case delete it
            size_t o_size = _objects.size() ;
			for (size_t ne=0; ne<o_size;++ne)
			{
 	            dbInventoryObject *oe = dynamic_cast<dbInventoryObject *>(_objects[ne]) ; // PRQA S 3081
 	            if (oe && record->getFileName() == oe->getFileName() && record->getFileVersion() == oe->getFileVersion()) {
 	                record.reset() ;
 	                break ;
                }
 	        }
 	        if (0 != record.get()) {
                newObjects.push_back(record.release()) ;
 	        }
 	    }
	}

	if (incremental) {
    
        size_t no_size = newObjects.size() ;
        for (size_t i=0; i < no_size; ++i)
        {
            _objects.insert(_objects.begin(),newObjects[i]) ;
            ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Added a new InventoryObject") ;
        }
	}

	return rows ;
}

void dbInventoryOrderTriggeringFilesQuery::resetParameters()
{
	dbInventoryObjectSpecialQuery::resetParameters() ;
}
_ACS_END_NAMESPACE
