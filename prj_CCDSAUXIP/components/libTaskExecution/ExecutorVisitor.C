// PRQA S 1050 EOF
/*

	Copyright 1995-2003, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. TaskTableExecution Library $

	$Id$

	$Author$

	$Log$
	Revision 2.35  2017/10/06 16:03:11  marfav
	ECICMF-76
	Added support for enable/disable TKR messages
	
	Revision 2.34  2017/10/03 15:28:24  marfav
	Adding dbKey to BuildParameters
	
	Revision 2.33  2017/09/20 13:12:36  davide.tiriticco
	Refactoring parameter class
	
	Revision 2.32  2017/09/19 09:54:21  davide.tiriticco
	Introduced factory class. Default implementation moved in DefaultExecutorVisitor
	
	Revision 2.31  2017/03/17 16:00:47  marfav
	Posix Exit Code compliancy flag embedded in ScOrder file
	
	Revision 2.30  2017/03/17 11:24:26  marfav
	APF-411
	Added support to GEN-PROC-ICD exit code conventions
	Default mode is POSIX
	
	Revision 2.29  2017/01/20 16:49:40  marfav
	got rid of binary_function inheritance
	
	Revision 2.28  2016/04/20 07:46:08  marfav
	APF-371
	Added management and notification of the signals killing the tasks
	
	Revision 2.27  2015/12/23 10:39:45  marfav
	Splunk messages updated
	
	Revision 2.26  2015/11/27 15:13:34  marfav
	Checking configurable limits for syslog notification on per task base
	
	Revision 2.25  2015/11/26 15:04:52  marfav
	mc_params are now lowercase
	
	Revision 2.24  2015/08/21 07:41:49  marfav
	processingId get from the name of the current WD
	
	Revision 2.23  2015/08/12 15:58:32  marfav
	APF-271 work in progress
	
	Revision 2.22  2015/07/17 14:09:18  marfav
	Reverting changes of APF-285
	
	Revision 2.20  2015/04/30 13:01:39  marfav
	Clashing operations with base classes renamed
	
	Revision 2.19  2014/07/16 09:13:17  nicvac
	Adding signature info also on trk messages
	
	Revision 2.18  2014/05/21 08:25:44  marfav
	Added check for compiler version less than 4.3.0 (reference to reference issue)
	
	Revision 2.17  2014/05/09 12:37:38  marfav
	S2PDGS-784 using the log name do sign the filterables emitted
	
	Revision 2.16  2014/05/06 11:16:50  marfav
	Taking into account negative exit codes as errors
	
	Revision 2.15  2014/02/06 09:25:23  marpas
	dir load executed twice - fixed
	
	Revision 2.14  2013/12/16 14:04:07  marpas
	coding best practices applied
	
	Revision 2.13  2013/11/07 16:42:13  marfav
	Setting the exit code before declaring that exit code exists
	
	Revision 2.12  2013/11/06 18:10:22  marpas
	adopting libException 5.x interface
	
	Revision 2.11  2013/08/26 09:55:46  marfav
	Select period set to 100 mseconds
	
	Revision 2.10  2013/07/25 16:18:28  marfav
	Using synchronous waitin loop monitoring udergoing processes
	
	Revision 2.9  2013/02/21 14:44:20  marfav
	Compiler warnings removed
	
	Revision 2.8  2011/03/01 09:10:46  marant
	Select errors managed to avoid potential infinite loops
	
	Revision 2.7  2011/02/28 16:49:12  marfav
	File descriptors selected until a timeout is returned from the select
	upon completion of the running tasks to allow complete streams reading
	
	Revision 2.6  2009/06/26 10:50:24  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.5  2009/03/19 11:22:28  marfav
	Using regular expressions from ScOrder when checking for mandatory outputs
	
	Revision 2.4  2009/03/04 21:15:53  marpas
	DO NOT USE and INSTEAD OF && !!!
	
	Revision 2.3  2008/06/26 11:06:50  marfav
	pool exit visti logic improved when stop required
	
	Revision 2.2  2008/06/24 14:10:39  marfav
	Added quit request check in enterVisit
	New tasks will not be executed after a stop/quit request
	and an error exit code will be reported if an enterVisit is called
	after the reception of a stop/quit request
	
	Revision 2.1  2008/04/22 11:13:21  marfav
	Executors implemented using the conditioned visitor patter
	
	
*/


