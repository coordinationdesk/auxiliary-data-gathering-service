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

	$Prod: A.C.S. ThinLayer PreConditionChecker Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2008/08/04 10:16:22  marfav
	Library split in definition and implementation
	in order to support plugins management
	
	Revision 1.1  2008/04/29 12:45:40  marfav
	Added support for PreconditionCheckFactoryChain
	
	Revision 1.1.1.1  2008/04/22 13:08:31  marfav
	Importing libTlPreconditionChecker
	
	

*/

#include <PreconditionCheckerFactoryChain.h>
#include <tlpccV.h>

#include <exStream.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace { // unnamed
    tlpccV version;
}

_ACS_END_NAMESPACE
