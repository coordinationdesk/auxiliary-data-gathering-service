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

	$Prod: A.C.S. ThinLayer Scheduler $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2004/12/03 17:55:32  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/09/14 16:57:30  marfav
	Import libScLogManager
	


*/


#ifndef _ScLogHandlerFactoryChain_H_
#define _ScLogHandlerFactoryChain_H_

#include <acs_c++config.hpp>
#include <ScLogHandlerGroup.h>
#include <scOrder.h>

#include <exException.h>
#include <StringKeyFactory.h>



_ACS_BEGIN_NAMESPACE(acs)

typedef StringKeyFactory<ScLogHandlerGroup, scOrder>::SingletonType ScLogHandlerFactoryChain;



_ACS_END_NAMESPACE



#endif // _ScLogHandlerFactoryChain_H_


