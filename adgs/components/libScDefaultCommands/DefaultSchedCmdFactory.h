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

#ifndef _DefaultSchedCmdFactory_H_
#define _DefaultSchedCmdFactory_H_

#include <ScCommandFactoryChain.h>
#include <ScAbstractCommand.h>
#include <StringKeyFactory.h>


_ACS_BEGIN_NAMESPACE(acs)

class DefaultSuspendCmdFactory : public StringKeyFactory<ScAbstractCommand> // PRQA S 2153
{
public:
	DefaultSuspendCmdFactory();
	virtual ~DefaultSuspendCmdFactory() throw() {} // PRQA S 2131
	static ScAbstractCommand *builder ();
private:
	DefaultSuspendCmdFactory(const DefaultSuspendCmdFactory &); // not implemented
	DefaultSuspendCmdFactory& operator=(const DefaultSuspendCmdFactory &); // not implemented
};

class DefaultResumeCmdFactory : public StringKeyFactory<ScAbstractCommand> // PRQA S 2153
{
public:
	DefaultResumeCmdFactory();
	virtual ~DefaultResumeCmdFactory() throw() {} // PRQA S 2131
	static ScAbstractCommand *builder ();
private:
	DefaultResumeCmdFactory(const DefaultResumeCmdFactory &); // not implemented
	DefaultResumeCmdFactory& operator=(const DefaultResumeCmdFactory &); // not implemented
};

class DefaultAbortCmdFactory : public StringKeyFactory<ScAbstractCommand> // PRQA S 2153
{
public:
	DefaultAbortCmdFactory();
	virtual ~DefaultAbortCmdFactory() throw() {} // PRQA S 2131
	static ScAbstractCommand *builder ();
private:
	DefaultAbortCmdFactory(const DefaultAbortCmdFactory &); // not implemented
	DefaultAbortCmdFactory& operator=(const DefaultAbortCmdFactory &); // not implemented
};

class DefaultStopCmdFactory : public StringKeyFactory<ScAbstractCommand> // PRQA S 2153
{
public:
	DefaultStopCmdFactory();
	virtual ~DefaultStopCmdFactory() throw() {} // PRQA S 2131
	static ScAbstractCommand *builder ();
private:
	DefaultStopCmdFactory(const DefaultStopCmdFactory &); // not implemented
	DefaultStopCmdFactory& operator=(const DefaultStopCmdFactory &); // not implemented
};

class DefaultUserCmdFactory : public StringKeyFactory<ScAbstractCommand> // PRQA S 2153
{
public:
	DefaultUserCmdFactory();
	virtual ~DefaultUserCmdFactory() throw() {} // PRQA S 2131
	static ScAbstractCommand *builder ();
private:
	DefaultUserCmdFactory(const DefaultUserCmdFactory &); // not implemented
	DefaultUserCmdFactory& operator=(const DefaultUserCmdFactory &); // not implemented
};

class DefaultKillCmdFactory : public StringKeyFactory<ScAbstractCommand> // PRQA S 2153
{
public:
	DefaultKillCmdFactory();
	virtual ~DefaultKillCmdFactory() throw() {} // PRQA S 2131
	static ScAbstractCommand *builder ();
private:
	DefaultKillCmdFactory(const DefaultKillCmdFactory &); // not implemented
	DefaultKillCmdFactory& operator=(const DefaultKillCmdFactory &); // not implemented
};


_ACS_END_NAMESPACE

#endif //_DefaultSchedCmdFactory_H_

