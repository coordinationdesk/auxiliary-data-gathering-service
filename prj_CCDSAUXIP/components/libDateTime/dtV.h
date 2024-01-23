/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DateTime Library $

	$Id$

	$Author$

	$Log$
	Revision 2.3  2012/12/02 20:04:10  marpas
	qa rules
	
	Revision 2.2  2012/02/08 09:52:55  marpas
	refactoring
	
	Revision 2.1  2009/03/23 13:55:09  marpas
	JD70 class added
	rule violations fixed
	
	Revision 2.0  2006/02/28 08:40:49  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/02/07 09:39:41  marpas
	headers fixed
	
	Revision 1.1.1.1  2002/11/27 17:17:19  paoscu
	Import libDateTime
	

*/

#ifndef _dtV_H_
#define _dtV_H_

namespace acs {

// support class for versioning

class dtV { // PRQA S 2109
public:
	dtV() = default ; 
	~dtV() = default ;
	static const char*  tag() noexcept ;
	static const char*  name() noexcept ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _dtV_H_



