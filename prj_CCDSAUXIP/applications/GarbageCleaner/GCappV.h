// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. GarbageCleaner $

	$Id$

	$Author$
	
	$Log$
	Revision 1.1  2005/09/07 08:17:22  marpas
	versioning fixed
	

*/

#ifndef _GCappV_H_
#define _GCappV_H_

namespace acs {

//! \brief support class for versioning
    
class GCappV { // PRQA S 2109
public:
	GCappV() ;
	~GCappV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	GCappV(const GCappV &) ; // not implemented
    GCappV& operator=(const GCappV &) ; // not implemented
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _RMV_H_
