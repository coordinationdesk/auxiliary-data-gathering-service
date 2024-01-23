// PRQA S 1050 EOF
/*

 Copyright 1995-2021, Advanced Computer Systems , Inc.
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.acsys.it

 All Rights Reserved.

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 the contents of this file may not be disclosed to third parties, copied or
 duplicated in any form, in whole or in part, without the prior written
 permission of Advanced Computer Systems, Inc.

 $Prod: A.C.S. libDataProvider Library $

 $Id$

 $Author$

 $Log$
 Revision 5.12  2017/02/21 18:08:38  nicvac
 S3EVOL-13: Datapackage serialization.

 Revision 5.11  2017/02/02 09:49:20  davide.tiriticco
 S2PDGS-1655: packageId type changed from string to dbPersistent::IdType

 Revision 5.10  2016/05/10 21:46:39  danalt
 use of ACS_THROW and ACS_COND_THROW

 Revision 5.9  2016/04/11 13:38:52  marpas
 adopting new db interfaces

 Revision 5.8  2016/03/26 21:33:01  marpas
 using new db I/F

 Revision 5.7  2015/08/20 16:18:49  nicvac
 ALSAT1B-44: cart handling

 Revision 5.6  2014/01/30 13:44:29  francesco.avanzi
 coding best practices applied & new db::Transaction used

 Revision 5.5  2013/11/04 13:56:31  marpas
 coding best practices applied
 robustness improved
 performances improved
 compilation & qa warnings removed

 Revision 5.4  2013/10/28 17:34:09  marpas
 work in progress

 Revision 5.3  2013/10/03 14:08:01  nicvac
 acs sdk warnings processed.

 Revision 5.2  2013/07/15 16:47:18  marpas
 compilation warnings fixed

 Revision 5.1  2013/06/12 15:47:59  marpas
 wrong macros: fixed

 Revision 5.0  2013/06/06 18:51:19  marpas
 adopting libException 5.x standards

 Revision 1.19  2013/05/17 15:33:00  nicvac
 S2PDGS-300: priority handling.

 Revision 1.18  2013/02/22 12:39:23  marpas
 resource set interface changed

 Revision 1.17  2012/12/12 16:12:12  nicvac
 minor

 Revision 1.16  2012/12/07 18:07:24  nicvac
 cache files copy added.

 Revision 1.15  2012/10/19 15:04:06  micmaz
 updated library

 Revision 1.14  2012/10/08 13:59:19  micmaz
 http://jira.acsys.it/browse/S2GPP-611

 Revision 1.13  2012/09/19 15:57:11  micmaz
 ACCEPTED - issue S2GPP-611: Development of Distribution Formatter framework for Export
 http://jira.acsys.it/browse/S2GPP-611

 Revision 1.12  2012/09/14 16:31:32  micmaz
 S2GPP-611
 Development of Distribution Formatter framework for Export

 Revision 1.10  2012/09/11 16:42:31  micmaz
 removed compiling warning

 Revision 1.9  2012/01/23 12:03:53  micmaz
 removed LOG(...)

 Revision 1.8  2011/05/12 13:27:59  crivig
 changed interface in order to fix a bug for suspended packages

 Revision 1.7  2011/05/06 10:46:46  crivig
 modified for quality check; all i++ modified into ++i

 Revision 1.6  2011/03/11 11:18:02  crivig
 setduedate method added

 Revision 1.5  2011/03/01 17:29:52  crivig
 getCartInfo() made constant.

 Revision 1.4  2011/02/25 14:53:05  crivig
 bug on cartId fixed

 Revision 1.3  2010/12/17 10:22:37  crivig
 getInvFilesFileTypes overloaded

 Revision 1.2  2010/12/03 15:00:48  crivig
 cache cleaning handled with Cache class

 Revision 1.1  2010/11/29 11:06:49  crivig
 added new class

 */

#include <DataPackage.h>
#include <dbDistributionItemQuery.h>
#include <dbDistributionItem.h>
#include <dbQueryFactoryChain.h>
#include <dbGenQueryScopeGuard.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbInventoryObject.h>
#include <Filterables.h>
#include <DataProvider.h>
#include <ConfigurationSingleton.h>
#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>

#include <boost/archive/xml_oarchive.hpp> // PRQA S 1013 2
#include <boost/archive/xml_iarchive.hpp>


_ACS_BEGIN_NESTED_NAMESPACE(acs, dataprovider)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL (DataPackage);

int const DataPackage::NullCartId =  -1;

