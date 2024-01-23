/*
	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageProcessing$

	$Id$

	$Author$

	$Log$
	Revision 1.8  2015/02/18 09:42:48  matteo.airoldi
	nicvac: throw() to destructor.
	
	Revision 1.7  2013/11/07 17:15:44  nicvac
	Task table management simplified.
	
	Revision 1.6  2013/11/04 14:27:20  marpas
	adopting libException 5.x interface
	
	Revision 1.5  2013/07/16 16:02:38  nicvac
	Used Filterables.h
	
	Revision 1.4  2013/06/17 13:56:44  nicvac
	New macro used.
	
	Revision 1.3  2013/05/16 12:43:00  nicvac
	S2PDGS-317: If task table not found, task tables are refreshed from Db.
	
	Revision 1.2  2013/01/28 14:52:47  nicvac
	Singleton pattern updated
	
	Revision 1.1  2012/12/12 16:21:17  nicvac
	PackageTaskTableManager added.
	
        
*/

#ifndef _PackageTaskTableManager_H_
#define _PackageTaskTableManager_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Singleton.hpp>

#include <File.h>

#include <Filterables.h>

#include <map>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

/**
* \brief
* Manager for Package Task Tables
**/

class PackageTaskTableManager :
		public Singleton <PackageTaskTableManager>,
		public ThreadSafe {
public:

	/** PackageTaskTableManager Exceptions */
	exDECLARE_EXCEPTION(exPackageTaskTableManagerException, exException) ; // Base PackageTaskTableManager Exception.
	exDECLARE_EXCEPTION(exPackageTaskTableManagerCriticalException, exPackageTaskTableManagerException) ; // Critical Exception.


	virtual ~PackageTaskTableManager() throw ();

protected:  // Constructor cannot be called explicitly
	/** Default Class constructor */
	PackageTaskTableManager();
	

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	PackageTaskTableManager(const PackageTaskTableManager & );
	/** Operator = */
	PackageTaskTableManager &operator=(const PackageTaskTableManager &);
	friend PackageTaskTableManager* Singleton<PackageTaskTableManager>::instance(); //SINGLETON

public:

	/** Download TaskTables from DB */
	void downloadTaskTables();

	/** Release Task Table */
	void releaseTaskTables();

	/** Return the tt filename,  given the TT name.
	 * Returned filename is locked exclusively:
	 * - only this process (and children) can read/write the file.
	 * - Cache cannot roll it. */
	std::string getTTFilename(std::string const& ttName);

	/** Return the tt filename,  given the TT id.
	 * Cf. Lock comment on the other getTTFilename method. */
	std::string getTTFilename(int const& ttId);


private:
	/** template getTTFilename */
	template <typename Tkey, class Tmap >
	std::string getTTFilename(std::string const& keyDescr, Tkey const& key, Tmap& ttMap);


private:

	class TTInfo {
	public:
		TTInfo(): _idf(-1), _name(""), _filename("") {}
		TTInfo(int const& idf, std::string const& name, std::string const& filename):
			_idf(idf), _name(name), _filename(filename) {}
		int getIdf()                {return _idf;};
		std::string getName()       {return _name;};
		std::string getFilename()   {return _filename;};
	private:
		int _idf;
		std::string _name;
		std::string _filename;
	};

	/** Map containing id - tt info */
	std::map<int, TTInfo> _ttIdMap;
	/** Map containing name - tt info */
	std::map<std::string, TTInfo> _ttNameMap;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageTaskTableManager)

};

/** getTTFilename template: cf. real method description */
template <typename Tkey, class Tmap >
std::string PackageTaskTableManager::getTTFilename(std::string const& keyDescr, Tkey const& key, Tmap& ttMap) {

	typename Tmap::iterator ttMapPtr = ttMap.find(key);

	//If Task Table not found in map, maybe something changed in configuration (eg. added new TaskTable))
	std::ostringstream keyOss; keyOss<<key;
	ACS_COND_THROW( ttMapPtr == ttMap.end(),
			exPackageTaskTableManagerException(std::string("Task Table with ")+keyDescr+" ["+keyOss.str()+"] is unknown."
			+" This can be caused by misconfiguration (eg. a new Task Table added in DB while PackageExporter is running)."+
			+" Please, restart the PackageProcessor to fix the problem.") );

	std::string ttFilename = ttMapPtr->second.getFilename();

	//Task table find in map. Check if it is on disk (Cache cannot roll the file because the exclusive lock, but the file can be manual removed from Disk).
	ACS_COND_THROW( ! File::exists( ttFilename ),
			exPackageTaskTableManagerException(std::string("Task Table with ")+keyDescr+" ["+keyOss.str()+"], filename ["+ttFilename+"]"
			+" was been downloaded in disk cache, but cannot be found on disk."
			+" This file has not been rolled by cache (because the exclusive lock on file), so "
			+" this can be caused by forced remotion of file from disk (by another process or by manual operation)."+
			+" Please, restart the PackageProcessor to fix the problem.") );

	return ttFilename;
}



_ACS_END_NAMESPACE

#endif //_PackageTaskTableManager_H_

