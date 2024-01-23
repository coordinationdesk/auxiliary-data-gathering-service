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
	Revision 2.12  2017/10/03 15:28:24  marfav
	Adding dbKey to BuildParameters
	
	Revision 2.11  2017/09/20 13:12:36  davide.tiriticco
	Refactoring parameter class
	
	Revision 2.10  2017/09/19 09:54:21  davide.tiriticco
	Introduced factory class. Default implementation moved in DefaultExecutorVisitor
	
	Revision 2.9  2017/03/17 16:00:47  marfav
	Posix Exit Code compliancy flag embedded in ScOrder file
	
	Revision 2.8  2017/03/17 11:24:26  marfav
	APF-411
	Added support to GEN-PROC-ICD exit code conventions
	Default mode is POSIX
	
	Revision 2.7  2015/04/30 13:01:39  marfav
	Clashing operations with base classes renamed
	
	Revision 2.6  2013/12/16 14:04:07  marpas
	coding best practices applied
	
	Revision 2.5  2013/11/06 18:10:22  marpas
	adopting libException 5.x interface
	
	Revision 2.4  2013/07/25 16:18:28  marfav
	Using synchronous waitin loop monitoring udergoing processes
	
	Revision 2.3  2013/02/21 14:44:20  marfav
	Compiler warnings removed
	
	Revision 2.2  2009/06/26 10:50:24  marpas
	some classes do not throw any exception from their dtors
	
	Revision 2.1  2008/04/22 11:13:21  marfav
	Executors implemented using the conditioned visitor patter
	
	
*/


#ifndef _ExecutorVisitor_H_
#define _ExecutorVisitor_H_

#include <TaskTableVisitor.h>
#include <ExecutorBase.h>
#include <scOrder.h>
#include <tlTaskLauncher.h>
#include <TaskTableElementExecutor.h>

_ACS_BEGIN_NAMESPACE(acs)

class ExecutorVisitor : public TaskTableVisitor, public ExecutorBase // PRQA S 2109, 2153
{

public:

	/**
	 * \brief This class wraps the input parameters that must be passed to ExecutorVisitor constructors.
	 */
	class BuildParameters {	// PRQA S 2109

	public:
		/**
		 * \brief Constructor
		 */
		BuildParameters(const scOrder& order,
				const StringKey& key,
				bool detached = false, 
				int kills = 15, 
				int quits = 2, 
				TaskTableElementExecutor* theParent = 0);

		/**
		 * \brief Destructor
		 */			
		virtual ~BuildParameters() throw();
		
		/**
		 * \brief Copy constructor
		 */			
		BuildParameters(const BuildParameters&);

		/**
		 * \brief Assignment operator overload
		 */			
		BuildParameters& operator=(const BuildParameters&);
		
	public:
		scOrder _order;
		StringKey _key;
		bool _detached;
		int _kills;
		int _quits;
		TaskTableElementExecutor* _theParent;
		std::string _dbKey;
		bool _emitTrk;
		
	private:
		/**
		 * \brief Default constructor
		 */
		BuildParameters(); // Defined but not implemented		
	};
	
public:				
	virtual ~ExecutorVisitor() throw();

protected:

	/**
	 * Ctor and Dtor are empty. 
	 */ 
	explicit ExecutorVisitor(const BuildParameters& _params);

protected:
	
	BuildParameters _params;
		
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ExecutorVisitor) ;

private:
	ExecutorVisitor(); // Defined but not implemented
	
	ExecutorVisitor(const ExecutorVisitor&); // Defined but not implemented
	
	ExecutorVisitor& operator=(const ExecutorVisitor&); // Defined but not implemented 
};

_ACS_END_NAMESPACE


#endif //_ExecutorVisitor_H_

