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

	$Prod: A.C.S. Generic File Action Library $

	$Id$

	$Author$

	$Log$
	Revision 7.0  2015/02/17 11:38:13  marfav
	Field unique_id added to table t_smactions
	
	Revision 6.0  2013/09/19 15:40:44  marfav
	InventorySm and SmActions now use inventory id for reference
	
	Revision 5.0  2013/07/23 17:07:21  marpas
	adopting libException 5.x interface
	coding best practices applied
	compilation warnings fixed
	qa warning fixed
	
	Revision 1.4  2013/01/22 18:14:55  marpas
	qa rules work in progress
	
	Revision 1.3  2012/02/13 16:49:03  marpas
	refactoring
	
	Revision 1.2  2008/12/03 17:19:16  ivafam
	Work in progress
	
	Revision 1.1.1.1  2008/10/09 17:15:51  ivafam
	First Import
	
	

*/

#include <GFASMTmp.h>


using namespace acs ;
using namespace std ;


GFASMTmp::GFASMTmp(dbPersistent::IdType smid, const std::string &smtype, dbPersistent::IdType recid) :
	id(smid),
	cost(0),
	hasParameters(false),
	type(smtype),
	sminvrecordid(recid),
	synchronous(false)
{
}

GFASMTmp::GFASMTmp() :
	id(0),
	cost(0),
	hasParameters(false),
    type(),
	sminvrecordid(0),
	synchronous(false)
{
}
	
GFASMTmp::~GFASMTmp() throw() {}


GFASMTmp::GFASMTmp(const GFASMTmp &rhs) :
	id(rhs.id),
	cost(rhs.cost),
	hasParameters(rhs.hasParameters),
	type(rhs.type),
	sminvrecordid(rhs.sminvrecordid),
	synchronous(rhs.synchronous)
{
	// empty
}
 
GFASMTmp &GFASMTmp::operator=(const GFASMTmp &rhs) 
{
	if (this != &rhs) {
		id = rhs.id ;
		cost = rhs.cost ;
		hasParameters = rhs.hasParameters ;
		type = rhs.type ;
		sminvrecordid = rhs.sminvrecordid ;
		synchronous = rhs.synchronous;
	}
	
	return *this ;
}

