// PRQA S 1050 EOF
/*

	Copyright 1995-2002, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

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

using namespace acs ;
using namespace std ;

//////////////////////////////
//							//
// classe ChDir				//
//							//
//////////////////////////////

ChDir::ChDir() :
    _oldp(),
    _autoReturn(false)    
{
}

ChDir::ChDir(const string &path, bool autoReturn) : 
    _oldp(),
	_autoReturn(autoReturn)
{
	if (not path.empty()) {
    	if (_autoReturn) { 
			_oldp = File::getCurrentPath() ;
		}
		File::chdir(path) ;
	}
}

ChDir::ChDir(const ChDir &o) :
    _oldp(),
    _autoReturn(false)    
{
    *this = o ;
}

ChDir &ChDir::operator=(const ChDir &o) // PRQA S 4072
{
    if (this != &o) {
        _oldp = o._oldp ;
        _autoReturn = o._autoReturn ;
        if (_autoReturn) {
            o._autoReturn = false ; // this one owns the autoreturn
        }
    }
	return *this ;
}

ChDir::~ChDir() throw() 
{
// PRQA S 4631 L1
    try {
	    if (_autoReturn && not _oldp.empty()) {
		    File::chdir(_oldp.c_str()) ;
        }
    }
    catch(exception &x) {
        excerr << x ;
    }
// PRQA L:L1
}

string ChDir::getCwd() 
{
	return File::getCurrentPath() ;
}


