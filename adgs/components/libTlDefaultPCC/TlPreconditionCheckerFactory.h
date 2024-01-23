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
	Revision 1.2  2012/02/10 17:31:22  marpas
	refactoring in progress
	
	Revision 1.1.1.1  2008/08/04 10:10:39  marfav
	Importing libTlDefaultPCC
	
*/

#ifndef _TlPreconditionCheckerFactory_H_
#define _TlPreconditionCheckerFactory_H_

#include <StringKeyFactory.h>

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class Alternative_PCC;
class ThinLayer_PCC;

class TlPreconditionCheckerFactory 
{
	public:
		TlPreconditionCheckerFactory();
		~TlPreconditionCheckerFactory() throw() ;
		static Alternative_PCC* buildDbAlternative_PCC(const PCC_Base::PCC_ParamsPtr& parameters);
		static Alternative_PCC* buildDefaultAlternative_PCC(const PCC_Base::PCC_ParamsPtr& parameters);
		static ThinLayer_PCC* buildDefaultTl_PCC (const StringKey& parameter);

	private:
		TlPreconditionCheckerFactory (const TlPreconditionCheckerFactory&);
		TlPreconditionCheckerFactory& operator= (const TlPreconditionCheckerFactory&);

		ACS_CLASS_DECLARE_DEBUG_LEVEL(TlPreconditionCheckerFactory)
};


_ACS_END_NAMESPACE


#endif //  _TlPreconditionCheckerFactory_H_
       // 
