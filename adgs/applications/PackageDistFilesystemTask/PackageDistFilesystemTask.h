/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. PackageDistFilesystemTask$

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2013/07/25 12:28:01  nicvac
	Import PackageDistFilesystemTask
	
        
*/

#ifndef _PackageDistFilesystemTask_H_
#define _PackageDistFilesystemTask_H_

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
 * Hard link each file/dir (first level only) in the datapackage path in the
 * path indicated in distribution Exp IF file.
 * The hard link is on the original real file (not on symbolic link)
 * Only file:// or fs:// protocols are allowed.
**/

class PackageDistFilesystemTask: public PROJECT_APP { 
public:

	/** PackageDistFilesystemTask Exceptions */
	exDECLARE_EXCEPTION(exPackageDistFilesystemTaskException, exException); // Base PackageDistFilesystemTask Exception.
	exDECLARE_EXCEPTION(exPackageDistFilesystemTaskCriticalException, exPackageDistFilesystemTaskException); // Critical Exception.


	/** Class constructor */
	PackageDistFilesystemTask(const std::string& app, const std::string& subsys);
	/** Destructor */
	virtual ~PackageDistFilesystemTask() throw();

	/** Run entry point */
	virtual int main(int argc, char const * const * argv, 
					 char const * const * env);


private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	PackageDistFilesystemTask(const PackageDistFilesystemTask & );
	/** Operator = */
	PackageDistFilesystemTask &operator=(const PackageDistFilesystemTask &);
	

protected:
	/** Core of the app */
	int execute(std::string const& expProcInfoFilename);

	/** Load and set configuration */
	void loadConf();

private:
	/** Hard Link src to dest. If src is symbolic link, the actual file is used */
	void resolveAndLink(std::string const& src, std::string const& dest) const;

private:
	/** I/O interface with the PackageExporter */
	ExpProcInfo _expProcInfo;

	/** Distribution info */
	DistributionSerializable _distribution;

	/** Cf. Configuration */
	std::string _confProtocolRe;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageDistFilesystemTask)

};

_ACS_END_NAMESPACE

#endif //_PackageDistFilesystemTask_H_