#include <ExecutorVisitor.h>
//#include <TaskTableExecutorFactoryChain.h>
#include <Timer.h>

#include <TaskDataTypes.h>
#include <Task.h>
#include <TaskTable.h>
#include <Pool.h>
#include <Sequence.h>

#include <StringUtils.h>
#include <ScErrFilterable.h>
#include <ScOutFilterable.h>
#include <ScLogManager.h>
#include <TrkFilterable.h>
#include <MCFilterables.h>

#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>

#include <scOrder.h>

#include <Functional.h>

#include <Dir.h>
#include <File.h>
#include <RegExp.h>

#include <algorithm>
#include <numeric>
#include <memory>
#include <string>

#if !(__GNUC__ > 4) || (__GNUC__ == 4 && (__GNUC_MINOR__ >= 3))
#define GCC_LESS_THAN_4_3_0
#endif


_ACS_BEGIN_NAMESPACE(acs)


using namespace std;
using namespace acs::mc;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ExecutorVisitor)

/********************************************************************
 * 		BUILD PARAMETERS IMPLEMENTATION
 ********************************************************************/

/*
 * Constructor
 */
ExecutorVisitor::BuildParameters::BuildParameters(const scOrder& order,
				const StringKey& key,
				bool detached, 
				int kills, 
				int quits, 
				TaskTableElementExecutor* theParent) :
	_order(order),
	_key(key),
	_detached(detached),
	_kills(kills),
	_quits(quits),
	_theParent(theParent),
	_dbKey(""),
	_emitTrk(true)
{ }

/**
 * Destructor
 */			
ExecutorVisitor::BuildParameters::~BuildParameters() throw() { }


ExecutorVisitor::BuildParameters::BuildParameters(const BuildParameters& param) : 
	_order(param._order),
	_key(param._key),
	_detached(param._detached),
	_kills(param._kills),
	_quits(param._quits),
	_theParent(param._theParent),
	_dbKey(	param._dbKey),
	_emitTrk ( param._emitTrk )
{ }


ExecutorVisitor::BuildParameters& ExecutorVisitor::BuildParameters::operator=(const BuildParameters& param) {
	if (this != &param) {
		_order = param._order;
		_key = param._key;
		_detached = param._detached;
		_kills = param._kills;
		_quits = param._quits;
		_theParent = param._theParent;
		_dbKey = param._dbKey;
		_emitTrk = param._emitTrk;
	}
	
	return *this;
}

ExecutorVisitor::ExecutorVisitor(const BuildParameters& params) :
    	TaskTableVisitor(),                            
    	ExecutorBase(), 
	_params(params)
{

	ExecutorBase::setDetached(_params._detached);
	ExecutorBase::setDefKillSignal(_params._kills);
	ExecutorBase::setDefQuitSignal(_params._quits);
	ExecutorBase::setDefaultPosixExitCode(_params._order.getPosixExitCode());

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "KillSignal        = " << ExecutorBase::getDefKillSignal()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "QuitSignal        = " << ExecutorBase::getDefQuitSignal()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Detached          = " << boolalpha << _params._detached ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Posix exit code   = " << boolalpha << _params._order.getPosixExitCode() ) ;	
}


ExecutorVisitor::~ExecutorVisitor() throw()
{
	// vector DTOR will not delete the pointed objects
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "DESTROYNG EXECUTOR VISITOR") ;
    // PRQA L:L1
}

_ACS_END_NAMESPACE
