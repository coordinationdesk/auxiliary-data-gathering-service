/*

	Copyright 1995-2001, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. FieldReplacer $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2008/11/18 13:43:44  manuel.maccaroni
	smaller changes
	
	Revision 1.1  2008/11/15 13:52:25  manuel.maccaroni
	Fixed wrong named of support class and files for versioning
	
	Revision 1.2  2008/11/14 14:50:13  manuel.maccaroni
	fixed cvs tags
	
	Revision 1.1.1.1  2008/11/14 14:47:24  manuel.maccaroni
	Import libFieldReplacer
	
	
	
	

*/

#ifndef _FieldReplacerV_H_
#define _FieldReplacerV_H_

namespace acs {

// support class for versioning

class FieldReplacerV {
public:
	FieldReplacerV() throw() ;
	~FieldReplacerV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _FieldReplacerV_H_



