// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ThinLayer Order Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/01/25 12:02:59  marpas
	using pattern::Singleton from libException
	
	Revision 2.0  2006/02/28 10:06:01  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/03/09 09:26:08  marfav
	TlOrderQueue added
	

*/


#include <TlOrderQueueSingleton.h>

using namespace std;
using namespace acs;


TlOrderQueueSingleton::TlOrderQueueSingleton () : acs::pattern::Singleton<TlOrderQueue> ()
{
}


