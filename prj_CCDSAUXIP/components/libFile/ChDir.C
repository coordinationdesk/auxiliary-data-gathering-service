/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. File Tools Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/20 16:43:52  marpas
	adoption of libException 5.x in progress
	
	Revision 2.6  2012/11/29 19:31:29  marpas
	implementing qa rules
	optimizing code and coding best practices
	coverage framework initiated
	
	Revision 2.5  2012/02/13 10:59:04  marpas
	refactoring in progress
	
	Revision 2.4  2012/02/07 17:57:43  marpas
	refactoring in progress
	
	Revision 2.3  2008/11/25 14:48:02  marpas
	regression fixed ... did not go to previous directory
	
	Revision 2.2  2008/11/17 16:55:48  marpas
	regression fixed
	
	Revision 2.1  2008/11/17 14:12:34  marpas
	getCurrentPath method in class File
	
	Revision 2.0  2006/02/28 08:33:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/30 22:06:41  marpas
	headers fixed
	
	Revision 1.2  2004/04/29 13:16:02  marpas
	getCwd method added
	
	Revision 1.1  2004/04/21 09:44:59  marpas
	new class added
	
	
		

*/

#include <ChDir.h>
#include <File.h>
#include <exException.h>

#include <climits>

_ACS_BEGIN_NAMESPACE(acs)

//////////////////////////////
//							//
// classe ChDir				//
//							//
//////////////////////////////

ChDir::ChDir(const std::string &path, bool autoReturn) : 
	_autoReturn(autoReturn)
{
	if (! path.empty()) {
    	if (_autoReturn) { 
			_oldp = File::getCurrentPath() ;
		}
		File::chdir(path) ;
	}
}

ChDir::ChDir(const ChDir &r) :
    _oldp(r._oldp),
    _autoReturn(r._autoReturn)    
{
    if (_autoReturn) {  // this one owns the autoreturn
        r._autoReturn = false ; // remove other 
    }
}

ChDir &ChDir::operator=(const ChDir &r) 
{
    if (this != &r) {
        _oldp = r._oldp ;
        _autoReturn = r._autoReturn ;
        if (_autoReturn) {
            r._autoReturn = false ; // this one owns the autoreturn
        }
    }
	return *this ;
}

ChDir::~ChDir() noexcept 
{
    try {
	    if (_autoReturn && ! _oldp.empty()) {
		    File::chdir(_oldp.c_str()) ;
        }
    }
    catch(const std::exception &x) { // NOSONAR - any exception 
        ACS_LOG_NOTIFY_EX(x) ;
    }
}

std::string ChDir::getCwd() 
{
	return File::getCurrentPath() ;
}

_ACS_END_NAMESPACE

