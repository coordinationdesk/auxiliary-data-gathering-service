// PRQA S 1050 EOF
/*

    Copyright 2020, Exprivia SpA 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.com

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    SRVRemoveInventory  version file

*/ 


#ifndef _sriV_H_
#define _sriV_H_

namespace acs {

// support class for versioning

class sriV {
public:
	sriV() ;
	~sriV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace

#endif // _sriV_H_

