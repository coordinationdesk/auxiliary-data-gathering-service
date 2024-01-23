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

	$Prod: 

	$Id$

	$Author$

	$Log$
	Revision 5.11  2017/07/10 12:49:15  lucio.pulvirenti
	ECICMF-108: Boolean flag added to versionControlByFileName IF to start searching filename and version from the current version.
	Deprecated method dbQuery::execOneRow() substituted.
	
	Revision 5.10  2016/04/11 13:33:42  marpas
	adopting new db interfaces
	
	Revision 5.9  2015/05/25 13:45:05  marpas
	coding best practice applied
	
	Revision 5.8  2014/05/14 15:39:10  marpas
	versionControlByFileName overload: new one returns a pair<filename,fileversion> more generic because can deal also with filename not havig the extension in the filename
	
	Revision 5.7  2014/05/14 14:54:58  marpas
	versionControlByFileName method now can also manage filename with baseline in the version e.g. filename_B001
	
	Revision 5.6  2014/02/11 11:03:02  francesco.avanzi
	dbConv moved to db::toSQL./FileInventory.C
	
	Revision 5.5  2014/02/07 19:00:53  marpas
	adopting ACS_THROW and ACS_COND_THROW macros
	transactions through db::Transaction class
	minor changes
	
	Revision 5.4  2013/10/24 14:18:16  marpas
	query statistics upgraded
	
	Revision 5.3  2013/08/30 10:45:18  marpas
	FileInventoryVersion::versionControlByFileName query optimized and tested on Cryosat
	
	Revision 5.2  2013/07/23 17:21:48  marpas
	compilation warnings removed
	
	Revision 5.1  2013/07/15 16:31:31  marpas
	compilation warnings fixed
	
	Revision 5.0  2013/06/20 07:48:54  marpas
	implementing SENT-37 about messaging.
	adoption of libException 5.x standards
	rationalization and coding best practices applied.
	uselsess SW removed
	qa warnings removed
	compilation warnings removed
	
	Revision 2.6  2013/02/21 19:25:15  marpas
	interface redesign to support explicit connections
	coding best practices applied
	qa rules (still in progress)
	
	Revision 2.5  2012/02/13 17:06:11  marpas
	refactoring in progress
	
	Revision 2.4  2010/05/19 09:26:28  marpas
	useless include removed
	
	Revision 2.3  2009/09/28 17:02:53  marpas
	dbParam interface obsolecence
	
	Revision 2.2  2009/03/06 11:55:48  marpas
	local headers included with double quotes
	
	Revision 2.1  2008/11/12 17:13:05  marpas
	useless include removed
	
	Revision 2.0  2006/02/28 09:44:58  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.18  2005/04/18 13:42:21  seralb
	connection time values changed
	
	Revision 1.17  2004/12/03 10:17:51  giucas
	Header fixed
	
	Revision 1.16  2004/12/03 10:15:29  giucas
	Fixed exINITClassDEBUG value
	
	Revision 1.15  2004/10/27 14:37:44  seralb
	New method: controlAndGetVersion
	
	Revision 1.14  2004/09/16 12:28:29  seralb
	Method versionControl renamed
	
	Revision 1.13  2004/09/01 09:46:42  seralb
	Added new method: versionControlByFileName
	
	Revision 1.12  2004/08/31 14:51:28  seralb
	Modify delta time in query for version control
	
	Revision 1.11  2004/08/30 09:45:26  seralb
	MJD class renamed MJD2000
	
	Revision 1.10  2004/08/24 12:52:10  seralb
	versionControlByFileName method removed, added delta time for query
	
	Revision 1.9  2004/08/23 13:39:40  seralb
	Added versionControlByFileName method
	
	Revision 1.8  2004/02/16 09:51:09  giucas
	Modified debug printouts
	
	Revision 1.7  2003/09/12 09:15:23  seralb
	Bug solved in db query
	
	Revision 1.6  2003/09/11 14:46:37  seralb
	Added getVersion method
	
	Revision 1.5  2003/09/09 14:47:16  seralb
	Constructor and versionControl method improved, all the others method removed
	
	Revision 1.4  2003/07/09 10:22:22  seralb
	Improvement
	
	Revision 1.3  2003/02/20 14:53:38  giucas
	Solved bug in query
	
	Revision 1.2  2003/01/31 14:10:12  seralb
	removed version file not needed
	
	Revision 1.1  2003/01/28 10:02:03  seralb
	Control version class
	
	

*/

#include "FileInventoryVersion.h"

