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

	$Prod: A.C.S. Scheduler Abstract Commands Library $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2004/12/03 17:57:14  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:31:00  marfav
	Import libSchedCommands
	

*/

#ifndef _ScCommandFactoryChain_H_
#define _ScCommandFactoryChain_H_

#include<ScAbstractCommand.h>
#include<StringFactoryChain.h>

_ACS_BEGIN_NAMESPACE(acs)

//class ScCommandFactoryChain;

typedef StringFactoryChain<ScAbstractCommand> ScCommandFactoryChain;


void registerScCommandFactoryMethod(const ScCommandFactoryChain::FactoryType::keyType&,  ScCommandFactoryChain::FactoryType::BuilderPointer);
void unregisterScCommandFactoryMethod(const ScCommandFactoryChain::FactoryType::keyType&, ScCommandFactoryChain::FactoryType::BuilderPointer);

_ACS_END_NAMESPACE

#endif // _ScCommandFactoryChain_H_
