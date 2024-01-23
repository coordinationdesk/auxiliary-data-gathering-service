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

	$Prod: A.C.S. libIsmSoapClient$

	$Id$

	$Author$

	$Log$
	Revision 5.1  2014/06/27 14:38:31  marpas
	qa warnings fixed
	
	Revision 5.0  2013/07/22 18:15:54  marpas
	adopting libException 5.x standards
	
	Revision 1.2  2013/07/12 14:16:54  marpas
	work in progress
	
	Revision 1.1.1.1  2008/05/07 17:48:45  paoscu
	Imported into CVS
	
        
*/

#ifndef _IsmSoapClientV_H_
#define _IsmSoapClientV_H_

namespace acs {

   //! \brief support class for versioning

class IsmSoapClientV { // PRQA S 2109
public:
   	IsmSoapClientV() ;
   	~IsmSoapClientV() ;
   	static const char*  tag() { return _tag ; }
   	static const char*  name() { return _name ; }
private:
   	IsmSoapClientV(const IsmSoapClientV &) ; // not implemented
   	IsmSoapClientV& operator= (const IsmSoapClientV &) ; // not implemented
private:
   	static const char* 	_tag ;
   	static const char* 	_name ;
} ;

} // Close namespace

#endif 


