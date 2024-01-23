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

	$Prod: A.C.S. ThinLayer JobOrder Library $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2006/02/28 10:11:29  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2004/12/03 17:47:54  marfav
	Header fixed
	
	Revision 1.4  2004/08/24 14:12:17  marfav
	Using template factory library
	
	Revision 1.3  2003/07/10 14:04:56  marfav
	clone method added to exceptions
	
	Revision 1.2  2003/04/18 12:52:21  marfav
	Using namespaces removed from .h
	
	Revision 1.1  2003/03/24 14:49:28  marfav
	Added JobOrder Factory
	
		

*/

#ifndef _JobOrderFactoryChain_H_
#define _JobOrderFactoryChain_H_

#include <acs_c++config.hpp>
#include <JobOrder.h>

#include<exException.h>
#include<StringKeyFactory.h>



_ACS_BEGIN_NAMESPACE(acs)

typedef StringKeyFactory<JobOrder>::SingletonType JobOrderFactoryChain;

_ACS_END_NAMESPACE


#endif // _JobOrderFactoryChain_H_
