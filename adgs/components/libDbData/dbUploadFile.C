// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2016/05/10 15:03:51  lucio.pulvirenti
	PDSEV-34: _deleteUploadedFilepathOnSuccess boolean attribute added to IF along with set and get methods.
	
	Revision 5.1  2015/02/04 14:53:42  marfav
	S2PDGS-1072 adding support to options noactions and restoreonsync
	
	Revision 5.0  2013/06/13 14:53:06  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.6  2013/03/12 17:44:15  marpas
	dbConnectionPool used in all file actions interfaces and related ones
	
	Revision 2.5  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.4  2013/01/22 17:37:08  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.3  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.2  2011/04/05 16:52:58  marpas
	Tree management work in progress
	
	Revision 2.1  2009/06/26 10:40:13  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.0  2006/02/28 08:47:38  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/09/26 13:26:03  marpas
	interface changed
	
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
	
	Revision 1.1  2002/11/15 12:11:48  paoscu
	Imported.
	
*/


#include <dbUploadFile.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(dbUploadFile)

// constructor
dbUploadFile::dbUploadFile(dbConnectionPool &pool, dbPersistent &p, const std::string &path) :
	dbFileAction(pool, p),
    Thread("dbUploadFile"),
	_path(path),
    _restoreOnSync(),
    _noActions(),
	_deleteUploadedFilepathOnSuccess(),
	_deleteUploadedFilepathMutex() 
    
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ThreadName = \"" << threadName() << "\"") ;
}



dbUploadFile::~dbUploadFile() ACS_NOEXCEPT
{
}

bool dbUploadFile::isComplete() const 
{
	return getCompleted() ;
}


const std::string & dbUploadFile::getPath() const // PRQA S 4120
{
	return _path ;
}


void dbUploadFile::setDeleteUploadedFilepathOnSuccess(bool v)
{ 
	Lock guard(_deleteUploadedFilepathMutex) ;
	_deleteUploadedFilepathOnSuccess = v; 
}

bool dbUploadFile::deleteUploadedFilepathOnSuccess() const 
{ 	
	Lock guard(_deleteUploadedFilepathMutex) ;
	return _deleteUploadedFilepathOnSuccess; 
}



_ACS_END_NAMESPACE