#include <dbInventoryObjectQuery.h>
#include <dbInventoryObject.h>
#include <dbQueryFactoryChain.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbQueryParameters.h>
#include <dbGenQueryScopeGuard.h>
#include <Filterables.h>
#include <Metadata.h>
#include <DateTime.h>
#include <dbConv.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

const double FileInventoryVersion::_deltaTime = 0.00002315; 		// 500 milliseconds

ACS_CLASS_DEFINE_DEBUG_LEVEL(FileInventoryVersion)
/*
 * Class constructor
 */

FileInventoryVersion::FileInventoryVersion(const string& satName) : 
    _rsConf(),
    _satName(satName),
    _prodFileName(),
    _fileNameHandler()
    

//: _rsConf(ConfigurationSingleton::instance()->get())
	
{
} 

/*
 * Class destructor
 */
FileInventoryVersion::~FileInventoryVersion() throw() {}

/////////////////////////////////////////////////////////////////////////////////////////
// Public methods 
/////////////////////////////////////////////////////////////////////////////////////////


// Control the version number, return the updated file name and return true if it is changed
bool FileInventoryVersion::versionControlByDate(const string& filNam, string& outputFilNam, dbConnection & conn)
{

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called...") ;		

	//Execute query with the following parameters: File_Class, File_Type, Validity_Start and Validity_Stop
	dbInventoryObjectQuery* dbInvObjectQuery = 0; 
	
	bool isTheVersionChanged = false;
	_fileNameHandler = _prodFileName.newObject(_satName);
	
	string filclas 			= _fileNameHandler->getFileClass(filNam); 		// TEST
	string filtyp 			= _fileNameHandler->getProductIDFromName(filNam);      // SIR2LRM_0_ 
	long double validitstar		= _fileNameHandler->getStartTime(filNam);
	long double validitstarmin 	= validitstar - _deltaTime;
	long double validitstarmax 	= validitstar + _deltaTime;
	long double validitsto		= _fileNameHandler->getStopTime(filNam);
	long double validitstomin 	= validitsto - _deltaTime;
	long double validitstomax 	= validitsto + _deltaTime;
	DateTime::MJD2000 mjdStartMin(validitstarmin) ;
    DateTime::MJD2000 mjdStartMax(validitstarmax) ;
    DateTime::MJD2000 mjdStopMin(validitstomin) ;
    DateTime::MJD2000 mjdStopMax(validitstomax);
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Executing query with start date " 
		    << validitstar << ", stop date " << validitsto << ", file type " << filtyp << " and file class "  << filclas) ;
	
	//query 
	const string & dbKey = conn.getKey() ;
	dbInvObjectQuery=dynamic_cast<dbInventoryObjectQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery", dbKey)); // PRQA S 3081, 4412
	dbQueryParameters parameters(dbKey) ; 
	parameters.addParameter(dbParam(dbKey, "T_Inventory","FileType", dbParam::EQUAL_TO, db::toSQL(dbKey, filtyp))) ; // PRQA S 3050, 3081 4
	parameters.addParameter(dbParam(dbKey, "T_Inventory","FileClass", dbParam::EQUAL_TO, db::toSQL(dbKey, filclas))) ;
	parameters.addParameter(dbParam(dbKey, "T_Inventory","ValidityStart", dbParam::BETWEEN,db::toSQL(dbKey, DateTime(mjdStartMin),dbConv::YearToFraction),db::toSQL(dbKey, DateTime(mjdStartMax),dbConv::YearToFraction))) ;
	parameters.addParameter(dbParam(dbKey, "T_Inventory","ValidityStop", dbParam::BETWEEN,db::toSQL(dbKey, DateTime(mjdStopMin),dbConv::YearToFraction),db::toSQL(dbKey, DateTime(mjdStopMax),dbConv::YearToFraction))) ;
	dbInvObjectQuery->addParameter(parameters) ;

	size_t number = dbInvObjectQuery->doQuery(conn);
			
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "num = dbInventoryObjectQuery->doQuery() = " << number) ;
		
	if (number > 0)
	{
		ostringstream lastVers;
		int maxVers=0;
		for(size_t i=0; i<number; i++)
		{				
			string version = ((*dbInvObjectQuery)[i])->getFileVersion();
			int vers = atoi(version.c_str());
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "dbInvObjectQuery->getFileVersion-> " << vers) ;
			maxVers = max(maxVers, vers) ;
		}	
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "maxVers = " << maxVers) ;
		int newVers = maxVers + 1;
		
		lastVers << newVers;
			
		//delete dbInvObjectQuery;
		dbGenQueryScopeGuard stationQueryGuard(dbInvObjectQuery);			
			
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed with lastVers = " << lastVers.str()) ;
	
		outputFilNam = 	_fileNameHandler->nameWVersion(filNam,lastVers.str()) ;	
		isTheVersionChanged = true;
	}
	else
	{
		outputFilNam = 	filNam;
		isTheVersionChanged = false;
	}	
	
	return isTheVersionChanged;
}
// Control the version number, return the updated file name and return true if it is changed
pair<string, string> FileInventoryVersion::versionControlByFileName(const string& filNam, 
                                                    dbConnection &conn, 
                                                    const string &versPrefix,
													bool startFromCurrentVersion)
{

    ACS_COND_THROW(versPrefix.length() > 3, FileInventoryVersionException("Version prefix: " + versPrefix + " too long (max 3 char)")) // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called, inputfname: \"" << filNam << "\"");		
	
	_fileNameHandler = _prodFileName.newObject(_satName);
	
    string origvers = _fileNameHandler->getVersion(filNam) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Version from name: \"" << origvers << '\"');	
	int start = 1;
	if( startFromCurrentVersion && not origvers.empty() ) {
		istringstream iss( origvers.substr(versPrefix.length()) ); // skip version prefix
		iss >> start ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Starting from version number " << start);	
	}
    int end = int(pow(10.0,4.0-versPrefix.length())) ; // PRQA S 3011, 3081, 3084

    for (int i=start; i < end; ++i) {
        ostringstream guesversN;
        guesversN << versPrefix << setw(4-versPrefix.length()) << setfill('0') << i ; // PRQA S 3000, 3010, 3084, 4400
        ostringstream guesvers ;
        guesvers << setw(4) << setfill('0') << i ; // PRQA S 4400
	
        string guessfname = _fileNameHandler->nameWVersion(filNam, guesversN.str()); 		
	
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying filename: \"" << guessfname << "\" version: \"" << guesvers.str() << '"' );
	    //query 
	    const string & dbKey = conn.getKey() ;
        SQLString findq(dbKey, SQLString::SELECT ) ;

        SQLString countFun(dbKey, SQLString::FUNCTION ) ;
        countFun.setFunctionName("COUNT");
        countFun.addFunctionParameter("*");

        // Add Select list
        findq.addSelectList(countFun) ;

        // Add Table list
	    findq.addTableList("t_inventory");
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, "t_inventory","filename" , dbParam::EQUAL_TO , db::toSQL(conn, guessfname) )) ;
        pp.addParameter(dbParam(dbKey, "t_inventory","fileversion" , dbParam::EQUAL_TO , db::toSQL(conn, guesvers.str() ))) ;
        findq.addANDWhereParameter(pp) ;
        dbQuery query(conn, findq, "SRCH version");
	    dbRow row(dbKey) ;
		{
			dbSet set;
			query.execForSet(set) ;
			set.getNextRow(row);
		}
        int count = 0 ;
		row.column(0, count) ;
        if (count == 0) {
	        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "good filename: \"" << guessfname << "\" version: \"" << guesvers.str() ); // PRQA S 3050
		    return make_pair(guessfname, guesvers.str()) ;
        }
        else { continue ; }
    }
	ACS_THROW(FileInventoryVersionException("Cannot find new version for filename" + filNam)) ; // PRQA S 3081
}


