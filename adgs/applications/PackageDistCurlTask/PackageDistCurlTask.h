/*
	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. PackageDistCurlTask$

	$Id$

	$Author$

	$Log$
	Revision 1.2  2016/01/15 14:14:16  nicvac
	SNAV-41: implementation
	
	Revision 1.1.1.1  2016/01/15 09:47:48  nicvac
	Import PackageDistCurlTask
	
        
*/

#ifndef _PackageDistCurlTask_H_
#define _PackageDistCurlTask_H_

#ifdef HAS_APPNAME_DEFINITIONS
#include <AppnameDefinitions>
#endif

#if defined (HAS_MISSION_INCLUDES)
#include <ProjectAppIncludes>
#endif

#include <ExpProcInfo.h>
#include <DistributionSerializable.h>

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Task running in Distribution TaskTable.
 * Use Curl (process) to upload each file/dir (first level only) in the datapackage path in the
 * path indicated in distribution Exp IF file.
**/

class PackageDistCurlTask: public PROJECT_APP {
public:

	/** PackageDistCurlTask Exceptions */
	exDECLARE_EXCEPTION(exPackageDistCurlTaskException, exException); // PackageDistCurlTask Exception.
	exDECLARE_EXCEPTION(exPackageDistCurlTaskCriticalException, exCriticalException); // PackageDistCurlTask Critical Exception.

	/** Class constructor */
	PackageDistCurlTask(const std::string& app, const std::string& subsys);
	/** Destructor */
	virtual ~PackageDistCurlTask() throw();

	/** Run entry point */
	virtual int main(int argc, char const * const * argv,
					 char const * const * env);


private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	PackageDistCurlTask(const PackageDistCurlTask & );
	/** Operator = */
	PackageDistCurlTask &operator=(const PackageDistCurlTask &);
	

protected:
	/** Core of the app */
	int execute(std::string const& expProcInfoFilename);

	/** Load and set configuration */
	void loadConf();

private:
	/** relativePathLocals: list of items to upload. Relative path to datapackage dir */
	void doCurl( std::vector<std::string> const& relativePathLocals ) const;

private:
	/** I/O interface with the PackageExporter */
	ExpProcInfo _expProcInfo;

	/** Distribution info */
	DistributionSerializable _distribution;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageDistCurlTask)

};

_ACS_END_NAMESPACE

#endif //_PackageDistCurlTask_H_

