/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: Sentinel3 Auxiliary Reformatter application $

	$Id$

	$Author$
	
	$Log$
	

*/

#ifndef _S3AuxRefCoreV_H_
#define _S3AuxRefCoreV_H_
#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

//! \brief support class for versioning

class S3AuxRefCoreV { // PRQA S 2109
public:
	S3AuxRefCoreV() throw() ;
	~S3AuxRefCoreV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	S3AuxRefCoreV(const S3AuxRefCoreV &) ; // not implemented
	S3AuxRefCoreV & operator= (const S3AuxRefCoreV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

_ACS_END_NAMESPACE

#endif // _S3AuxRefCoreV_H_

