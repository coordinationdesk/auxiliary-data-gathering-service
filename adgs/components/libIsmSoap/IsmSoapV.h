// PRQA S 1050 EOF
/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libIsmSoap$

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2008/05/05 16:54:47  paoscu
	Imported into CVS
	
        
*/

#ifndef _IsmSoapV_H_
#define _IsmSoapV_H_

namespace acs {

   //! \brief support class for versioning

class IsmSoapV { // PRQA S 2109
public:
    IsmSoapV() ;
    ~IsmSoapV() ;
    static const char*  tag() { return _tag ; }
    static const char*  name() { return _name ; }
private:
    IsmSoapV(const IsmSoapV &) ; // not implemented
    IsmSoapV& operator= (const IsmSoapV &) ; // not implemented

private:
    static const char* 	_tag ;
    static const char* 	_name ;
} ;

} // Close namespace

#endif 


