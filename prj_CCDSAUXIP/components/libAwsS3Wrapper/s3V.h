/*

	Copyright 2019-, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. S3 Wrapper Library Version File Header $

	$Log$

*/
 
#ifndef _s3V_H_
#define _s3V_H_

namespace acs {

// support class for versioning

class s3V { // PRQA S 2109
public:
	s3V() throw() ;
	~s3V() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	s3V(const s3V &) ;
	s3V &operator=(const s3V &) ;
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _s3V_H_



