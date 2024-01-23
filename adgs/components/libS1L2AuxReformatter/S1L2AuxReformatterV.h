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

	$Prod: A.C.S. libS1L2AuxReformatter$

	$Id$

	$Author$

	$Log$
	Revision 1.2  2013/10/17 15:56:12  nicvac
	compilation warnings processed.
	
	Revision 1.1.1.1  2012/03/27 11:50:11  damdec
	Import libS1L2AuxReformatter
	
        
*/

#ifndef _S1L2AuxReformatterV_H_
#define _S1L2AuxReformatterV_H_

#include <acs_c++config.hpp>

_ACS_BEGIN_NAMESPACE(acs)

//! \brief support class for versioning

class S1L2AuxReformatterV {
public:
	S1L2AuxReformatterV() ;
	~S1L2AuxReformatterV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

_ACS_END_NAMESPACE

#endif 


