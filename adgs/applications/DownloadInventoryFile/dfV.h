/*

	Copyright 1995-2005, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DownloadInventoryFile $

	$Id$

	$Author$

	$Log$
	Revision 2.0  2006/02/28 10:38:17  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/02/23 11:33:52  ivafam
	Import DownloadInventoryFile 
	
	

*/

#ifndef _dfV_H_
#define _dfV_H_

namespace acs {

// support class for versioning

class dfV {
public:
	dfV() throw() ;
	~dfV() throw() ;
	static const char*  tag() throw() ;
	static const char*  name() throw() ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _dfV_H_

