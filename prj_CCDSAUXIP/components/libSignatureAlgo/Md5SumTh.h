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
	Revision 2.1  2007/04/13 17:40:35  marpas
	threaded md5 signature added
	
	
		
*/


#ifndef _Md5SumTh_H_
#define _Md5SumTh_H_ 

#include <SignatureAlgo.h>
#include <FileMD5Th.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class Md5SumTh : public SignatureAlgo // PRQA S 2109
{
public:
	
	explicit Md5SumTh(const std::string &filename) ;
	virtual ~Md5SumTh() throw() ;

	virtual std::string computeSignature();
private:
	Md5SumTh() ; // not implemented
	Md5SumTh(const Md5SumTh &) ; // not implemented
	Md5SumTh& operator=(const Md5SumTh &) ; // not implemented
private:
	FileMD5Th _md5Th ;
	
} ; 

_ACS_END_NAMESPACE



#endif // _SignatureAlgo_H_
