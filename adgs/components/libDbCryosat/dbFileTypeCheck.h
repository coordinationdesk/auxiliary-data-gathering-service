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
	Revision 6.0  2016/07/07 13:44:04  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.1  2016/04/15 10:13:43  marpas
	coding best practices application in progress
	QA warning fixing in progress
	
	Revision 5.0  2015/02/17 11:25:25  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.1  2013/12/03 19:06:16  marpas
	qa warnings
	
	Revision 4.0  2013/09/30 12:30:55  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:23  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.6  2013/06/19 17:24:47  marpas
	adopting libException 5.x standards and removing qa warnings
	
	Revision 2.5  2013/04/29 15:42:32  marpas
	dbConnection in interfaces enforced
	
	Revision 2.4  2012/02/14 12:52:46  marpas
	 refactoring and introducing dbInventory::uniqueId
	
	Revision 2.3  2011/02/10 13:37:29  crivig
	connection passed as optional parameter
	
	Revision 2.2  2009/01/30 14:28:57  marpas
	new schema for virtual file types implemented
	
	Revision 2.1  2006/03/27 17:23:44  fracar
	added retrieval of regular expression to class
	
	Revision 2.0  2006/02/28 09:09:35  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2003/09/03 10:49:31  paoscu
	Imported.
	
		

*/

#ifndef _dbFileTypeCheck_H_
#define _dbFileTypeCheck_H_

#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnection;

class dbFileTypeCheck // PRQA S 2109
{
public:


	explicit dbFileTypeCheck(std::string const & filetype) ;
	~dbFileTypeCheck() throw() ;

	/*! returns true if the given fname matches the filetype (even if it is virtual)
	  */
	bool check(std::string const & name, dbConnection &) const ;

	/*! returns the regular expression associated with that filetype 
	  * in case filetype is virtual no reg exp is returned (empty string)
	  */
	std::string getRegExp(dbConnection &) const ;
	/*! returns the regular expression associated with the filetype 
	  * in case filetype is virtual the recgular expression is the composition 
	  * of regular expressions for children ftypes
	  */
	std::string getRealRegExp(dbConnection &) const ;

private:
	dbFileTypeCheck() ; // not implemented
	dbFileTypeCheck(const dbFileTypeCheck &) ; // not implemented
	dbFileTypeCheck &operator=(const dbFileTypeCheck &) ;  // not implemented
private:
	std::string _filetype ;
} ;


_ACS_END_NAMESPACE


#endif // _dbFileTypeCheckRecognition_H_
