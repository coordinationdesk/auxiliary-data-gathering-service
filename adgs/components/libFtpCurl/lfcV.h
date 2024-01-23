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

	$Prod: A.C.S. Ftp Curl Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/04/05 09:44:15  marpas
	coding best practices applied
	qa warnings (partial)
	libDCUtility interface update
	
	Revision 2.0  2011/10/18 14:43:52  marpas
	new statistics adopted
	
	Revision 1.2  2009/10/06 09:51:36  marpas
	passive mode implemented
	rules applied
	
	Revision 1.1.1.1  2008/05/14 12:44:00  matteo.airoldi
	libFtpCurl first Issue
	
	
*/
 
#ifndef _lfcV_H_
#define _lfcV_H_

namespace acs {

// support class for versioning

class lfcV { // PRQA S 2109
public:
	lfcV() ;
	~lfcV() ;
	static const char*  tag() ;
	static const char*  name() ;
private:
	// declared but not defined
	lfcV(const lfcV &) ;
	lfcV & operator=(const lfcV &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _lfcV_H_



