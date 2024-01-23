// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA_AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.1  2015/11/03 15:11:53  damdec
	PDSEV-12: Added possibility to disable actions on t_smactions table.
	
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.6  2013/03/12 17:44:15  marpas
	dbConnectionPool used in all file actions interfaces and related ones
	
	Revision 2.5  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.4  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.3  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.2  2009/06/26 10:40:13  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.1  2008/11/11 10:47:44  ivafam
	Exception Added
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/06/15 07:48:06  integrator
	multiple storage manager implemented
	
	Revision 1.1  2005/01/03 14:22:01  ivafam
	FileAction basic classes moved in libDbData
	
	Revision 1.4  2004/11/12 11:29:09  paoscu
	dbFileAction interface robustness improved
	
	Revision 1.3  2004/10/07 09:40:56  ivafam
	dbFileAction added
	
	Revision 1.2  2004/07/13 15:15:44  paoscu
	using namespace std no longer in includes
	exDbPersistent is an abstract class
	
	Revision 1.1.1.1  2002/10/25 09:57:24  danalt
	Import libDbCryosat
		

*/


#include <dbDownloadFile.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbDownloadFile)

// constructor
dbDownloadFile::dbDownloadFile(dbConnectionPool &pool, dbPersistent &p, const string &path) :
	dbFileAction(pool, p),
    Thread("dbDownloadFile"), 
	_path(path),
	_smActionsDisabled(false)
{
}



dbDownloadFile::~dbDownloadFile() noexcept
{
}

bool dbDownloadFile::isComplete() const 
{
	return getCompleted() ;
}



_ACS_END_NAMESPACE
