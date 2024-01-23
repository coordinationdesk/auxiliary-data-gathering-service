// PRQA S 1050 EOF
/*

	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  A.C.S. WS Config Builder $

	$Id$

	$Author$

	$Log$
	Revision 2.5  2013/06/21 13:44:08  marfav
	Warning removed. Aligned to libException V5
	
	Revision 2.4  2012/02/14 15:19:08  marpas
	fixing dtor
	
	Revision 2.3  2010/08/03 14:41:38  nicvac
	Task Table plugins management
	
	Revision 2.2  2010/06/15 11:31:13  marpas
	HAS_MISSION_INCLUDES management added
	
	Revision 2.1  2007/04/19 08:52:21  giucas
	Removed dependencies from StationApp for ENVISAT
	
	Revision 2.0  2006/02/28 17:57:01  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/06/10 13:13:53  fracar
	Import WSConfigBuilder
	
	


*/


#ifndef _TASK_TABLE_MERGER_H
#define _TASK_TABLE_MERGER_H

#include <Application.h>

#if defined(HAS_MISSION_INCLUDES)
    #include <ProjectAppIncludes>
#elif COSMO
	#include <CosmoApp.h>
#elif CAL_VAL_APP
	#include <CvfSchApp.h>
#else 
	#include <CryosatApp.h>
#endif 


#include <WSConfig.h>
#include <TaskTable.h>

#include <ConfigurablePluginLoader.h>

#include <rsResourceSet.h>
#include <exException.h>

#include <boost/program_options.hpp>

#include <string>



#if defined(HAS_MISSION_INCLUDES)
#	define BASE_APP acs::PROJECT_APP
#elif COSMO
#	define BASE_APP acs::CosmoApp
#elif CAL_VAL_APP
#	define BASE_APP acs::CvfSchApp
#else 
#	define BASE_APP acs::CryosatApp
#endif 
_ACS_BEGIN_NAMESPACE(acs)


class WSConfigBuilder : public BASE_APP
{
public:
	/*! class WSConfigBuilderException declaration */
	exDECLARE_EXCEPTION(WSConfigBuilderException,acs::exException) ;


	WSConfigBuilder (const std::string &appName, const std::string &subSystem);
	virtual ~WSConfigBuilder() throw() ;

	virtual int main(int argc, char const * const * argv, char const * const * env);
	int execute (int argc, char const * const * argv, char const * const * env);

private: // methods
	int usage(const std::string&) const;
	void print_manual();
	void loadPlugIns();
	bool parseArgv(int argc, char const * const * argv, char const * const * env);
	void initKeyMap(const std::string& ttKeyFile);
	void build (const std::vector<std::string>& filenames);


	// defined but not implemented (disabled)
	WSConfigBuilder& operator = (const WSConfigBuilder& d);
	WSConfigBuilder (const WSConfigBuilder& d);


private: // attributes

	boost::program_options::options_description _allParDef;
	boost::program_options::variables_map _argvParameters;

	std::string _wscFile;
	std::string _taskTableListFile;
	std::string _dirName;
	WSConfig _wsConfiguration;

	std::string _ttPluginsPath;

	//Map containing pairs: (TTregExp, key)
	std::map<std::string, std::string> _keyMap;

	// The loader must have the same lifetime of the application
	acs::ConfigurablePluginLoader _pluginLoader;

private: // static stuff...

	ACS_CLASS_DECLARE_DEBUG_LEVEL (WSConfigBuilder)
};

_ACS_END_NAMESPACE

#endif // _TASK_TABLE_MERGER_H
