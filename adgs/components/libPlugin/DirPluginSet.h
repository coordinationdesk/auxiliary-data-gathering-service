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

	$Prod: A.C.S. Plugin Library $

	$Id$

	$Author$

	$Log$
	Revision 2.5  2013/02/19 14:35:03  marpas
	coding best practices applied
	PluginLoaderNotifier class introduced
	qa rules
	
	Revision 2.4  2012/12/01 19:43:00  marpas
	qa rules
	
	Revision 2.3  2012/02/13 11:00:26  marpas
	refactoring in progress
	
	Revision 2.2  2010/08/03 14:17:56  nicvac
	Multiple paths handling.
	
	Revision 2.1  2008/11/24 11:16:06  marpas
	getPluginList method added
	
	Revision 2.0  2006/02/28 08:36:44  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:48:44  marfav
	Header fixed
	
	Revision 1.1  2004/06/04 08:19:14  marfav
	Importing DirPluginSet
	

*/


#ifndef _DirPluginSet_H_
#define _DirPluginSet_H_

#include <acs_c++config.hpp>
#include <PluginSet.h>
#include <PluginLoader.h>

#include <exException.h>
#include <vector>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)

// This class reads the content of a basePath searching for
// file matching the given regular expression
// It uses a PluginLoader class to load all the matching plugins

class DirPluginSet : protected PluginSet // PRQA S 2109, 2194
{
public:
	// Default is to load all *.so objects as plugins
	explicit DirPluginSet(const std::vector<std::string> &basePaths, const std::string &regExp = ".*\\.so");
	explicit DirPluginSet(const std::string &basePath,               const std::string &regExp = ".*\\.so");
	virtual ~DirPluginSet() ACS_NOEXCEPT ;

	// Returns TRUE and the list of matching files on success
	// Returns FALSE on errors
	bool scanDir (std::vector<std::string>&);
	void load (PluginLoader&, bool optional=true, int mode = RTLD_LAZY|RTLD_GLOBAL); // PRQA S 2121
	const std::vector<std::string> &getPluginList() const ;

private:
	// Declared but not implemented
	DirPluginSet ();
	DirPluginSet (const DirPluginSet&);
	DirPluginSet& operator= (const DirPluginSet&);


private:
	std::vector<std::string> _basePaths;
	std::string _currentScannedPath;
	std::string _regExp;
	std::vector <std::string> _theList ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(DirPluginSet) ;
};


_ACS_END_NAMESPACE


#endif //_DirPluginSet_H_

