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

	$Prod: A.C.S. Scheduler Default Commands Library $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2008/04/22 11:00:06  marfav
	Aligned to new visitable task table elements
	
	Revision 1.3  2004/12/03 17:53:02  marfav
	Header fixed
	
	Revision 1.2  2004/09/14 16:28:08  marfav
	Align to new library TaskExecution
	
	Revision 1.1.1.1  2004/04/07 13:31:48  marfav
	Import libScDefaultCommands
	

*/


#include <DefaultUserCmd.h>
#include <exStream.h>

#include <ExecutorBase.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


DefaultUserCmd::DefaultUserCmd() : ScAbstractCommand()
{}


DefaultUserCmd::DefaultUserCmd(const DefaultUserCmd& r) : ScAbstractCommand(r)
{
}


DefaultUserCmd& DefaultUserCmd::operator= (const DefaultUserCmd& r)
{
	if (this != &r)
	{
		ScAbstractCommand::operator=(r) ;
	}
	return *this;
}

void DefaultUserCmd::execute (ExecutorBase& t)
{
	ACS_LOG_INFO("DefaultUserCmd : user command not defined [USER, " << value() << ", " << param() << "]") ;
}

string DefaultUserCmd::getSignature () const
{
	return "DefaultUserCmd";
}

_ACS_END_NAMESPACE


