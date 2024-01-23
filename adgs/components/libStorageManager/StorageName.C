// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/18 14:40:02  marpas
	adoption of libException 5.x standards
	
	Revision 1.5  2013/04/05 09:36:45  marpas
	NameIdAssociator partial refactoring
	
	Revision 1.4  2013/02/25 17:00:18  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 1.3  2012/02/13 17:08:58  marpas
	refactoring in proegress
	
	Revision 1.2  2008/11/11 11:00:54  ivafam
	Added
	
	Revision 1.1  2008/09/18 06:51:30  ivafam
	Added
	
	Revision 1.1  2008/07/28 09:01:09  manuel.maccaroni
	new library StorageName to query tOriginators table
	



*/
#include <StorageName.h>

_ACS_BEGIN_NAMESPACE(acs)

//
//	StorageName Name
//	Singleton Implementation
//
//



StorageName::StorageName(const std::string &tbl, const std::string &pk, const std::string &col,const std::string &key) :
    NameIdAssociator<unsigned int>(tbl,pk,col,key),
    pattern::Singleton<StorageName>()
{
}

StorageName::~StorageName() throw()
{
}

_ACS_END_NAMESPACE

