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

	$Prod: A.C.S. Packager Optimiser Library $

	$Id$

	$Author$

	$Log$
	Revision 2.13  2014/06/25 16:24:12  marpas
	coding best practices applied
	
	Revision 2.12  2014/03/10 15:00:33  francesco.avanzi
	better code practices applied
	
	Revision 2.11  2013/04/30 13:34:51  marpas
	dbPersistent interface changed
	
	Revision 2.10  2013/04/17 17:13:23  marpas
	new dbPersistent interface
	
	Revision 2.9  2013/03/29 19:52:07  marpas
	removing html related functions
	
	Revision 2.8  2012/02/13 17:10:55  marpas
	refactoring in progress
	
	Revision 2.7  2009/09/29 15:46:46  marpas
	useless include removed
	
	Revision 2.6  2009/09/28 17:03:38  marpas
	dbParam interface obsolecence
	
	Revision 2.5  2009/07/13 13:11:03  enrcar
	EC:: c++rules
	
	Revision 2.4  2009/03/24 14:33:14  enrcar
	EC:: c++rules
	
	Revision 2.3  2008/10/08 15:08:29  crivig
	porting to postgres8
	
	Revision 2.2  2008/01/30 16:12:40  enrcar
	EC:: Extended to handle also T_Inventory.filetype and T_Inventory.qualifierfile
	
	Revision 2.1  2006/10/23 10:07:19  marpas
	variables and/or methods related to file size, header size and blob sized now take into account 64 bits
	
	Revision 2.0  2006/02/28 10:05:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/07/22 13:15:06  enrcar
	Interface updated
	
	Revision 1.4  2005/07/19 16:11:21  enrcar
	DBEXPRESS handled
	
	Revision 1.3  2005/06/20 15:23:39  enrcar
	Added handling for namespace std
	
	Revision 1.2  2005/06/08 10:28:25  enrcar
	catched exc. passed as reference
	
	Revision 1.1  2004/09/14 16:28:37  marpas
	object extended to limit query number
	

*/

#include <dbDistributionItemExtended.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbConv.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <sstream>
#include <XMLOstream.h>
#include <dbQueryParameters.h>
#include <SQLString.h>


_ACS_BEGIN_NAMESPACE(acs)


using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbDistributionItemExtended)

// constructor
dbDistributionItemExtended::dbDistributionItemExtended() :
    dbDistributionItem(),
	_validityFlag(false),
	_validityFlagSet(false),
	_dataSize(0),
	_dataSizeSet(false),
    _headerSize(0),
    _headerSizeSet(false),
    _fileType(""),
    _fileTypeSet(false),
    _qualifierFile(""),
    _qualifierFileSet(false)
{
    dbDistributionItemExtended::reset() ;
}


// copy constructor
dbDistributionItemExtended::dbDistributionItemExtended(const dbDistributionItemExtended &o):
	dbDistributionItem(o),
	_validityFlag(o._validityFlag),
	_validityFlagSet(o._validityFlagSet),
	_dataSize(o._dataSize),
    _dataSizeSet(o._dataSizeSet),
    _headerSize(o._headerSize),
    _headerSizeSet(o._headerSizeSet),
    _fileType(o._fileType),
    _fileTypeSet(o._fileTypeSet),
    _qualifierFile(o._qualifierFile),
    _qualifierFileSet(o._qualifierFileSet)
{
}


dbDistributionItemExtended::~dbDistributionItemExtended() throw() {}


// operator =
dbDistributionItemExtended &dbDistributionItemExtended::operator=(const dbDistributionItemExtended &o)
{
	Lock guard(*this);	// mutex // PRQA S 3050

	if(this != &o)
	{
		dbDistributionItem::operator=(o) ;	// call base-class copy c'tor
		_validityFlag = o._validityFlag ;
		_validityFlagSet = o._validityFlagSet ;
		_dataSize = o._dataSize ;
		_dataSizeSet = o._dataSizeSet ;
		_headerSize = o._headerSize ;
        _headerSizeSet = o._headerSizeSet ;
        _fileType = o._fileType ;
        _fileTypeSet = o._fileTypeSet ;
        _qualifierFile = o._qualifierFile ;
        _qualifierFileSet = o._qualifierFileSet ;
	}
	return *this ;
}



// load the class from the database. Return true if object exist
bool dbDistributionItemExtended::load(dbConnection &conn, IdType id) // PRQA S 4020
{
        // This method has multiple returns in order to keep its complexity
        // as low as possible


	Lock lock(*this) ;	// PRQA S 3050

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB);

	IdType Id=id;

	const string &cKey = conn.getKey();
    if(Id == 0) {
		Id=getId() ;	// from base class
    }
    
	if(Id==0)
	{
		 ;
		ACS_THROW(IdNotProvided("Cannot obtain \"Id\"")) ; // PRQA S 3081
	}
	
	dbDistributionItemExtended tempRecord(*this); //store current record.

	reset() ; 

	dbRow row(cKey) ;

	try
	{

        SQLString queryString(cKey, SQLString::SELECT) ;
        queryString.addSelectList( getFields() ) ;
        queryString.addTableList( getTable() ) ;
        queryString.addANDWhereParameter( whereJoin(conn) ) ;
        {
            dbQueryParameters pp(cKey) ;
            {
                dbParam param(cKey, "T_DistributionItems","Id", dbParam::EQUAL_TO, db::toSQL(cKey, Id)) ;
                pp.addParameter(param) ;
            }
            queryString.addANDWhereParameter(pp) ;
        }
        string getOrderByString(privateGetOrderBy()) ;
		if(!getOrderByString.empty() ) {
            queryString.addOrderByList(getOrderByString);
        }
		dbQuery query(conn, queryString, "dbDistributionItemExtended::load" ) ;	// setup query

        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL = " << queryString.getSQL() );
     
		query.execOneRow(row) ;		// execute query
	}
	catch (dbQuery::QueryNoDataFound& e)
	{
		*this = tempRecord;  //Nothing found: restore previuos situation.
		return false ;
	}

	loadRow(row, 0) ;	// call privateLoadRow

	setFromDatabase() ;

	resetModifiedFlag() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Dump : \"" << *this );

	return true;

}