// Control the version number, return the updated file name and return true if it is changed
bool FileInventoryVersion::versionControlByFileName(const string& filNam, 
                                                    string& outputFilNam, 
                                                    dbConnection &conn, 
                                                    const string &versPrefix,
													bool startFromCurrentVersion)
{
    outputFilNam = versionControlByFileName(filNam, conn, versPrefix, startFromCurrentVersion).first ;
    return outputFilNam != filNam ;
}


// Given a file name return the version of it as string
string FileInventoryVersion::getVersion(const string& filNam)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called...") ;
	
	_fileNameHandler = _prodFileName.newObject(_satName);
	string ver = _fileNameHandler->getVersion(filNam);
	return ver;
		
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed with num=0") ;
}

// Do a db query by filename (WITHOUT ANY FILENAME CHECK), control the version numbers and return the updated version (true) or 0001 (false)
bool FileInventoryVersion::controlAndGetVersion(const string& filNam, string& versionUpdate, dbConnection &conn)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called...") ;
    const string &dbKey = conn.getKey() ;
	//Execute query 
	dbInventoryObjectQuery* dbInvObjectQuery =0; 
	
	bool isTheVersionChanged = false;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Executing query with filename " << filNam) ;
	
	//query 
	dbInvObjectQuery=dynamic_cast<dbInventoryObjectQuery*>(dbQueryFactoryChain::instance()->newObject("dbInventoryObjectQuery", dbKey)); // PRQA S 3081, 4412
	dbQueryParameters parameters(dbKey) ; 
	parameters.addParameter(dbParam(dbKey, "T_Inventory","FileName", dbParam::EQUAL_TO, db::toSQL(dbKey, filNam))) ; // PRQA S 3050, 3081
	dbInvObjectQuery->addParameter(parameters) ;
	size_t number = dbInvObjectQuery->doQuery(conn);
			
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "num = dbInventoryObjectQuery->doQuery() = " << number) ;
		
	if (number > 0)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "(number > 0)... go") ;
		
		ostringstream lastVers;
		int maxVers=0;
		for(size_t i=0; i<number; ++i)
		{				
			
			string version = ((*dbInvObjectQuery)[i])->getFileVersion();
			int vers = atoi(version.c_str());
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "dbInvObjectQuery->getFileVersion-> version = " << vers) ;
			maxVers = max(maxVers, vers) ;
		}	
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "maxVers = " << maxVers) ;
		int newVers = maxVers + 1;
		
		lastVers << setw(4) << setfill('0') << newVers; // PRQA S 4400
			
		//delete dbInvObjectQuery;
		dbGenQueryScopeGuard stationQueryGuard(dbInvObjectQuery);			
			
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed with lastVers = " << lastVers.str()) ;
	
		versionUpdate = lastVers.str();	
		isTheVersionChanged = true;
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed with num=0") ;
		
		versionUpdate = "0001";	
		isTheVersionChanged = false;
	}	
	
	return isTheVersionChanged;
}


