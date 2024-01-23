// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia, SpA

	$Prod: GarbageCleaner $
	
	$Log$
	Revision 1.8  2016/01/29 11:47:05  marpas
	coding best practices application in progress
	
	Revision 1.7  2015/07/08 09:59:26  damdec
	Debug Log macros updated to fix compilation warnings.
	
	Revision 1.6  2013/05/22 14:35:21  marco.mancini
	Implemented logging of removed files by the means of the optional
	boolean LogAct tag into configuration file.
	
	Revision 1.5  2012/02/14 15:18:15  marpas
	fixing dtor
	
	Revision 1.4  2005/09/28 10:12:40  marpas
	file/dir remotion now uses File::remove
	includes reorganized
	namespaces too
	
	Revision 1.3  2005/09/07 08:14:27  marpas
	Versioning fixed.
	

*/


#ifndef _GARBAGECLEANER_H_
#define _GARBAGECLEANER_H_


#include <RegExp.h>
#include <DateTime.h>

#include <string>


struct dirent ;

namespace acs
{
    class GarbageCleaner // PRQA S 2109
    {
	public:
	    GarbageCleaner();
	    ~GarbageCleaner();
	    void init();
	    int run();

	private:
	    std::string sopen(std::string const& /* cmd */);
	    void remove(std::string const& /* dir */, RegExp const&, DateTime const&, bool const&, const bool, const bool); // PRQA S 2502
        void removeIfOversize(off_t maxSize, const std::string &folder) ;
        GarbageCleaner(const GarbageCleaner &) ; // not implemented
        GarbageCleaner& operator=(const GarbageCleaner &) ; // not implemented
	private:
		ACS_CLASS_DECLARE_DEBUG_LEVEL(GarbageCleaner)
    };

} // namespace

#endif

