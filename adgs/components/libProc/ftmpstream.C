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

	$Prod: A.C.S. Process library $

	$Id$

	$Author$

	$Log$
	Revision 2.6  2013/07/24 10:50:56  marfav
	adopting libException 5.x standards
	
	Revision 2.5  2012/12/01 20:06:30  marpas
	qa rules
	
	Revision 2.4  2012/02/09 18:40:11  marpas
	refactoring in progress
	
	Revision 2.3  2012/01/31 17:20:32  marpas
	removing compiler warnings
	
	Revision 2.2  2008/11/18 14:22:32  marpas
	setAutoremove changed int setAutoRemove
	
	Revision 2.1  2008/11/18 14:20:31  marpas
	setAutoRemove method added
	
	Revision 2.0  2006/02/28 08:34:06  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/07/25 15:42:35  ivafam
	Header format fixed
	
	Revision 1.4  2004/03/08 15:46:39  envisat
	rewritten to use ifdstream and ofdstream from libFile 1_17
	
	Revision 1.3  2003/04/30 16:30:06  marpas
	using namespace std no longer into acs includes
	
	Revision 1.2  2003/03/26 14:49:17  marpas
	oftmpstream default ctor without parameter fixed: tempnam cannot be called if
	the cwd is not writable.
	
	Revision 1.1  2003/02/25 18:51:57  marpas
	ftmpstream classes added
	
	

*/

#include <ftmpstream.h>
#include <cstdio>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(oftmpstream)

oftmpstream::oftmpstream() :
	_autoRemove(true),
    _path(),
    _theStream()
{
}

oftmpstream::~oftmpstream() throw() 
{
// PRQA S 4631 L1
    try {
	    closeAndRemove() ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
// PRQA L:L1
}

oftmpstream::oftmpstream(const string &prefix, const string &ext, File::CompressionMode m, bool autoRemove) :
	_autoRemove(autoRemove),
	_path(File::tempname(prefix,ext)), // PRQA S 3050
	_theStream(_path.c_str(),m)
{
}

const string & oftmpstream::path() const throw() // PRQA S 4120
{
	return _path ;
}


void oftmpstream::closeAndRemove()
{
	close() ;
	try {
		if (_autoRemove) { File::unlink(_path) ; }
	} 
	catch (exception &) {
		// ignore 
	}
}

bool oftmpstream::setAutoRemove(bool n) 
{
	bool old = _autoRemove ;
	_autoRemove = n ;
	return old ;
}


void oftmpstream::close() 
{
	_theStream.close() ;
}

void oftmpstream::open(const string &prefix, const string &ext, File::CompressionMode m, bool autoRemove) 
{
	closeAndRemove() ;
	_autoRemove = autoRemove ;
	_path = File::tempname(prefix,ext) ; // PRQA S 3050
	_theStream.open(_path.c_str(), m) ;
}


ostream &oftmpstream::operator()()
{
	return _theStream() ;
}
	

_ACS_END_NAMESPACE
