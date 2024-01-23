// PRQA S 1050 EOF
/*

    Copyright 1995-2021, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    LTAInventoryMetricsTracker version file

*/ 


#ifndef _ltainvmetV_H_
#define _ltainvmetV_H_

namespace acs {

// support class for versioning

class ltainvmetV {
public:
	ltainvmetV() throw() ;
	~ltainvmetV() throw() ;
	static const char* tag() throw() ;
	static const char* name() throw() ;
private:
	static const char* _tag ;
	static const char* _name ;
} ;

} // Close namespace

#endif // _ltainvmetV_H_
