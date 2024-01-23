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
	Revision 6.0  2016/07/07 13:44:05  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.4  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
	Revision 5.3  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.2  2016/04/11 13:28:14  marpas
	adopting new db interfaces
	coding best practices application in progress
	
	Revision 5.1  2016/03/26 21:42:50  marpas
	using new db I/F
	
	Revision 5.0  2015/02/17 11:25:26  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.0  2013/09/30 12:31:05  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:32  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.3  2013/03/26 17:57:30  marpas
	file2file method added
	interface redesigned to force dbConnection use
	
	Revision 2.2  2012/02/14 12:52:51  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.1  2011/03/01 16:00:22  marpas
	GCC 4.4.x support
	
	Revision 2.0  2006/02/28 09:09:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/03/09 18:03:24  marpas
	some messages fixed
	namespaces fixed
	
	Revision 1.4  2004/04/13 16:57:09  paoscu
	ThreadSafe::Lock used.
	clobe() method added.
	
	Revision 1.3  2004/01/20 17:00:46  paoscu
	Method flush() added.
	
	Revision 1.2  2003/10/17 18:32:45  paoscu
	Better debug messages.
	
	Revision 1.1  2003/10/07 16:16:28  paoscu
	dbXMLTransformer class added.
	
		

*/


#include <dbXMLTransformer.h>
#include <dbConfigurationStyleSheet.h>

#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <XMLTransformer.h>
#include <File.h>
#include <unistd.h>
#include <cstring>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbXMLTransformer)


dbXMLTransformer::dbXMLTransformer()
: _paths(), _workingDir("/tmp")
{
}


dbXMLTransformer::~dbXMLTransformer() throw()
{
	flush() ; 
}


void dbXMLTransformer::string2string(dbConnection &conn, string const & XmlString, string & OutString, string const & styleSheetName)
{
	string filename= getFileName(styleSheetName, conn) ;
	XMLTransformer::string2string(filename , XmlString ,OutString);
}


void dbXMLTransformer::file2file(dbConnection &conn, const string & XmlInput, const string &Output, const string & styleSheetName, bool removeInput)
{
	string filename= getFileName(styleSheetName, conn) ;
	XMLTransformer::file2file(filename , XmlInput ,Output, removeInput);
}


void dbXMLTransformer::flush() throw()
{
	for(map<string , string>::iterator it=_paths.begin() ; it != _paths.end() ; it++) { // PRQA S 4238
        try {
		    File::unlink(it->second) ;
        }
        catch(exception &x) {
            ACS_LOG_NOTIFY_EX(x) ;
        }
    }
	
	_paths.clear();

}


bool dbXMLTransformer::haveStyleSheet(const string &styleSheetName)
{
	return (_paths.find(styleSheetName) != _paths.end() );
}


string dbXMLTransformer::getFileName(const string &styleSheetName, dbConnection &conn)
{
	if(!haveStyleSheet(styleSheetName)) {
		loadFile(styleSheetName, conn) ;
    } 
	
	//Is the file still there ??
	if(!File::exists(_paths[styleSheetName])) {
		loadFile(styleSheetName, conn) ;
    }
	
	return _paths[styleSheetName];
}


void dbXMLTransformer::loadFile(const string &styleSheetName, dbConnection &conn)
{
	if(haveStyleSheet(styleSheetName))
	{
		 File::unlink(_paths[styleSheetName]) ;
		 _paths.erase(styleSheetName) ;
	}
	
	//prepare temp file path string
	char inputFile[_workingDir.length() + 32] ; // PRQA S 3084, 4403
	int inputFileDesc=-1;
	if(!_workingDir.empty())
	{
		strcpy(inputFile,_workingDir.c_str()) ;
		strcat(inputFile , "/") ;
		strcat(inputFile ,"dbXMLTransformer-XXXXXX") ;
	}
	else {
		strcpy(inputFile,"XMLTransformer-XXXXXX") ;
    }

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "inputFile = \"" << inputFile << "\".") ;

	//load a dbConfigurationStyleSheet
	dbConfigurationStyleSheet *s= dynamic_cast<dbConfigurationStyleSheet*>(dbPersistentFactoryChain::instance()->newObject("dbConfigurationStyleSheet", conn.getKey())); // PRQA S 3081, 4412
	if(!s)
	{
		ACS_THROW(exIllegalValueException("Error in dynamic cast: Expecting a dbConfigurationStyleSheet*")) ; // PRQA S 3081
	}
	dbPersistentScopeGuard guard(s);

	s->setName(styleSheetName) ;
	if(!s->load(conn))
	{
		ACS_THROW(dbPersistent::ObjectNotLoaded("Can't load \"" + styleSheetName + "\" style sheet.")) ; // PRQA S 3081
	}

	//create (and open) file
	inputFileDesc=::mkstemp(inputFile);
	if( inputFileDesc == -1 )
	{
		ACS_THROW(exIOException("Error while creating temporary input file", errno)) ; // PRQA S 3081
	}
	::close(inputFileDesc);
	File::unlink(inputFile);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "inputFile = \"" << inputFile << "\".") ;

	s->saveStyleSheetFile(inputFile,"", conn);
	_paths[styleSheetName]=inputFile;
	
}



_ACS_END_NAMESPACE

