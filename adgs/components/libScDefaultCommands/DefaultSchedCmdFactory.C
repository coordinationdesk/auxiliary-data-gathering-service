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
	Revision 1.2  2004/12/03 17:53:02  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:31:48  marfav
	Import libScDefaultCommands
	

*/

#include <DefaultSchedCmdFactory.h>
#include <DefaultSuspendCmd.h>
#include <DefaultResumeCmd.h>
#include <DefaultStopCmd.h>
#include <DefaultAbortCmd.h>
#include <DefaultUserCmd.h>
#include <DefaultKillCmd.h>

#include <scdcV.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

namespace {
 scdcV version;

 const char* _suspendRSpace   =   "TlScheduler.FactoryConfigurationSettings.DefaultSuspendCmdFactory";
 const char* _resumeRSpace    =   "TlScheduler.FactoryConfigurationSettings.DefaultResumeCmdFactory";
 const char* _abortRSpace     =   "TlScheduler.FactoryConfigurationSettings.DefaultAbortCmdFactory";
 const char* _stopRSpace      =   "TlScheduler.FactoryConfigurationSettings.DefaultStopCmdFactory";
 const char* _userRSpace      =   "TlScheduler.FactoryConfigurationSettings.DefaultUserCmdFactory";
 const char* _killRSpace      =   "TlScheduler.FactoryConfigurationSettings.DefaultKillCmdFactory";

 // EMBEDDED FACTORIES
 DefaultSuspendCmdFactory _theSuspendFactory;
 DefaultResumeCmdFactory _theResumeFactory;
 DefaultStopCmdFactory _theStopFactory;
 DefaultAbortCmdFactory _theAbortFactory;
 DefaultUserCmdFactory _theUserFactory;
 DefaultKillCmdFactory _theKillFactory;
}


// SUSPEND

DefaultSuspendCmdFactory::DefaultSuspendCmdFactory() :
	StringKeyFactory<ScAbstractCommand> (_suspendRSpace, &DefaultSuspendCmdFactory::builder)
{}

ScAbstractCommand *
DefaultSuspendCmdFactory::builder ()
{
	return new DefaultSuspendCmd();
}


// RESUME

DefaultResumeCmdFactory::DefaultResumeCmdFactory() :
	StringKeyFactory<ScAbstractCommand> (_resumeRSpace, &DefaultResumeCmdFactory::builder)
{}

ScAbstractCommand *
DefaultResumeCmdFactory::builder ()
{
	return new DefaultResumeCmd();
}


// ABORT

DefaultAbortCmdFactory::DefaultAbortCmdFactory() :
	StringKeyFactory<ScAbstractCommand> (_abortRSpace, &DefaultAbortCmdFactory::builder)
{}

ScAbstractCommand *
DefaultAbortCmdFactory::builder ()
{
	return new DefaultAbortCmd();
}


// STOP

DefaultStopCmdFactory::DefaultStopCmdFactory() :
	StringKeyFactory<ScAbstractCommand> (_stopRSpace, &DefaultStopCmdFactory::builder)
{}

ScAbstractCommand *
DefaultStopCmdFactory::builder ()
{
	return new DefaultStopCmd();
}



// USER

DefaultUserCmdFactory::DefaultUserCmdFactory() :
	StringKeyFactory<ScAbstractCommand> (_userRSpace, &DefaultUserCmdFactory::builder)
{}

ScAbstractCommand *
DefaultUserCmdFactory::builder ()
{
	return new DefaultUserCmd();
}


// KILL

DefaultKillCmdFactory::DefaultKillCmdFactory() :
	StringKeyFactory<ScAbstractCommand> (_killRSpace, &DefaultKillCmdFactory::builder)
{}

ScAbstractCommand *
DefaultKillCmdFactory::builder ()
{
	return new DefaultKillCmd();
}

_ACS_END_NAMESPACE