DataPackage::DataPackage(const dbPersistent::IdType& dataPackageId) :
		_inventories(),
		_packageId(dataPackageId),
		_userId(""),
		_format(""),
		_fileType(""),
		_dueDate(),
		_priority(0),
		_maxConnRetry(3), // PRQA S 4400
		_size(0),
		_cacheFiles(),
		_cartId(NullCartId),
		_rset()
{
}

DataPackage::DataPackage() :
		_inventories(),
		_packageId(),
		_userId(""),
		_format(""),
		_fileType(""),
		_dueDate(),
		_priority(0),
		_maxConnRetry(3), // PRQA S 4400
		_size(0),
		_cacheFiles(),
		_cartId(NullCartId),
		_rset()
{
}

DataPackage::~DataPackage() throw () {
	//empty
}

DataPackage::DataPackage(DataPackage const& rhl) :
		_inventories(rhl._inventories),
		_packageId(rhl._packageId),
		_userId(rhl._userId),
		_format(rhl._format),
		_fileType(rhl._fileType),
		_dueDate(rhl._dueDate),
		_priority(rhl._priority),
		_maxConnRetry(rhl._maxConnRetry),
		_size(rhl._size),
		_cacheFiles(rhl._cacheFiles),
		_cartId(rhl._cartId),
		_rset(rhl._rset)
{
//empty
}

DataPackage& DataPackage::operator=(DataPackage const& rhl) {
	if (this != &rhl) {
		_inventories = rhl._inventories;
		_packageId = rhl._packageId;
		_userId = rhl._userId;
		_format = rhl._format;
		_fileType = rhl._fileType;
		_dueDate = rhl._dueDate;
		_priority = rhl._priority;
		_maxConnRetry = rhl._maxConnRetry;
		_size = rhl._size;
		_cacheFiles = rhl._cacheFiles;
		_cartId = rhl._cartId;
		_rset = rhl._rset;
	}
	return *this;
}

bool DataPackage::operator==(const DataPackage& p) const {
	return _packageId == p._packageId;
}

//
// get dataPackageId
//
const dbPersistent::IdType& DataPackage::getPackageId() const throw() // PRQA S 4120
{
	return _packageId;
}

//
// get UserId
//
const string& DataPackage::getUserId() const throw() // PRQA S 4120
{
	return _userId;
}

int DataPackage::getCartId() const throw()  // PRQA S 4120
{
	return _cartId;
}

//
// get DataPackage FileType
//
const string& DataPackage::getFileType() const throw()  // PRQA S 4120
{
	return _fileType;
}
//
// set DataPackage FileType
//
void DataPackage::setFileType(const string& ftype) // PRQA S 4121
{
	_fileType = ftype;
}

//
// get format
//
const string& DataPackage::getFormat() const throw()  // PRQA S 4120
{
	return _format;
}

//
// brief get dueDate of DataPackage
//
const DateTime& DataPackage::getDueDate() const throw()  // PRQA S 4120
{
	return _dueDate;
}

void DataPackage::setDueDate(DateTime& time)  // PRQA S 4121
{
	_dueDate = time;
}


int DataPackage::getPriority() const throw()  // PRQA S 4120
{
	return _priority;
}

void DataPackage::setPriority(int priority) throw()  // PRQA S 4121 
{
	_priority = priority;
}


//
// get size of the whole files of DataPackage
//
unsigned long long DataPackage::getPackageSize() const throw()  // PRQA S 4120
{
	return _size;
}
// It returns the num of inventory file of the DataPackage
//
size_t DataPackage::getNumInventoryFiles() const throw()  // PRQA S 4120
{
	return _inventories.size();
}
//
// It gets next inventory file 
// it returns exBoundsException
//

void DataPackage::getInvFiles(vector<string>& invFiles) {

	map<string,string>::iterator it = _inventories.begin();
	while(it != _inventories.end())
	{
		invFiles.push_back(it->first);
		++it;
	}
}

void DataPackage::getInvFilesFileTypes(list<string>* fileTypes) {

	// get all package filetypes stored in _inventories map and sort them
	map<string, string>::iterator it = _inventories.begin();
	while (it != _inventories.end()) {
		fileTypes->push_back(it->second);
		++it;
	}
	fileTypes->sort();
}

void DataPackage::getInvFilesFileTypes(vector<string>* fileTypes) {

	map<string, string>::iterator it = _inventories.begin();
	while (it != _inventories.end()) {
		fileTypes->push_back(it->second);
		++it;
	}
}
void DataPackage::addCacheFile(const string & file) {
	_cacheFiles.push_back(file);
}
void DataPackage::getCacheFiles(vector<string> & files) const 
{
	files = _cacheFiles;
}

