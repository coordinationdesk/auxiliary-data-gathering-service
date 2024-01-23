/*
	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageDistCirculatorTask$

	$Id$

	$Author$

	$Log$
	Revision 1.12  2013/06/14 16:30:29  nicvac
	ADC-182: Temp transder strategy implemented.
	
	Revision 1.11  2013/05/17 15:34:00  nicvac
	S2PDGS-300: priority handling.
	
	Revision 1.10  2013/05/03 15:26:34  nicvac
	_defaultDbKey init in load configuration.
	
	Revision 1.9  2013/04/22 11:04:33  marpas
	NameIdAssociator and related classes are now using Singleton template pattern
	
	Revision 1.8  2013/04/16 18:18:42  nicvac
	Soap Circulation if implemented.
	
	Revision 1.7  2013/04/11 17:25:30  nicvac
	From url settings changed: network source if destination is not filesystem
	
	Revision 1.6  2013/03/28 14:35:47  nicvac
	Circulation DB from configuration.
	
	Revision 1.5  2013/03/15 19:46:19  nicvac
	comment minor
	
	Revision 1.4  2013/03/15 17:27:32  nicvac
	ADC-83: Synch and Asynch distribution.
	
	Revision 1.3  2012/12/12 16:31:41  nicvac
	No JobOrder, new Distribution.
	
	Revision 1.2  2012/11/27 14:45:48  nicvac
	Implementation.
	
	Revision 1.1.1.1  2012/11/27 11:44:37  nicvac
	Import PackageDistCirculatorTask
	
        
*/

#ifndef _PackageDistCirculatorTask_H_
#define _PackageDistCirculatorTask_H_

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
 * //@@@ DESCRIZIONE DA RIVEDERE!!!
 * It interfaces the Data Circulator to circulate the datapackage passed by the JobOrder.
 * According to the media type the task calls the Data Circulation service requesting an FTP circulation 
 * or a Media circulation. The media type is got from the distribution file read from JobOrder.
 * In case of asynch circulation (e.g. via media):
 * - the task returns immediately with "Accepted" status.
 * - the actual transfer will be monitored by another dedicated Task Table.
 * Otherwise (the circulation is sunch) the task monitors the request and return with success 
 * or with error depending on the actual outcome of the transfer.
 * If the received datapackage dir to transfer is empty, the datapackage is not transferred 
 * and the task exit with success. An empty datapackage can be caused by previous pachage processing.
**/

class PackageDistCirculatorTask: public PROJECT_APP, public PackageCirculatorTask {
public:

	/** PackageDistCirculatorTask Exceptions */
	exDECLARE_EXCEPTION(exPackageDistCirculatorTaskException, exException) ; // Base PackageDistCirculatorTask Exception.
	exDECLARE_EXCEPTION(exPackageDistCirculatorTaskCriticalException, exPackageDistCirculatorTaskException) ; // Critical Exception.


	/** Class constructor */
	PackageDistCirculatorTask(const std::string& app, const std::string& subsys);
	/** Destructor */
	virtual ~PackageDistCirculatorTask() throw();

	/** Run entry point */
	virtual int main(int argc, char const * const * argv, 
					 char const * const * env);


	/** Signal handlers */
	virtual void customSigIntHandler() ;
	virtual void customSigTermHandler() ;

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	PackageDistCirculatorTask(const PackageDistCirculatorTask & );
	/** Operator = */
	PackageDistCirculatorTask &operator=(const PackageDistCirculatorTask &);
	

protected:

	/** Core of the app */
	int execute(std::string const& expProcInfoFilename);

	/** Load and set configuration */
	void loadConf();


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageDistCirculatorTask)

};

_ACS_END_NAMESPACE

#endif //_PackageDistCirculatorTask_H_

