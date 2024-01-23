// PRQA S 1050 EOF
/*

	Copyright 2020-2021, Exprivia SpA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. LTA File Action Library $

	$Id$

	$Author$

	$Log$
	
*/


#ifndef _LTAInventoryObjectOperatorIF_H_
#define _LTAInventoryObjectOperatorIF_H_ 

#include <dbPersistentOperatorIF.h>

#include <LTAInventoryObject.h>
#include <FileType.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbPersistent ;
class dbConnection ;

//
//
//
// class LTAInventoryObjectOperatorIF
//
//
//
class LTAInventoryObjectOperatorIF : public dbPersistentOperatorIF // PRQA S 2109
{
public:

	/*! class dbInventoryObjectOperatorException declaration */
	exDECLARE_EXCEPTION(LTAInventoryObjectOperatorException,dbPersistentOperatorException) ; // PRQA S 2131, 2153, 2502

	LTAInventoryObjectOperatorIF() = default ;
	LTAInventoryObjectOperatorIF(const LTAInventoryObjectOperatorIF &) = default ;
	LTAInventoryObjectOperatorIF &operator=(const LTAInventoryObjectOperatorIF &) = default ;
	LTAInventoryObjectOperatorIF(LTAInventoryObjectOperatorIF &&) = default ;
	LTAInventoryObjectOperatorIF &operator=(LTAInventoryObjectOperatorIF &&) = default ;
	virtual ~LTAInventoryObjectOperatorIF() = default ;

	virtual dbPersistentOperatorIF *clone() const ; // PRQA S 2502

	virtual dbPersistent::IdType getId(const dbPersistent &) const ;
	virtual const std::string getFileName(const dbPersistent &) const ;
	virtual const std::string getFileVersion(const dbPersistent &) const ;
	virtual const std::string getFileType(const dbPersistent &) const ;

	virtual const std::string getGroup(const dbPersistent &, dbConnection &) const ;
	
	virtual bool canGetFromSynchSM(const dbPersistent &, dbConnection &) const ;

protected:	

private:
	void loadFileType(const db::LTAInventoryObject* , dbConnection &,  db::FileType &  ) const ;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAInventoryObjectOperatorIF) ;
} ; 


_ACS_END_NAMESPACE


#endif // _LTAInventoryObjectOperatorIF_H_
