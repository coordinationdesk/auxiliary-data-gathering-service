// PRQA S 1050 EOF
/*

    Copyright 1995-2019, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    PRIPRolling  version file

*/ 


#ifndef _priprollV_H_
#define _priprollV_H_

namespace acs {

// support class for versioning

class priprollV {
public:
	priprollV() throw() ;
	~priprollV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	static const char* _tag ;
	static const char* _name ;
} ;

} // Close namespace

#endif // _priprollV_H_
