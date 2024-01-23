/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. PackageCheckCirculatorTask$

	$Id$

	$Author$

	$Log$
	Revision 1.5  2013/05/03 15:25:39  nicvac
	_defaultDbKey init in load configuration.
	
	Revision 1.4  2013/04/22 11:04:21  marpas
	NameIdAssociator and related classes are now using Singleton template pattern
	
	Revision 1.3  2013/04/16 18:19:05  nicvac
	Soap Circulation If used.
	
	Revision 1.2  2013/03/28 14:35:31  nicvac
	Circulation DB from configuration.
	
	Revision 1.1.1.1  2013/03/15 19:34:05  nicvac
	Import PackageCheckCirculatorTask
	
        
*/

#ifndef _PackageCheckCirculatorTask_H_
#define _PackageCheckCirculatorTask_H_

#include <acs_c++config.hpp>

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#if defined (HAS_MISSION_INCLUDES)
#include <ProjectAppIncludes>
#endif

#include <PackageCirculatorTask.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
 * Processing Task running in Task Table. 
 * It interfaces the Data Circulator to Check the distribution outcome .
 * If requested Synch ==> wait untill the distribution has been performed
 * If requested ASynch ==> tells the PackageExporter if distribution performed or if still to be monitored
**/

class PackageCheckCirculatorTask: public PROJECT_APP, public PackageCirculatorTask { 
public:

	/** PackageCheckCirculatorTask Exceptions */
	exDECLARE_EXCEPTION(exPackageCheckCirculatorTaskException, exException) ; // Base PackageCheckCirculatorTask Exception.
	exDECLARE_EXCEPTION(exPackageCheckCirculatorTaskCriticalException, exPackageCheckCirculatorTaskException) ; // Critical Exception.


	/** Class constructor */
	PackageCheckCirculatorTask(const std::string& app, const std::string& subsys);
	/** Destructor */
	virtual ~PackageCheckCirculatorTask() throw();

	/** Run entry point */
	virtual int main(int argc, char const * const * argv, 
					 char const * const * env);

	/** Signal handlers */
	virtual void customSigIntHandler() ;
	virtual void customSigTermHandler() ;

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	PackageCheckCirculatorTask(const PackageCheckCirculatorTask & );
	/** Operator = */
	PackageCheckCirculatorTask &operator=(const PackageCheckCirculatorTask &);
	
protected:

	/** Core of the app */
	int execute(std::string const& expProcInfoFilename);

	/** Load and set configuration */
	void loadConf();

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageCheckCirculatorTask)

};

_ACS_END_NAMESPACE

#endif //_PackageCheckCirculatorTask_H_

