// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	libADGSStandaloneApp  version file
		
*/ 

 
#ifndef _ladgssaV_H_
#define _ladgssaV_H_

namespace acs {

// support class for versioning

class ladgssaV { // PRQA S 2109
public:
	ladgssaV() throw() ;
	~ladgssaV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
    ladgssaV(const ladgssaV &) ; // not implemented
    ladgssaV & operator= (const ladgssaV &) ; // not implemented 
    
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _ladgssaV_H_



