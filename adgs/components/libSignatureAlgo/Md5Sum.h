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

	$Prod: A.C.S. Signature Algo Library $

	$Id$

	$Author$

	$Log$
	Revision 2.1  2006/03/08 14:37:24  ivafam
	Added in PDS Rose Model
	
	Revision 2.0  2006/02/28 09:12:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2006/01/03 10:01:12  ivafam
	First Import
	
		
*/


#ifndef _Md5Sum_H_
#define _Md5Sum_H_ 

#include <string>
#include <SignatureAlgo.h>

_ACS_BEGIN_NAMESPACE(acs)


class Md5Sum : public SignatureAlgo
{
public:
	
    
	explicit Md5Sum(const std::string &filename) ;
    
	virtual ~Md5Sum() throw() ;
    
	virtual std::string computeSignature();
private:
	Md5Sum() ; // not implemented
	Md5Sum(const Md5Sum &) ; // not implemented
	Md5Sum& operator=(const Md5Sum &) ; // not implemented
	
} ; 

_ACS_END_NAMESPACE


#endif // _SignatureAlgo_H_
