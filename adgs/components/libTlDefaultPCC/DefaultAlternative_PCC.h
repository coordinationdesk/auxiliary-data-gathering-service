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
	Revision 1.3  2013/02/21 13:52:28  marfav
	Removing compiler warnings
	
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

#ifndef _DEFAULTALTERNATIVE_PCC_H_
#define _DEFAULTALTERNATIVE_PCC_H_

#include <Alternative_PCC.h>

_ACS_BEGIN_NAMESPACE(acs)

class DefaultAlternative_PCC : public Alternative_PCC
{

using Alternative_PCC::visit;

public:
	exDECLARE_EXCEPTION (DefaultAlternative_PCC_Exception, Alternative_PCC_Exception) ;

	DefaultAlternative_PCC (const PCC_ParamsPtr& theParameters);
	virtual ~DefaultAlternative_PCC () throw() ;
	
	
	virtual bool visit (InputAlternative&);

protected:

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DefaultAlternative_PCC)

};

_ACS_END_NAMESPACE

#endif //_DEFAULTALTERNATIVE_PCC_H_
