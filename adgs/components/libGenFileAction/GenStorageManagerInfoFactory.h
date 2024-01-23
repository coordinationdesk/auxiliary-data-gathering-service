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

	$Prod: A.C.S. Generic File Action Library $

	$Id$

	$Author$

	$Log$
		

*/


#ifndef _GenStorageManagerInfoFactory_H_
#define _GenStorageManagerInfoFactory_H_ 

#include <StorageManagerInfoFactory.h>
#include <StorageManagerInfo.h>

_ACS_BEGIN_NAMESPACE(acs)


class GenStorageManagerInfoFactory: public StorageManagerInfoFactory // PRQA S 2109
{	
public:
	GenStorageManagerInfoFactory() ;
	virtual ~GenStorageManagerInfoFactory() throw() ;

	virtual StorageManagerInfo *newStorageManagerInfo(dbConnectionPool &);
	virtual std::string capabilities() const ;
	
private:
	// declared but not implemented	
	GenStorageManagerInfoFactory(const GenStorageManagerInfoFactory &) ;
	GenStorageManagerInfoFactory &operator=(const GenStorageManagerInfoFactory &) ;	
	
private:
	static GenStorageManagerInfoFactory _theFactory ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(GenStorageManagerInfoFactory) ;
} ;

_ACS_END_NAMESPACE
 
#endif /* _GenStorageManagerInfoFactory_H_ */

