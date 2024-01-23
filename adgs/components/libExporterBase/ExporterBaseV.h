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

	$Prod: A.C.S. libExporterBase$

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/07 15:51:57  marpas
	adopting libException 5.x standards
	
	Revision 1.1.1.1  2012/10/15 14:07:46  nicvac
	Import libExporterBase
	
        
*/

#ifndef _ExporterBaseV_H_
#define _ExporterBaseV_H_

namespace acs {

//! \brief support class for versioning

class ExporterBaseV {
public:
	ExporterBaseV() ;
	~ExporterBaseV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif 


