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

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.5  2013/03/12 17:44:15  marpas
	dbConnectionPool used in all file actions interfaces and related ones
	
	Revision 2.4  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.3  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.2  2012/02/13 16:51:16  marpas
	refactoring in progress
	
	Revision 2.1  2008/10/09 17:12:48  ivafam
	dbEraseFile I/F changed
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/06/15 07:48:06  integrator
	multiple storage manager implemented
	
	Revision 1.1  2005/01/03 14:22:01  ivafam
	FileAction basic classes moved in libDbData
	
	Revision 1.3  2004/11/12 11:29:09  paoscu
	dbFileAction interface robustness improved
	
	Revision 1.2  2004/10/07 09:40:56  ivafam
	dbFileAction added
	
	Revision 1.1  2003/02/18 11:25:32  paoscu
	just added
	
	Revision 1.1  2002/11/15 12:11:48  paoscu
	Imported.
	

*/


#include <dbEraseFile.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(dbEraseFile)

// constructor
dbEraseFile::dbEraseFile(dbConnectionPool &pool, dbPersistent &p, int storageid) :
	dbFileAction(pool, p),
	_storageid(storageid),
	_remainingSyncSM(-1)
{
}



dbEraseFile::~dbEraseFile() throw()
{
}

bool dbEraseFile::isComplete() const 
{
	return getCompleted() ;
}


int dbEraseFile::getStorageId() const // PRQA S 4120
{
	return _storageid ;
}

int dbEraseFile::getRemainingSyncSM() const { 
	ACS_COND_THROW(_remainingSyncSM < 0, EraseException("The remaing number of SM still holding the product has not been set."));
	return _remainingSyncSM;
}

void dbEraseFile::setRemainingSyncSM(int cnt) {
	_remainingSyncSM = cnt;
}

void dbEraseFile::resetRemainingSyncSM() {
	_remainingSyncSM = -1;
}


_ACS_END_NAMESPACE
