// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: ThinLayerDaemon File Cache Manager $

	$Id$

	$Author$

	$Log$
	Revision 3.1  2016/10/12 08:09:06  marfav
	S3MPM-843
	Cache is now able to use copy instead of hardlink if configured
	
	Revision 3.0  2016/10/11 08:27:16  marfav
	Major release due to cache management modifications
	
	Revision 2.9  2016/10/11 08:23:37  marfav
	S3MPM-842
	Using libFile Cache object to manage a rolling cache
	Cache size is configurable and all filetypes are cached by default
	Linked products are put read-only (mode 0444)
	In case of Header+Binary split, unsplit files are removed ASAP to avoid cache misuse
	
	Revision 2.8  2015/12/01 15:49:53  marfav
	Fixing metric types
	
	Revision 2.7  2015/11/27 17:38:06  marpas
	coding best practice applicationin progress
	
	Revision 2.6  2015/04/30 15:49:17  marfav
	Compiler warning removed
	
	Revision 2.5  2012/02/09 17:26:24  marpas
	refactoring in progress
	
	Revision 2.4  2011/09/16 14:17:19  marfav
	Addedd support to stream input files
	
	Revision 2.3  2009/03/18 13:29:57  marfav
	Using database table to retrieve downloaded file extensions
	
	Revision 2.2  2009/03/17 15:59:02  marfav
	Addedd modification date in cached files prefix
	to handle replaceFile inventory mode
	
	Revision 2.1  2008/04/24 14:48:40  marfav
	Addedd support for file version information
	Extensions retrieved from SDF will not be stripped anymore
	
	Revision 2.0  2006/02/28 17:02:07  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.3  2006/01/19 18:00:58  marfav
	Using the local stored file if multiple download requests for the same file are issued
	
	Revision 1.2  2005/06/22 12:36:20  enrcar
	Added handling for namespace std
	
	Revision 1.1  2005/03/02 14:06:18  marfav
	Added cache support for downloaded files
	

*/


#ifndef _CacheManager_H_
#define _CacheManager_H_

#include <dbInventoryObject.h>
#include <exStream.h>
#include <exException.h>

#include <Cache.h>

#include <string>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

class CacheManager : public ThreadSafe // PRQA S 2109, 2153
{

public:
	CacheManager ();
	explicit CacheManager (const std::string& cachePath);
	virtual ~CacheManager () throw() ;
	
	std::string getFile (dbInventoryObject &, const std::string& path, bool) const;
	std::string getFinalFileName (dbInventoryObject &, const std::string& path) const;
	
	CacheManager (const CacheManager&);
	CacheManager& operator= (const CacheManager&);

protected:

private:
	void init();
	void initFileCache();
	std::string downloadFile (dbInventoryObject &, const std::string& path, bool addExtraInfo, bool isStream) const;
	std::string deliverFromCache (dbInventoryObject &, const std::string& path, bool hardlink=true) const;
	bool isCacheable (const std::string& fileType) const;   
	bool existsInPath (dbInventoryObject&, const std::string&, bool addExtraInfo) const;
	bool isInCache (dbInventoryObject &) const;
	std::pair <std::string, std::string> getFileNameToSearch (dbInventoryObject&, const std::string& thePath, bool addExtraInfo) const;
    std::string computePrefix (dbInventoryObject&) const;

	
private:
	std::string _cachePath;
	bool _cacheActive;
	std::vector <std::string> _cacheableTypes;
	double _cacheSize;
	ACS_SMARTPTR <Cache> _fileCache;
	bool _useHardLinks;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL (CacheManager)


};
						 
_ACS_END_NAMESPACE
						 
#endif //_CacheManager_H_
