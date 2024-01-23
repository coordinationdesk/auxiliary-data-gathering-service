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

	$Prod: A.C.S. DataBase Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:03  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.4  2016/04/15 10:13:43  marpas
	coding best practices application in progress
	QA warning fixing in progress
	
	Revision 5.3  2016/03/29 16:27:33  marpas
	changed FileTypeDetails to adhere to new db i/f and calling protocols.
	TL uses it during precondition check
	
	Revision 5.2  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.1  2015/12/01 16:07:15  marpas
	coding best practices applied, improved robustness
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:30:51  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.1  2013/09/23 18:49:35  marpas
	some qa warnings removed
	
	Revision 3.0  2013/09/19 14:57:19  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.6  2013/03/25 20:01:09  marpas
	using StringUtils::trim template methods
	
	Revision 2.5  2013/03/11 16:08:54  marfav
	Connection passed to the initialization function
	
	Revision 2.4  2013/01/25 11:55:32  marpas
	using pattern::Singleton from libException
	
	Revision 2.3  2012/02/14 12:52:44  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.2  2009/09/29 14:51:56  marpas
	sqlResourceSet no longer used
	
	Revision 2.1  2009/03/06 16:48:04  marfav
	Added FileTypeDetails class
	
    
*/

#include <FileTypeDetails.h>

#include <dbQuery.h>
#include <dbSet.h>
#include <dbConnection.h>
#include <dbConnectionPool.h>
#include <SQLString.h>
#include <StringUtils.h>
#include <Application.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


// Static attriubute
// Default value for the initialization
const string FileTypeDetails::defaultInitValue ("");

FileTypeDetails::FileTypeDetails () :
    ParamInitializableSingleton<FileTypeDetails, string>(),
    DbLUT<std::string, FileTypeDetailsStruct>()

{
    // CTOR
}

FileTypeDetails::~FileTypeDetails () throw()
{
    // NOP
}

void FileTypeDetails::load(const string &dbKey)  // PRQA S 4020
{
    internalLoad(dbKey) ; 
}

void FileTypeDetails::internalLoad(const string &dbKey) 
{
    Lock lock(*this) ;
	
	clear() ; 
    dbConnectionWrapper cw(db::ConnPools::instance()->getPool(dbKey)) ;
    dbConnection &conn=cw ; // PRQA S 3050
    // Create the SQL string to select the fields from the table
    SQLString sqlString(dbKey, SQLString::SELECT ) ;

    // Columns to be selected
    sqlString.addSelectList("filetype"); 
    sqlString.addSelectList("mngsngext");
    sqlString.addSelectList("sngext");
    sqlString.addSelectList("hdrext");
    sqlString.addSelectList("dblext");
    sqlString.addSelectList("nameregularexpression");

    // Table to be used
    sqlString.addTableList("T_FileTypes");

      
	dbQuery query(conn,sqlString, "FileTypeDetails::internalLoad") ;
	
	dbSet rset ; 
	query.execForSet(rset) ;
	dbRow row(conn.getKey()) ; 

    // Fill the value array
	while(rset.getNextRow(row))
	{
        bool set = false ;

		string filetype ; 
		row.column(0, filetype) ;

        FileTypeDetailsStruct details;

        bool manageext = false ;
        row.columnSet (1, manageext, set);
        details.manageSingleExt = (set?manageext:true); // PRQA S 3380

        string tmp;

        // Single extension
		row.columnSet (2, tmp, set) ; // PRQA S 4400
        details.singleExt = (set?StringUtils::trim(tmp, StringUtils::is_not_space):"");  // PRQA S 3380, 3385

        // Heeader extension
        row.columnSet(3, tmp, set) ; // PRQA S 4400
        details.hdrExt = (set?StringUtils::trim(tmp, StringUtils::is_not_space):"");  // PRQA S 3380, 3385

        // Binary extension
		row.columnSet(4, tmp, set) ; // PRQA S 4400
        details.dblExt = (set?StringUtils::trim(tmp, StringUtils::is_not_space):"");  // PRQA S 3380, 3385

        // Regular expression
        row.columnSet(5, tmp, set) ; // PRQA S 4400
        details.regExp = (set?StringUtils::trim(tmp, StringUtils::is_not_space):"");  // PRQA S 3380, 3385
        
        value(StringUtils::trim(filetype, StringUtils::is_not_space)) = details ;
	}

}


std::ostream &operator<<(std::ostream &os, const FileTypeDetailsStruct &n) 	// output to stream
{
    os << "(" << boolalpha << n.manageSingleExt << ", \"" << n.singleExt << "\", \"" << n.hdrExt << "\", \"" <<
          n.dblExt << "\", \"" << n.regExp << "\")";
	return os ; // PRQA S 4028
}

exostream &operator<<(exostream &os, const FileTypeDetailsStruct &n) 	// output to stream // PRQA S 4222
{
    os << "(" << boolalpha << n.manageSingleExt << ", \"" << n.singleExt << "\", \"" << n.hdrExt << "\", \"" <<
          n.dblExt << "\", \"" << n.regExp << "\")";
    return os ; // PRQA S 4028 
}


_ACS_END_NAMESPACE
