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

	$Prod: libFileInventory $ 

	$Id$

	$Author$

	$Log$
	Revision 2.3  2013/02/21 19:25:15  marpas
	interface redesign to support explicit connections
	coding best practices applied
	qa rules (still in progress)
	

*/

#ifndef _FileInvV_H_
#define _FileInvV_H_

namespace acs {

// support class for versioning

class FileInvV { // PRQA S 2109
public:
	FileInvV() throw() ;
	~FileInvV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	// declared but not implemented to prevent their use
	FileInvV(const FileInvV&) ;
	FileInvV &operator=(const FileInvV&) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _FileInvV_H_

