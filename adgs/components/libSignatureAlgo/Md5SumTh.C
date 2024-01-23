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
	
	Revision 2.0  2006/02/28 09:12:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2006/01/03 10:01:12  ivafam
	First Import
	

*/


#include <Md5SumTh.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


Md5SumTh::Md5SumTh(const string &filename) : 
	SignatureAlgo(filename),
	_md5Th(filename, 1048576) // PRQA S 4400
{
	_md5Th.start() ;
}
	
Md5SumTh::~Md5SumTh() throw()
{
}

string Md5SumTh::computeSignature()
{
	return _md5Th.getResult() ;
}


_ACS_END_NAMESPACE
