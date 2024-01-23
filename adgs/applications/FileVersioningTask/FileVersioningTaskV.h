/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	
        
*/

#ifndef _FileVersioningTaskV_H_
#define _FileVersioningTaskV_H_
#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

//! \brief support class for versioning

class FileVersioningTaskV { // PRQA S 2109
public:
	FileVersioningTaskV() throw() ;
	~FileVersioningTaskV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	FileVersioningTaskV(const FileVersioningTaskV &) ; // not implemented
	FileVersioningTaskV & operator= (const FileVersioningTaskV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

_ACS_END_NAMESPACE

#endif // _FileVersioningTaskV_H_