// Control the version number, return the updated file name and return true if it is changed
string FileInventoryVersion::controlByFileName(const string& filNam, 
                                                dbConnection &conn, 
                                                const string &versPrefix)
{

    ACS_COND_THROW(versPrefix.length() > 3, FileInventoryVersionException("Version prefix: " + versPrefix + " too long (max 3 char)")) // PRQA S 3081
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called, inputfname: \"" << filNam << "\"");		
	
	_fileNameHandler = _prodFileName.newObject(_satName);
	
    string origvers = _fileNameHandler->getVersion(filNam) ;
	ACS_COND_THROW( origvers.empty(), FileInventoryVersionException("Version not occurring in name: \""+filNam+'\"') );
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Version from name: \"" << origvers << '\"');
	int start = 1;
	{
		istringstream iss( origvers.substr(versPrefix.length()) ); // skip version prefix
		iss >> start ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Starting from version number " << start);	
	}
    int end = int(pow(10.0,4.0-versPrefix.length())) ; // PRQA S 3011, 3081, 3084

    for (int i=start; i < end; ++i) {
		
		string guessfname;
		{
        	ostringstream guesversN;
        	guesversN << versPrefix << setw(4-versPrefix.length()) << setfill('0') << i ; // PRQA S 3000, 3010, 3084, 4400
        	guessfname = _fileNameHandler->nameWVersion(filNam, guesversN.str()); 
		}		
	
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying filename: \"" << guessfname << '\"' );
	    //query 
	    const string & dbKey = conn.getKey() ;
        SQLString findq(dbKey, SQLString::SELECT ) ;

        // Add Table list
	    findq.addTableList("t_inventory");
		findq.addSelectList("id_inv");
        dbQueryParameters pp(dbKey) ;
        pp.addParameter(dbParam(dbKey, "t_inventory","filename" , dbParam::EQUAL_TO , db::toSQL(conn, guessfname) )) ;
        findq.addANDWhereParameter(pp) ;
		findq.setLimit(2) ; // limit 1 is very slow on postgres with huge amount of data
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to perform the following query: " << findq.getSQL() );
		
		dbSet set;
		{
        	dbQuery query(conn, findq, "SRCH version");
			query.execForSet(set) ;
		}
		if( set.empty() ) {
	        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "good filename: \"" << guessfname << '\"'); // PRQA S 3050
		    return guessfname;
		}
        else { continue ; }
    }
	ACS_THROW(FileInventoryVersionException("Cannot find new version for filename" + filNam)) ; // PRQA S 3081
}

// Control the version number in the filename, return the updated file name and return true if it is changed
bool FileInventoryVersion::controlByFileName(const string& filNam, 
                                            string& outputFilNam, 
                                            dbConnection &conn, 
                                            const string &versPrefix)
{
    outputFilNam = controlByFileName(filNam, conn, versPrefix) ;
    return outputFilNam != filNam ;
}


_ACS_END_NAMESPACE
