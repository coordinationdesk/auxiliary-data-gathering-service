// PRQA S 1050 EOF
/*
 * 
	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Storage Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/18 14:40:01  marpas
	adoption of libException 5.x standards
	
	Revision 2.3  2013/02/25 17:00:17  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.2  2008/11/11 17:53:33  ivafam
	ScopeGuard implemetation changed
	
	Revision 2.1  2008/11/11 11:00:54  ivafam
	Added
	
	

*/


#ifndef _AsynchSMInterfaceScopeGuard_H_
#define _AsynchSMInterfaceScopeGuard_H_ 

#include <acs_c++config.hpp>
#include <memory>

_ACS_BEGIN_NAMESPACE(acs)

class AsynchSMInterface ;
typedef ACS_SMARTPTR<AsynchSMInterface> AsynchSMInterfaceScopeGuard ;

_ACS_END_NAMESPACE


#endif // _AsynchSMInterfaceScopeGuard_H_
