/*

	Copyright 1995-2004, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. TlDaemon Default PCC factory library $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2011/09/16 11:07:43  marfav
	moved to 2.0
	
	Revision 1.1.1.1  2008/08/04 10:10:39  marfav
	Importing libTlDefaultPCC
	
*/

#ifndef _tldpccfV_H_
#define _tldpccfV_H_

namespace acs {

// support class for versioning


class tldpccfV {
public:
	tldpccfV() throw() ;
	~tldpccfV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _tldpccfV_H_



