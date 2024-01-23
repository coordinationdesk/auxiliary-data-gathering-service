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

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.2  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.1.1.1  2012/03/15 09:17:48  damdec
	Import libACSSafeFormatter
	
        
*/
#ifndef _ACSSafeFormatterV_H_
#define _ACSSafeFormatterV_H_

namespace acs {

//! \brief support class for versioning

class ACSSafeFormatterV { // PRQA S 2109
public:
	ACSSafeFormatterV() ;
	~ACSSafeFormatterV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:    
    ACSSafeFormatterV(const ACSSafeFormatterV &) ; // not implemented
    ACSSafeFormatterV& operator=(const ACSSafeFormatterV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif 


