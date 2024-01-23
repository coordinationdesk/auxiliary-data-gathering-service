// PRQA S 1050 EOF
/*
 * 
 * 	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Storage Manager Library$

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/18 14:40:02  marpas
	adoption of libException 5.x standards
	
	Revision 2.2  2013/02/25 17:00:18  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.1  2008/11/11 17:53:33  ivafam
	ScopeGuard implemetation changed
	
	Revision 2.0  2006/02/28 09:11:13  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/08/05 07:02:59  ivafam
	Added classes for packing strategy management
	
	

*/


#ifndef _SMPackingStrategyScopeGuard_H_
#define _SMPackingStrategyScopeGuard_H_ 

#include <acs_c++config.hpp>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)

class SMPackingStrategy ;
typedef ACS_SMARTPTR<SMPackingStrategy> SMPackingStrategyScopeGuard ;

_ACS_END_NAMESPACE


#endif // _SMPackingStrategyScopeGuard_H_