dbPersistent *dbDistributionItemExtended::clone() const
{
	Lock guard(*this); // PRQA S 3050

	return new dbDistributionItemExtended(*this) ;
}


string dbDistributionItemExtended::className() const
{
	return staticClassName() ;
}


string dbDistributionItemExtended::staticClassName()
{
	return "dbDistributionItemExtended" ;
}


// initialize the class
void dbDistributionItemExtended::reset()
{
	Lock guard(*this);	 // PRQA S 3050

	// reset dbPersistent
	dbDistributionItem::reset() ;	// call the reset method of base class
	_validityFlag = false ;
	_validityFlagSet = false ;
	_dataSize = 0 ;
	_dataSizeSet = false ;
	_headerSize = 0 ;
	_headerSizeSet = false ;
    _fileType = "" ;
    _fileTypeSet = false ;
    _qualifierFile = "" ;
    _qualifierFileSet = false ;
}

  

// load the object from a row
int dbDistributionItemExtended::loadRow(const dbRow &row, int offset)
{
	Lock guard(*this); // PRQA S 3050
	offset += dbDistributionItem::loadRow(row,offset) ;
	return privateLoadRow(row, offset) ;

}


// load the object from a row
int dbDistributionItemExtended::privateLoadRow(const dbRow &row, int offset)
{
	Lock guard(*this);	// PRQA S 3050

	row.columnSet(offset + 0, _validityFlag, _validityFlagSet) ; // PRQA S 3000 5
	row.columnSet(offset + 1, _headerSize, _headerSizeSet) ;
	row.columnSet(offset + 2, _dataSize, _dataSizeSet) ; // PRQA S 4400 3
	row.columnSet(offset + 3, _fileType, _fileTypeSet) ;
	row.columnSet(offset + 4, _qualifierFile, _qualifierFileSet) ;

	return offset + 5 ; // PRQA S 4400
}


string dbDistributionItemExtended::getWhere()
{
	string res ;
	return res ;
}



// get the field names
string dbDistributionItemExtended::getFields()
{
	string s ;
	s += dbDistributionItem::getFields() + ", " + privateGetFields() ;
	return s ;
}


// get the field names
string dbDistributionItemExtended::privateGetFields()
{
	string s = "" ;
	s += "T_Inventory.ValidityFlag, ";
	s += "T_Inventory.HeaderSize, ";
	s += "T_Inventory.DataSize, ";
	s += "T_Inventory.FileType, ";
	s += "T_Inventory.QualifierFile ";
	return s ;	// return private fields
}


// get the table name
string dbDistributionItemExtended::getTable()
{
	return dbDistributionItem::getTable() + "," + "T_Inventory" ;
}


// get the table name
dbQueryParameters dbDistributionItemExtended::whereJoin(const dbConnection &conn)
{
    const string & cKey = conn.getKey();
    dbQueryParameters tmp(cKey);
    tmp.addParameter(dbParam(cKey, "T_Inventory","FileName", dbParam::EQUAL_TO, "T_DistributionItems.FileName")) ;
    tmp.addParameter(dbParam(cKey, "T_Inventory","FileVersion", dbParam::EQUAL_TO, "T_DistributionItems.FileVersion")) ;

	return tmp ;
}


// get the order by clause
string dbDistributionItemExtended::getOrderBy()
{
	string res ;
	res += privateGetOrderBy() ;
	return res ;
}


// get the order by clause
string dbDistributionItemExtended::privateGetOrderBy()
{
	string res ;
	return res ;
}


bool dbDistributionItemExtended::getValidityFlag(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _validityFlagSet ;
    }
	return _validityFlag ;
}

unsigned long long int dbDistributionItemExtended::getHeaderSize(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _headerSizeSet ;
    }
	return _headerSize ;
}

unsigned long long int dbDistributionItemExtended::getDataSize(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _dataSizeSet ;
    }
	return _dataSize ;
}

const string &dbDistributionItemExtended::getFileType(bool *set) const
{
	Lock lock(*this) ; // PRQA S 3050

	if (set) {
		*set = _fileTypeSet ;
    }
	return _fileType ;
}

const string &dbDistributionItemExtended::getQualifierFile(bool *set) const
{
	Lock lock(*this) ;  // PRQA S 3050

	if (set) {
		*set = _qualifierFileSet ;
    }
	return _qualifierFile ;
}


_ACS_END_NAMESPACE
