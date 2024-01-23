/*

	Copyright 1995-2004, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. TlDaemon Default PCC factory library $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2015/06/22 12:43:32  damdec
	Compilation warnings removed.
	
	Revision 1.2  2012/02/10 17:31:22  marpas
	refactoring in progress
	
	Revision 1.1.1.1  2008/08/04 10:10:39  marfav
	Importing libTlDefaultPCC
	
*/
/*

	Old log from previous library
	Revision 1.1.1.1  2008/04/22 13:08:32  marfav
	Importing libTlPreconditionChecker
	
	

*/

#include <DefaultAlternative_PCC.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(DefaultAlternative_PCC)


DefaultAlternative_PCC::DefaultAlternative_PCC(const PCC_ParamsPtr& theParameters) : Alternative_PCC(theParameters)
{
	// Default CTOR
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

}

DefaultAlternative_PCC::~DefaultAlternative_PCC() throw() 
{
	// DTOR
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);
}



bool DefaultAlternative_PCC::visit (InputAlternative&)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"visiting InputAlternative&\n");		
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"default checker will alwais return true for alternatives!\n");		

    setCheckPassed (true);
	setStatus ("OK");

	return true;
}
_ACS_END_NAMESPACE