bool DataPackage::fillPackage(dbConnection & conn) {

	//@@@ To be improved with smart persistant

	rsResourceSet &rset = ConfigurationSingleton::instance()->get();
	rset.getValue("maxDbRetries", _maxConnRetry);
	DateTime firstDueDate;

	// having the primary key, next code gets now the item related to dataPackageId
	dbDistributionItemQuery * query = dynamic_cast<dbDistributionItemQuery*>(dbQueryFactoryChain::instance()->newObject( // PRQA S 3081
			"dbDistributionItemQuery", conn.getKey())); // PRQA S 4412
	dbGenQueryScopeGuard queryGuard(query);

    const string &dbKey = conn.getKey() ;
    
	try {
		query->resetParameters();
		dbQueryParameters parameters(dbKey);
		parameters.addParameter(dbParam(dbKey, "T_DistributionItems", "packageid", dbParam::EQUAL_TO, db::toSQL(dbKey, _packageId)));
		query->addParameter(parameters);

		query->doQuery(conn);

		if (query->empty()) {
			ACS_THROW(DataPackageException("DataPackage::fillDataPackage: No distributionitems with key: " + _packageId)) ; // PRQA S 3081
		}
	} 
    catch (exception &e) {
		ACS_THROW(DataPackageException(e, "Query Error."));
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"For Package:  " << _packageId << " Records found in T_DistributionItems " << query->size());

	dbInventoryObject* s = dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject( // PRQA S 3081
			"dbInventoryObject", dbKey)); // PRQA S 4412
	ACS_SMARTPTR<dbInventoryObject> sGuard(s);
	//
	// filling the DataPackage with the name of files
	//
	unsigned long long size = 0;
	bool validity = true;
    size_t q_size = query->size() ;
	for (size_t i = 0; i < q_size; ++i) {

		// storing the due date of first product to distribute
		if ((*query)[i]->getDueDate() < firstDueDate) {
			firstDueDate = (*query)[i]->getDueDate();
        }

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB," FileName " << (*query)[i]->getFileName() << " FileVersion " << (*query)[i]->getFileVersion());

		try {
			s->reset();
			// get the file size of file in the inventory table pointing by FileName and version fields
			// having the primary key, get now the files size from inventory table
			s->setFileName((*query)[i]->getFileName());
			s->setFileVersion((*query)[i]->getFileVersion());
			if (!s->load(conn)) {
				DataPackageException e(
						"DataPackage::fillDataPackage: No inventory for " + 
                            (*query)[i]->getFileName() + " " + 
							(*query)[i]->getFileVersion());
				ACS_THROW(e);
			}
		} catch (exception & ex) {

			ACS_THROW(DataPackageException(ex, "Cannot query for filename: " +
                                                        (*query)[i]->getFileName() + " version "  +
                                                        (*query)[i]->getFileVersion()));
		}
		// check if file MUST BE DISTRIBUTE: Checking validity flag value to TRUE
		if (!s->getValidityFlag()) {
			ACS_LOG_WARNING(
					" Validity flag is " << s->getValidityFlag() << ". DataPackage " << _packageId << " "
							<< " will not be distributed and status set to SUSPENDED!");
			validity = false;
		}

		// add the file in the container
		string filename = (*query)[i]->getFileName();
		string key = filename.substr(0, filename.find(" ")) + "@"
				+ (*query)[i]->getFileVersion().substr(0, filename.find(" "));
		_inventories[key] = s->getFileType();

		// sum the size of file
		size += s->getDataSize(); // PRQA S 3084 2 
		size += s->getHeaderSize();

	}
	// set the size of file to distribute
	_size = size;

	// set due date of DataPackages
	_dueDate = firstDueDate;

	// set format
	_format = (*query)[query->size() - 1]->getFormat(); // PRQA S 3084

	_userId = (*query)[0]->getUserId();
	bool set = false ;
	_cartId = (*query)[0]->getCartId(&set);
	if (!set) {
		_cartId = NullCartId;
	} else {
		loadCartsref(conn);
	}

