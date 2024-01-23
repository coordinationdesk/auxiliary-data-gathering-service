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
	
	Revision 5.1  2016/04/18 15:22:52  marpas
	coding best practices application in progress
	
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
	
	Revision 2.3  2013/07/02 12:03:19  marpas
	qa warnings and coding best practices
	
	Revision 2.2  2013/03/26 17:57:30  marpas
	file2file method added
	interface redesigned to force dbConnection use
	
	Revision 2.1  2012/02/14 12:52:51  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.0  2006/02/28 09:09:36  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/01/20 17:00:46  paoscu
	Method flush() added.
	
	Revision 1.1  2003/10/07 16:16:28  paoscu
	dbXMLTransformer class added.
	
		

*/


#ifndef _dbXMLTransformer_H_
#define _dbXMLTransformer_H_

#include <exException.h>

#include <map>


_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ;

class dbXMLTransformer // PRQA S 2109
{
public:
	dbXMLTransformer() ;
	~dbXMLTransformer() throw() ;


	void setWorkingDir(const std::string &) ;

	void string2string(dbConnection &, std::string const & XmlString, std::string & OutString, std::string const & styleSheetName)	;
	void file2file(dbConnection &, const std::string & XmlInput, const std::string & Output, std::string const & styleSheetName, bool removeInput=false)	;

	void flush() throw() ; // PRQA S 2502

private:
	//Not implemented
	dbXMLTransformer(dbXMLTransformer const &) ;
	dbXMLTransformer& operator=(dbXMLTransformer const &) ;

	bool haveStyleSheet(const std::string &styleSheetName) ;
	std::string  getFileName(const std::string &styleSheetName, dbConnection &) ;
	void loadFile(const std::string &styleSheetName, dbConnection &) ;

private:
	std::map<std::string , std::string> _paths ;
	std::string _workingDir;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbXMLTransformer) 
};



_ACS_END_NAMESPACE

#endif

