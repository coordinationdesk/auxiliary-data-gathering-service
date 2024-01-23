// PRQA S 1050 EOF
/*

	Copyright 2017, Advanced Computer Systems , Inc.
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
	Revision 7.2  2017/11/09 10:06:31  lucio.pulvirenti
	S2PDGS-1843: qac++ directive applied.
	
	Revision 7.1  2017/11/07 16:22:29  lucio.pulvirenti
	S2PDGS-1843: classes added.
	
	
		
*/


#ifndef _GenStorageManagerInfo_H_
#define _GenStorageManagerInfo_H_ 

#include <StorageManagerInfo.h>

#include <GenStorageManagerInfoFactory.h>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnectionPool;

//
//
//
// class GenStorageManagerInfo
//
//
//
class GenStorageManagerInfo: public StorageManagerInfo // PRQA S 2109
{
public:	

	friend class GenStorageManagerInfoFactory ; // PRQA S 2107

	exDECLARE_EXCEPTION(GenStorageManagerInfoExc,exException) ; // PRQA S 2131, 2153, 2502
	
	virtual ~GenStorageManagerInfo() throw() ;

	virtual void getAvailableStorage( std::map<std::string, std::vector<std::pair<std::string, long long int> > > &) ;

protected:
	explicit GenStorageManagerInfo(dbConnectionPool &);
	
private:
	GenStorageManagerInfo() ;
	GenStorageManagerInfo(const GenStorageManagerInfo &) ;
	GenStorageManagerInfo &operator=(const GenStorageManagerInfo &) ;
	
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(GenStorageManagerInfo)
	
} ; 

_ACS_END_NAMESPACE

#endif // _GenStorageManagerInfo_H_
