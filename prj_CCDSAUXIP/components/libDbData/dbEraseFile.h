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
	Revision 5.0  2013/06/13 14:53:04  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.7  2013/03/12 17:44:15  marpas
	dbConnectionPool used in all file actions interfaces and related ones
	
	Revision 2.6  2013/02/28 14:06:45  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.5  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.4  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.2  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.1  2008/10/09 17:12:48  ivafam
	dbEraseFile I/F changed
	
	Revision 2.0  2006/02/28 08:47:37  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/06/15 07:48:06  integrator
	multiple storage manager implemented
	
	Revision 1.1  2005/01/03 14:22:01  ivafam
	FileAction basic classes moved in libDbData
	
	Revision 1.4  2004/12/30 22:47:59  paoscu
	headers fixed
	
	Revision 1.3  2004/10/07 09:40:56  ivafam
	dbFileAction added
	
	Revision 1.2  2004/07/13 15:15:44  paoscu
	using namespace std no longer in includes
	exDbPersistent is an abstract class
	
	Revision 1.1  2003/02/18 11:25:32  paoscu
	just added
	
	Revision 1.1  2002/11/15 12:11:48  paoscu
	Imported.
	
		

*/


#ifndef _dbEraseFile_H_
#define _dbEraseFile_H_ 

#include <acs_c++config.hpp>
#include <exException.h>
#include <dbFileAction.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent ;
class dbConnection ;

//
//
//
// class dbEraseFile
//
//
//
class dbEraseFile : public dbFileAction // PRQA S 2108, 2109
{
public:


	/*! class EraseException declaration 
	 *
	 * This exception is thrown by start() if there was an error
	 * while saving record in database.
	 */
	exDECLARE_EXCEPTION(EraseException,Exception) ; // PRQA S 2131, 2153, 2502


	dbEraseFile() = delete ;
	explicit dbEraseFile(dbConnectionPool &, dbPersistent &, int storageid=0);
	dbEraseFile(const dbEraseFile &) = delete ;
	dbEraseFile &operator=(const dbEraseFile &) = delete ;
	dbEraseFile( dbEraseFile &&) = delete ;
	dbEraseFile &operator=( dbEraseFile &&) = delete ;
	virtual ~dbEraseFile() noexcept ;

	virtual void start() = 0 ;
	virtual bool isComplete() const ;
	int getStorageId() const;
	int getRemainingSyncSM() const;

protected:
	virtual void setRemainingSyncSM(int cnt);
	virtual void resetRemainingSyncSM();

private:
	
protected:
	int _storageid ; // PRQA S 2101
	int _remainingSyncSM;
	
private:	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(dbEraseFile)
} ; 


_ACS_END_NAMESPACE

#endif // _dbEraseFile_H_
