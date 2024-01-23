// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
	Revision 2.2  2015/10/01 13:45:28  lucio.pulvirenti
	S2PDGS-1247/1249: bug fixed: if input file does not exist throws exceptiom instead of exit(-1). In case of reading errors,
	exception thrown.
	
	Revision 2.1  2013/11/27 16:21:24  marpas
	coding best practices applied
	warnings removed
	
	Revision 2.0  2006/02/28 09:12:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2006/01/03 10:01:12  ivafam
	First Import
	

*/


#include <DigestTree_md5.h> 
#include <Md5Sum.h>
#include <File.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


Md5Sum::Md5Sum(const string &filename) : SignatureAlgo(filename)
{

}
	
Md5Sum::~Md5Sum() throw()
{

}

string Md5Sum::computeSignature()
{
	
	string signature;
	string filename = getFileName() ;
	if( File::isDir(filename) && getManageDirectories() ) { // if directories to be managed compute md5Sum on input directory GIT style
	
		DigestTree_md5 digest;
		digest.setObject(filename, false) ;
		signature = digest.getObjectID();
	}
	else {
		signature = File::computeMd5Sum(filename);
	}

	return  signature;

}


_ACS_END_NAMESPACE
