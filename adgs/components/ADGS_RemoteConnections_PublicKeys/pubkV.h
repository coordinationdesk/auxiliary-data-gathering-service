//
//  Copyright (c) 2021 EXPRIVIA
//  License: Proprietary

#ifndef _pubkV_H_
#define _pubkV_H_

namespace acs {

// support class for versioning

class pubkV {
public:
	pubkV() ;
	~pubkV() ;
	static const char*  tag() { return _tag ; }
	static const char*  name() { return _name ; }
private:
	static const char* 	_tag ;
	static const char* 	_name ;
} ;

} // Close namespace


#endif // _pubkV_H_