#ifndef PDS_OLD_SCHEMA
	//*/ Set priority
	dbSet dbset;
	SQLString querySql( dbKey, SQLString::SELECT );
	try {
		querySql.addSelectList ( "priority" );
		querySql.addTableList( "t_distributionrules" );
		dbQueryParameters pp(dbKey) ; 
		pp.addParameter ( dbParam (dbKey,  "t_distributionrules", "id_distributionrule", dbParam::EQUAL_TO, 
									db::toSQL(dbKey, (*query)[0]->getDistributionRuleId() ) ) );
		querySql.addANDWhereParameter ( pp );

		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "executing query " << querySql.getSQL() );
		dbQuery query2( conn, querySql, "SEL rule priority" );
		query2.execForSet( dbset );

	} catch(exception& ex) {
		ACS_THROW(DataPackageException(ex, "Error querying DB to get datapackage priority. Sql is "+querySql.getSQL() ));
	}

    dbRow row(dbKey);
	ACS_COND_THROW( dbset.empty() || // PRQA S 3081
							   !dbset.getNextRow( row ),
							   DataPackageException("Cannot find records to get package priority. Sql is: "+querySql.getSQL() ) );

	int priority = 0 ;
	unsigned short i = 0 ;
    row.column(i++, priority); 

    _priority = priority;

	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "DataPackage::fillDataPackage: packageId: "<<_packageId<<"; priority: "<<_priority );
	//*/
#endif

	return validity;
}

void DataPackage::getCartInfo(rsResourceSet& rset) const {
	rset = _rset;
}

void DataPackage::loadCartsref(dbConnection & conn) {
	// download the clob data from t_cartsref and retreive all infos from it
	ostringstream tmp;
	tmp << "/tmp/cartsrefData_" << _cartId;
	string completePath = tmp.str();
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Deleting " << completePath);

	File::unlink(completePath);

    const string &dbKey = conn.getKey() ;
	string table = "t_cartsref";
	SQLString queryString( dbKey, SQLString::LOBJ_DOWNLOAD);
	queryString.setColumnAndFileName("data", completePath);
	queryString.addTableList(table);
	{
		dbQueryParameters p(dbKey) ;
		p.addParameter(dbParam(dbKey, table, "id", dbParam::EQUAL_TO, db::toSQL(dbKey, _cartId)));
		queryString.addANDWhereParameter(p);
	}
	// download clob to completePath  file
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Downloading file: " << completePath);
	// inside a transaction
	dbQuery query(conn, queryString, "LOB dwnl t_cartsref");

	try {
		query.downloadClob(); /* Retrieve CLOB */
	} catch (exception &x) {
		ACS_THROW(exIOException(x, "Couldn't download File from Clob."));
	}

	// change permissions allowing other users to override this file later.
	File::chmod(completePath, 0664); // PRQA S 4400

	ifstream fs(completePath.c_str());
	if (not fs.good()) {
		exIOException e("Cannot open: " + completePath, errno);
		ACS_THROW(e);
	}
	fs >> _rset;
	fs.close();
	// remove
	File::unlink(completePath);

}

string const DataPackage::str() const 
{
	ostringstream os;
	os << "DataPackage{ "
	<< " packageId: " <<_packageId
	<< " userId: " << _userId
	<< " format: " << _format
	<< " fileType: " << _fileType
	<< " dueDate: " << _dueDate.YYYYMMDDThhmmss()
	<< " priority: " << _priority 
	<< " size: " << _size
	<< " cartId: " << _cartId
	<<"}";
	return os.str();

}


//----- SERIALIZATION -----
void DataPackage::dump(const string& filename) const {

	std::ofstream ofs(filename.c_str());

	ACS_COND_THROW( ! ofs.good(), DataPackageSerializationException(string("Cannot dump Data Package on file ")+filename) ); // PRQA S 3081

	this->dump( ofs );

}

void DataPackage::dump(ostream& oss) const {
	//Dump the datapackage
	boost::archive::xml_oarchive oa(oss);

	const DataPackage& dataPackage = *this;

	// dump into the archive
	oa << BOOST_SERIALIZATION_NVP( dataPackage );
}

void DataPackage::load(const string& filename) {

	std::ifstream ifs( filename.c_str() );

	ACS_COND_THROW( ! ifs.good(), DataPackageSerializationException(string("Cannot load Data Package from file ")+filename) ); // PRQA S 3081

	this->load( ifs );

}

void DataPackage::load(istream& iss) {
	//Load the datapackage
	boost::archive::xml_iarchive ia(iss);

	DataPackage& dataPackage = *this;

	// restore from the archive
	ia >> BOOST_SERIALIZATION_NVP( dataPackage );
}

//----- ----- ----- -----

_ACS_END_NESTED_NAMESPACE
