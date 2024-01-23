// PRQA S 1050 EOF
/*
 * 
 	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Storage Manager Library $
 
    libStorageManager  version file

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/18 14:40:02  marpas
	adoption of libException 5.x standards
	
	Revision 2.6  2013/02/25 17:00:18  marpas
	coding best practices applied, useless compilation unti removed
	
	Revision 2.5  2012/02/13 17:08:58  marpas
	refactoring in proegress
	
	Revision 2.4  2008/06/20 10:19:52  marpas
	version file moved
	
	Revision 2.3  2008/06/20 10:16:51  marpas
	version file moved
	
	Revision 2.0  2006/02/28 09:11:13  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/06/07 09:56:56  integrator
	split in progress
	
	Revision 1.1.1.1  2005/05/18 07:57:07  marpas
	Importing libStorageManager
	
	

*/

#ifndef _stormgrV_H_
#define _stormgrV_H_

namespace acs {

// support class for versioning

class stormgrV { // PRQA S 2109
public:
	stormgrV() noexcept ;
	~stormgrV() noexcept ;
	static const char* tag() noexcept ;
	static const char* name() noexcept ;
private:
    stormgrV(const stormgrV &) ; // not implementesd
    stormgrV & operator= (const stormgrV &) ; // not implementesd
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _stormgrV_H_

