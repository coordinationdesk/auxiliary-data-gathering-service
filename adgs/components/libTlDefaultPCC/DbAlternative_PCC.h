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
	Revision 1.5  2013/02/21 13:52:28  marfav
	Removing compiler warnings
	
	Revision 1.4  2012/02/10 17:31:22  marpas
	refactoring in progress
	
	Revision 1.3  2009/09/29 14:32:16  marpas
	useless private method removed
	
	Revision 1.2  2008/11/07 15:56:14  marfav
	Retrieving deltaT from internal parameter
	
	Revision 1.1.1.1  2008/08/04 10:10:39  marfav
	Importing libTlDefaultPCC
	
*/
/*

	Old log from previous library
	Revision 1.1.1.1  2008/04/22 13:08:31  marfav
	Importing libTlPreconditionChecker
	
	

*/

#ifndef _DBALTERNATIVE_PCC_H_
#define _DBALTERNATIVE_PCC_H_

#include <Alternative_PCC.h>

#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class DbAlternative_PCC : public Alternative_PCC
{

using Alternative_PCC::visit;

public:
	exDECLARE_EXCEPTION (DbAlternative_PCC_Exception, Alternative_PCC_Exception) ;

	DbAlternative_PCC (const PCC_ParamsPtr& theParameters);
	virtual ~DbAlternative_PCC () throw() ;
	
	
	virtual bool visit (InputAlternative&);

protected:


private:
	PCC_Base::SymTableParams check(InputAlternative&);

private:
	bool _disableCheck;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DbAlternative_PCC)

};

_ACS_END_NAMESPACE

#endif //_DBALTERNATIVE_PCC_H_
